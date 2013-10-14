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

	/* PB6: TIM4 OC1, PB7: TIM4 OC2 */
	gpio_config_altfn(GPIO_TIM3_4, GPIO_PUSHPULL, GPIO_2MHZ, GPIO_NOPUPD,
			  GPIO_PB(TIM4_CH1, TIM4_CH2));
}

static void tim_setup(void)
{
	/* Enable TIM4 clock. */
	rcc_enable_clock(RCC_TIM4);

	/* Load prescaler value (100kHz). */
	// TIM4_PSC = TIMX_CLK_APB1 / 100000 - 1;
	// TIM4_EGR = TIM_EGR_UG;

	/* Set auto-reload value (256). */
	// TIM4_ARR = 256 - 1;

	tim_setup_counter(TIM4, TIMX_CLK_APB1 / 100000 - 1, 256 - 1);

	/* OC2: PWM mode 1, OC1: PWM mode 2 */
	// TIM4_CCMR1 = (TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE |
	//		 TIM_CCMR1_OC1M_PWM2 | TIM_CCMR1_OC1PE);

	/* Enable OC2 and OC1 output. */
	// TIM4_CCER = (TIM_CCER_CC2E | TIM_CCER_CC1E);
	tim_set_capture_compare_mode(TIM4_CC2, TIM_OC_PRELOAD | TIM_OC_PWM1 |
				     TIM_CC_ENABLE);
	tim_set_capture_compare_mode(TIM4_CC1, TIM_OC_PRELOAD | TIM_OC_PWM2 |
				     TIM_CC_ENABLE);
	tim_start_capture_compare(TIM4);

	/* Enable counter. */
	// TIM_CR1(TIM4) |= TIM_CR1_CEN;
	tim_enable_counter(TIM4);
}

int main(void)
{
	int i, j;

	clock_setup();
	gpio_setup();
	tim_setup();

	while (1) {
		/* Increment */
		for (j = 0; j < 256; j++) {
			/* Set channel 1 (PB6). */
			// TIM4_CCR1 = j;
			tim_set_capture_compare_value(TIM4_CC1, j);

			/* Set channel 2 (PB7). */
			// TIM4_CCR2 = j;
			tim_set_capture_compare_value(TIM4_CC2, j);

			/* Wait a bit. */
			for (i = 0; i < 80000; i++)
				__asm__ ("nop");
		}
		/* Decrement */
		for (j = 255; j >= 0; j--) {
			/* Set channel 1 (PB6). */
			// TIM4_CCR1 = j;
			tim_set_capture_compare_value(TIM4_CC1, j);

			/* Set channel 2 (PB7). */
			// TIM4_CCR2 = j;
			tim_set_capture_compare_value(TIM4_CC2, j);

			/* Wait a bit. */
			for (i = 0; i < 80000; i++)
				__asm__ ("nop");
		}
	}

	return 0;
}
