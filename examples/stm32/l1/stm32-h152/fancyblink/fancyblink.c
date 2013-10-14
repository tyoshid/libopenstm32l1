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

/* Set STM32 to 32 MHz. */
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
	flash_enable_64bit_access(1);

	/* Enable external high-speed oscillator 8MHz. */
	rcc_enable_osc(RCC_HSE);

	/*
	 * Set the PLL multiplication factor to 12.
	 * 8MHz (external) * 12 (multiplier) = 96MHz
	 * 96MHz / 3 = 32MHz
	 * Select HSE as PLL source.
	 * APB2 32MHz (Max. 32MHz)
	 * APB1 32MHz (Max. 32MHz)
	 * AHB  32MHz (Max. 32MHz)
	 */
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
	/* Enable GPIOE clock. */
	rcc_enable_clock(RCC_GPIOE);

	/* Set PE10 and PE11 to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PE(10, 11));

	/* LED off */
	gpio_set(GPIO_PE10);

	/* LED on */
	gpio_clear(GPIO_PE11);
}

int main(void)
{
	int i;

	clock_setup();
	gpio_setup();

	/* Blink the LED (PE10 and PE11) on the board. */
	while (1) {
		/* LED on/off */
		gpio_toggle(GPIO_PE(10, 11));

		/* Wait a bit. */
		for (i = 0; i < 800000; i++)
			__asm__ ("nop");
	}

	return 0;
}
