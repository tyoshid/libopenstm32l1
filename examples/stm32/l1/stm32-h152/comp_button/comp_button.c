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
#include <exti.h>
#include <nvic.h>
#include <ri.h>
#include <comp.h>

#define TIMX_CLK_APB1	32000000

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
	/* Enable GPIOA and GPIOE clock. */
	rcc_enable_clock(RCC_GPIOA);
	rcc_enable_clock(RCC_GPIOE);

	/* Set GPIO0 (in GPIO port A) to 'analog'. */
	gpio_config_analog(GPIO_PA0);

	/* Set GPIO10 (in GPIO port E) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD, GPIO_PE10);

	/* LED off */
	gpio_set(GPIO_PE10);
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

static void exti_setup(void)
{
	/* Enable COMP interrupt. */
	nvic_enable_irq(NVIC_COMP_IRQ);

	/* Set EXTI21 to both edge trigger. */
	exti_set_trigger(EXTI_COMP1_WAKEUP, EXTI_BOTH);

	/* Enable interrupt. */
	exti_enable_interrupt(EXTI_COMP1_WAKEUP);
}

void comp_isr(void)
{
	if (exti_get_interrupt_mask(EXTI_COMP1_WAKEUP) &&
	    exti_get_interrupt_status(EXTI_COMP1_WAKEUP)) {
		/*
		 * The LED (PE10) is off, but turns on when the button is
		 * pressed.
		 */
		if (comp_get_status(COMP1_OUT))
			gpio_clear(GPIO_PE10); /* LED on */
		else
			gpio_set(GPIO_PE10); /* LED off */

		/* Clear interrupt. */
		exti_clear_interrupt(EXTI_COMP1_WAKEUP);
	}
}

static void comp_setup(void)
{
	/* Enable COMP (and RI) clock. */
	rcc_enable_clock(RCC_COMP);
}

int main(void)
{
	int as[2];

	clock_setup();
	gpio_setup();
	tim_setup();
	exti_setup();
	comp_setup();

	comp_set_control(COMP1_ENABLE);

	delay_us(COMP1_T_START);

	as[0] = RI_PA0;
	as[1] = RI_ADC | RI_VCOMP;
	ri_close_analog_switch(2, as);

	while (1)
		__asm__ ("nop");

	return 0;
}
