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
#include <lcd.h>
#include <tim.h>
#include <dma.h>
#include <nvic.h>
#include <adc.h>

#define TIMX_CLK_APB1	16000000
#define TIMX_CLK_APB2	16000000

#define TS_VREFINT	10	/* 10 usec */
#define AVG_SLOPE	1.61

#define VDDA		3.
#define RESOLUTION	4096.

volatile int conv_index;
volatile u32 conv[10];
volatile bool dma_busy;
volatile int tim9_int_count;
volatile int adc_regular_int_count;
volatile int adc_injected_int_count;
int injected_length;

/* LCD connections */
struct segmap {
	int com;
	int seg;
};

/* A, B, ..., Q */
struct segmap charseg[6][14] = {{{1, 28}, {0, 28}, {1, 1}, {1, 0}, {0, 0},
				 {1, 29}, {0, 29}, {3, 29}, {3, 28}, {2, 28},
				 {0, 1}, {3, 0}, {2, 0}, {2, 29}},
				{{1, 26}, {0, 26}, {1, 7}, {1, 2}, {0, 2},
				 {1, 27}, {0, 27}, {3, 27}, {3, 26}, {2, 26},
				 {0, 7}, {3, 2}, {2, 2}, {2, 27}},
				{{1, 24}, {0, 24}, {1, 9}, {1, 8}, {0, 8},
				 {1, 25}, {0, 25}, {3, 25}, {3, 24}, {2, 24},
				 {0, 9}, {3, 8}, {2, 8}, {2, 25}},
				{{1, 20}, {0, 20}, {1, 11}, {1, 10}, {0, 10},
				 {1, 21}, {0, 21}, {3, 21}, {3, 20}, {2, 20},
				 {0, 11}, {3, 10}, {2, 10}, {2, 21}},
				{{1, 18}, {0, 18}, {1, 13}, {1, 12}, {0, 12},
				 {1, 19}, {0, 19}, {3, 19}, {3, 18}, {2, 18},
				 {0, 13}, {3, 12}, {2, 12}, {2, 19}},
				{{1, 17}, {0, 17}, {1, 15}, {1, 14}, {0, 14},
				 {1, 16}, {0, 16}, {3, 16}, {3, 17}, {2, 17},
				 {0, 15}, {3, 14}, {2, 14}, {2, 16}}};

/* DP */
struct segmap dpseg[4] = {{3, 1}, {3, 7}, {3, 9}, {3, 11}};

/*
 * Character font (14 segments)
 *
 *
 * -
 * 01234567
 * 89
 * V
 * oC
 */
u16 font[] = {0x0000,
	      0x0440,
	      0x003f, 0x0006, 0x045b, 0x044f, 0x0466, 0x046d, 0x047d, 0x0007,
	      0x047f, 0x046f,
	      0x2230,
	      0x0463, 0x0039};

/* Set STM32 to 16 MHz. */
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

	/* AHB, APB1 and APB2 prescaler value is default. */
	// rcc_set_prescaler(1, 1, 1);

	/* Select SYSCLK source. */
	rcc_set_sysclk_source(RCC_HSI);

	/*
	 * Enable write access to RTC, RTC backup register and RCC CSR
	 * (LSEON, LSEBYP, RTCSEL and RTCEN)
	 */
	pwr_disable_backup_write_protection();

	/* Enable LSE. */
	rcc_enable_osc(RCC_LSE);

	/* LCDCLK = 32.768kHz */
	rcc_setup_rtc_lcd_clock(RCC_LSE, 0);
}

static void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	rcc_enable_clock(RCC_GPIOB);

	/* Set PB6 and PB7 to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PB(6, 7));

	/* LED off */
	gpio_clear(GPIO_PB(6, 7));
}

static void lcd_setup(void)
{
	/* Enable GPIOA, GPIOB and GPIOC clock. */
	rcc_enable_clock(RCC_GPIOA);
	// rcc_enable_clock(RCC_GPIOB);
	rcc_enable_clock(RCC_GPIOC);

	/* Set GPIO1-3,8-10,15 (in GPIO port A) to 'alternate function'. */
	gpio_config_altfn(GPIO_LCD, GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			  GPIO_PA(LCD_SEG0, LCD_SEG1, LCD_SEG2, LCD_COM0,
				  LCD_COM1, LCD_COM2, LCD_SEG17));

	/* Set GPIO3-5,8-15 (in GPIO port B) to 'alternate function'. */
	gpio_config_altfn(GPIO_LCD, GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			  GPIO_PB(LCD_SEG7, LCD_SEG8, LCD_SEG9, LCD_SEG16,
				  LCD_COM3, LCD_SEG10, LCD_SEG11, LCD_SEG12,
				  LCD_SEG13, LCD_SEG14, LCD_SEG15));

	/* Set GPIO0-3,6-11 (in GPIO port C) to 'alternate function'. */
	gpio_config_altfn(GPIO_LCD, GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			  GPIO_PC(LCD_SEG18, LCD_SEG19, LCD_SEG20, LCD_SEG21,
				  LCD_SEG24, LCD_SEG25, LCD_SEG26, LCD_SEG27,
				  LCD_COM4_SEG28_SEG40, LCD_COM5_SEG29_SEG41));

	/* Enable LCD clock. */
	rcc_enable_clock(RCC_LCD);

	/* SEG[31:28] are multiplexed with SEG[43:40] */
	lcd_set_seg_pin_multiplex(true);

	/* 1/4 duty, 1/3 bias */
	lcd_set_duty_bias(LCD_DUTY_1_4, LCD_BIAS_1_3);

	/*
	 * fck_div = fLCDCLK / (8 * 16) = 256Hz
	 * fframe = fck_div * duty = 64Hz
	 */
	lcd_set_ck_div(8, 16);

	/* Wait for LCD_FCR register update. */
	lcd_wait_for_sync();

	/* Set contrast. */
	lcd_set_contrast(3);

	/* Wait for LCD_FCR register update. */
	lcd_wait_for_sync();

	/* Start display. */
	lcd_enable();

	/* Wait for step-up converter ready. */
	lcd_wait_for_ready();
}

static void write_char(int col, int fn)
{
	int i;
	int com;
	int seg;

	for (i = 0; i < 14; i++) {
		/* Common */
		com = charseg[col][i].com;

		/* Segment */
		seg = charseg[col][i].seg;

		/* Set pixel. */
		if (font[fn] & (1 << i))
			LCD_RAM[com * 2] |= (1 << seg);
		else
			LCD_RAM[com * 2] &= ~(1 << seg);
	}
}

static void write_dp(int data)
{
	int i;
	int com;
	int seg;

	for (i = 0; i < 4; i++) {
		/* Common */
		com = dpseg[i].com;

		/* Segment */
		seg = dpseg[i].seg;

		/* Set pixel. */
		if (data & (1 << i))
			LCD_RAM[com * 2] |= (1 << seg);
		else
			LCD_RAM[com * 2] &= ~(1 << seg);
	}
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
		gpio_set(GPIO_PB6);

		/* Clear interrupt. */
		adc_clear_interrupt(ADC_OVERRUN);
	}
}

/* Single conversion mode, regular channel, polling */
static void vrefint_single_regular_polling(void)
{
	int i;
	double d;
	double m;
	int n;

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

	d = 0.;
	for (i = 0; i < 10; i++)
		d += conv[i];
	d /= 10.;
	d = VDDA / RESOLUTION * d;

	/* Print data. */
	m = 1.;
	for (i = 0; i < 5; i++) {
		n = d * m;
		write_char(i, n % 10 + 2);
		m *= 10.;
	}
	write_dp(1);
	write_char(5, 12);

	/* Update display. */
	lcd_update();
}

static void print_temp(void)
{
	double d;
	int i;
	int n0;
	int n;

	d = 0.;
	for (i = 0; i < 10; i++)
		d += conv[i];
	d /= 10.;

	d = VDDA * 1000. / RESOLUTION * d;

	d = d / AVG_SLOPE - 273.15;

	if (d >= 0.) {
		n0 = d / 100.;
		if (n0)
			write_char(0, n0 % 10 + 2);
		else
			write_char(0, 0);
		n = d / 10.;
		if (n || n0)
			write_char(1, n % 10 + 2);
		else
			write_char(0, 0);
		n = d;
		write_char(2, n % 10 + 2);
		write_dp(4);
		n = d * 10.;
		write_char(3, n % 10 + 2);
	} else {
		d = -d;
		write_char(0, 1);
		n = d / 10.;
		if (n)
			write_char(1, n % 10 + 2);
		else
			write_char(0, 0);
		n = d;
		write_char(2, n % 10 + 2);
		write_dp(4);
		n = d * 10.;
		write_char(3, n % 10 + 2);
	}
	write_char(4, 13);
	write_char(5, 14);

	/* Update display. */
	lcd_update();
}

/* Single conversion mode, regular channel, interrupt */
static void temp_single_regular_int(void)
{
	int i;

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
		/* LED on */
		gpio_set(GPIO_PB7);

	/* Print data. */
	print_temp();
}

/* Discontinuous mode, regular channel, DMA */
static void temp_discontinuous_regular_dma(void)
{
	int i;
	int seq[10];

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
		/* LED on */
		gpio_set(GPIO_PB7);

	/* Print data. */
	print_temp();
}

/* Single conversion mode, injected channel, interrupt */
static void temp_single_injected_int(void)
{
	int i;
	int channel;

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
		/* LED on */
		gpio_set(GPIO_PB7);


	/* Check interrupt count. */
	if (adc_injected_int_count != 3)
		/* LED on */
		gpio_set(GPIO_PB7);

	/* Print data. */
	print_temp();
}

int main(void)
{
	clock_setup();
	gpio_setup();
	lcd_setup();
	tim_setup();
	dma_setup();
	adc_setup();

	vrefint_single_regular_polling();

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
