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
#include <usart.h>

/* USART clock frequency */
#define PCLK1	32000000
#define PCLK2	32000000

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
}

static void gpio_setup(void)
{
	/* Enable GPIOE clock. */
	rcc_enable_clock(RCC_GPIOE);

	/* Set GPIO10 (in GPIO port E) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD, GPIO_PE10);
}

static void usart_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_enable_clock(RCC_GPIOA);

	/* Enable USART1 clock. */
	rcc_enable_clock(RCC_USART1);

	/* Setup GPIO pin PA9. */
	gpio_config_altfn(GPIO_USART1_3, GPIO_PUSHPULL, GPIO_2MHZ, GPIO_NOPUPD,
			  GPIO_PA_USART1_TX);

	/* Setup USART1. */
	usart_init(USART1, PCLK2, 38400, 8, USART_STOP_1,
		   USART_PARITY_NONE, USART_FLOW_NONE, USART_TX);

	/* Enable GPIOD clock. */
	rcc_enable_clock(RCC_GPIOD);

	/* Enable USART2 clock. */
	rcc_enable_clock(RCC_USART2);

	/* Setup GPIO pin PD5. */
	gpio_config_altfn(GPIO_USART1_3, GPIO_PUSHPULL, GPIO_2MHZ, GPIO_NOPUPD,
			  GPIO_PD_USART2_TX);

	/* Setup USART2. */
	usart_init(USART2, PCLK1, 38400, 8, USART_STOP_1,
		   USART_PARITY_NONE, USART_FLOW_NONE, USART_TX);

	/* Enable GPIOC clock. */
	rcc_enable_clock(RCC_GPIOC);

	/* Enable USART3 clock. */
	rcc_enable_clock(RCC_USART3);

	/* Setup GPIO pin PC10. */
	gpio_config_altfn(GPIO_USART1_3, GPIO_PUSHPULL, GPIO_2MHZ, GPIO_NOPUPD,
			  GPIO_PC_USART3_TX);

	/* Setup USART3. */
	usart_init(USART3, PCLK1, 38400, 8, USART_STOP_1,
		   USART_PARITY_NONE, USART_FLOW_NONE, USART_TX);
}

int main(void)
{
	int i, j = 0, c = 0;

	clock_setup();
	gpio_setup();
	usart_setup();

	/* Blink the LED (PE10) on the board with every transmitted byte. */
	while (1) {
		gpio_toggle(GPIO_PE10);			/* LED on/off */
		usart_send_blocking(USART1, c + '0');	/* USART1: Send byte. */
		usart_send_blocking(USART2, c + '0');	/* USART2: Send byte. */
		usart_send_blocking(USART3, c + '0');	/* USART3: Send byte. */
		c = (c == 9) ? 0 : c + 1;	/* Increment c. */
		if ((j++ % 80) == 0) {	/* Newline after line full. */
			usart_send_blocking(USART1, '\r');
			usart_send_blocking(USART1, '\n');
			usart_send_blocking(USART2, '\r');
			usart_send_blocking(USART2, '\n');
			usart_send_blocking(USART3, '\r');
			usart_send_blocking(USART3, '\n');
		}
		for (i = 0; i < 800000; i++)	/* Wait a bit. */
			__asm__ ("nop");
	}

	return 0;
}
