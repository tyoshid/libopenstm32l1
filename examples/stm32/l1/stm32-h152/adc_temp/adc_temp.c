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
#include <usart.h>
#include <tim.h>
#include <dma.h>
#include <nvic.h>
#include <adc.h>

#include <syscall.h>
#include <stdio.h>

#define PCLK1		32000000
#define TIMX_CLK_APB1	32000000
#define TIMX_CLK_APB2	32000000

#define TS_VREFINT	10	/* 10 usec */
#define AVG_SLOPE	1.61

volatile int conv_index;
volatile u32 conv[10];
volatile bool dma_busy;
volatile int tim9_int_count;
volatile int adc_regular_int_count;
volatile int adc_injected_int_count;
int injected_length;

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

	/* Enable internal high-speed oscillator 16MHz. */
	rcc_enable_osc(RCC_HSI);
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

static void usart_setup(void)
{
	/* Enable GPIOD clock. */
	rcc_enable_clock(RCC_GPIOD);

	/* Enable USART1 clock. */
	rcc_enable_clock(RCC_USART2);

	/* Setup GPIO pin PD5 as alternate function. */
	gpio_config_altfn(GPIO_USART1_3, GPIO_PUSHPULL, GPIO_10MHZ,
			  GPIO_NOPUPD, GPIO_PD_USART2_TX);

	/* Setup USART1. */
	usart_init(USART2, PCLK1, 38400, 8, USART_STOP_1,
		   USART_PARITY_NONE, USART_FLOW_NONE, USART_TX);
}

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(USART2, ptr[i]);
		return i;
	}

	errno = EIO;
	return -1;
}

static void tim_setup(void)
{
	/* Enable TIM6, TIM7 and TIM9 clock. */
	rcc_enable_clock(RCC_TIM6);
	rcc_enable_clock(RCC_TIM7);
	rcc_enable_clock(RCC_TIM9);

	/* Enable TIM9 interrupr. */
	nvic_enable_irq(NVIC_TIM9_IRQ);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM6);
	tim_enable_one_pulse_mode(TIM7);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM6);
	tim_disable_update_interrupt_on_any(TIM7);

	/* Load prescaler value (2MHz). */
	tim_load_prescaler_value(TIM6, TIMX_CLK_APB1 / 2000000 - 1);

	/* Load prescaler value (2kHz). */
	tim_load_prescaler_value(TIM7, TIMX_CLK_APB1 / 2000 - 1);

	/* Set master(TRGO) mode. */
	tim_set_master_mode(TIM9, TIM_TRGO_UPDATE);
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

/* 1 - 32767 msec */
static void delay_ms(u16 ms)
{
	/* Set auto-reload value (ms * 2). */
	tim_set_autoreload_value(TIM7, (ms << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM7);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM7, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM7, TIM_UPDATE);
}

void tim9_isr(void)
{
	if (tim_get_interrupt_mask(TIM9, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM9, TIM_UPDATE)) {
		/* Increment interrupt count. */
		tim9_int_count++;

		/* Clear interrupt. */
		tim_clear_interrupt(TIM9, TIM_UPDATE);
	}
}

static void dma_setup(void)
{
	/* Enable DMA1 clock. */
	rcc_enable_clock(DMA_RCC_ADC);

	/* Enable DMA ADC(DMA1 channel1) interrupt. */
	nvic_enable_irq(DMA_ADC_IRQ);

}

/* DMA ADC(DMA1 Channel1) interrupt */
void dma_adc_isr(void)
{
	if (dma_get_interrupt_mask(DMA_ADC, DMA_COMPLETE) &&
	    dma_get_interrupt_status(DMA_ADC, DMA_GLOBAL | DMA_COMPLETE) ==
	    (DMA_GLOBAL | DMA_COMPLETE)) {
		/* Disable DMA. */
		dma_disable(DMA_ADC);

		/* Clear flag. */
		dma_busy = false;

		/* Clear interrupt. */
		dma_clear_interrupt(DMA_ADC, DMA_GLOBAL | DMA_COMPLETE);
	}
}

static void adc_setup(void)
{
	/* Enable ADC clock. */
	rcc_enable_clock(RCC_ADC);

	/* Enable ADC interrupt. */
	nvic_enable_irq(NVIC_ADC_IRQ);

	/* Set ADC prescaler to 1(default). */
	// adc_set_prescaler(1);

	/* Enable temperature sensor and VREFINT channel */
	adc_enable_ts_vref();

	/* Set temperature sensor (ADC_IN16) sampling time. */
	adc_set_sampling(ADC_IN_TEMP, 384); /* 24 usec */

	/* Enable overrun interrupt. */
	adc_enable_interrupt(ADC_OVERRUN);
}

void adc_isr(void)
{
	u32 mask;
	u32 status;
	int i;

	/* Get interrupt mask. */
	if (!(mask = adc_get_interrupt_mask(ADC_REGULAR_END |
					    ADC_INJECTED_END | ADC_OVERRUN)))
		return;
	/* Get interrupt status. */
	if (!(status = adc_get_interrupt_status(ADC_REGULAR_END |
						ADC_INJECTED_END |
						ADC_OVERRUN)))
		return;

	/* Regular channel end of conversion */
	if (mask & status & ADC_REGULAR_END) {
		/* Increment interrupt count. */
		adc_regular_int_count++;

		/* Get data and clear interrupt. */
		if (conv_index < 10)
			conv[conv_index++] = adc_get_regular_data();
		else
			adc_clear_interrupt(ADC_REGULAR_END);
	}
	/* Injected channel end of conversion */
	if (mask & status & ADC_INJECTED_END) {
		/* Increment interrupt count. */
		adc_injected_int_count++;

		/* Get data. */
		for (i = 0; i < injected_length && conv_index < 10; i++)
			conv[conv_index++] = adc_get_injected_data(i);

		/* Clear interrupt. */
		adc_clear_interrupt(ADC_INJECTED_END);
	}
	/* Overrun */
	if (mask & status & ADC_OVERRUN) {
		/* LED on */
		gpio_clear(GPIO_PE10);

		/* Clear interrupt. */
		adc_clear_interrupt(ADC_OVERRUN);
	}
}

/* Single conversion mode, regular channel, polling */
static void vrefint_single_regular_polling(void)
{
	int i;
	double d;

	printf("# Single conversion mode, regular channel, polling\r\n");

	/* Set channel number (ADC_IN17 = VREFINT). */
	adc_set_regular_channel(ADC_IN_VREFINT);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	for (i = 0; i < 10; i++) {
		/* Start conversion. */
		adc_start_regular_conversion();

		/* Wait for the end of conversion. */
		while (!adc_get_interrupt_status(ADC_REGULAR_END))
			;

		/* Get data and clear interrupt. */
		conv[i] = adc_get_regular_data();

		/* sampling time */
		delay_us(TS_VREFINT);
	}

	/* ADC off */
	adc_disable();

	/* Print data. */
	d = 0.;
	for (i = 0; i < 10; i++) {
		printf("%d\r\n", (int)conv[i]);
		d += conv[i];
	}
	d /= 10.;
	printf("Avg. %.1lf  %5.3lfV\r\n", d, 3.3 / 4096. * d);
}

static void print_temp(void)
{
	double d;
	int i;

	d = 0.;
	for (i = 0; i < 10; i++) {
		printf("%d\r\n", (int)conv[i]);
		d += conv[i];
	}
	d /= 10.;
	d = 3300. / 4096. * d;
	printf("VSENSE = %.1lfmV  %.1lf degrees Celsius\r\n", d,
	       d / AVG_SLOPE - 273.15);
}

/* Single conversion mode, regular channel, interrupt */
static void temp_single_regular_int(void)
{
	int i;

	printf("# Single conversion mode, regular channel, interrupt\r\n");

	/* Set channel number (ADC_IN16 = temperature sensor). */
	adc_set_regular_channel(ADC_IN_TEMP);

	/* Initialize data index. */
	conv_index = 0;

	/* Enable interrupt. */
	adc_enable_interrupt(ADC_REGULAR_END);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	for (i = 0; i < 10; i++) {
		/* Start conversion. */
		adc_start_regular_conversion();

		/* Wait for the end of conversion. */
		while (conv_index < i + 1)
			;
	}

	/* ADC off */
	adc_disable();

	/* Disable interrupt. */
	adc_disable_interrupt(ADC_REGULAR_END);

	/* Print data. */
	print_temp();
}

/* Continuous conversion mode, interrupt */
static void temp_continuous_int(void)
{
	printf("# Continuous conversion mode, interrupt\r\n");

	/* Set channel number (ADC_IN16 = temperature sensor). */
	adc_set_regular_channel(ADC_IN_TEMP);

	/* Continuous mode */
	adc_enable_continuous();

	/* Initialize data index. */
	conv_index = 0;

	/* Enable interrupt. */
	adc_enable_interrupt(ADC_REGULAR_END);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	/* Start conversion. */
	adc_start_regular_conversion();

	/* Wait for the end of conversion. */
	while (conv_index < 10)
		;

	/* Aborting a conversion. */
	adc_set_regular_channel(ADC_IN_TEMP);

	/* Wait for ready. */
	adc_wait_for_regular_ready();

	/* ADC off */
	adc_disable();

	/* Disable interrupt. */
	adc_disable_interrupt(ADC_REGULAR_END);

	/* Disable continuous mode. */
	adc_disable_continuous();

	/* Print data */
	print_temp();
}

/* Scan mode, regular channel, interrupt */
static void temp_scan_regular_int(void)
{
	int i;
	int seq[10];

	printf("# Scan mode, regular channel, interrupt\r\n");

	/* Set channel sequence (ADC_IN16 = temperature sensor). */
	for (i = 0; i < 10; i++)
		seq[i] = ADC_IN_TEMP;
	adc_set_regular_sequence(10, seq);

	/* Enable scan mode and each channel interrupt. */
	adc_enable_scan(true);

	/* Initialize data index. */
	conv_index = 0;

	/* Enable interrupt. */
	adc_enable_interrupt(ADC_REGULAR_END);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	/* Start conversion. */
	adc_start_regular_conversion();

	/* Wait for the end of conversion. */
	while (conv_index < 10)
		;

	/* ADC off */
	adc_disable();

	/* Disable interrupt. */
	adc_disable_interrupt(ADC_REGULAR_END);

	/* Disable scan mode. */
	adc_disable_scan();

	/* Print data. */
	print_temp();
}

/* Scan mode, regular channel, DMA */
static void temp_scan_regular_dma(void)
{
	int i;
	int seq[10];

	printf("# Scan mode, regular channel, DMA\r\n");

	/* Set channel sequence (ADC_IN16 = temperature sensor). */
	for (i = 0; i < 10; i++)
		seq[i] = ADC_IN_TEMP;
	adc_set_regular_sequence(10, seq);

	/* Enable scan mode. */
	adc_enable_scan(false);

	/* Enable DMA. */
	adc_enable_dma(false);

	/* Set flag. */
	dma_busy = true;

	/* Setup DMA controller. */
	dma_setup_channel(DMA_ADC, (u32)conv, (u32)&ADC_DR, 10,
			  DMA_M_32BIT | DMA_P_32BIT | DMA_M_INC |
			  DMA_COMPLETE | DMA_ENABLE);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	/* Start conversion. */
	adc_start_regular_conversion();

	/* Wait for the end of data transfer. */
	while (dma_busy)
		;

	/* ADC off */
	adc_disable();

	/* Disable DMA. */
	adc_disable_dma();

	/* Disable scan mode. */
	adc_disable_scan();

	/* Print data. */
	print_temp();
}

/* Discontinuous mode, regular channel, interrupt */
static void temp_discontinuous_regular_int(void)
{
	int i;
	int seq[10];

	printf("# Discontinuous mode, regular channel, interrupt\r\n");

	/* Set channel sequence (ADC_IN16 = temperature sensor). */
	for (i = 0; i < 10; i++)
		seq[i] = ADC_IN_TEMP;
	adc_set_regular_sequence(10, seq);

	/* Enable discontinuous mode and each channel interrupt. */
	adc_enable_regular_discontinuous(2, true);

	/* Initialize data index. */
	conv_index = 0;

	/* Enable interrupt. */
	adc_enable_interrupt(ADC_REGULAR_END);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	/* Setup timer. */
	tim_setup_counter(TIM9, TIMX_CLK_APB2 / 1000000 - 1, 100 - 1);
	tim_clear_interrupt(TIM9, TIM_UPDATE);

	/* Enable external trigger. */
	adc_set_regular_ext(ADC_RISING, ADC_TIM9_TRGO);

	/* Initialize timer interrupt count */
	tim9_int_count = 0;

	/* Enable timer interrupt. */
	tim_enable_interrupt(TIM9, TIM_UPDATE);

	/* Start timer. */
	tim_enable_counter(TIM9);

	/* Wait for the end of conversion. */
	while (conv_index < 10)
		;

	/* Stop timer. */
	tim_disable_counter(TIM9);

	/* Disable timer interrupt. */
	tim_disable_interrupt(TIM9, TIM_UPDATE);

	/* Disable external trigger. */
	adc_set_regular_ext(ADC_TRIGGER_DISABLE, 0);

	/* ADC off */
	adc_disable();

	/* Disable interrupt. */
	adc_disable_interrupt(ADC_REGULAR_END);

	/* Disable discontinuous mode. */
	adc_disable_regular_discontinuous();

	/* Check timer interrupt count. */
	if (tim9_int_count != 5)
		printf("### tim9_int_count = %d!!!(5)\r\n", tim9_int_count);

	/* Print data. */
	print_temp();
}

/* Discontinuous mode, regular channel, DMA */
static void temp_discontinuous_regular_dma(void)
{
	int i;
	int seq[10];

	printf("# Discontinuous mode, regular channel, DMA\r\n");

	/* Set channel sequence (ADC_IN16 = temperature sensor). */
	for (i = 0; i < 10; i++)
		seq[i] = ADC_IN_TEMP;
	adc_set_regular_sequence(10, seq);

	/* Enable discontinuous mode. */
	adc_enable_regular_discontinuous(2, false);

	/* Enable DMA. */
	adc_enable_dma(false);

	/* Set Flag */
	dma_busy = true;

	/* Setup DMA controller. */
	dma_setup_channel(DMA_ADC, (u32)conv, (u32)&ADC_DR, 10,
			  DMA_M_32BIT | DMA_P_32BIT | DMA_M_INC |
			  DMA_COMPLETE | DMA_ENABLE);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	/* Setup timer. */
	tim_setup_counter(TIM9, TIMX_CLK_APB2 / 1000000 - 1, 100 - 1);
	tim_clear_interrupt(TIM9, TIM_UPDATE);

	/* Enable external trigger. */
	adc_set_regular_ext(ADC_RISING, ADC_TIM9_TRGO);

	/* Initialize timer interrupt count */
	tim9_int_count = 0;

	/* Enable timer interrupt. */
	tim_enable_interrupt(TIM9, TIM_UPDATE);

	/* Start timer. */
	tim_enable_counter(TIM9);

	/* Wait for the end of data transfer. */
	while (dma_busy)
		;

	/* Stop timer. */
	tim_disable_counter(TIM9);

	/* Disable timer interrupt. */
	tim_disable_interrupt(TIM9, TIM_UPDATE);

	/* Disable external trigger. */
	adc_set_regular_ext(ADC_TRIGGER_DISABLE, 0);

	/* ADC off */
	adc_disable();

	/* Disable DMA. */
	adc_disable_dma();

	/* Disable discontinuous mode. */
	adc_disable_regular_discontinuous();

	/* Check timer interrupt count. */
	if (tim9_int_count != 5)
		printf("### tim9_int_count = %d!!!(5)\r\n", tim9_int_count);

	/* Print data. */
	print_temp();
}

/* Single conversion mode, injected channel, interrupt */
static void temp_single_injected_int(void)
{
	int i;
	int channel;

	printf("# Single conversion mode, injected channel, interrupt\r\n");

	/* Set length. */
	injected_length = 1;

	/* Set channel number. */
	channel = ADC_IN_TEMP;
	adc_set_injected_sequence(1, &channel);

	/* Initialize data index. */
	conv_index = 0;

	/* Enable interrupt. */
	adc_enable_interrupt(ADC_INJECTED_END);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	for (i = 0; i < 10; i++) {
		/* Start conversion. */
		adc_start_injected_conversion();

		/* Wait for the end of conversion. */
		while (conv_index < i + 1)
			;
	}

	/* ADC off */
	adc_disable();

	/* Disable interrupt. */
	adc_disable_interrupt(ADC_INJECTED_END);

	/* Print data. */
	print_temp();
}

/* Scan mode, injected channel, interrupt */
static void temp_scan_injected_int(void)
{
	int i;
	int seq[4];

	printf("# Scan mode, injected channel, interrupt\r\n");

	/* Set length. */
	injected_length = 3;

	/* Set channel sequence (ADC_IN16 = temperature sensor). */
	seq[0] = ADC_IN_TEMP;
	seq[1] = ADC_IN_TEMP;
	seq[2] = ADC_IN_TEMP;
	adc_set_injected_sequence(3, seq);

	/* Enable scan mode. */
	adc_enable_scan(false);

	/* Initialize data index. */
	conv_index = 0;

	/* Enable interrupt. */
	adc_enable_interrupt(ADC_INJECTED_END);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	for (i = 0; i < 4; i++) {
		/* Start conversion. */
		adc_start_injected_conversion();

		/* Wait for the end of conversion. */
		while (conv_index < 10 && conv_index < (i + 1) * 3)
			;
	}

	/* ADC off */
	adc_disable();

	/* Disable interrupt. */
	adc_disable_interrupt(ADC_INJECTED_END);

	/* Disable scan mode. */
	adc_disable_scan();

	/* Print data. */
	print_temp();
}

/* Discontinuous mode, injected channel, interrupt */
static void temp_discontinuous_injected_int(void)
{
	int seq[4];

	printf("# Discontinuous mode, injected channel, interrupt\r\n");

	/* Set length. */
	injected_length = 4;

	/* Set channel sequence (ADC_IN16 = temperature sensor). */
	seq[0] = ADC_IN_TEMP;
	seq[1] = ADC_IN_TEMP;
	seq[2] = ADC_IN_TEMP;
	seq[3] = ADC_IN_TEMP;
	adc_set_injected_sequence(4, seq);

	/* Enable discontinuous mode. */
	adc_enable_injected_discontinuous(2);

	/* Initialize data index. */
	conv_index = 0;

	/* Enable interrupt. */
	adc_enable_interrupt(ADC_INJECTED_END);

	/* ADC on */
	adc_enable();

	/* ADC power-up time (tSTAB) */
	delay_us(ADC_T_STAB);

	/* Setup timer. */
	tim_setup_counter(TIM9, TIMX_CLK_APB2 / 1000000 - 1, 100 - 1);
	tim_clear_interrupt(TIM9, TIM_UPDATE);

	/* Enable external trigger. */
	adc_set_injected_ext(ADC_RISING, ADC_TIM9_TRGO);

	/* Initialize timer interrupt count */
	tim9_int_count = 0;

	/* Initialize interrupt count */
	adc_injected_int_count = 0;

	/* Enable timer interrupt. */
	tim_enable_interrupt(TIM9, TIM_UPDATE);

	/* Start timer. */
	tim_enable_counter(TIM9);

	/* Wait for the end of conversion. */
	while (conv_index < 10)
		;

	/* Stop timer. */
	tim_disable_counter(TIM9);

	/* Disable timer interrupt. */
	tim_disable_interrupt(TIM9, TIM_UPDATE);

	/* Disable external trigger. */
	adc_set_injected_ext(ADC_TRIGGER_DISABLE, 0);

	/* ADC off */
	adc_disable();

	/* Disable interrupt. */
	adc_disable_interrupt(ADC_INJECTED_END);

	/* Disable discontinuous mode */
	adc_disable_injected_discontinuous();

	/* Check timer interrupt count. */
	if (tim9_int_count != 6)
		printf("### tim9_int_count = %d!!!(6)\r\n", tim9_int_count);

	/* Check interrupt count. */
	if (adc_injected_int_count != 3)
		printf("### adc_int_count = %d!!!(3)\r\n",
		       adc_injected_int_count);

	/* Print data. */
	print_temp();
}

int main(void)
{
	u32 r;
	double d;

	clock_setup();
	gpio_setup();
	usart_setup();
	tim_setup();
	dma_setup();
	adc_setup();

	r = ADC_VREFINT_CAL;
	d = r;
	d = 3. / 4096. * d;
	printf("VREFINT_CAL = %d  %5.3lfV\r\n", (int)r, d);

	vrefint_single_regular_polling();

	printf("TSENSE_CAL1 = %d\r\n", (int)ADC_TSENSE_CAL1);
	printf("TSENSE_CAL2 = %d\r\n", (int)ADC_TSENSE_CAL2);

	delay_ms(2000);

	while (1) {
		temp_single_regular_int();

		delay_ms(2000);

		temp_continuous_int();

		delay_ms(2000);

		temp_scan_regular_int();

		delay_ms(2000);

		temp_scan_regular_dma();

		delay_ms(2000);

		temp_discontinuous_regular_int();

		delay_ms(2000);

		temp_discontinuous_regular_dma();

		delay_ms(2000);

		temp_single_injected_int();

		delay_ms(2000);

		temp_scan_injected_int();

		delay_ms(2000);

		temp_discontinuous_injected_int();

		delay_ms(2000);
	}

	return 0;
}
