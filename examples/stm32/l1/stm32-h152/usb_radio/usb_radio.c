/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2013 Toshiaki Yoshida <yoshida@mpc.net>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <rcc.h>
#include <pwr.h>
#include <flash.h>
#include <gpio.h>
#include <tim.h>
#include <nvic.h>
#include <adc.h>
#include <dma.h>
#include <exti.h>
#include <syscfg.h>
#include <usbdevfs.h>
#include <i2c.h>

#include "usb_radio.h"
#include "control.h"

/*
 * Si4703 FM Tuner Basic Breakout
 * BOB-11083
 * SparkFun Electronics.
 */

/*
 * Pin Connection
 *
 * 1	GPIO2
 * 2	GPIO1
 * 3	nRST	PB8
 * 4	nSEN	PB9
 * 5	SCLK	PB10(SCL)
 * 6	SDIO	PB11(SDA)
 * 7	VCC
 * 8	GND
 * 9	ROUT	PB12(ADC_IN18)
 * 10	LOUT	PB13(ADC_IN19)
 */
#define MAXREG		16
#define SI4703_SLA	0x20	/* 0010000|R/W */

/* Clock frequency */
#define SYSCLK		32000000
#define PCLK1		32000000
#define TIMX_CLK_APB1	32000000
#define TIMX_CLK_APB2	32000000

/* I2C parameters */
#define MODE		I2C_FAST
#define SCL		400000	 /* 400kHz */
#define T_R_MAX		10	 /* tr.max(300ns) * PCLK1 = 10  */
#define START_TIMEOUT	100	 /* 100usec (10 / SCL = 25us (Min)) */
#define BUSY_TIMEOUT	(SYSCLK / SCL)
#define TIMEOUT		(10 * (SYSCLK / SCL))

/* Si4703 timing */
#define T_GSRST1	100	/* 100us: nRST pulse width and GPIO3 Setup */
#define T_BUF		2	/* 1.3us: STOP to START Time */
#define T_POWERUP	110	/* 110ms: Powerup Time */

/* USB packet buffer memory */
#define BUFFER_TABLE_ADDRESS	0
#define PACKET_MEMORY_START	(BUFFER_TABLE_ADDRESS + \
				 USBDEVFS_BUFFER_TABLE_SIZE)

/* Audio stream buffer */
#define MAX_BUF			4
#define BUF_QUEUE_SIZE		(MAX_BUF + 1)

/* Si4703 device request */
volatile device_request_t devreq;
volatile int devreq_id;
volatile u16 devreq_reg[MAXREG];
volatile u16 si4703_reg[MAXREG];

/* I2C control */
static volatile int sndbuflen;
static volatile u8 sndbuf[MAXREG * sizeof(u16)];
static volatile int sndindex;
static volatile int rcvbuflen;
static volatile u8 rcvbuf[MAXREG * sizeof(u16)];
static volatile int rcvindex;
static volatile bool i2c_busy;

/* Error flags */
static volatile bool i2c_status;
static volatile bool i2c_timeout;
static volatile bool adc_overrun;

/* Audio stream buffer queue */
static u32 adc_buf[MAX_BUF][AS_SIZE / sizeof(u32)];
static u32 *buf_queue[BUF_QUEUE_SIZE];
static volatile int buf_head;
static volatile int buf_tail;

/* Buffer pointer */
static volatile u32 *dma_buf;
static volatile u32 *exti_buf;
static volatile u32 *tx_buf;

/* DMA flag */
static bool dma_running;

/* Frame counter */
static volatile u32 frame_count;

/* Interrupt transfer flag */
static volatile bool interrupt_busy;

/* Statistics */
static u32 usb_error;
static u32 tx_loss;
static u32 tx_loss1;
static u32 interrupt_send;
static u32 interrupt_int;
static u32 dma_count;
static u32 alloc_count;
static u32 free_count;


/* Set STM32 to 32 MHz. */
static void clock_setup(void)
{
	/* Enable PWR clock. */
	rcc_enable_clock(RCC_PWR);

	/* Set VCORE to 1.8V */
	pwr_set_vos(PWR_1_8_V);

	/* Set Flash memory latency (1WS). */
	flash_enable_64bit_access(1);

	/* Enable external high-speed oscillator 8MHz. */
	rcc_enable_osc(RCC_HSE);

	 /* Setup PLL (8MHz * 12 / 3 = 32MHz). */
	rcc_setup_pll(RCC_HSE, 12, 3);

	/* AHB, APB1 and APB2 prescaler value is default. */
	// rcc_set_prescaler(1, 1, 1);

	/* Enable PLL and wait for it to stabilize. */
	rcc_enable_osc(RCC_PLL);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(RCC_PLL);

	/* Enable internal high-speed oscillator 16MHz. */
	rcc_enable_osc(RCC_HSI);
}

static void gpio_setup(void)
{
	/* Enable GPIOB and GPIOE clock. */
	rcc_enable_clock(RCC_GPIOB);
	rcc_enable_clock(RCC_GPIOE);

	/* Set nSEN and SCLK high. */
	gpio_set(GPIO_PB(9, 10));

	/* Set nRST and SDIO low. */
	gpio_clear(GPIO_PB(8, 11));

	/* Set GPIO8 - 11 (in GPIO port B) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_2MHZ, GPIO_NOPUPD,
			   GPIO_PB(8, 9, 10, 11));

	/* Set GPIO12 and 13 (in GPIO port B) to 'analog'. */
	gpio_config_analog(GPIO_PB(12, 13));

	/* Set GPIO10 and GPIO11 (in GPIO port E) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PE(10, 11));

	/* LED off */
	gpio_set(GPIO_PE(10, 11));
}

static void tim_setup(void)
{
	/* Enable TIM6, TIM7, TIM9 ,TIM2 and TIM3 clock. */
	rcc_enable_clock(RCC_TIM6);
	rcc_enable_clock(RCC_TIM7);
	rcc_enable_clock(RCC_TIM9);
	rcc_enable_clock(RCC_TIM2);
	rcc_enable_clock(RCC_TIM3);

	/* Enable TIM2 interrupt. */
	nvic_enable_irq(NVIC_TIM2_IRQ);

	/* Enable TIM3 interrupt. */
	nvic_enable_irq(NVIC_TIM3_IRQ);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM6);
	tim_enable_one_pulse_mode(TIM7);
	tim_enable_one_pulse_mode(TIM2);
	tim_enable_one_pulse_mode(TIM3);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM6);
	tim_disable_update_interrupt_on_any(TIM7);
	tim_disable_update_interrupt_on_any(TIM2);

	/* Load prescaler value (2MHz). */
	tim_load_prescaler_value(TIM6, TIMX_CLK_APB1 / 2000000 - 1);

	/* Load prescaler value (2kHz). */
	tim_load_prescaler_value(TIM7, TIMX_CLK_APB1 / 2000 - 1);

	/* Set master(TRGO) mode. */
	tim_set_master_mode(TIM9, TIM_TRGO_UPDATE);

	/* Set Auto-Reload value (prescaler = 0). */
	tim_set_autoreload_value(TIM9, TIMX_CLK_APB2 / SAMPLING_FREQ - 1);

	/* Enable TIM2 interrupt. */
	tim_enable_interrupt(TIM2, TIM_UPDATE);

	/* Set slave mode (Master = TIM9). */
	tim_set_slave_mode(TIM3, TIM_EXTERNAL_CLOCK_MODE1 | TIM_ITR0);

	/* Set Auto-Reload value (prescaler = 0). */
	tim_set_autoreload_value(TIM3, SAMPLING_FREQ / 1000 - 1);

	/* Enable TIM3 interrupt. */
	tim_enable_interrupt(TIM3, TIM_UPDATE);
}

/* 1 - 32767 usec */
static void delay_us(u16 us)
{
	/* Set auto-reload value (us * 2). */
	tim_set_autoreload_value(TIM6, (us << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM6);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM6, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM6, TIM_UPDATE);
}

/* 1 - 32767 msec */
static void delay_ms(u16 ms)
{
	/* Set auto-reload value (ms * 2). */
	tim_set_autoreload_value(TIM7, (ms << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM7);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM7, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM7, TIM_UPDATE);
}

/* start-timeout (1 - 32767 usec) */
static void start_timer(u16 us)
{
	/* Load prescaler value (2MHz). */
	/* Set auto-reload value (us * 2). */
	tim_setup_counter(TIM2, TIMX_CLK_APB1 / 2000000 - 1, (us << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM2);
}

static void stop_timer(void)
{
	/* Disable counter. */
	tim_disable_counter(TIM2);
}

void tim2_isr(void)
{
	if (tim_get_interrupt_mask(TIM2, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM2, TIM_UPDATE)) {
		/* Disable I2C interrupt */
		i2c_disable_interrupt(I2C2, I2C_BUFFER | I2C_EVENT | I2C_ERROR);

		/* Error */
		i2c_timeout = true;
		i2c_busy = false;

		/* Clear interrupt */
		tim_clear_interrupt(TIM2, TIM_UPDATE);
	}
}

void tim3_isr(void)
{
	if (tim_get_interrupt_mask(TIM3, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM3, TIM_UPDATE)) {
		/* Disable TIM9. */
		tim_disable_counter(TIM9);

		/* Clear interrupt */
		tim_clear_interrupt(TIM3, TIM_UPDATE);
	}
}

static void adc_setup(void)
{
	int seq[2];

	/* Enable ADC clock. */
	rcc_enable_clock(RCC_ADC);

	/* Enable ADC interrupt. */
	nvic_enable_irq(NVIC_ADC_IRQ);

	/* Set ADC prescaler to 1(default). */
	// adc_set_prescaler(1);

	/* Set ADC_IN18 sampling time. */
	adc_set_sampling(ADC_IN_PB12, 96); /* 6.75 usec */

	/* Set ADC_IN19 sampling time. */
	adc_set_sampling(ADC_IN_PB13, 96); /* 6.75 usec */

	/* Set channel sequence. */
	seq[0] = ADC_IN_PB12;
	seq[1] = ADC_IN_PB13;
	adc_set_regular_sequence(2, seq);

	/* Enable scan mode. */
	adc_enable_scan(false);

	/* Enable DMA. */
	adc_enable_dma(true);

	/* Set data alignment. */
	adc_set_left_alignment();

	/* Enable overrun interrupt. */
	adc_enable_interrupt(ADC_OVERRUN);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	/* Enable external trigger. */
	adc_set_regular_ext(ADC_RISING, ADC_TIM9_TRGO);
}

void adc_isr(void)
{
	if (adc_get_interrupt_mask(ADC_OVERRUN) &&
	    adc_get_interrupt_status(ADC_OVERRUN)) {

		adc_overrun = true;

		/* Clear interrupt. */
		adc_clear_interrupt(ADC_OVERRUN);
	}
}

static void dma_setup(void)
{
	/* Enable DMA1 clock. */
	rcc_enable_clock(DMA_RCC_ADC);

	/* Enable DMA1 channel1 interrupt. */
	nvic_enable_irq(DMA_ADC_IRQ);
}

static void init_buf(void)
{
	int i;

	for (i = 0; i < MAX_BUF; i++)
		buf_queue[i] = adc_buf[i];
	buf_head = 0;
	buf_tail = MAX_BUF;
}

static u32 *alloc_buf(void)
{
	u32 *r;

	alloc_count++;
	if (buf_head == buf_tail)
		return 0;
	r = buf_queue[buf_head++];
	buf_head %= BUF_QUEUE_SIZE;
	return r;
}

static void free_buf(u32 *buf)
{
	free_count++;
	if ((buf_tail + 1) % BUF_QUEUE_SIZE == buf_head)
		return;
	buf_queue[buf_tail++] = buf;
	buf_tail %= BUF_QUEUE_SIZE;
}

/* DMA1 (channel1) interrupt */
void dma_adc_isr(void)
{
	/* Interrupt mask */
	if (!dma_get_interrupt_mask(DMA_ADC, DMA_COMPLETE))
		return;

	/* Interrupt status */
	if (dma_get_interrupt_status(DMA_ADC, DMA_GLOBAL | DMA_COMPLETE) !=
	    (DMA_GLOBAL | DMA_COMPLETE))
		return;

	/* Disable DMA. */
	dma_disable(DMA_ADC);

	/* Save data buffer. */
	exti_buf = dma_buf;

	/* Set DMA mode and enable DMA. */
	if ((dma_buf = alloc_buf())) {
		dma_setup_channel(DMA_ADC,
				  (u32)dma_buf, (u32)&ADC_DR,
				  AS_SIZE / sizeof(u16),
				  DMA_M_16BIT | DMA_P_32BIT |
				  DMA_M_INC | DMA_P_TO_M |
				  DMA_COMPLETE | DMA_ENABLE);
		dma_count++;
	}

	/* Generate software interrupt. */
	exti_set_software_interrupt(EXTI5);

	/* Clear interrupt. */
	dma_clear_interrupt(DMA_ADC, DMA_COMPLETE | DMA_GLOBAL);
}

static void exti_setup(void)
{
	/* Enable SYSCFG clcok. */
	rcc_enable_clock(RCC_SYSCFG);

	/* Enable EXTI9_5 interrupt. */
	nvic_enable_irq(NVIC_EXTI9_5_IRQ);

	/* Enable interrupt. */
	exti_enable_interrupt(EXTI5);
}

void exti9_5_isr(void)
{
	int i;
	static int count;

	if (exti_get_interrupt_mask(EXTI5) &&
	    exti_get_interrupt_status(EXTI5)) {
		/* 0, 65535 => -32768, 32767 */
		for (i = 0; i < (int)(AS_SIZE / sizeof(u32)); i++)
			exti_buf[i] ^= 0x80008000;

		/* Disable USB high priority interrupt. */
		nvic_disable_irq(NVIC_USB_HP_IRQ);

		if (tx_buf) {
			free_buf((u32 *)tx_buf);
			tx_loss1++;
		}
		tx_buf = exti_buf;

		/* Enable USB high priority interrupt. */
		nvic_enable_irq(NVIC_USB_HP_IRQ);

		/* LED ON/OFF */
		if (++count == 100) {
			gpio_toggle(GPIO_PE11);
			count = 0;
		}

		/* Clear interrupt. */
		exti_clear_interrupt(EXTI5);
	}
}

static void usb_setup(void)
{
	int offset;

	/* Enable USB clock. */
	rcc_enable_clock(RCC_USB);

	/* Enable USB high priority interrupt. */
	nvic_enable_irq(NVIC_USB_HP_IRQ);

	/* Enable USB Low priority interrupt. */
	nvic_enable_irq(NVIC_USB_LP_IRQ);

	/* Exit Power Down. */
	usbdevfs_disable_function(USBDEVFS_POWER_DOWN);

	/* Wait T_STARTUP. */
	delay_us(USBDEVFS_T_STARTUP);

	/* Clear USB reset. */
	usbdevfs_disable_function(USBDEVFS_FORCE_RESET);

	/* Set buffer table address */
	usbdevfs_set_buffer_table_address(BUFFER_TABLE_ADDRESS);

	/* Assign packet memory to endpoint */
	offset = usbdevfs_assign_packet_memory_tx(0, PACKET_MEMORY_START,
						  MAXPACKETSIZE0);
	offset = usbdevfs_assign_packet_memory_rx(0, offset, MAXPACKETSIZE0);
	offset = usbdevfs_assign_packet_memory_tx0(1, offset, AS_SIZE);
	offset = usbdevfs_assign_packet_memory_tx1(1, offset, AS_SIZE);
	offset = usbdevfs_assign_packet_memory_tx(2, offset, INTERRUPT_SIZE);

	usbdevfs_write0(1, (u16 *)adc_buf[0], AS_SIZE);
	usbdevfs_write1(1, (u16 *)adc_buf[0], AS_SIZE);
}

/* USB (high priority) interrupt */
void usb_hp_isr(void)
{
	u16 status;
	u16 mask;
	int ep_id;
	u16 trans;

	/* Interrupt mask */
	mask = usbdevfs_get_interrupt_mask(USBDEVFS_CORRECT_TRANSFER);
	if (!mask)
		return;

	/* Interrupt status */
	status = usbdevfs_get_interrupt_status(USBDEVFS_CORRECT_TRANSFER |
					       USBDEVFS_EP_ID);
	if (!(status & USBDEVFS_CORRECT_TRANSFER))
		return;

	/* Correct transfer */
	ep_id = status & USBDEVFS_EP_ID;
	trans = usbdevfs_get_ep_status(ep_id, USBDEVFS_TX | USBDEVFS_TX_DATA1);

	/* Isochronous endpoint Tx (IN transaction) */
	if (ep_id != 1 || !(trans & USBDEVFS_TX))
		return;

	if (tx_buf) {
		if (endpoint_state[1] == EP_STATE_ENABLE) {
			if (trans & USBDEVFS_TX_DATA1)
				usbdevfs_write0(1, (u16 *)tx_buf, AS_SIZE);
			else
				usbdevfs_write1(1, (u16 *)tx_buf, AS_SIZE);
		}
		free_buf((u32 *)tx_buf);
		tx_buf = 0;
	} else {
		tx_loss++;
	}

	/* Clear interrupt. */
	usbdevfs_clear_endpoint_interrupt(ep_id);
}

static void sof(void)
{
	/* Audio Stream */
	if (endpoint_state[1] == EP_STATE_ENABLE) {
		if (!dma_running) {
			/* Set DMA mode and enable DMA. */
			if ((dma_buf = alloc_buf())) {
				dma_setup_channel(DMA_ADC,
						  (u32)dma_buf, (u32)&ADC_DR,
						  AS_SIZE / sizeof(u16),
						  DMA_M_16BIT | DMA_P_32BIT |
						  DMA_M_INC | DMA_P_TO_M |
						  DMA_COMPLETE | DMA_ENABLE);
				dma_count++;
				dma_running = true;
			}
		}

		/* Start timer. */
		if (dma_running) {
			/* Set counter. */
			tim_set_counter(TIM3, 0);

			/* Enable counter. */
			tim_enable_counter(TIM3);

			/* Set counter. */
			tim_set_counter(TIM9,
					TIMX_CLK_APB2 / SAMPLING_FREQ - 1);

			/* Enable counter. */
			tim_enable_counter(TIM9);
		}
	}

	/* Interrupt In */
	frame_count++;
	if (endpoint_state[2] == EP_STATE_ENABLE &&
	    idle_duration && frame_count > idle_duration * 4 &&
	    !interrupt_busy &&
	    devreq == DEVICE_REQUEST_NONE &&
	    wait_state != WAIT_STATE_DATA_DEVICE) {
		devreq = DEVICE_REQUEST_READ;
		devreq_id = 18;
	}
}

static void rx_packet(int ep_id, bool setup)
{
	if (ep_id == 0) {
		if (setup)
			/* Device request */
			control_setup();
		else
			/* OUT data */
			control_rx();
	}
}

static void tx_packet(int ep_id)
{
	if (ep_id == 0)
		/* IN data */
		control_tx();
	else {
		/* Clear interrupt. */
		usbdevfs_clear_endpoint_interrupt(ep_id);
		interrupt_int++;
		interrupt_busy = false;
	}
}

static void usb_reset(void)
{
	/* Disable TIM9. */
	tim_disable_counter(TIM9);

	/* Disable DMA. */
	dma_disable(DMA_ADC);
	dma_running = false;

	/* Reset buffer allocator. */
	init_buf();

	/* Clear buffer. */
	tx_buf = 0;

	/* Set endpoint type and address. */
	usbdevfs_setup_endpoint(0, USBDEVFS_CONTROL, 0);
	usbdevfs_setup_endpoint(1, USBDEVFS_ISOCHRONOUS, AS_ENUM);
	usbdevfs_setup_endpoint(2, USBDEVFS_INTERRUPT, INTERRUPT_ENUM);

	/* Set default address. */
	usbdevfs_set_device_address(0);

	/* Reset control transfer state. */
	control_reset();
}

/* USB (low priority) interrupt */
void usb_lp_isr(void)
{
	u16 mask;
	u16 status;
	int ep_id;
	u16 trans;

	/* Interrupt mask */
	mask = usbdevfs_get_interrupt_mask(USBDEVFS_CORRECT_TRANSFER |
					   USBDEVFS_ERROR | USBDEVFS_RESET |
					   USBDEVFS_SOF);
	if (!mask)
		return;

	/* Interrupt status */
	status = usbdevfs_get_interrupt_status(USBDEVFS_CORRECT_TRANSFER |
					       USBDEVFS_ERROR |
					       USBDEVFS_RESET |
					       USBDEVFS_SOF |
					       USBDEVFS_DIR | USBDEVFS_EP_ID);
	if (!(status & (USBDEVFS_CORRECT_TRANSFER | USBDEVFS_ERROR |
			USBDEVFS_RESET | USBDEVFS_SOF)))
		return;

	/* Start of frame */
	if (mask & status & USBDEVFS_SOF) {
		sof();
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_SOF);
	}

	/* Correct transfer */
	if (mask & status & USBDEVFS_CORRECT_TRANSFER) {
		ep_id = status & USBDEVFS_EP_ID;
		trans = usbdevfs_get_ep_status(ep_id, USBDEVFS_RX |
					       USBDEVFS_TX | USBDEVFS_SETUP);

		/* Rx (OUT/SETUP transaction) */
		if ((status & USBDEVFS_DIR) && (trans & USBDEVFS_RX))
			rx_packet(ep_id, trans & USBDEVFS_SETUP);

		/* Tx (IN transaction) */
		if (!(status & USBDEVFS_DIR) && (trans & USBDEVFS_TX))
			tx_packet(ep_id);
	}

	/* Error */
	if (mask & status & USBDEVFS_ERROR) {
		usb_error++;
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_ERROR);
	}

	/* USB RESET */
	if (mask & status & USBDEVFS_RESET) {
		usb_reset();
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_RESET);
	}
}

static int i2c_setup(void)
{
	int i;

	/* Set nRST high. */
	gpio_set(GPIO_PB8);

	/* Set SDIO high. */
	gpio_set(GPIO_PB11);

	/* Enable I2C2 clock. */
	rcc_enable_clock(RCC_I2C2);

	/* Enable I2C2 event interrupt. */
	nvic_enable_irq(NVIC_I2C2_EV_IRQ);

	/* Enable I2C2 error interrupt. */
	nvic_enable_irq(NVIC_I2C2_ER_IRQ);

	/* Set GPIO10 and GPIO11 (in GPIO port B) to 'altfn open-drain'. */
	gpio_config_altfn(GPIO_I2C1_2, GPIO_OPENDRAIN, GPIO_2MHZ, GPIO_NOPUPD,
			  GPIO_PB(I2C2_SCL, I2C2_SDA));

	/* Reset I2C. */
	if (i2c_get_status(I2C2, I2C_BUSY)) {
		i2c_enable_action(I2C2, I2C_RESET);
		for (i = 0; i < BUSY_TIMEOUT &&
			     i2c_get_status(I2C2, I2C_BUSY); i++)
			;
		i2c_disable_action(I2C2, I2C_RESET);
		if (i >= BUSY_TIMEOUT) {
			i2c_timeout = true;
			return -1;
		}
	}

	/* Set I2C clock frequency. */
	i2c_set_clock(I2C2, PCLK1, MODE, SCL, T_R_MAX);

	/* Enable I2C. */
	i2c_set_bus_mode(I2C2, I2C_ENABLE);

	return 0;
}

/* I2C Event interrupt */
void i2c2_ev_isr(void)
{
	u32 e;

	if (i2c_get_interrupt_mask(I2C2, I2C_EVENT) &&
	    (e = i2c_get_interrupt_status(I2C2, I2C_TXE | I2C_RXNE |
					  I2C_BTF | I2C_ADDR | I2C_START))) {
		switch (e) {
		case I2C_START: /* EV5 */
			/* Send I2C slave address. */
			if (sndbuflen > 0)
				i2c_put_data(I2C2, SI4703_SLA);
			else
				i2c_put_data(I2C2, SI4703_SLA | I2C_READ);
			/* Stop timer. */
			stop_timer();
			break;

		case I2C_ADDR: /* EV6 */
		case (I2C_TXE | I2C_ADDR): /* EV8_1 */
			if (sndbuflen <= 0) {
				if (rcvbuflen > 2)
					/* Enable Acknowledge. */
					i2c_enable_action(I2C2, I2C_ACK);
				else if (rcvbuflen == 2)
					/* Disable Acknowledge. */
					i2c_enable_action(I2C2, I2C_POS);
			}

			/* Clear ADDR bit. */
			i2c_get_status(I2C2, 0);

			/* Enable buffer interrupt */
			if (sndbuflen > 0)
				i2c_enable_interrupt(I2C2, I2C_BUFFER);
			break;

		case (I2C_RXNE | I2C_BTF): /* EV7, EV7_1 */
			if (rcvbuflen > 3) {
				/* Receive data. */
				rcvbuf[rcvindex++] = i2c_get_data(I2C2);
				rcvbuflen--;
			} else if (rcvbuflen == 3) {
				/* Disable Acknowledge. */
				i2c_disable_action(I2C2, I2C_ACK);

				/* Receive data. */
				rcvbuf[rcvindex++] = i2c_get_data(I2C2);
				rcvbuflen--;
			} else { /* rcvbuflen == 2) */
				/* Generate Stop condition. */
				i2c_enable_action(I2C2, I2C_STOP);

				/* Clear POS bit. */
				i2c_disable_action(I2C2, I2C_POS);

				/* Receive data. */
				rcvbuf[rcvindex++] = i2c_get_data(I2C2);
				rcvbuflen--;
				rcvbuf[rcvindex++] = i2c_get_data(I2C2);
				rcvbuflen--;

				/* Disable interrupt. */
				i2c_disable_interrupt(I2C2, I2C_BUFFER |
						      I2C_EVENT | I2C_ERROR);
				/* Complete transfer. */
				i2c_busy = false;
			}
			break;

		case I2C_TXE: /* EV8, EV8_1 */
		case (I2C_TXE | I2C_BTF): /* EV8_2 */
			if (sndbuflen > 0) {
				/* Send data. */
				i2c_put_data(I2C2, sndbuf[sndindex++]);
				sndbuflen--;

				if (sndbuflen == 0)
					/* Disable buffer interrupt */
					i2c_disable_interrupt(I2C2, I2C_BUFFER);
			} else {
				/* Generate Stop condition. */
				i2c_enable_action(I2C2, I2C_STOP);

				/* Disable interrupt. */
				i2c_disable_interrupt(I2C2, I2C_BUFFER |
						      I2C_EVENT | I2C_ERROR);

				/* Complete transfer. */
				i2c_busy = false;
			}
			break;

		default:	/* unknown event */
			/* Disable interrupt. */
			i2c_disable_interrupt(I2C2, I2C_BUFFER |
					      I2C_EVENT | I2C_ERROR);
			/* Stop timer. */
			stop_timer();

			/* Error */
			i2c_status = true;
			i2c_busy = false;
			break;
		}
	}
}

/* I2C Error interrupt */
void i2c2_er_isr(void)
{
	if (i2c_get_interrupt_mask(I2C2, I2C_ERROR) &&
	    i2c_get_interrupt_status(I2C2, I2C_ERROR)) {
		/* Disable interrupt. */
		i2c_disable_interrupt(I2C2, I2C_BUFFER |
				      I2C_EVENT | I2C_ERROR);

		/* Stop timer. */
		stop_timer();

		/* Error */
		i2c_status = true;
		i2c_busy = false;
	}
}

static int si4703_transfer(void)
{
	int i;

	/* Set flag. */
	i2c_busy = true;

	/* Enable interrupt. */
	i2c_enable_interrupt(I2C2, I2C_EVENT | I2C_ERROR);

	/* Start timer. */
	start_timer(START_TIMEOUT);

	/* Generate Start condition. */
	i2c_enable_action(I2C2, I2C_START);

	/* Wait until the completion of the transfer. */
	while (i2c_busy)
		;

	if (i2c_status) {
		/*
		printf("ERROR: I2C_SR1 = 0x%04x\r\n",
		       (unsigned int)i2c_get_interrupt_status(I2C2, 0xffff));
		*/
		return -1;
	}

	if (i2c_timeout) {
		/*printf("Timeout: No Start condition\r\n");*/
		return -1;
	}

	/* Wait until STOP bit is cleared. */
	for (i = 0; i < TIMEOUT && i2c_get_action_status(I2C2, I2C_STOP); i++)
		;
	if (i >= TIMEOUT) {
		/*
		printf("STOP Timeout: I2C_CR1 = 0x%04x\r\n",
		       (unsigned int)i2c_get_action_status(I2C2, 0xffff));
		*/
		i2c_timeout = true;
		return -1;
	}

	delay_us(T_BUF);

	return 0;
}

static int si4703_write(int reg_num, u16 data)
{
	int i;
	int j;

	si4703_reg[reg_num] = data;
	j = 0;
	for (i = 2; i != reg_num; i == MAXREG - 1 ? i = 0 : i++) {
		sndbuf[j++] = si4703_reg[i] >> 8;
		sndbuf[j++] = si4703_reg[i] & 0xff;
	}
	sndbuf[j++] = si4703_reg[i] >> 8;
	sndbuf[j++] = si4703_reg[i] & 0xff;
	sndbuflen = j;
	sndindex = 0;
	rcvbuflen = 0;
	rcvindex = 0;

	return si4703_transfer();
}

static int si4703_read(int reg_num)
{
	int i;
	int j;
	int r;

	rcvbuflen = 2;
	for (i = 0x0a; i != reg_num; i == MAXREG - 1 ? i = 0 : i++)
		rcvbuflen += 2;
	rcvindex = 0;
	sndbuflen = 0;
	sndindex = 0;

	r = si4703_transfer();
	if (r)
		return r;

	j = 0;
	for (i = 0x0a; i != reg_num; i == MAXREG - 1 ? i = 0 : i++) {
		si4703_reg[i] = rcvbuf[j++] << 8;
		si4703_reg[i] |= rcvbuf[j++];
	}
	si4703_reg[i] = rcvbuf[j++] << 8;
	si4703_reg[i] |= rcvbuf[j];

	return si4703_reg[reg_num];
}

static int si4703_init(void)
{
	int r;

	/* Read all registers. */
	if ((r = si4703_read(9)) < 0)
		return r;

	/* Set the XOSCEN bit. */
	if ((r = si4703_write(7, 0x8100)) < 0)
		return r;

	/* Wait for crystal to power up. */
	delay_ms(500);

	/* Set RDSD = 0x0000 (Si4703-C19 Erratta Solution 2). */
	if ((r = si4703_write(15, 0x0000)) < 0)
		return r;

	/* Set the ENABLE bit high. */
	if ((r = si4703_write(2, 0x4001)) < 0)
		return r;

	/* Wait for device power up. */
	delay_ms(T_POWERUP);

	/* Read all registers. */
	if ((r = si4703_read(9)) < 0)
		return r;

	return 0;
}

static void send_interrupt(void)
{
	int i;
	u8 buf[13] __attribute__ ((aligned(2)));

	if (endpoint_state[2] == EP_STATE_ENABLE &&
	    idle_duration && frame_count > idle_duration * 4) {
		buf[0] = 18;	/* Report ID */
		for (i = 0; i < 6; i++) {
			buf[i * 2 + 1] = si4703_reg[10 + i] >> 8;
			buf[i * 2 + 2] = si4703_reg[10 + i] & 0xff;
		}
		usbdevfs_write(2, (u16 *)buf, sizeof(buf));
		interrupt_busy = true;

		/* Enable endpoint. */
		usbdevfs_start_endpoint_tx(2);

		frame_count = 0;
		interrupt_send++;
	}
}

int main(void)
{
	int regnum;
	u16 data;
	int i;
	int r;

	clock_setup();
	gpio_setup();
	tim_setup();
	adc_setup();
	dma_setup();
	exti_setup();
	usb_setup();

	delay_us(T_GSRST1);

	if (i2c_setup()) {
		/*printf("i2c_setup: Bus busy\r\n");*/
		goto usb_radio_error;
	}

	if ((r = si4703_init()) < 0) {
		goto usb_radio_error;
	}

	/* Attach the device to USB. */
	syscfg_enable_usb_pullup();

	/* Clear interrupt. */
	usbdevfs_clear_interrupt(USBDEVFS_ALL_INTERRUPT);

	/* Enable interrupt. */
	usbdevfs_enable_interrupt(USBDEVFS_CORRECT_TRANSFER | USBDEVFS_ERROR |
				  USBDEVFS_RESET | USBDEVFS_SOF);

	while (1) {
		if (devreq != DEVICE_REQUEST_NONE) {
			regnum = devreq_id - 1;
			if (devreq == DEVICE_REQUEST_READ) {
				if (regnum < 16) {
					/* Read a register. */
					if (si4703_read(regnum) < 0)
						goto usb_radio_error;
				} else if (regnum == 16) {
					/* Read all registers. */
					if (si4703_read(9) < 0)
						goto usb_radio_error;
				} else { /* regnum == 17 */
					/* Read 0x0A - 0x0F registers. */
					if (si4703_read(15) < 0)
						goto usb_radio_error;
				}
			} else if (devreq == DEVICE_REQUEST_WRITE) {
				if (regnum < 16) {
					/* Write a register. */
					data = devreq_reg[regnum];
					if (si4703_write(regnum, data) < 0)
						goto usb_radio_error;
				} else if (regnum == 16) {
					/* Write all registers. */
					for (i = 0; i < 16; i++)
						si4703_reg[i] =
							devreq_reg[i];
					data = devreq_reg[1];
					if (si4703_write(1, data) < 0)
						goto usb_radio_error;
				}
			}

			/* Disable USB low priority interrupt. */
			nvic_disable_irq(NVIC_USB_LP_IRQ);

			if (devreq == DEVICE_REQUEST_READ && devreq_id == 18)
				send_interrupt();

			devreq = DEVICE_REQUEST_NONE;
			control_restart();

			/* Enable USB low priority interrupt. */
			nvic_enable_irq(NVIC_USB_LP_IRQ);
		}

		if (adc_overrun)
			goto usb_radio_error;
	}

usb_radio_error:
	while (1) {
		/* LED on */
		gpio_clear(GPIO_PE10);
		if (i2c_status) {
			delay_ms(200);
			/* LED off */
			gpio_set(GPIO_PE10);
			delay_ms(200);
		} else if (i2c_timeout) {
			delay_ms(500);
			/* LED off */
			gpio_set(GPIO_PE10);
			delay_ms(500);
		}
	}

	return 0;
}
