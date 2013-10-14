/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Edward Cheeseman <evbuilder@users.sourceforge.org>
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

#include <stm32/l1/tim.h>

static u32 ccmr1[MAXTIM];
static u32 ccmr2[MAXTIM];
static u32 ccer[MAXTIM];

static u32 base_addr(tim_t tim)
{
	switch (tim & ~3) {
	case TIM2:
		return TIM2_BASE;
	case TIM3:
		return TIM3_BASE;
	case TIM4:
		return TIM4_BASE;
	case TIM5:
		return TIM5_BASE;

	case TIM6:
		return TIM6_BASE;
	case TIM7:
		return TIM7_BASE;

	case TIM9:
		return TIM9_BASE;
	case TIM10:
		return TIM10_BASE;
	case TIM11:
		return TIM11_BASE;

	default:
		break;
	}
	return 0;
}

static int tim_index(tim_t tim)
{
	switch (tim & ~3) {
	case TIM2:
		return 0;
	case TIM3:
		return 1;
	case TIM4:
		return 2;
	case TIM5:
		return 3;
	case TIM6:
		return 4;
	case TIM7:
		return 5;
	case TIM9:
		return 6;
	case TIM10:
		return 7;
	case TIM11:
		return 8;
	default:
		break;
	}
	return 0;
}

/* Time-base unit */

void tim_setup_counter(tim_t tim, int prescaler, int autoreload)
{
	u32 base;

	base = base_addr(tim);
	TIM_ARR(base) = autoreload;
	TIM_PSC(base) = prescaler;
	TIM_EGR(base) = TIM_EGR_UG;
}

void tim_load_prescaler_value(tim_t tim, int prescaler)
{
	u32 base;

	base = base_addr(tim);
	TIM_PSC(base) = prescaler;
	TIM_EGR(base) = TIM_EGR_UG;
}

void tim_set_autoreload_value(tim_t tim, int autoreload)
{
	TIM_ARR(base_addr(tim)) = autoreload;
}

void tim_set_counter(tim_t tim, int count)
{
	TIM_CNT(base_addr(tim)) = count;
}

int tim_get_counter(tim_t tim)
{
	return TIM_CNT(base_addr(tim));
}

void tim_enable_counter(tim_t tim)
{
	TIM_CR1(base_addr(tim)) |= TIM_CR1_CEN;
}

void tim_disable_counter(tim_t tim)
{
	TIM_CR1(base_addr(tim)) &= ~TIM_CR1_CEN;
}

void tim_enable_preload(tim_t tim)
{
	TIM_CR1(base_addr(tim)) |= TIM_CR1_ARPE;
}

void tim_disable_preload(tim_t tim)
{
	TIM_CR1(base_addr(tim)) &= ~TIM_CR1_ARPE;
}

void tim_enable_interrupt(tim_t tim, int interrupt)
{
	TIM_DIER(base_addr(tim)) |= interrupt;
}

void tim_disable_interrupt(tim_t tim, int interrupt)
{
	TIM_DIER(base_addr(tim)) &= ~interrupt;
}

int tim_get_interrupt_mask(tim_t tim, int interrupt)
{
	return TIM_DIER(base_addr(tim)) & interrupt;
}

int tim_get_interrupt_status(tim_t tim, int interrupt)
{
	return TIM_SR(base_addr(tim)) & interrupt;
}

void tim_clear_interrupt(tim_t tim, int interrupt)
{
	TIM_SR(base_addr(tim)) &= ~interrupt;
}

void tim_generate_event(tim_t tim, int event)
{
	TIM_EGR(base_addr(tim)) = event;
}

/* Counter modes */

void tim_set_upcounting_mode(tim_t tim)
{
	u32 base;

	base = base_addr(tim);
	/* Edge-aligned mode */
	TIM_CR1(base) &= ~(TIM_CR1_CMS1 | TIM_CR1_CMS0);
	/* Upcounter */
	TIM_CR1(base) &= ~TIM_CR1_DIR;
}

void tim_set_downcounting_mode(tim_t tim)
{
	u32 base;

	base = base_addr(tim);
	/* Edge-aligned mode */
	TIM_CR1(base) &= ~(TIM_CR1_CMS1 | TIM_CR1_CMS0);
	/* Downcounter */
	TIM_CR1(base) |= TIM_CR1_DIR;
}

void tim_set_center_aligned_mode(tim_t tim, tim_center_aligned_mode_t mode)
{
	u32 base;
	u32 reg32;

	base = base_addr(tim);
	reg32 = TIM_CR1(base);
	reg32 &= ~(TIM_CR1_CMS1 | TIM_CR1_CMS0);
	reg32 |= mode;
	TIM_CR1(base) = reg32;
}

void tim_disable_update_event(tim_t tim)
{
	TIM_CR1(base_addr(tim)) |= TIM_CR1_UDIS;
}

void tim_enable_update_event(tim_t tim)
{
	TIM_CR1(base_addr(tim)) &= ~TIM_CR1_UDIS;
}

void tim_disable_update_interrupt_on_any(tim_t tim)
{
	TIM_CR1(base_addr(tim)) |= TIM_CR1_URS;
}

void tim_enable_update_interrupt_on_any(tim_t tim)
{
	TIM_CR1(base_addr(tim)) &= ~TIM_CR1_URS;
}

/* Capture/compare */

void tim_set_capture_compare_mode(tim_cc_t tim_cc, int mode)
{
	int index;

	index = tim_index(tim_cc);
	switch (tim_cc & 3) {
	case 0:
		ccmr1[index] |= ((mode & 0xff) | (1 << 16));
		ccer[index] |= ((mode & 0xf00) >> 8);
		break;
	case 1:
		ccmr1[index] |= (((mode & 0xff) << 8) | (1 << 16));
		ccer[index] |= ((mode & 0xf00) >> 4);
		break;
	case 2:
		ccmr2[index] |= ((mode & 0xff) | (1 << 16));
		ccer[index] |= (mode & 0xf00);
		break;
	case 3:
		ccmr2[index] |= (((mode & 0xff) << 8) | (1 << 16));
		ccer[index] |= ((mode & 0xf00) << 4);
		break;
	default:
		break;
	}
}

void tim_start_capture_compare(tim_t tim)
{
	u32 base;
	int index;

	base = base_addr(tim);
	index = tim_index(tim);

	if (ccmr1[index] & (1 << 16)) {
		TIM_CCMR1(base) = (ccmr1[index] & 0xffff);
		ccmr1[index] = 0;
	}
	if (ccmr2[index] & (1 << 16)) {
		TIM_CCMR2(base) = (ccmr2[index] & 0xffff);
		ccmr2[index] = 0;
	}
	TIM_CCER(base) = ccer[index];
	ccer[index] = 0;
}

void tim_set_dts_division(tim_t tim, tim_clock_div_t div)
{
	u32 base;
	u32 reg32;

	base = base_addr(tim);
	reg32 = TIM_CR1(base);
	reg32 &= ~(TIM_CR1_CKD1 | TIM_CR1_CKD0);
	reg32 |= div;
	TIM_CR1(base) = reg32;
}

void tim_set_capture_compare_value(tim_cc_t tim_cc, int value)
{
	u32 base;

	base = base_addr(tim_cc);
	switch (tim_cc & 3) {
	case 0:
		TIM_CCR1(base) = value;
		break;
	case 1:
		TIM_CCR2(base) = value;
		break;
	case 2:
		TIM_CCR3(base) = value;
		break;
	case 3:
		TIM_CCR4(base) = value;
		break;
	default:
		break;
	}
}

int tim_get_capture_compare_value(tim_cc_t tim_cc)
{
	u32 base;
	u32 reg32;

	base = base_addr(tim_cc);
	switch (tim_cc & 3) {
	case 0:
		reg32 = TIM_CCR1(base);
		break;
	case 1:
		reg32 = TIM_CCR2(base);
		break;
	case 2:
		reg32 = TIM_CCR3(base);
		break;
	case 3:
		reg32 = TIM_CCR4(base);
		break;
	default:
		reg32 = 0;
		break;
	}
	return reg32;
}

/* One-pulse mode */

void tim_enable_one_pulse_mode(tim_t tim)
{
	TIM_CR1(base_addr(tim)) |= TIM_CR1_OPM;
}

void tim_disable_one_pulse_mode(tim_t tim)
{
	TIM_CR1(base_addr(tim)) &= ~TIM_CR1_OPM;
}

/* Timer input XOR function */

void tim_enable_ti1_ch123_xor(tim_t tim)
{
	TIM_CR2(base_addr(tim)) |= TIM_CR2_TI1S;
}

void tim_disable_ti1_ch123_xor(tim_t tim)
{
	TIM_CR2(base_addr(tim)) &= ~TIM_CR2_TI1S;
}

/* Master mode */

void tim_set_master_mode(tim_t tim, tim_master_mode_t mode)
{
	u32 base;
	u32 reg32;

	base = base_addr(tim);
	reg32 = TIM_CR2(base);
	reg32 &= ~(TIM_CR2_MMS2 | TIM_CR2_MMS1 | TIM_CR2_MMS0);
	reg32 |= mode;
	TIM_CR2(base) = reg32;
}

/* Slave mode */

void tim_set_slave_mode(tim_t tim, int mode)
{
	u32 base;
	u32 reg32;

	base = base_addr(tim);
	reg32 = TIM_SMCR(base);
	if ((reg32 & (TIM_SMCR_TS2 | TIM_SMCR_TS1 | TIM_SMCR_TS0)) !=
	    (mode & (TIM_SMCR_TS2 | TIM_SMCR_TS1 | TIM_SMCR_TS0)) &&
	    (reg32 & (TIM_SMCR_SMS2 | TIM_SMCR_SMS1 | TIM_SMCR_SMS0)) &&
	    (mode & (TIM_SMCR_SMS2 | TIM_SMCR_SMS1 | TIM_SMCR_SMS0)))
		TIM_SMCR(base) = reg32 & ~(TIM_SMCR_SMS2 | TIM_SMCR_SMS1 |
					   TIM_SMCR_SMS0);
	TIM_SMCR(base) = mode;
}

/* DMA */

void tim_enable_ccx_dma_on_update_event(tim_t tim)
{
	TIM_CR2(base_addr(tim)) |= TIM_CR2_CCDS;
}

void tim_disable_ccx_dma_on_update_event(tim_t tim)
{
	TIM_CR2(base_addr(tim)) &= ~TIM_CR2_CCDS;
}

void tim_setup_dma(tim_t tim, int dba, int dbl)
{
	TIM_DCR(base_addr(tim)) = ((dbl & 0x1f) << 8) | (dba & 0x1f);
}
