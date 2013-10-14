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
#include <dma.h>
#include <dac.h>

#define TIMX_CLK_APB1 32000000

#define MAXDATA 32

/* sine wave 1kHz: sampling 32kHz */
u32 data[MAXDATA] = {
	2048,
	2447,
	2831,
	3185,
	3495,
	3750,
	3939,
	4056,
	4095,
	4056,
	3939,
	3750,
	3495,
	3185,
	2831,
	2447,
	2048,
	1649,
	1265,
	911,
	601,
	346,
	157,
	40,
	1,
	40,
	157,
	346,
	601,
	911,
	1265,
	1649
};

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
	/* Enable GPIOA clock. */
	rcc_enable_clock(RCC_GPIOA);

	/* Set GPIO5 (in GPIO port A)(DAC_OUT1) to 'analog'. */
	gpio_config_analog(GPIO_PA5);
}

static void tim_setup(void)
{
	/* Enable TIM6 and TIM7 clock. */
	rcc_enable_clock(RCC_TIM6);
	rcc_enable_clock(RCC_TIM7);

	/* TIM6 */
	/* Load prescaler value (320kHz) and set auto-reload value (10). */
	tim_setup_counter(TIM6, TIMX_CLK_APB1 / 320000 - 1, 10 - 1);

	/* Enable update DMA request. */
	tim_enable_dma(TIM6, TIM_DMA_UPDATE);

	/* TIM7 */
	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM7);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM7);

	/* Load prescaler value (2MHz). */
	tim_load_prescaler_value(TIM7, TIMX_CLK_APB1 / 2000000 - 1);
}

/* 1 - 32767 usec */
static void delay_us(u16 us)
{
	/* Set auto-reload value (us * 2). */
	tim_set_autoreload_value(TIM7, (us << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM7);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM7, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM7, TIM_UPDATE);
}

static void dma_setup(void)
{
	/* Enable DMA1 clock. */
	rcc_enable_clock(DMA_RCC_TIM6_UP);

	/* TIM6_UP is connected to DMA1 channel2 */

	/* Set memory address. */
	// DMA1_CMAR2 = (u32)data;

	/* Set Peripheral address. */
	// DMA1_CPAR2 = (u32)&DAC_DHR12R1;

	/* Set number of data to transfer. */
	// DMA1_CNDTR2 = MAXDATA;

	/* Set DMA mode and enable DMA. */
	// DMA1_CCR2 = (DMA_CCR_MSIZE_32BIT | DMA_CCR_PSIZE_32BIT |
	//	     DMA_CCR_MINC | DMA_CCR_CIRC | DMA_CCR_DIR |
	//	     DMA_CCR_EN);

	dma_setup_channel(DMA_TIM6_UP, (u32)data, (u32)&DAC_DHR12R2, MAXDATA,
			  DMA_M_32BIT | DMA_P_32BIT | DMA_M_INC |
			  DMA_CIRCULAR | DMA_M_TO_P | DMA_ENABLE);
}

static void dac_setup(void)
{
	/* Enable DAC clock. */
	rcc_enable_clock(RCC_DAC);

	/* Enable DAC channel2. */
	dac_enable(DAC_CH2);

	/* Wait tWAKEUP. */
	delay_us(DAC_T_WAKEUP);
}

int main(void)
{
	clock_setup();
	gpio_setup();
	tim_setup();
	dma_setup();
	dac_setup();

	/* Enable TIM6 counter. */
	tim_enable_counter(TIM6);

	while (1)
		__asm__ ("nop");

	return 0;
}
