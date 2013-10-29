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
#include <nvic.h>
#include <tim.h>

#define TIMX_CLK_APB1 32000000
#define TIMX_CLK_APB2 32000000

static int isr_count;

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

	/* Set GPIO10 and GPIO11 (in GPIO port E) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PE(10, 11));

	/* LED off */
	gpio_set(GPIO_PE(10, 11));
}

/*
 * Time-base unit
 *
 * 1) Load prescaler value.
 * 2) Set auto-reload value.
 * 3) Enable counter.
 * That's all!
 */
static void tim2_setup(void)
{
	/* Enable TIM2 clock. */
	rcc_enable_clock(RCC_TIM2);

	/* Enable TIM2 interrupt. */
	nvic_enable_irq(NVIC_TIM2_IRQ);

	/* Manually: */
	/* Load prescaler value (2kHz). */
	// TIM2_PSC = TIMX_CLK_APB1 / 2000 - 1;
	// TIM2_EGR = TIM_EGR_UG;

	/* Clear update interrupt. */
	// TIM2_SR &= ~TIM_SR_UIF;

	/* Set auto-reload value (1000). */
	// TIM2_ARR = 1000 - 1;

	/* Using API functions: */
	tim_setup_counter(TIM2, TIMX_CLK_APB1 / 2000 - 1, 1000 - 1);
	tim_clear_interrupt(TIM2, TIM_UPDATE);
}

static void tim3_setup(void)
{
	/* Enable TIM3 clock. */
	rcc_enable_clock(RCC_TIM3);

	/* Enable TIM3 interrupt. */
	nvic_enable_irq(NVIC_TIM3_IRQ);

	/* Load prescaler value (2kHz). */
	// TIM3_PSC = TIMX_CLK_APB1 / 2000 - 1;
	// TIM3_EGR = TIM_EGR_UG;

	/* Clear update interrupt. */
	// TIM3_SR &= ~TIM_SR_UIF;

	/* Set auto-reload value (1000). */
	// TIM3_ARR = 1000 - 1;

	tim_setup_counter(TIM3, TIMX_CLK_APB1 / 2000 - 1, 1000 - 1);
	tim_clear_interrupt(TIM3, TIM_UPDATE);
}

static void tim4_setup(void)
{
	/* Enable TIM4 clock. */
	rcc_enable_clock(RCC_TIM4);

	/* Enable TIM4 interrupt. */
	nvic_enable_irq(NVIC_TIM4_IRQ);

	/* Load prescaler value (2kHz). */
	// TIM4_PSC = TIMX_CLK_APB1 / 2000 - 1;
	// TIM4_EGR = TIM_EGR_UG;

	/* Clear update interrupt. */
	// TIM4_SR &= ~TIM_SR_UIF;

	/* Set auto-reload value (1000). */
	// TIM4_ARR = 1000 - 1;

	tim_setup_counter(TIM4, TIMX_CLK_APB1 / 2000 - 1, 1000 - 1);
	tim_clear_interrupt(TIM4, TIM_UPDATE);
}

static void tim6_setup(void)
{
	/* Enable TIM6 clock. */
	rcc_enable_clock(RCC_TIM6);

	/* Enable TIM6 interrupt. */
	nvic_enable_irq(NVIC_TIM6_IRQ);

	/* Load prescaler value (4kHz). */
	// TIM6_PSC = TIMX_CLK_APB1 / 4000 - 1;
	// TIM6_EGR = TIM_EGR_UG;

	/* Clear update interrupt. */
	// TIM6_SR &= ~TIM_SR_UIF;

	/* Set auto-reload value (1000). */
	// TIM6_ARR = 1000 - 1;

	tim_setup_counter(TIM6, TIMX_CLK_APB1 / 4000 - 1, 1000 - 1);
	tim_clear_interrupt(TIM6, TIM_UPDATE);
}

static void tim7_setup(void)
{
	/* Enable TIM7 clock. */
	rcc_enable_clock(RCC_TIM7);

	/* Enable TIM7 interrupt. */
	nvic_enable_irq(NVIC_TIM7_IRQ);

	/* Load prescaler value (4kHz). */
	// TIM7_PSC = TIMX_CLK_APB1 / 4000 - 1;
	// TIM7_EGR = TIM_EGR_UG;

	/* Clear update interrupt. */
	// TIM7_SR &= ~TIM_SR_UIF;

	/* Set auto-reload value (1000). */
	// TIM7_ARR = 1000 - 1;

	tim_setup_counter(TIM7, TIMX_CLK_APB1 / 4000 - 1, 1000 - 1);
	tim_clear_interrupt(TIM7, TIM_UPDATE);
}

static void tim9_setup(void)
{
	/* Enable TIM9 clock. */
	rcc_enable_clock(RCC_TIM9);

	/* Enable TIM9 interrupt. */
	nvic_enable_irq(NVIC_TIM9_IRQ);

	/* Load prescaler value (1kHz). */
	// TIM9_PSC = TIMX_CLK_APB2 / 1000 - 1;
	// TIM9_EGR = TIM_EGR_UG;

	/* Clear update interrupt. */
	// TIM9_SR &= ~TIM_SR_UIF;

	/* Set auto-reload value (1000). */
	// TIM9_ARR = 1000 - 1;

	tim_setup_counter(TIM9, TIMX_CLK_APB2 / 1000 - 1, 1000 - 1);
	tim_clear_interrupt(TIM9, TIM_UPDATE);
}

static void tim10_setup(void)
{
	/* Enable TIM10 clock. */
	rcc_enable_clock(RCC_TIM10);

	/* Enable TIM10 interrupt. */
	nvic_enable_irq(NVIC_TIM10_IRQ);

	/* Load prescaler value (1kHz). */
	// TIM10_PSC = TIMX_CLK_APB2 / 1000 - 1;
	// TIM10_EGR = TIM_EGR_UG;

	/* Clear update interrupt. */
	// TIM10_SR &= ~TIM_SR_UIF;

	/* Set auto-reload value (1000). */
	// TIM10_ARR = 1000 - 1;

	tim_setup_counter(TIM10, TIMX_CLK_APB2 / 1000 - 1, 1000 - 1);
	tim_clear_interrupt(TIM10, TIM_UPDATE);
}

static void tim11_setup(void)
{
	/* Enable TIM11 clock. */
	rcc_enable_clock(RCC_TIM11);

	/* Enable TIM11 interrupt. */
	nvic_enable_irq(NVIC_TIM11_IRQ);

	/* Load prescaler value (1kHz). */
	// TIM11_PSC = TIMX_CLK_APB2 / 1000 - 1;
	// TIM11_EGR = TIM_EGR_UG;

	/* Clear update interrupt. */
	// TIM11_SR &= ~TIM_SR_UIF;

	/* Set auto-reload value (1000). */
	// TIM11_ARR = 1000 - 1;

	tim_setup_counter(TIM11, TIMX_CLK_APB2 / 1000 - 1, 1000 - 1);
	tim_clear_interrupt(TIM11, TIM_UPDATE);
}

static void tim_setup(void)
{
	tim2_setup();
	tim3_setup();
	tim4_setup();
	tim6_setup();
	tim7_setup();
	tim9_setup();
	tim10_setup();
	tim11_setup();

	/* Enable update interrupt. */
	// TIM2_DIER |= TIM_DIER_UIE;
	tim_enable_interrupt(TIM2, TIM_UPDATE);

	/* Enable counter. */
	// TIM2_CR1 |= TIM_CR1_CEN;
	tim_enable_counter(TIM2);
}

void tim2_isr(void)
{
	// if (TIM2_SR & TIM2_DIER & TIM_SR_UIF) {
	if (tim_get_interrupt_mask(TIM2, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM2, TIM_UPDATE)) {
		gpio_toggle(GPIO_PE10);
		if (++isr_count >= 10) {
			// TIM2_DIER &= ~TIM_DIER_UIE;
			tim_disable_interrupt(TIM2, TIM_UPDATE);
			// TIM2_CR1 &= ~TIM_CR1_CEN;
			tim_disable_counter(TIM2);
			isr_count = 0;
			// TIM3_DIER |= TIM_DIER_UIE;
			tim_enable_interrupt(TIM3, TIM_UPDATE);
			// TIM3_CR1 |= TIM_CR1_CEN;
			tim_enable_counter(TIM3);
		}
		// TIM2_SR &= ~TIM_SR_UIF;
		tim_clear_interrupt(TIM2, TIM_UPDATE);
	}
}

void tim3_isr(void)
{
	//if (TIM3_SR & TIM3_DIER & TIM_SR_UIF) {
	if (tim_get_interrupt_mask(TIM3, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM3, TIM_UPDATE)) {
		gpio_toggle(GPIO_PE11);
		if (++isr_count >= 10) {
			// TIM3_DIER &= ~TIM_DIER_UIE;
			tim_disable_interrupt(TIM3, TIM_UPDATE);
			// TIM3_CR1 &= ~TIM_CR1_CEN;
			tim_disable_counter(TIM3);
			isr_count = 0;
			// TIM4_DIER |= TIM_DIER_UIE;
			tim_enable_interrupt(TIM4, TIM_UPDATE);
			// TIM4_CR1 |= TIM_CR1_CEN;
			tim_enable_counter(TIM4);
		}
		// TIM3_SR &= ~TIM_SR_UIF;
		tim_clear_interrupt(TIM3, TIM_UPDATE);
	}
}

void tim4_isr(void)
{
	// if (TIM4_SR & TIM4_DIER & TIM_SR_UIF) {
	if (tim_get_interrupt_mask(TIM4, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM4, TIM_UPDATE)) {
		gpio_toggle(GPIO_PE(10, 11));
		if (++isr_count >= 10) {
			// TIM4_DIER &= ~TIM_DIER_UIE;
			tim_disable_interrupt(TIM4, TIM_UPDATE);
			// TIM4_CR1 &= ~TIM_CR1_CEN;
			tim_disable_counter(TIM4);
			isr_count = 0;
			// TIM6_DIER |= TIM_DIER_UIE;
			tim_enable_interrupt(TIM6, TIM_UPDATE);
			// TIM6_CR1 |= TIM_CR1_CEN;
			tim_enable_counter(TIM6);
		}
		// TIM4_SR &= ~TIM_SR_UIF;
		tim_clear_interrupt(TIM4, TIM_UPDATE);
	}
}

void tim6_isr(void)
{
	// if (TIM6_SR & TIM6_DIER & TIM_SR_UIF) {
	if (tim_get_interrupt_mask(TIM6, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM6, TIM_UPDATE)) {
		gpio_toggle(GPIO_PE10);
		if (++isr_count >= 10) {
			// TIM6_DIER &= ~TIM_DIER_UIE;
			tim_disable_interrupt(TIM6, TIM_UPDATE);
			// TIM6_CR1 &= ~TIM_CR1_CEN;
			tim_disable_counter(TIM6);
			isr_count = 0;
			// TIM7_DIER |= TIM_DIER_UIE;
			tim_enable_interrupt(TIM7, TIM_UPDATE);
			// TIM7_CR1 |= TIM_CR1_CEN;
			tim_enable_counter(TIM7);
		}
		// TIM6_SR &= ~TIM_SR_UIF;
		tim_clear_interrupt(TIM6, TIM_UPDATE);
	}
}

void tim7_isr(void)
{
	// if (TIM7_SR & TIM7_DIER & TIM_SR_UIF) {
	if (tim_get_interrupt_mask(TIM7, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM7, TIM_UPDATE)) {
		gpio_toggle(GPIO_PE11);
		if (++isr_count >= 10) {
			// TIM7_DIER &= ~TIM_DIER_UIE;
			tim_disable_interrupt(TIM7, TIM_UPDATE);
			// TIM7_CR1 &= ~TIM_CR1_CEN;
			tim_disable_counter(TIM7);
			isr_count = 0;
			// TIM9_DIER |= TIM_DIER_UIE;
			tim_enable_interrupt(TIM9, TIM_UPDATE);
			// TIM9_CR1 |= TIM_CR1_CEN;
			tim_enable_counter(TIM9);
		}
		// TIM7_SR &= ~TIM_SR_UIF;
		tim_clear_interrupt(TIM7, TIM_UPDATE);
	}
}

void tim9_isr(void)
{
	// if (TIM9_SR & TIM9_DIER & TIM_SR_UIF) {
	if (tim_get_interrupt_mask(TIM9, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM9, TIM_UPDATE)) {
		gpio_toggle(GPIO_PE(10, 11));
		if (++isr_count >= 10) {
			// TIM9_DIER &= ~TIM_DIER_UIE;
			tim_disable_interrupt(TIM9, TIM_UPDATE);
			// TIM9_CR1 &= ~TIM_CR1_CEN;
			tim_disable_counter(TIM9);
			isr_count = 0;
			// TIM10_DIER |= TIM_DIER_UIE;
			tim_enable_interrupt(TIM10, TIM_UPDATE);
			// TIM10_CR1 |= TIM_CR1_CEN;
			tim_enable_counter(TIM10);
		}
		// TIM9_SR &= ~TIM_SR_UIF;
		tim_clear_interrupt(TIM9, TIM_UPDATE);
	}
}

void tim10_isr(void)
{
	// if (TIM10_SR & TIM10_DIER & TIM_SR_UIF) {
	if (tim_get_interrupt_mask(TIM10, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM10, TIM_UPDATE)) {
		gpio_toggle(GPIO_PE10);
		if (++isr_count >= 10) {
			// TIM10_DIER &= ~TIM_DIER_UIE;
			tim_disable_interrupt(TIM10, TIM_UPDATE);
			// TIM10_CR1 &= ~TIM_CR1_CEN;
			tim_disable_counter(TIM10);
			isr_count = 0;
			// TIM11_DIER |= TIM_DIER_UIE;
			tim_enable_interrupt(TIM11, TIM_UPDATE);
			// TIM11_CR1 |= TIM_CR1_CEN;
			tim_enable_counter(TIM11);
		}
		// TIM10_SR &= ~TIM_SR_UIF;
		tim_clear_interrupt(TIM10, TIM_UPDATE);
	}
}

void tim11_isr(void)
{
	// if (TIM11_SR & TIM11_DIER & TIM_SR_UIF) {
	if (tim_get_interrupt_mask(TIM11, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM11, TIM_UPDATE)) {
		gpio_toggle(GPIO_PE11);
		if (++isr_count >= 10) {
			// TIM11_DIER &= ~TIM_DIER_UIE;
			tim_disable_interrupt(TIM11, TIM_UPDATE);
			// TIM11_CR1 &= ~TIM_CR1_CEN;
			tim_disable_counter(TIM11);
			isr_count = 0;
			// TIM2_DIER |= TIM_DIER_UIE;
			tim_enable_interrupt(TIM2, TIM_UPDATE);
			// TIM2_CR1 |= TIM_CR1_CEN;
			tim_enable_counter(TIM2);
		}
		// TIM11_SR &= ~TIM_SR_UIF;
		tim_clear_interrupt(TIM11, TIM_UPDATE);
	}
}

int main(void)
{
	clock_setup();
	gpio_setup();
	tim_setup();

	while (1)
		__asm__ ("nop");

	return 0;
}
