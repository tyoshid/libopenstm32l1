/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Edward Cheeseman <evbuilder@users.sourceforge.net>
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

#include <stm32/l1/adc.h>

void adc_start_regular_conversion(void)
{
	ADC_CR2 |= ADC_CR2_SWSTART;
}

void adc_start_injected_conversion(void)
{
	ADC_CR2 |= ADC_CR2_JSWSTART;
}

u16 adc_get_regular_data(void)
{
	return ADC_DR;
}

u16 adc_get_injected_data(int index)
{
	switch (index) {
	case 0:
		return ADC_JDR1;
	case 1:
		return ADC_JDR2;
	case 2:
		return ADC_JDR3;
	case 3:
		return ADC_JDR4;
	default:
		break;
	}
	return 0;
}

/* ADC power on-off control */

void adc_config_power_down(bool idle, bool delay)
{
	u32 reg32;

	reg32 = ADC_CR1;
	if (idle)
		reg32 |= ADC_CR1_PDI;
	else
		reg32 &= ~ADC_CR1_PDI;
	if (delay)
		ADC_CR1 = (reg32 | ADC_CR1_PDD);
	else
		ADC_CR1 = (reg32 & ~ADC_CR1_PDD);
}

void adc_enable(void)
{
	ADC_CR2 |= ADC_CR2_ADON;
	while (!(ADC_SR & ADC_SR_ADONS))
		;
}

void adc_disable(void)
{
	ADC_CR2 &= ~ADC_CR2_ADON;
	while (ADC_SR & ADC_SR_ADONS)
		;
}

/* ADC clock */

void adc_set_prescaler(int pre)
{
	u32 reg32;

	reg32 = ADC_CCR;
	switch (pre) {
	case 1:
		ADC_CCR = (reg32 & ~(ADC_CCR_ADCPRE1 | ADC_CCR_ADCPRE0));
		break;
	case 2:
		reg32 &= ~ADC_CCR_ADCPRE1;
		ADC_CCR = (reg32 | ADC_CCR_ADCPRE0);
		break;
	case 4:
		reg32 |= ADC_CCR_ADCPRE1;
		ADC_CCR = (reg32 & ~ADC_CCR_ADCPRE0);
		break;
	default:
		break;
	}
}

/* Channel selection */

void adc_set_bank(int bank)
{
	if (bank == 0)		/* Bank A */
		ADC_CR2 &= ~ADC_CR2_ADC_CFG;
	else if (bank == 1)	/* Bank B */
		ADC_CR2 |= ADC_CR2_ADC_CFG;
}

void adc_set_regular_channel(int channel)
{
	u32 reg32;

	reg32 = ADC_SQR5;
	reg32 &= ~0x1f;
	ADC_SQR5 = (reg32 | (channel & 0x1f));
}

void adc_set_regular_sequence(int length, int *channel)
{
	u32 sqr[5];
	int i;

	if (length > 28 || length <= 0)
		return;

	sqr[0] = ADC_SQR5;
	if (length > 6)
		sqr[1] = ADC_SQR4;
	if (length > 12)
		sqr[2] = ADC_SQR3;
	if (length > 18)
		sqr[3] = ADC_SQR2;
	sqr[4] = ADC_SQR1;
	for (i = 0; i < length; i++) {
		sqr[i / 6] &= ~(0x1f << (i % 6 * 5));
		sqr[i / 6] |= ((*channel++ & 0x1f) << (i % 6 * 5));
	}
	sqr[4] &= ~(0x1f << 20);
	sqr[4] |= (((length - 1) & 0x1f) << 20);

	ADC_SQR5 = sqr[0];
	if (length > 6)
		ADC_SQR4 = sqr[1];
	if (length > 12)
		ADC_SQR3 = sqr[2];
	if (length > 18)
		ADC_SQR2 = sqr[3];
	ADC_SQR1 = sqr[4];
}

void adc_set_injected_sequence(int length, int *channel)
{
	u32 reg32;
	int i;

	if (length > 4 || length <= 0)
		return;

	reg32 = ADC_JSQR;
	for (i = 0; i < length; i++) {
		reg32 &= ~(0x1f << ((4 - length + i) * 5));
		reg32 |= ((*channel++ & 0x1f) << ((4 - length + i) * 5));
	}
	reg32 &= ~(3 << 20);
	ADC_JSQR = (reg32 | (((length - 1) & 3) << 20));
}

/* Single / Continuous conversion mode */

void adc_enable_continuous(void)
{
	ADC_CR2 |= ADC_CR2_CONT;
}

void adc_disable_continuous(void)
{
	ADC_CR2 &= ~ADC_CR2_CONT;
}

/* Analog watchdog */

void adc_set_analog_watchdog(int low, int high, bool regular, bool injected,
			     bool single, int channel)
{
	u32 reg32;

	ADC_LTR = low;
	ADC_HTR = high;
	reg32 = ADC_CR1;
	if (regular)
		reg32 |= ADC_CR1_AWDEN;
	else
		reg32 &= ~ADC_CR1_AWDEN;
	if (injected)
		reg32 |= ADC_CR1_JAWDEN;
	else
		reg32 &= ~ADC_CR1_JAWDEN;

	if (single) {
		reg32 &= ~0x1f;
		ADC_CR1 = (reg32 | (ADC_CR1_AWDSGL | (channel & 0x1f)));
	} else {
		ADC_CR1 = (reg32 & ~ADC_CR1_AWDSGL);
	}
}

/* Scan mode */

void adc_enable_scan(bool channel_eoc)
{
	if (channel_eoc)
		ADC_CR2 |= ADC_CR2_EOCS;
	else
		ADC_CR2 &= ~ADC_CR2_EOCS;
	ADC_CR1 |= ADC_CR1_SCAN;
}

void adc_disable_scan(void)
{
	ADC_CR1 &= ~ADC_CR1_SCAN;
}

/* Injected channel management */

void adc_enable_auto_injected(void)
{
	ADC_CR1 |= ADC_CR1_JAUTO;
}

void adc_disable_auto_injected(void)
{
	ADC_CR1 &= ~ADC_CR1_JAUTO;
}

/* Discontinuous mode */

void adc_enable_regular_discontinuous(int num, bool channel_eoc)
{
	u32 reg32;

	if (channel_eoc)
		ADC_CR2 |= ADC_CR2_EOCS;
	else
		ADC_CR2 &= ~ADC_CR2_EOCS;
	reg32 = ADC_CR1;
	reg32 &= ~(7 << 13);
	ADC_CR1 = (reg32 | ADC_CR1_DISCEN | ADC_CR1_SCAN |
		   (((num - 1) & 7)  << 13));
}

void adc_disable_regular_discontinuous(void)
{
	ADC_CR1 &= ~(ADC_CR1_DISCEN | ADC_CR1_SCAN);
}

void adc_enable_injected_discontinuous(int num)
{
	u32 reg32;

	reg32 = ADC_CR1;
	reg32 &= ~(7 << 13);
	ADC_CR1 = (reg32 | ADC_CR1_JDISCEN | ADC_CR1_SCAN |
		   (((num - 1) & 7) << 13));
}

void adc_disable_injected_discontinuous(void)
{
	ADC_CR1 &= ~(ADC_CR1_JDISCEN | ADC_CR1_SCAN);
}

/* Data alignment */

void adc_set_right_alignment(void)
{
	ADC_CR2 &= ~ADC_CR2_ALIGN;
}

void adc_set_left_alignment(void)
{
	ADC_CR2 |= ADC_CR2_ALIGN;
}

void adc_set_offset(int index, int offset)
{
	switch (index) {
	case 0:
		ADC_JOFR1 = (offset & 0xfff);
		break;
	case 1:
		ADC_JOFR2 = (offset & 0xfff);
		break;
	case 2:
		ADC_JOFR3 = (offset & 0xfff);
		break;
	case 3:
		ADC_JOFR4 = (offset & 0xfff);
		break;
	default:
		break;
	}
}

/* Channel-wise programmable sampling time */

void adc_set_sampling(int channel, int sampling)
{
	u32 smp;
	u32 reg32;

	if (sampling <= 4)
		smp = 0;
	else if (sampling <= 9)
		smp = 1;
	else if (sampling <= 16)
		smp = 2;
	else if (sampling <= 24)
		smp = 3;
	else if (sampling <= 48)
		smp = 4;
	else if (sampling <= 96)
		smp = 5;
	else if (sampling <= 192)
		smp = 6;
	else
		smp = 7;

	switch (channel / 10) {
	case 0:
		reg32 = ADC_SMPR3;
		reg32 &= ~(7 << (channel % 10 * 3));
		ADC_SMPR3 = (reg32 | (smp << (channel % 10 * 3)));
		break;
	case 1:
		reg32 = ADC_SMPR2;
		reg32 &= ~(7 << (channel % 10 * 3));
		ADC_SMPR2 = (reg32 | (smp << (channel % 10 * 3)));
		break;
	case 2:
		reg32 = ADC_SMPR1;
		reg32 &= ~(7 << (channel % 10 * 3));
		ADC_SMPR1 = (reg32 | (smp << (channel % 10 * 3)));
		break;
	case 3:
		reg32 = ADC_SMPR0;
		reg32 &= ~(7 << (channel % 10 * 3));
		ADC_SMPR0 = (reg32 | (smp << (channel % 10 * 3)));
		break;
	default:
		break;
	}
}

/* Conversion on external trigger */

void adc_set_regular_ext(adc_trigger_edge_t edge, adc_trigger_source_t source)
{
	u32 reg32;

	reg32 = ADC_CR2;
	reg32 &= ~(3 << 28);
	reg32 |= ((edge & 3) << 28);
	if (edge == ADC_TRIGGER_DISABLE) {
		ADC_CR2 = reg32;
		return;
	}

	reg32 &= ~(15 << 24);
	switch (source) {
	case ADC_TIM9_CC2:
		ADC_CR2 = (reg32 | (0 << 24));
		break;
	case ADC_TIM9_TRGO:
		ADC_CR2 = (reg32 | (1 << 24));
		break;
	case ADC_TIM2_CC3:
		ADC_CR2 = (reg32 | (2 << 24));
		break;
	case ADC_TIM2_CC2:
		ADC_CR2 = (reg32 | (3 << 24));
		break;
	case ADC_TIM3_TRGO:
		ADC_CR2 = (reg32 | (4 << 24));
		break;
	case ADC_TIM4_CC4:
		ADC_CR2 = (reg32 | (5 << 24));
		break;
	case ADC_TIM2_TRGO:
		ADC_CR2 = (reg32 | (6 << 24));
		break;
	case ADC_TIM3_CC1:
		ADC_CR2 = (reg32 | (7 << 24));
		break;
	case ADC_TIM3_CC3:
		ADC_CR2 = (reg32 | (8 << 24));
		break;
	case ADC_TIM4_TRGO:
		ADC_CR2 = (reg32 | (9 << 24));
		break;
	case ADC_TIM6_TRGO:
		ADC_CR2 = (reg32 | (10 << 24));
		break;
	case ADC_EXTI11:
		ADC_CR2 = (reg32 | (15 << 24));
		break;
	default:
		break;
	}
}

void adc_set_injected_ext(adc_trigger_edge_t edge, adc_trigger_source_t source)
{
	u32 reg32;

	reg32 = ADC_CR2;
	reg32 &= ~(3 << 20);
	reg32 |= (edge << 20);
	if (edge == ADC_TRIGGER_DISABLE) {
		ADC_CR2 = reg32;
		return;
	}

	reg32 &= ~(15 << 16);
	switch (source) {
	case ADC_TIM9_CC1:
		ADC_CR2 = (reg32 | (0 << 16));
		break;
	case ADC_TIM9_TRGO:
		ADC_CR2 = (reg32 | (1 << 16));
		break;
	case ADC_TIM2_TRGO:
		ADC_CR2 = (reg32 | (2 << 16));
		break;
	case ADC_TIM2_CC1:
		ADC_CR2 = (reg32 | (3 << 16));
		break;
	case ADC_TIM3_CC4:
		ADC_CR2 = (reg32 | (4 << 16));
		break;
	case ADC_TIM4_TRGO:
		ADC_CR2 = (reg32 | (5 << 16));
		break;
	case ADC_TIM4_CC1:
		ADC_CR2 = (reg32 | (6 << 16));
		break;
	case ADC_TIM4_CC2:
		ADC_CR2 = (reg32 | (7 << 16));
		break;
	case ADC_TIM4_CC3:
		ADC_CR2 = (reg32 | (8 << 16));
		break;
	case ADC_TIM10_CC1:
		ADC_CR2 = (reg32 | (9 << 16));
		break;
	case ADC_TIM7_TRGO:
		ADC_CR2 = (reg32 | (10 << 16));
		break;
	case ADC_EXTI15:
		ADC_CR2 = (reg32 | (15 << 16));
		break;
	default:
		break;
	}
}

/* Aborting a conversion */

void adc_wait_for_regular_ready(void)
{
	while (ADC_SR & ADC_SR_RCNR)
		;
}

void adc_wait_for_injected_ready(void)
{
	while (ADC_SR & ADC_SR_JCNR)
		;
}

/* Conversion resolution */

void adc_set_resolution(int res)
{
	u32 reg32;

	switch (res) {
	case 12:
		ADC_CR1 &= ~(ADC_CR1_RES1 | ADC_CR1_RES0);
		break;
	case 10:
		reg32 = ADC_CR1;
		reg32 &= ~ADC_CR1_RES1;
		ADC_CR1 = (reg32 | ADC_CR1_RES0);
		break;
	case 8:
		reg32 = ADC_CR1;
		reg32 |= ADC_CR1_RES1;
		ADC_CR1 = (reg32 & ~ADC_CR1_RES0);
		break;
	case 6:
		ADC_CR1 |= (ADC_CR1_RES1 | ADC_CR1_RES0);
		break;
	default:
		break;
	}
}

/* Hardware freeze and delay insertion modes for slow conversions */

void adc_set_delay(int delay)
{
	u32 dels;
	u32 reg32;

	if (delay == 0)
		dels = 0;	/* No delay */
	else if (delay == 1)
		dels = (1 << 4); /* freeze mode */
	else if (delay <= 7)
		dels = (2 << 4);
	else if (delay <= 15)
		dels = (3 << 4);
	else if (delay <= 31)
		dels = (4 << 4);
	else if (delay <= 63)
		dels = (5 << 4);
	else if (delay <= 127)
		dels = (6 << 4);
	else
		dels = (7 << 4);

	reg32 = ADC_CR2;
	reg32 &= ~(7 << 4);
	ADC_CR2 = (reg32 | dels);
}

/* Data management and overrun detection */

void adc_enable_dma(bool circular)
{
	u32 reg32;

	if (circular) {
		ADC_CR2 |= (ADC_CR2_DMA | ADC_CR2_DDS);
	} else {
		reg32 = ADC_CR2;
		reg32 &= ~ADC_CR2_DDS;
		ADC_CR2 = (reg32 | ADC_CR2_DMA);
	}
}

void adc_disable_dma(void)
{
	ADC_CR2 &= ~ADC_CR2_DMA;
}

/* Temperature sensor / Internal reference voltage (V_REFINT) coversion */

void adc_enable_ts_vref(void)
{
	ADC_CCR |= ADC_CCR_TSVREFE;
}

void adc_disable_ts_vref(void)
{
	ADC_CCR &= ~ADC_CCR_TSVREFE;
}

/* ADC interrupts */

void adc_enable_interrupt(int interrupt)
{
	u32 reg32;

	reg32 = ADC_CR1;
	if (interrupt & ADC_ANALOG_WATCHDOG)
		reg32 |= ADC_CR1_AWDIE;
	if (interrupt & ADC_REGULAR_END)
		reg32 |= ADC_CR1_EOCIE;
	if (interrupt & ADC_INJECTED_END)
		reg32 |= ADC_CR1_JEOCIE;
	if (interrupt & ADC_OVERRUN)
		reg32 |= ADC_CR1_OVRIE;
	ADC_CR1 = reg32;
}

void adc_disable_interrupt(int interrupt)
{
	u32 reg32;

	reg32 = ADC_CR1;
	if (interrupt & ADC_ANALOG_WATCHDOG)
		reg32 &= ~ADC_CR1_AWDIE;
	if (interrupt & ADC_REGULAR_END)
		reg32 &= ~ADC_CR1_EOCIE;
	if (interrupt & ADC_INJECTED_END)
		reg32 &= ~ADC_CR1_JEOCIE;
	if (interrupt & ADC_OVERRUN)
		reg32 &= ~ADC_CR1_OVRIE;
	ADC_CR1 = reg32;
}

int adc_get_interrupt_mask(int interrupt)
{
	u32 reg32;
	u32 r = 0;

	reg32 = ADC_CR1;
	if ((interrupt & ADC_ANALOG_WATCHDOG) &&
	    (reg32 & ADC_CR1_AWDIE))
		r |= ADC_ANALOG_WATCHDOG;
	if ((interrupt & ADC_REGULAR_END) &&
	    (reg32 & ADC_CR1_EOCIE))
		r |= ADC_REGULAR_END;
	if ((interrupt & ADC_INJECTED_END) &&
	    (reg32 & ADC_CR1_JEOCIE))
		r |= ADC_INJECTED_END;
	if ((interrupt & ADC_OVERRUN) &&
	    (reg32 & ADC_CR1_OVRIE))
		r |= ADC_OVERRUN;

	return r;
}

int adc_get_interrupt_status(int interrupt)
{
	return ADC_SR & interrupt;
}

void adc_clear_interrupt(int interrupt)
{
	ADC_SR &= ~interrupt;
}
