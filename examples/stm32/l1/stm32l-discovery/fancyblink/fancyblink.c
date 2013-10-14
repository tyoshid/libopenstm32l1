/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
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

/* Set STM32 to 16 MHz. */
static void clock_setup(void)
{
	/* Enable PWR clock. */
	rcc_enable_clock(RCC_PWR);

	/* Set VCORE to 1.8V */
	pwr_set_vos(PWR_1_8_V);

	/*
	 * Set Flash memory latency.
	 *
	 *          VCORE=1.8V  VCORE=1.5V  VCORE=1.2V
	 * 0WS from 0-16MHz     0-8MHz      0-2MHz
	 * 1WS from 16-32MHz    8-16MHz     2-4MHz
	 */
	flash_enable_64bit_access(0);

	/* Enable internal high-speed oscillator (16MHz). */
	rcc_enable_osc(RCC_HSI);

	/*
	 * APB2 16MHz (Max. 32MHz)
	 * APB1 16MHz (Max. 32MHz)
	 * AHB  16MHz (Max. 32MHz)
	 */
	/* AHB, APB1 and APB2 prescaler value is default. */
	// rcc_set_prescaler(1, 1, 1);

	/* Select SYSCLK source. */
	rcc_set_sysclk_source(RCC_HSI);
}

static void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	rcc_enable_clock(RCC_GPIOB);

	/* Set PB6 and PB7 to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PB(6, 7));

	/* LED(PB6) on */
	gpio_set(GPIO_PB6);

	/* LED(PB7) off */
	gpio_clear(GPIO_PB7);
}

int main(void)
{
	int i;

	clock_setup();
	gpio_setup();

	/* Blink the LED (PB6 and PB7) on the board. */
	while (1) {
		gpio_toggle(GPIO_PB(6, 7));	/* LED on/off */
		for (i = 0; i < 800000; i++)	/* Wait a bit. */
			__asm__ ("nop");
	}

	return 0;
}
