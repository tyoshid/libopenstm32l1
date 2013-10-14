/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>,
 *               2011 Piotr Esden-Tempski <piotr@esden.net>
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
#include <usart.h>
#include <tim.h>

#include <syscall.h>
#include <stdio.h>

/* USART clock frequency */
#define PCLK1		32000000

/* Timer clock frequency */
#define TIMX_CLK_APB1	32000000

/* Maximum queue size */
#define QUEUESIZE 	1024

/* Tx, Rx queue */
volatile u8 queue[QUEUESIZE];
volatile int head;
volatile int tail;

/* Flag */
volatile bool wakeup;

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

	/* Setup GPIO10 and 11 (in GPIO port E) for LED use. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_10MHZ, GPIO_NOPUPD,
			   GPIO_PE(10, 11));
}

static void usart_setup(void)
{
	/* Enable GPIOD clock. */
	rcc_enable_clock(RCC_GPIOD);

	/* Enable USART2 clock. */
	rcc_enable_clock(RCC_USART2);

	/* Enable the USART2 interrupt. */
	nvic_enable_irq(NVIC_USART2_IRQ);

	/* Setup GPIO pin PD5 and PD6 as alternate function. */
	gpio_config_altfn(GPIO_USART1_3, GPIO_PUSHPULL, GPIO_10MHZ,
			  GPIO_NOPUPD, GPIO_PD(USART2_TX, USART2_RX));

	/* Enable USART2 Receive interrupt. */
	usart_enable_interrupt(USART2, USART_RXNE);

	/* Setup USART2. */
	usart_init(USART2, PCLK1, 115200, 8, USART_STOP_1,
		   USART_PARITY_NONE, USART_FLOW_NONE, USART_TX_RX);
}

void usart2_isr(void)
{
	u32 mask;
	u32 status;

	/* Check sprious. */
	if (!(mask =
	      usart_get_interrupt_mask(USART2, USART_RXNE | USART_TXE)) ||
	    !(status =
	      usart_get_interrupt_status(USART2, USART_RXNE | USART_TXE)))
		return;

	/* Check if we were called because of RXNE. */
	if (mask & status & USART_RXNE) {
		if ((tail + 1) % QUEUESIZE != head) {
			/* Indicate that we got data. */
			gpio_toggle(GPIO_PE10);

			/* Retrieve the data from the peripheral. */
			queue[tail++] = usart_recv(USART2);

			/* next pointer */
			tail %= QUEUESIZE;

			/* Enable transmit interrupt */
			if (!(mask & USART_TXE))
				usart_enable_interrupt(USART2, USART_TXE);
		} else {	/* overflow */
			/* Clear interrupt. */
			usart_recv(USART2);
		}

	}

	/* Check if we were called because of TXE. */
	if (mask & status & USART_TXE) {
		if (head != tail) {
			/* Indicate that we are sending out data. */
			gpio_toggle(GPIO_PE11);

			/* Put data into the transmit register. */
			usart_send(USART2, queue[head++]);

			/* next pointer */
			head %= QUEUESIZE;
		} else {
			/* Disable the TXE interrupt */
			usart_disable_interrupt(USART2, USART_TXE);
		}
	}
}

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		/* Disable the USART2 interrupt. */
		nvic_disable_irq(NVIC_USART2_IRQ);

		for (i = 0; i < len && (tail + 1) % QUEUESIZE != head; i++) {
			/* Enqueue. */
			queue[tail++] = *ptr++;

			/* next pointer */
			tail %= QUEUESIZE;
		}

		/* Enable transmit interrupt. */
		usart_enable_interrupt(USART2, USART_TXE);

		/* Enable the USART2 interrupt. */
		nvic_enable_irq(NVIC_USART2_IRQ);

		return i;
	}

	errno = EIO;
	return -1;
}

static void tim_setup(void)
{
	/* Enable TIM6 clock. */
	rcc_enable_clock(RCC_TIM6);

	/* Enable TIM6 interrupt. */
	nvic_enable_irq(NVIC_TIM6_IRQ);

	/* Load prescaler value (100kHz) and set auto-reload value (1000). */
	tim_setup_counter(TIM6, TIMX_CLK_APB1 / 100000 - 1, 1000 - 1);

	/* Clear update interrupt. */
	tim_clear_interrupt(TIM6, TIM_UPDATE);

	/* Enable interrupt. */
	tim_enable_interrupt(TIM6, TIM_UPDATE);

	/* Start counter. */
	tim_enable_counter(TIM6);
}

void tim6_isr(void)
{
	if (tim_get_interrupt_mask(TIM6, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM6, TIM_UPDATE)) {
		/* Set flag. */
		wakeup = true;

		/* Clear interrupt. */
		tim_clear_interrupt(TIM6, TIM_UPDATE);
	}
}

int main(void)
{
	int counter = 0;
	float fcounter = 0.0;
	double dcounter = 0.0;

	clock_setup();
	gpio_setup();
	usart_setup();
	tim_setup();

	while (1) {
		if (wakeup) {
			printf("Hello World! %i %f %lf\r\n", counter, fcounter,
			       dcounter);
			counter++;
			fcounter += 0.01;
			dcounter += 0.01;

			/* Clear flag. */
			wakeup = false;
		}
	}

	return 0;
}
