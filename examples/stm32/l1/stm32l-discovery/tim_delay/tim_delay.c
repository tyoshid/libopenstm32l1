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

#define TIMX_CLK_APB1 16000000

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

	/* Set GPIO6 and GPIO7 (in GPIO port B) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PB(6, 7));

	/* LED off */
	gpio_clear(GPIO_PB(6, 7));
}

static void tim_setup(void)
{
	/* Enable TIM6 and TIM7 clock. */
	rcc_enable_clock(RCC_TIM6);
	rcc_enable_clock(RCC_TIM7);

	/* Enable one-pulse mode. */
	/* Generate update interrupt on counter overflow. */
	// TIM6_CR1 |= (TIM_CR1_OPM | TIM_CR1_URS);
	// TIM7_CR1 |= (TIM_CR1_OPM | TIM_CR1_URS);
	tim_enable_one_pulse_mode(TIM6);
	tim_disable_update_interrupt_on_any(TIM6);
	tim_enable_one_pulse_mode(TIM7);
	tim_disable_update_interrupt_on_any(TIM7);

	/* Load prescaler value (2MHz). */
	// TIM6_PSC = TIMX_CLK_APB1 / 2000000 - 1;
	// TIM6_EGR = TIM_EGR_UG;
	tim_load_prescaler_value(TIM6, TIMX_CLK_APB1 / 2000000 - 1);

	/* Load prescaler value (2kHz). */
	// TIM7_PSC = TIMX_CLK_APB1 / 2000 - 1;
	// TIM7_EGR = TIM_EGR_UG;
	tim_load_prescaler_value(TIM7, TIMX_CLK_APB1 / 2000 - 1);
}

/* 1 - 32767 usec */
static void delay_us(u16 us)
{
	/* Manually: */
	/* Set auto-reload value (us * 2). */
	// TIM6_ARR = (us << 1) - 1;

	/* Enable counter. */
	// TIM6_CR1 |= TIM_CR1_CEN;
	/* Wait for update interrupt flag. */
	// while (!(TIM6_SR & TIM_SR_UIF))
	//	;
	/* Clear update interrupt flag. */
	// TIM6_SR &= ~TIM_SR_UIF;

	/* Using API functions: */
	tim_set_autoreload_value(TIM6, (us << 1) - 1);

	tim_enable_counter(TIM6);
	while (!tim_get_interrupt_status(TIM6, TIM_UPDATE))
		;
	tim_clear_interrupt(TIM6, TIM_UPDATE);
}

/* 1 - 32767 msec */
static void delay_ms(u16 ms)
{
	/* Manually: */
	/* Set auto-reload value (ms * 2). */
	// TIM7_ARR = (ms << 1) - 1;

	/* Enable counter. */
	// TIM7_CR1 |= TIM_CR1_CEN;
	/* Wait for update interrupt flag. */
	// while (!(TIM7_SR & TIM_SR_UIF))
	//	;
	/* Clear update interrupt flag. */
	// TIM7_SR &= ~TIM_SR_UIF;

	/* Using API functions: */
	tim_set_autoreload_value(TIM7, (ms << 1) - 1);

	tim_enable_counter(TIM7);
	while (!tim_get_interrupt_status(TIM7, TIM_UPDATE))
		;
	tim_clear_interrupt(TIM7, TIM_UPDATE);
}

int main(void)
{
	int i;
	int j;

	clock_setup();
	gpio_setup();
	tim_setup();

	while (1) {
		for (i = 0; i < 10; i++) {
			gpio_toggle(GPIO_PB6); /* LED(PB6) on/off */
			delay_ms(1000);
		}
		for (i = 0; i < 10; i++) {
			gpio_toggle(GPIO_PB7); /* LED(PB7) on/off */
			for (j = 0; j < 100; j++)
				delay_us(10000);
		}
	}

	return 0;
}
