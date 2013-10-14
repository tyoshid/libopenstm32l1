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

#define TIMX_CLK_APB1 32000000

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

	/* PE10: TIM2 OC2, PE11: TIM2 OC3 */
	gpio_config_altfn(GPIO_TIM2, GPIO_PUSHPULL, GPIO_2MHZ, GPIO_NOPUPD,
			  GPIO_PE(TIM2_CH2, TIM2_CH3));
}

static void tim_setup(void)
{
	/* Enable TIM2 clock. */
	rcc_enable_clock(RCC_TIM2);

	/* Load prescaler value (100kHz). */
	// TIM2_PSC = TIMX_CLK_APB1 / 100000 - 1;
	// TIM2_EGR = TIM_EGR_UG;

	/* Set auto-reload value (256). */
	// TIM2_ARR = 256 - 1;

	tim_setup_counter(TIM2, TIMX_CLK_APB1 / 100000 - 1, 256 - 1);

	/* OC2: PWM mode 1, OC3: PWM mode 2 */
	// TIM2_CCMR1 = (TIM_CCMR1_OC2M_PWM1 | TIM_CCMR1_OC2PE);
	// TIM2_CCMR2 = (TIM_CCMR2_OC3M_PWM2 | TIM_CCMR2_OC3PE);

	/* Enable OC3 and OC2 output. */
	// TIM2_CCER = (TIM_CCER_CC3E | TIM_CCER_CC2E);
	tim_set_capture_compare_mode(TIM2_CC2, TIM_OC_PRELOAD | TIM_OC_PWM1 |
				     TIM_CC_ENABLE);
	tim_set_capture_compare_mode(TIM2_CC3, TIM_OC_PRELOAD | TIM_OC_PWM2 |
				     TIM_CC_ENABLE);
	tim_start_capture_compare(TIM2);

	/* Enable counter. */
	// TIM2_CR1 |= TIM_CR1_CEN;
	tim_enable_counter(TIM2);
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
			/* Set channel 2 (PE10). */
			// TIM2_CCR2 = j;
			tim_set_capture_compare_value(TIM2_CC2, j);

			/* Set channel 3 (PE11). */
			// TIM2_CCR3 = j;
			tim_set_capture_compare_value(TIM2_CC3, j);

			/* Wait a bit. */
			for (i = 0; i < 80000; i++)
				__asm__ ("nop");
		}
		/* Decrement */
		for (j = 255; j >= 0; j--) {
			/* Set channel 2 (PE10). */
			// TIM2_CCR2 = j;
			tim_set_capture_compare_value(TIM2_CC2, j);

			/* Set channel 3 (PE11). */
			// TIM2_CCR3 = j;
			tim_set_capture_compare_value(TIM2_CC3, j);

			/* Wait a bit. */
			for (i = 0; i < 80000; i++)
				__asm__ ("nop");
		}
	}

	return 0;
}
