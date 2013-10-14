/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
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
#include <usart.h>
#include <syscfg.h>
#include <nvic.h>
#include <usbdevfs.h>
#include <dbgmcu.h>
#include <desig.h>

#include "usb_cdcacm.h"
#include "control.h"
#include "interrupt.h"
#include "bulk.h"

/* Timer clock frequency */
#define TIMX_CLK_APB1		32000000
#define TIMX_CLK_APB2		32000000

/* USART clock frequency */
#define PCLK1			32000000

/* Packet buffer memory */
#define BUFFER_TABLE_ADDRESS	0
#define PACKET_MEMORY_START	(BUFFER_TABLE_ADDRESS + \
				 USBDEVFS_BUFFER_TABLE_SIZE)

/* USART Tx queue */
u8 tx_queue[TXQUEUESIZE];
volatile int tx_head;
volatile int tx_tail;

/* USART Rx queue */
u8 rx_queue[RXQUEUESIZE];
volatile int rx_head;
volatile int rx_tail;

/* USART parameter */
static int data_size;
static int parity;
bool rts_enable;
bool cts_enable;

/* USART error status */
volatile u32 serial_status;

/* V.25 ter command state */
volatile bool command_state = true;

/* Escape sequence */
volatile bool escape_timer_enable;
volatile escape_state_t escape_state;

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
	/* Enable TIM6 clock. */
	rcc_enable_clock(RCC_TIM6);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM6);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM6);

	/* Load prescaler value (2MHz). */
	tim_load_prescaler_value(TIM6, TIMX_CLK_APB1 / 2000000 - 1);

	/* Enable TIM7 clock. */
	rcc_enable_clock(RCC_TIM7);

	/* Enable TIM7 interrupt. */
	nvic_enable_irq(NVIC_TIM7_IRQ);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM7);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM7);

	/* Enable update interrupt */
	tim_enable_interrupt(TIM7, TIM_UPDATE);

	/* Enable TIM9 clock. */
	rcc_enable_clock(RCC_TIM9);

	/* Enable TIM9 interrupt. */
	nvic_enable_irq(NVIC_TIM9_IRQ);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM9);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM9);

	/* Enable update interrupt */
	tim_enable_interrupt(TIM9, TIM_UPDATE);
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

void tim7_isr(void)
{
	if (tim_get_interrupt_mask(TIM7, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM7, TIM_UPDATE)) {
		/* Stop break signal. */
		gpio_set_mode(GPIO_ALTFN, GPIO_PD_USART2_TX);

		/* Clear interrupt.*/
		tim_clear_interrupt(TIM7, TIM_UPDATE);
	}
}

void tim9_isr(void)
{
	bool empty;

	if (tim_get_interrupt_mask(TIM9, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM9, TIM_UPDATE)) {
		/* Disable USB interrupt. */
		nvic_disable_irq(NVIC_USB_LP_IRQ);

		/* Clear flag. */
		escape_timer_enable = false;

		if (escape_state == ESCAPE_STATE3) {
			/* Enter command state. */
			command_state = true;

			/* Print "OK". */
			bulk_puts(0, "\r\nOK\r\n");

			/* Reset state. */
			escape_state = ESCAPE_STATE0;
		} else if (escape_state != ESCAPE_STATE0) {
			/* Timeout */

			/* Empty? */
			empty = (tx_head == tx_tail);

			/* Enqueue escape characters. */
			bulk_escape_timeout();

			if (empty)
				/* Enable USART Tx interrupt */
				usart_enable_interrupt(USART2, USART_TXE);

			/* Reset state. */
			escape_state = ESCAPE_STATE0;
		}

		/* Enable USB interrupt. */
		nvic_enable_irq(NVIC_USB_LP_IRQ);

		/* Clear interrupt.*/
		tim_clear_interrupt(TIM9, TIM_UPDATE);
	}
}

void start_escape_timer(void)
{
	/* Initialize timer (1 sec). */
	tim_setup_counter(TIM9, TIMX_CLK_APB2 / 1000 - 1, 1000 - 1);

	/* Enable counter. */
	tim_enable_counter(TIM9);

	/* Set flag. */
	escape_timer_enable = true;
}

static void usart_setup(void)
{
	/* Enable GPIOD clock for GPIOD_USART2_TX, RX, CTS, RTS */
	rcc_enable_clock(RCC_GPIOD);

	/* Enable USART2 clock. */
	rcc_enable_clock(RCC_USART2);

	/* Enable the USART2 interrupt. */
	nvic_enable_irq(NVIC_USART2_IRQ);

	/* Setup GPIO pin PD3-6 as alternate function. */
	gpio_config_altfn(GPIO_USART1_3, GPIO_PUSHPULL, GPIO_10MHZ,
			  GPIO_NOPUPD, GPIO_PD(USART2_CTS, USART2_RTS,
					       USART2_TX, USART2_RX));

	/* Enable USART2 Receive interrupt. */
	usart_enable_interrupt(USART2, USART_RXNE);
}

static void usart_put_data(u8 data)
{
	u16 d;

	if (data_size != 8 && data_size != 9)
		return;

	d = data;
	if (parity == 3) {
		/* Mark */
		if (data_size == 9)
			d |= 0x100;
		else
			d |= 0x80;
	} else if (parity == 4) {
		/* Space */
		if (data_size == 9)
			d &= ~0x100;
		else
			d &= ~0x80;
	}
	usart_send(USART2, d);
}

static u8 usart_get_data(void)
{
	u16 d;
	u8 data;
	bool pe = false;

	d = usart_recv(USART2);

	if (parity == 3) {
		/* Mark */
		if (data_size == 9) {
			if (!(d & 0x100))
				pe = true;
		} else {
			if (!(d & 0x80))
				pe = true;
		}
	} else if (parity == 4) {
		/* Space */
		if (data_size == 9) {
			if (d & 0x100)
				pe = true;
		} else {
			if (d & 0x80)
				pe = true;
		}
	}

	/* Parity error */
	if (pe)
		serial_status |= USART_PE;

	/* Strip parity bit. */
	data = d;
	if (data_size == 8 && parity != 0)
		data &= 0x7f;

	return data;
}

void usart2_isr(void)
{
	u32 mask;
	u32 status;
	bool empty;

	/* Check sprious. */
	if (!(mask =
	      usart_get_interrupt_mask(USART2, USART_RXNE | USART_TXE)) ||
	    !(status =
	      usart_get_interrupt_status(USART2, USART_RXNE | USART_TXE |
					 USART_FE | USART_PE | USART_ORE)))
		return;

	/* Check if we were called because of RXNE. */
	if (mask & status & USART_RXNE) {
		/* Error status */
		if (status & (USART_FE | USART_PE | USART_ORE))
			serial_status |= status;

		/* Indicate that we got data. */
		gpio_toggle(GPIO_PE10);

		/* Disable USB interrupt. */
		nvic_disable_irq(NVIC_USB_LP_IRQ);

		/* Retrieve the data from the peripheral. */
		rx_queue[rx_tail++] = usart_get_data();
		rx_tail %= RXQUEUESIZE;

		if (!command_state)
			/* Send data. */
			bulk_tx();

		/* Full? */
		if ((rx_tail + 1) % RXQUEUESIZE == rx_head)
			/* Disable the RXNE interrupt */
			usart_disable_interrupt(USART2, USART_RXNE);

		/* Notification */
		if (serial_status)
			interrupt_notify();

		/* Enable USB interrupt. */
		nvic_enable_irq(NVIC_USB_LP_IRQ);
	}

	/* Check if we were called because of TXE. */
	if (mask & status & USART_TXE) {
		/* Disable USB interrupt. */
		nvic_disable_irq(NVIC_USB_LP_IRQ);

		/* Disable escape timer interrupt. */
		nvic_disable_irq(NVIC_TIM9_IRQ);

		/* Empty? */
		empty = (tx_head == tx_tail);

		if (!empty) {
			/* Indicate that we are sending out data. */
			gpio_toggle(GPIO_PE11);

			/* Put data into the transmit register. */
			usart_put_data(tx_queue[tx_head++]);
			tx_head %= TXQUEUESIZE;
		}

		/* Enqueue */
		bulk_rx_data_cont();

		if (empty && tx_head != tx_tail) {
			/* Indicate that we are sending out data. */
			gpio_toggle(GPIO_PE11);

			/* Put data into the transmit register. */
			usart_put_data(tx_queue[tx_head++]);
			tx_head %= TXQUEUESIZE;
		}

		if (tx_head == tx_tail)
			/* Disable the TXE interrupt */
			usart_disable_interrupt(USART2, USART_TXE);

		/* Enable escape timer interrupt. */
		nvic_enable_irq(NVIC_TIM9_IRQ);

		/* Enable USB interrupt. */
		nvic_enable_irq(NVIC_USB_LP_IRQ);
	}
}

void usart_set_param(int b, int d, int s, int p)
{
	usart_stop_t stop;
	usart_flowcontrol_t flow;

	data_size = d;
	parity = p;

	switch (s) {
	case 0:
		stop = USART_STOP_1;
		break;
	case 1:
		stop = USART_STOP_1_5;
		break;
	case 2:
	default:
		stop = USART_STOP_2;
		break;
	}

	if (rts_enable) {
		if (cts_enable)
			flow = USART_RTS_CTS;
		else
			flow = USART_FLOW_RTS;
	} else {
		if (cts_enable)
			flow = USART_FLOW_CTS;
		else
			flow = USART_FLOW_NONE;
	}

	usart_init(USART2, PCLK1, b, d, stop, p, flow, USART_TX_RX);
}

void usart_set_flow(bool rts, bool cts)
{
	usart_flowcontrol_t flow;

	rts_enable = rts;
	cts_enable = cts;

	if (rts_enable) {
		if (cts_enable)
			flow = USART_RTS_CTS;
		else
			flow = USART_FLOW_RTS;
	} else {
		if (cts_enable)
			flow = USART_FLOW_CTS;
		else
			flow = USART_FLOW_NONE;
	}

	usart_set_flow_control(USART2, flow);
}

void usart_start_break(u16 period)
{
	/* Break signale (Tx = 0). */
	gpio_set_mode(GPIO_OUTPUT, GPIO_PD_USART2_TX);

	/* Infinity */
	if (period == 0xffff)
		return;

	/* Set timer. */
	if (period == 1)
		tim_setup_counter(TIM7, TIMX_CLK_APB1 / 2000 - 1, 2 - 1);
	else
		tim_setup_counter(TIM7, TIMX_CLK_APB1 / 1000 - 1, period - 1);

	/* Enable counter. */
	tim_enable_counter(TIM7);
}

void usart_stop_break(void)
{
	/* Stop break signal. */
	gpio_set_mode(GPIO_ALTFN, GPIO_PD_USART2_TX);
}

static void usb_setup(void)
{
	int offset;

	/* Enable USB and SYSCFG clock. */
	rcc_enable_clock(RCC_USB);
	rcc_enable_clock(RCC_SYSCFG);

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
	offset = usbdevfs_assign_packet_memory_tx(1, offset, NOTIFICATION_SIZE);
	offset = usbdevfs_assign_packet_memory_rx(2, offset, DATA_SIZE);
	offset = usbdevfs_assign_packet_memory_tx(3, offset, DATA_SIZE);
}

static void rx_packet(int ep_id, bool setup)
{
	bool empty;

	if (ep_id == 0) {
		if (setup) {
			/* Device request */
			control_setup();

			/* Notification (if it exists) */
			interrupt_notify();
		} else {
			/* OUT data */
			control_rx();
		}
	} else if (ep_id == 2) {
		if (command_state) {
			/* Receive command. */
			bulk_rx_command();
		} else {
			/* Empty? */
			empty = (tx_head == tx_tail);

			/* Enqueue data. */
			bulk_rx_data();

			if (empty)
				/* Enable USART Tx interrupt */
				usart_enable_interrupt(USART2, USART_TXE);
		}
	}
}

static void tx_packet(int ep_id)
{
	bool full;

	switch (ep_id) {
	case 0:
		control_tx();
		break;
	case 1:
		/* Clear interrupt. */
		usbdevfs_clear_endpoint_interrupt(1);

		/* Notification (if it exists) */
		interrupt_notify_cont();
		break;
	case 3:
		/* Clear interrupt. */
		usbdevfs_clear_endpoint_interrupt(3);

		if (command_state) {
			/* Send command. */
			bulk_puts_cont();
		} else {
			/* Full? */
			full = ((rx_tail + 1) % RXQUEUESIZE == rx_head);

			/* Dequeue data. */
			bulk_tx_cont();

			if (full)
				/* Enable USART Rx interrupt */
				usart_enable_interrupt(USART2, USART_RXNE);
		}
		break;
	default:
		break;
	}
}

static void usb_reset(void)
{
	/* Disable TIM7. */
	tim_disable_counter(TIM7);

	/* Disable TIM9. */
	tim_disable_counter(TIM9);

	/* Disable USART */
	usart_disable(USART2);

	/* Stop break signal. */
	gpio_set_mode(GPIO_ALTFN, GPIO_PD_USART2_TX);

	/* Clear queue */
	tx_head = 0;
	tx_tail = 0;
	rx_head = 0;
	rx_tail = 0;

	/* Clear USART error status. */
	serial_status = 0;

	/* Disable flow control. */
	rts_enable = false;
	cts_enable = false;

	/* Clear escape state. */
	escape_timer_enable = false;
	escape_state = ESCAPE_STATE0;

	/* Enable USART2 Receive interrupt. */
	usart_enable_interrupt(USART2, USART_RXNE);

	/* Set endpoint type and address. */
	usbdevfs_setup_endpoint(0, USBDEVFS_CONTROL, 0);
	usbdevfs_setup_endpoint(1, USBDEVFS_INTERRUPT, NOTIFICATION_ENUM);
	usbdevfs_setup_endpoint(2, USBDEVFS_BULK, DATA_RX_ENUM);
	usbdevfs_setup_endpoint(3, USBDEVFS_BULK, DATA_TX_ENUM);

	/* Set default address. */
	usbdevfs_set_device_address(0);

	/* Reset control transfer state. */
	control_reset();

	/* Reset interrupt transfer state. */
	interrupt_reset();

	/* Reset bulk transfer state. */
	bulk_reset();
}

/* USB (low priority) interrupt */
void usb_lp_isr(void)
{
	u16 mask;
	u16 status;
	int ep_id;
	u16 trans;

	mask = usbdevfs_get_interrupt_mask(USBDEVFS_CORRECT_TRANSFER |
					   USBDEVFS_RESET);
	if (!mask)
		return;

	status = usbdevfs_get_interrupt_status(USBDEVFS_CORRECT_TRANSFER |
					       USBDEVFS_RESET |
					       USBDEVFS_DIR | USBDEVFS_EP_ID);
	if (!(status & (USBDEVFS_CORRECT_TRANSFER | USBDEVFS_RESET)))
	    return;

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

	/* USB RESET */
	if (mask & status & USBDEVFS_RESET) {
		usb_reset();
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_RESET);
	}
}

int main(void)
{
	u16 dev_id;
	u32 uid[3];

	clock_setup();
	gpio_setup();
	tim_setup();
	usart_setup();
	usb_setup();

	/* Set serial number (unique device ID). */
	dev_id = dbgmcu_get_device_id() & DBGMCU_IDCODE_DEV_ID_MASK;
	desig_get_unique_id(dev_id, uid);
	set_serial_number(uid);

	/* Attach the device to USB. */
	syscfg_enable_usb_pullup();

	/* Clear interrupt. */
	usbdevfs_clear_interrupt(USBDEVFS_ALL_INTERRUPT);

	/* Enable interrupt. */
	usbdevfs_enable_interrupt(USBDEVFS_CORRECT_TRANSFER | USBDEVFS_RESET);

	/* Wait forever and do nothing. */
	while (1)
		__asm__ ("nop");

	return 0;
}
