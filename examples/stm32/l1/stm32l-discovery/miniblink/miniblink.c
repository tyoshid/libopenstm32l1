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
#include <gpio.h>

static void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	/* Manually: */
	// RCC_AHBENR |= RCC_AHBENR_GPIOBEN;
	/* Using API functions: */
	rcc_enable_clock(RCC_GPIOB);

	/* Set GPIO6 (in GPIO port B) to 'output push-pull'. */
	/* Manually: */
	// GPIOB_MODER = (GPIO_MODER_OUTPUT << (6 * 2));
	// GPIOB_OTYPER = (GPIO_OTYPER_PUSHPULL << 6);
	// GPIOB_OSPEEDR = (GPIO_OSPEEDR_400_KHZ << (6 * 2));
	// GPIOB_PUPDR = (GPIO_PUPDR_NONE << (6 * 2));
	/* Using API functions: */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD, GPIO_PB6);
}

int main(void)
{
	int i;

	gpio_setup();

	/* Blink the LED (PB6) (Blue) on the board. */
	while (1) {
		/* Manually: */
		// GPIOB_BSRR = GPIO6;		/* LED on */
		// for (i = 0; i < 800000; i++)	/* Wait a bit. */
		//	__asm__ ("nop");
		// GPIOB_BSRR = (GPIO6 << 16);	/* LED off */
		// for (i = 0; i < 800000; i++)	/* Wait a bit. */
		//	__asm__ ("nop");

		/* Using API functions gpio_set()/gpio_clear(): */
		// gpio_set(GPIO_PB6);	/* LED on */
		// for (i = 0; i < 800000; i++)	/* Wait a bit. */
		//	__asm__ ("nop");
		// gpio_clear(GPIO_PB6);	/* LED off */
		// for (i = 0; i < 800000; i++)	/* Wait a bit. */
		//	__asm__ ("nop");

		/* Using API function gpio_toggle(): */
		gpio_toggle(GPIO_PB6);	/* LED on/off */
		for (i = 0; i < 800000; i++)	/* Wait a bit. */
			__asm__ ("nop");
	}

	return 0;
}
