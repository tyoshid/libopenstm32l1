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
#include <spi.h>
#include <nvic.h>
#include <dac.h>
#include <dma.h>
#include <syscfg.h>
#include <usbdevfs.h>

#include "usb_speaker.h"
#include "control.h"

extern u8 *outp;
extern int outlen;

/* Timer clock frequency */
#define TIMX_CLK_APB1		32000000

/* Packet buffer memory */
#define BUFFER_TABLE_ADDRESS	0
#define PACKET_MEMORY_START	(BUFFER_TABLE_ADDRESS + \
				 USBDEVFS_BUFFER_TABLE_SIZE)

/* Maximum queue size */
#define QUEUESIZE		4

/* Rx queue */
static u32 queue[QUEUESIZE][AS_SIZE / sizeof(u32)];
static volatile int head;
static volatile int tail;
static volatile int data_len[QUEUESIZE]; /* Reception word count */

/* Statistics */
static u32 usb_error;
static u32 short_packet;

/* Set STM32 to 32 MHz. */
static void clock_setup(void)
{
	/* Enable PWR clock. */
	rcc_enable_clock(RCC_PWR);

	/* Set VCORE to 1.8V */
	pwr_set_vos(PWR_1_8_V);

	/* Enable 64bit flash memory access (1WS). */
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
}

static void gpio_setup(void)
{
	/* GPIOE clock (for LED). */
	rcc_enable_clock(RCC_GPIOE);

	/* Setup GPIO10 and GPIO11 (in GPIO port E) for LED use. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PE(10, 11));

	/* LED off */
	gpio_set(GPIO_PE(10, 11));
}

static void tim_setup(void)
{
	/* Enable TIM6 and TIM7 clock. */
	rcc_enable_clock(RCC_TIM6);
	rcc_enable_clock(RCC_TIM7);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM6);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM6);

	/* Load prescaler value (2MHz). */
	tim_load_prescaler_value(TIM6, TIMX_CLK_APB1 / 2000000 - 1);

	/* Set Auto-Reload value (prescaler = 0). */
	tim_set_autoreload_value(TIM7, TIMX_CLK_APB1 / SAMPLING_FREQ - 1);

	/* Enable update DMA request. */
	tim_enable_dma(TIM7, TIM_DMA_UPDATE);
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

/* DS1801(Dual Audio Taper Potentiometer) control */
/* Maxim Integrated Products (Dallas Semiconductor) */
/*
 * nRST:	PB11
 * CLK:		SPI2_SCK(PB13)
 * D:		SPI2_MOSI(PB15)
 */

static void spi_setup(void)
{
	/* Enable GPIOB clock. */
	rcc_enable_clock(RCC_GPIOB);

	/* 'nRST' LOW */
	gpio_clear(GPIO_PB11);

	/* Set GPIO11 (in GPIO port B) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_40MHZ, GPIO_NOPUPD, GPIO_PB11);

	/* Enable SPI2 clock. */
	rcc_enable_clock(RCC_SPI2);

	/* Set GPIO13, 15 (in GPIO port B) to 'altfn push-pull'. */
	gpio_config_altfn(GPIO_SPI1_2, GPIO_PUSHPULL, GPIO_40MHZ, GPIO_NOPUPD,
			  GPIO_PB(SPI2_SCK, SPI2_MOSI));

	/* Set SPI mode (32 / 4 = 8MHz). */
	spi_set_mode(SPI2, 4, SPI_16BIT | SPI_LSB_FIRST | SPI_NSS_SOFTWARE |
		     SPI_NSS_HIGH | SPI_MASTER | SPI_ENABLE);
}

void volume_set(s16 ch1, s16 ch2)
{
	/* Check value. */
	if (ch1 > MAX_VOLUME)
		ch1 = MAX_VOLUME;
	if (ch1 < MUTE_VOLUME)
		ch1 = MUTE_VOLUME;

	if (ch2 > MAX_VOLUME)
		ch2 = MAX_VOLUME;
	if (ch2 < MUTE_VOLUME)
		ch2 = MUTE_VOLUME;

	/* nRST = High */
	gpio_set(GPIO_PB11);

	/* Wait for TX buffer empty. */
	while (!spi_get_interrupt_status(SPI2, SPI_TXE))
		;

	/* Send 16bit data. */
	spi_send(SPI2, (-ch1 / 256) | -ch2);

	/* Wait for the end of transfer. */
	while (spi_get_interrupt_status(SPI2, SPI_BUSY))
		;

	/* nRST = Low */
	gpio_clear(GPIO_PB11);
}

static void dac_setup(void)
{
	/* Enable DAC clock. */
	rcc_enable_clock(RCC_DAC);

	/* Enable GPIOA clock. */
	rcc_enable_clock(RCC_GPIOA);

	/* Setup PA4 and PA5 for DAC use. */
	gpio_config_analog(GPIO_PA(4, 5));

	/* Enable DAC channel1 and channel2. */
	dac_enable(DAC_DUAL);

	/* Wait tWAKEUP. */
	delay_us(DAC_T_WAKEUP);
}

static void dma_setup(void)
{
	/* Enable DMA1 clock. */
	rcc_enable_clock(DMA_RCC_TIM7_UP);

	/* Enable DMA1 channel3 interrupt. */
	nvic_enable_irq(DMA_TIM7_UP_IRQ);
}

/* DMA1 (channel3) interrupt */
void dma_tim7_up_isr(void)
{
	/* Interrupt mask */
	if (!dma_get_interrupt_mask(DMA_TIM7_UP, DMA_COMPLETE))
		return;

	/* Interrupt status */
	if (dma_get_interrupt_status(DMA_TIM7_UP, DMA_GLOBAL | DMA_COMPLETE) !=
	    (DMA_GLOBAL | DMA_COMPLETE))
		return;

	/* Disable TIM7. */
	tim_disable_counter(TIM7);

	/* Disable DMA. */
	dma_disable(DMA_TIM7_UP);

	/* Dequeue. */
	head = (head + 1) % QUEUESIZE;

	/* Clear interrupt. */
	dma_clear_interrupt(DMA_TIM7_UP, DMA_COMPLETE | DMA_GLOBAL);
}

static void usb_setup(void)
{
	int offset;

	/* Enable USB and SYSCFG clock. */
	rcc_enable_clock(RCC_USB);
	rcc_enable_clock(RCC_SYSCFG);

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
	offset = usbdevfs_assign_packet_memory_rx0(1, offset, AS_SIZE);
	offset = usbdevfs_assign_packet_memory_rx1(1, offset, AS_SIZE);
}

/* USB (high priority) interrupt */
void usb_hp_isr(void)
{
	u16 status;
	u16 mask;
	int ep_id;
	u16 trans;
	int n;
	int i;
	u32 *p;
	u32 tmp;
	static int count;

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
	trans = usbdevfs_get_ep_status(ep_id, USBDEVFS_RX | USBDEVFS_RX_DATA1);

	/* Isochronous endpoint Rx (OUT transaction) */
	if (ep_id != 1 || !(trans & USBDEVFS_RX))
		return;

	/* Read data from packet buffer. */
	if (trans & USBDEVFS_RX_DATA1)
		n = usbdevfs_read0(ep_id, (u16 *)queue[tail], AS_SIZE);
	else
		n = usbdevfs_read1(ep_id, (u16 *)queue[tail], AS_SIZE);

	/* too short packet (data error ?) */
	if (n < AS_SIZE)
		short_packet++;

	if (n >= (int)sizeof(u32)) {
		/* -32768, 32767 => 0, 0xfff0 */
		p = queue[tail];
		for (i = 0; i < n; i += 4) {
			tmp = *p;
			*p++ = (tmp & 0xfff0fff0) ^ 0x80008000;
		}

		/* Data length (word count) */
		data_len[tail] = n / sizeof(u32);

		/* Disable DMA1 channel3 interrupt. */
		nvic_disable_irq(DMA_TIM7_UP_IRQ);

		/* Enqueue. */
		if ((tail + 1) % QUEUESIZE != head) {
			tail = (tail + 1) % QUEUESIZE;

			/* LED ON/OFF */
			if (++count == 100) {
				gpio_toggle(GPIO_PE11);
				count = 0;
			}
		}

		/* Enable DMA1 channel3 interrupt. */
		nvic_enable_irq(DMA_TIM7_UP_IRQ);
	}

	/* Clear interrupt. */
	usbdevfs_clear_endpoint_interrupt(1);
}

static void sof(void)
{
	if (head != tail) {
		/* Set DMA mode and enable DMA. */
		dma_setup_channel(DMA_TIM7_UP,
				  (u32)queue[head], (u32)&DAC_DHR12LD,
				  data_len[head],
				  DMA_M_32BIT | DMA_P_32BIT |
				  DMA_M_INC | DMA_M_TO_P |
				  DMA_COMPLETE | DMA_ENABLE);

		/* Set counter. */
		tim_set_counter(TIM7,
				TIMX_CLK_APB1 / SAMPLING_FREQ - 1);

		/* Enable counter. */
		tim_enable_counter(TIM7);
	}
}

static void rx_packet(int ep_id, bool setup)
{
	if (ep_id != 0)
		return;

	if (setup)
		/* Device request */
		control_setup();
	else
		/* OUT data */
		control_rx();
}

static void tx_packet(int ep_id)
{
	if (ep_id == 0)
		/* IN data */
		control_tx();
}

static void usb_reset(void)
{
	/* Disable TIM7. */
	tim_disable_counter(TIM7);

	/* Disable DMA. */
	dma_disable(DMA_TIM7_UP);

	/* Clear queue. */
	head = tail = 0;

	/* Set endpoint type and address. */
	usbdevfs_setup_endpoint(0, USBDEVFS_CONTROL, 0);
	usbdevfs_setup_endpoint(1, USBDEVFS_ISOCHRONOUS, AS_ENUM);

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

int main(void)
{
	clock_setup();
	gpio_setup();
	tim_setup();
	spi_setup();
	dac_setup();
	dma_setup();
	usb_setup();

	/* Attach the device to USB. */
	syscfg_enable_usb_pullup();

	/* Clear interrupt. */
	usbdevfs_clear_interrupt(USBDEVFS_ALL_INTERRUPT);

	/* Enable interrupt. */
	usbdevfs_enable_interrupt(USBDEVFS_CORRECT_TRANSFER | USBDEVFS_ERROR |
				  USBDEVFS_RESET | USBDEVFS_SOF);

	/* Wait forever and do nothing. */
	while (1)
		__asm__ ("nop");

	return 0;
}
