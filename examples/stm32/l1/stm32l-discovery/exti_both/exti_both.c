/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 *               2010 Piotr Esden-Tempski <piotr@esden.net>
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
#include <nvic.h>
#include <syscfg.h>
#include <exti.h>

/* Set STM32 to 16 MHz. */
static void clock_setup(void)
{
	/* Enable PWR clock. */
	rcc_enable_clock(RCC_PWR);

	/* Set VCORE to 1.8V */
	pwr_set_vos(PWR_1_8_V);

	/* Set Flash memory latency (0WS). */
	flash_enable_64bit_access(0);

	/* Enable internal high-speed oscillator (16MHz). */
	rcc_enable_osc(RCC_HSI);

	/* AHB, APB1 and APB2 prescaler value is default */
	// rcc_set_prescaler(1, 1, 1);

	/* Select SYSCLK source. */
	rcc_set_sysclk_source(RCC_HSI);
}

static void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	rcc_enable_clock(RCC_GPIOB);

	/* Set GPIO7 (in GPIO port B) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD, GPIO_PB7);

	/* LED(PB7) on */
	gpio_set(GPIO_PB7);
}

static void exti_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_enable_clock(RCC_GPIOA);

	/* Enable SYSCFG clcok. */
	rcc_enable_clock(RCC_SYSCFG);

	/* Enable EXTI0 interrupt. */
	nvic_enable_irq(NVIC_EXTI0_IRQ);

	/* Set GPIO0 (in GPIO port A) to 'input float'. */
	gpio_config_input(GPIO_FLOAT, GPIO_PA0);

	/* Connect PA0 to EXTI0 */
	syscfg_select_exti_source(EXTI0, SYSCFG_PA);

	/* Set EXTI0 to both edge trigger. */
	exti_set_trigger(EXTI0, EXTI_BOTH);

	/* Enable interrupt. */
	exti_enable_interrupt(EXTI0);
}

void exti0_isr(void)
{
	if (exti_get_interrupt_mask(EXTI0) &&
	    exti_get_interrupt_status(EXTI0)) {
		/*
		 * The LED (PB7) is on, but turns off when the button is
		 * pressed.
		 */
		if (gpio_get(GPIO_PA0) == 0)
			gpio_set(GPIO_PB7); /* LED on */
		else
			gpio_clear(GPIO_PB7); /* LED off */

		/* Clear interrupt. */
		exti_clear_interrupt(EXTI0);
	}
}

int main(void)
{
	clock_setup();
	gpio_setup();
	exti_setup();

	while (1)
		__asm__ ("nop");

	return 0;
}
