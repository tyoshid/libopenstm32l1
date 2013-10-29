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
#include <syscfg.h>
#include <nvic.h>
#include <usbdevfs.h>
#include <scb.h>
#include <dbgmcu.h>
#include <desig.h>
#include <wwdg.h>

#include "usb_dfu.h"
#include "control.h"

/* Timer clock frequency */
#define TIMX_CLK_APB1		32000000

/* USB packet buffer memory */
#define BUFFER_TABLE_ADDRESS	0
#define PACKET_MEMORY_START	(BUFFER_TABLE_ADDRESS + \
				 USBDEVFS_BUFFER_TABLE_SIZE)

extern unsigned _stack;

static u32 flash_size;

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
}

static void usb_reset(void)
{
	if (dfu_wait_reset()) {
		gpio_clear(GPIO_PE10);

		/* Enable WWDG clock. */
		rcc_enable_clock(RCC_WWDG);

		/* WWDG reset */
		wwdg_reset(0);
	}

	/* Set endpoint type and address. */
	usbdevfs_setup_endpoint(0, USBDEVFS_CONTROL, 0);

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
					   USBDEVFS_RESET);
	/* Spurious */
	if (!mask)
		return;

	/* Interrupt status */
	status = usbdevfs_get_interrupt_status(USBDEVFS_CORRECT_TRANSFER |
					       USBDEVFS_RESET |
					       USBDEVFS_DIR | USBDEVFS_EP_ID);
	/* Spurious */
	if (!(status & (USBDEVFS_CORRECT_TRANSFER | USBDEVFS_RESET)))
		return;

	/* Correct transfer */
	if (mask & status & USBDEVFS_CORRECT_TRANSFER) {
		/* Endpoint ID (0) */
		ep_id = status & USBDEVFS_EP_ID;

		/* Get endpoint status */
		trans = usbdevfs_get_ep_status(ep_id, USBDEVFS_RX |
					       USBDEVFS_TX | USBDEVFS_SETUP);

		/* Rx (OUT/SETUP transaction) */
		if ((status & USBDEVFS_DIR) && (trans & USBDEVFS_RX)) {
			if (trans & USBDEVFS_SETUP)
				/* SETUP */
				control_setup();
			else
				/* OUT */
				control_rx();
		}

		/* Tx (IN transaction) */
		if (!(status & USBDEVFS_DIR) && (trans & USBDEVFS_TX))
			/* IN */
			control_tx();
	}

	/* USB RESET */
	if (mask & status & USBDEVFS_RESET) {
		/* Reset USB state. */
		usb_reset();

		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_RESET);
	}
}

/* Check address. */
bool program_addr_error(u32 addr)
{
	if (addr < APP_ADDRESS || addr >= FLASH_MEMORY_BASE + flash_size)
		return true;
	return false;
}

int program_time(int n)
{
	return FLASH_T_PROG * n;
}

void program_erase(u32 addr)
{
	gpio_clear(GPIO_PE11);
	flash_unlock_pecr();
	flash_unlock_program_memory();
	flash_erase_page(addr);
	flash_wait_for_last_operation();
	flash_lock_program_memory();
	flash_lock_pecr();
	gpio_set(GPIO_PE11);
}

void program_memory(u32 *d, u32 *s, int nword)
{
	int i;

	gpio_clear(GPIO_PE11);
	flash_unlock_pecr();
	flash_unlock_program_memory();
	for (i = 0; i < nword; i++) {
		*d++ = *s++;
		flash_wait_for_last_operation();
	}
	flash_lock_program_memory();
	flash_lock_pecr();
	gpio_set(GPIO_PE11);
}

int main(void)
{
	bool softreset;
	int i;
	bool button = false;
	u32 sp;
	u32 pc;
	u16 dev_id;
	u32 uid[3];

	/* Check software reset (DFU_DETACH). */
	softreset = rcc_get_reset_flag(RCC_SOFTWARE);

	/* Clear reset flag. */
	rcc_clear_reset_flag();

	if (!softreset) {
		/* Enable GPIOA clock (for button). */
		rcc_enable_clock(RCC_GPIOA);

		/* Wait a bit. */
		for (i = 0; i < 80000; i++)
			__asm__ ("nop");

		/* Get button status. */
		button = gpio_get(GPIO_PA0);

		/* Disable GPIOA clock. */
		rcc_disable_clock(RCC_GPIOA);
	}

	/* Get Flash memory size. */
	dev_id = dbgmcu_get_device_id() & DBGMCU_IDCODE_DEV_ID_MASK;
	flash_size = desig_get_flash_size(dev_id);

	if (!softreset && !button) {
		/* Boot the application if it's valid. */
		sp = *(u32 *)APP_ADDRESS;
		pc = *(u32 *)(APP_ADDRESS + 4);
		if (sp > SRAM_BASE && sp <= (u32)&_stack &&
		    !program_addr_error(pc)) {
			/* Set vector table base address. */
			scb_set_vector_table_offset(APP_ADDRESS);
			/* Jump to application. */
			(*(void (*)(void))pc)();
		}
	}

	/* Start DFU mode. */
	clock_setup();
	gpio_setup();
	tim_setup();
	usb_setup();

	/* Set serial number (unique device ID). */
	desig_get_unique_id(dev_id, uid);
	set_serial_number(uid);

	/* Attach the device to USB. */
	syscfg_enable_usb_pullup();

	/* Clear interrupt. */
	usbdevfs_clear_interrupt(USBDEVFS_ALL_INTERRUPT);

	/* Enable interrupt. */
	usbdevfs_enable_interrupt(USBDEVFS_CORRECT_TRANSFER | USBDEVFS_RESET);

	while (1)
		block_transfer();

	return 0;
}
