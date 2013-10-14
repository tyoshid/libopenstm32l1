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
#include <syscfg.h>
#include <usbdevfs.h>
#include <nvic.h>
#include <exti.h>
#include <scb.h>
#include <dbgmcu.h>
#include <desig.h>

#include <usb/standard.h>

#include "usb_custom_irq_dfu.h"
#include "control.h"

/* Timer clock frequency */
#define TIMX_CLK_APB1		32000000

/* Packet buffer memory */
#define BUFFER_TABLE_ADDRESS	0
#define PACKET_MEMORY_START	(BUFFER_TABLE_ADDRESS + \
				 USBDEVFS_BUFFER_TABLE_SIZE)

/* statistics */
unsigned int usb_overrun;
unsigned int usb_error;

/* GPIOE data */
u16 led_state;

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

	/* Disable internal multispeed oscillator. */
	rcc_disable_osc(RCC_MSI);

	/* Set the frequency range of MSI to 65 kHz. */
	/* (for low power run mode) */
	rcc_set_msi_range(RCC_MSI_65KHZ);

	/* Enable internal multispeed oscillator */
	rcc_enable_osc(RCC_MSI);
}

static void gpio_setup(void)
{
	/* Enable GPIO clock. */
	rcc_enable_clock(RCC_GPIOA);
	rcc_enable_clock(RCC_GPIOB);
	rcc_enable_clock(RCC_GPIOC);
	rcc_enable_clock(RCC_GPIOD);
	rcc_enable_clock(RCC_GPIOE);

	/* Set unused ports to analog input */
	gpio_config_analog(GPIO_PA(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 14, 15));
	gpio_config_analog(GPIO_PB(0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13,
				   14, 15));
	gpio_config_analog(GPIO_PC(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
				   13));
	gpio_config_analog(GPIO_PD_ALL);
	gpio_config_analog(GPIO_PE(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14,
				   15));

	/* Setup GPIO10 and GPIO11 (in GPIO port E) for LED use. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PE(10, 11));

	/* Set GPIO0 (in GPIO port A) to 'input float'. */
	gpio_config_input(GPIO_FLOAT, GPIO_PA0);
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

	/* Enable USB Device FS Wakeup interrupt. */
	nvic_enable_irq(NVIC_USB_FS_WKUP_IRQ);

	/* EXTI18 is connected to USB Device FS wakeup event. */
	exti_set_trigger(EXTI_USB_WAKEUP, EXTI_RISING);

	/* Enable EXTI interrupt. */
	exti_enable_interrupt(EXTI_USB_WAKEUP);

	/* Exit power down. */
	usbdevfs_disable_function(USBDEVFS_POWER_DOWN);

	/* Wait T_STATUP. */
	delay_us(USBDEVFS_T_STARTUP);

	/* Clear USB reset. */
	usbdevfs_disable_function(USBDEVFS_FORCE_RESET);

	/* Set buffer table address */
	usbdevfs_set_buffer_table_address(BUFFER_TABLE_ADDRESS);

	/* Assign packet memory to endpoint0 */
	offset = usbdevfs_assign_packet_memory_tx(0, PACKET_MEMORY_START,
						  MAXPACKETSIZE0);
	usbdevfs_assign_packet_memory_rx(0, offset, MAXPACKETSIZE0);
}

static void exti_setup(void)
{
	/* Enable EXTI0 interrupt. */
	nvic_enable_irq(NVIC_EXTI0_IRQ);

	/* Connect PA0 to EXTI0 */
	syscfg_select_exti_source(EXTI0, SYSCFG_PA);

	/* Set EXTI0 to rising edge trigger. */
	exti_set_trigger(EXTI0, EXTI_RISING);
}

/* USB wakeup interrupt */
void usb_fs_wkup_isr(void)
{
	if (exti_get_interrupt_mask(EXTI_USB_WAKEUP) &&
	    exti_get_interrupt_status(EXTI_USB_WAKEUP)) {
		if (usbdevfs_get_interrupt_mask(USBDEVFS_WAKEUP) &&
		    usbdevfs_get_interrupt_status(USBDEVFS_WAKEUP)) {
			/* Run mode */
			pwr_set_run_mode();
			pwr_wait_for_regulator_main_mode();
			scb_set_sleep(0); /* Reset SLEEPDEEP bit. */

			/* SYSCLK 65kHz -> 32MHz */
			pwr_set_vos(PWR_1_8_V);
			flash_enable_64bit_access(1);
			rcc_enable_osc(RCC_HSE);
			rcc_enable_osc(RCC_PLL);
			rcc_set_sysclk_source(RCC_PLL);

			/* Disable resume request. */
			usbdevfs_disable_function(USBDEVFS_RESUME);

			/* Restore LED state. */
			gpio_port_write(GPIOE, led_state);

			/* Exit low-power mode. */
			usbdevfs_disable_function(USBDEVFS_LOW_POWER_MODE);

			/* Exit suspend mode. */
			usbdevfs_disable_function(USBDEVFS_FORCE_SUSPEND);

			/* Clear interrupt. */
			usbdevfs_clear_interrupt(USBDEVFS_WAKEUP);
		}
		/* Clear EXTI18 interrupt. */
		exti_clear_interrupt(EXTI_USB_WAKEUP);
	}
}

static void usb_reset(void)
{
	/* Disable button interrupt. */
	exti_disable_interrupt(EXTI0);

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
					   USBDEVFS_OVERRUN | USBDEVFS_ERROR |
					   USBDEVFS_SUSPEND | USBDEVFS_RESET);
	if (!mask)
		return;

	/* Interrupt status */
	status = usbdevfs_get_interrupt_status(USBDEVFS_CORRECT_TRANSFER |
					       USBDEVFS_OVERRUN |
					       USBDEVFS_ERROR |
					       USBDEVFS_SUSPEND |
					       USBDEVFS_RESET |
					       USBDEVFS_DIR | USBDEVFS_EP_ID);
	if (!(status & (USBDEVFS_CORRECT_TRANSFER | USBDEVFS_OVERRUN |
			USBDEVFS_ERROR | USBDEVFS_SUSPEND | USBDEVFS_RESET)))
		return;

	/* Correct transfer */
	if (mask & status & USBDEVFS_CORRECT_TRANSFER) {
		ep_id = status & USBDEVFS_EP_ID;
		trans = usbdevfs_get_ep_status(ep_id, USBDEVFS_RX |
					       USBDEVFS_TX | USBDEVFS_SETUP);
		/* Rx (OUT/SETUP transaction) */
		if ((status & USBDEVFS_DIR) && (trans & USBDEVFS_RX)) {
			if (trans & USBDEVFS_SETUP)
				control_setup();
			else
				control_rx();
		}
		/* Tx (IN transaction) */
		if (!(status & USBDEVFS_DIR) && (trans & USBDEVFS_TX))
			control_tx();
	}

	/* Packet memory area over/underrun */
	if (mask & status & USBDEVFS_OVERRUN) {
		usb_overrun++;
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_OVERRUN);
	}

	/* Error */
	if (mask & status & USBDEVFS_ERROR) {
		usb_error++;
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_ERROR);
	}

	/* Suspend mode request */
	/* (I can not decrease average current less than 500 uA.) */
	if (mask & status & USBDEVFS_SUSPEND) {
		/* Enter suspend mode. */
		usbdevfs_enable_function(USBDEVFS_FORCE_SUSPEND);

		/* Enter Low-power mode. */
		usbdevfs_enable_function(USBDEVFS_LOW_POWER_MODE);

		/* Save LED state. */
		led_state = gpio_get(GPIO_PE(11, 10));

		/* LED off */
		gpio_set(GPIO_PE(11, 10));

		/* Clear EXTI pending bits */
		exti_clear_interrupt(0xffffff);

		/* Enable button interrupt. */
		if (device_status & USB_DEV_STATUS_REMOTE_WAKEUP)
			exti_enable_interrupt(EXTI0);

		/* Slow down clock (32MHz -> 65kHz) */
		rcc_set_sysclk_source(RCC_MSI);
		flash_disable_64bit_access();

		/* VCORE range 2 */
		pwr_set_vos(PWR_1_5_V);

		/* Stop mode */
		scb_set_sleep(SCB_SCR_SLEEPDEEP);
		pwr_set_stop_mode();

		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_SUSPEND);
	}

	/* USB RESET */
	if (mask & status & USBDEVFS_RESET) {
		usb_reset();
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_RESET);
	}
}

void exti0_isr(void)
{
	if (exti_get_interrupt_mask(EXTI0) &&
	    exti_get_interrupt_status(EXTI0)) {
		/* Enable resume request. */
		usbdevfs_enable_function(USBDEVFS_RESUME);

		/* Disable interrupt. */
		exti_disable_interrupt(EXTI0);

		/* Clear interrupt. */
		exti_clear_interrupt(EXTI0);
	}
}

int main(void)
{
	u16 dev_id;
	u32 uid[3];

	clock_setup();
	gpio_setup();
	tim_setup();
	usb_setup();
	exti_setup();

	/* Set serial number (unique device ID). */
	dev_id = dbgmcu_get_device_id() & DBGMCU_IDCODE_DEV_ID_MASK;
	desig_get_unique_id(dev_id, uid);
	set_serial_number(uid);

	/* Attach the device to USB. */
	syscfg_enable_usb_pullup();

	/* Clear interrupt. */
	usbdevfs_clear_interrupt(USBDEVFS_ALL_INTERRUPT);

	/* Enable interrupt. */
	usbdevfs_enable_interrupt(USBDEVFS_CORRECT_TRANSFER |
				  USBDEVFS_OVERRUN | USBDEVFS_ERROR |
				  USBDEVFS_WAKEUP | USBDEVFS_SUSPEND |
				  USBDEVFS_RESET);

	/* Sleep */
	while (1)
		__asm__ ("wfi");

	return 0;
}
