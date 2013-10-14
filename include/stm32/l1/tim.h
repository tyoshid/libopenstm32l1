/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Piotr Esden-Tempski <piotr@esden.net>
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

#include <stm32/l1/memorymap.h>
#include <libopencm3.h>

/*
 * STMicroelectronics(www.st.com)
 *
 * RM0038: STM32L100xx, STM32L151xx, STM32L152xx and STM32L162xx
 *         advanced ARM-based 32-bit MCUs
 * (19-Apr-2013 Rev 8)
 *
 * 16 General-purpose timers (TIM2 to TIM5)
 * 17 General-purpose timers (TIM9/10/11)
 * 18 Baseic timers (TIM6&TIM7)
 */

/*
 * Note: Registers and bits marked (**) only exist in high and medium+ density
 * devices.
 */

/* --- TIM registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		TIMx_CR1	TIMx control register 1
 * 0x04		TIMx_CR2	TIMx control register 2
 * 0x08		TIMx_SMCR	TIMx slave mode control register
 * 0x0c		TIMx_DIER	TIMx DMA/Interrupt enable register
 * 0x10		TIMx_SR		TIMx status register
 * 0x14		TIMx_EGR	TIMx event generation register
 * 0x18		TIMx_CCMR1	TIMx capture/compare mode register 1
 * 0x1c		TIMx_CCMR2	TIMx capture/compare mode register 2
 * 0x20		TIMx_CCER	TIMx capture/compare enable register
 * 0x24		TIMx_CNT	TIMx counter
 * 0x28		TIMx_PSC	TIMx prescaler
 * 0x2c		TIMx_ARR	TIMx auto-reload register
 * 0x34		TIMx_CCR1	TIMx capture/compare register 1
 * 0x38		TIMx_CCR2	TIMx capture/compare register 2
 * 0x3c		TIMx_CCR3	TIMx capture/compare register 3
 * 0x40		TIMx_CCR4	TIMx capture/compare register 4
 * 0x48		TIMx_DCR	TIMx DMA control register
 * 0x4c		TIMx_DMAR	TIMx DMA address for full transfer
 * 0x50		TIMx_OR		TIMx option register
 */

/* TIMx_CR1 */
/* x=2..7,9..11 */
#define TIM_CR1(tim_base)               MMIO32((tim_base) + 0x00)
#define TIM2_CR1			TIM_CR1(TIM2_BASE)
#define TIM3_CR1			TIM_CR1(TIM3_BASE)
#define TIM4_CR1			TIM_CR1(TIM4_BASE)
#define TIM5_CR1			TIM_CR1(TIM5_BASE)	/* (**) */
#define TIM6_CR1			TIM_CR1(TIM6_BASE)
#define TIM7_CR1			TIM_CR1(TIM7_BASE)
#define TIM9_CR1			TIM_CR1(TIM9_BASE)
#define TIM10_CR1			TIM_CR1(TIM10_BASE)
#define TIM11_CR1			TIM_CR1(TIM11_BASE)

/* TIMx_CR2 */
/* x=2..7,9 */
#define TIM_CR2(tim_base)		MMIO32((tim_base) + 0x04)
#define TIM2_CR2			TIM_CR2(TIM2_BASE)
#define TIM3_CR2			TIM_CR2(TIM3_BASE)
#define TIM4_CR2			TIM_CR2(TIM4_BASE)
#define TIM5_CR2			TIM_CR2(TIM5_BASE)	/* (**) */
#define TIM6_CR2			TIM_CR2(TIM6_BASE)
#define TIM7_CR2			TIM_CR2(TIM7_BASE)
#define TIM9_CR2			TIM_CR2(TIM9_BASE)

/* TIMx_SMCR */
/* x=2..5,9..11 */
#define TIM_SMCR(tim_base)		MMIO32((tim_base) + 0x08)
#define TIM2_SMCR			TIM_SMCR(TIM2_BASE)
#define TIM3_SMCR			TIM_SMCR(TIM3_BASE)
#define TIM4_SMCR			TIM_SMCR(TIM4_BASE)
#define TIM5_SMCR			TIM_SMCR(TIM5_BASE)	/* (**) */
#define TIM9_SMCR			TIM_SMCR(TIM9_BASE)
#define TIM10_SMCR			TIM_SMCR(TIM10_BASE)
#define TIM11_SMCR			TIM_SMCR(TIM11_BASE)

/* TIMx_DIER */
/* x=2..7,9..11 */
#define TIM_DIER(tim_base)		MMIO32((tim_base) + 0x0C)
#define TIM2_DIER			TIM_DIER(TIM2_BASE)
#define TIM3_DIER			TIM_DIER(TIM3_BASE)
#define TIM4_DIER			TIM_DIER(TIM4_BASE)
#define TIM5_DIER			TIM_DIER(TIM5_BASE)	/* (**) */
#define TIM6_DIER			TIM_DIER(TIM6_BASE)
#define TIM7_DIER			TIM_DIER(TIM7_BASE)
#define TIM9_DIER			TIM_DIER(TIM9_BASE)
#define TIM10_DIER			TIM_DIER(TIM10_BASE)
#define TIM11_DIER			TIM_DIER(TIM11_BASE)

/* TIMx_SR */
/* x=2..7,9..11 */
#define TIM_SR(tim_base)		MMIO32((tim_base) + 0x10)
#define TIM2_SR				TIM_SR(TIM2_BASE)
#define TIM3_SR				TIM_SR(TIM3_BASE)
#define TIM4_SR				TIM_SR(TIM4_BASE)
#define TIM5_SR	       			TIM_SR(TIM5_BASE)	/* (**) */
#define TIM6_SR				TIM_SR(TIM6_BASE)
#define TIM7_SR				TIM_SR(TIM7_BASE)
#define TIM9_SR				TIM_SR(TIM9_BASE)
#define TIM10_SR			TIM_SR(TIM10_BASE)
#define TIM11_SR			TIM_SR(TIM11_BASE)

/* TIMx_EGR */
/* x=2..7,9..11 */
#define TIM_EGR(tim_base)		MMIO32((tim_base) + 0x14)
#define TIM2_EGR			TIM_EGR(TIM2_BASE)
#define TIM3_EGR			TIM_EGR(TIM3_BASE)
#define TIM4_EGR			TIM_EGR(TIM4_BASE)
#define TIM5_EGR			TIM_EGR(TIM5_BASE)	/* (**) */
#define TIM6_EGR			TIM_EGR(TIM6_BASE)
#define TIM7_EGR			TIM_EGR(TIM7_BASE)
#define TIM9_EGR			TIM_EGR(TIM9_BASE)
#define TIM10_EGR			TIM_EGR(TIM10_BASE)
#define TIM11_EGR			TIM_EGR(TIM11_BASE)

/* TIMx_CCMR1 */
/* x=2..5,9..11 */
#define TIM_CCMR1(tim_base)		MMIO32((tim_base) + 0x18)
#define TIM2_CCMR1			TIM_CCMR1(TIM2_BASE)
#define TIM3_CCMR1			TIM_CCMR1(TIM3_BASE)
#define TIM4_CCMR1			TIM_CCMR1(TIM4_BASE)
#define TIM5_CCMR1			TIM_CCMR1(TIM5_BASE)	/* (**) */
#define TIM9_CCMR1			TIM_CCMR1(TIM9_BASE)
#define TIM10_CCMR1			TIM_CCMR1(TIM10_BASE)
#define TIM11_CCMR1			TIM_CCMR1(TIM11_BASE)

/* TIMx_CCMR2 */
/* x=2..5 */
#define TIM_CCMR2(tim_base)		MMIO32((tim_base) + 0x1C)
#define TIM2_CCMR2			TIM_CCMR2(TIM2_BASE)
#define TIM3_CCMR2			TIM_CCMR2(TIM3_BASE)
#define TIM4_CCMR2			TIM_CCMR2(TIM4_BASE)
#define TIM5_CCMR2			TIM_CCMR2(TIM5_BASE)	/* (**) */

/* TIMx_CCER */
/* x=2..5,9..11 */
#define TIM_CCER(tim_base)		MMIO32((tim_base) + 0x20)
#define TIM2_CCER			TIM_CCER(TIM2_BASE)
#define TIM3_CCER			TIM_CCER(TIM3_BASE)
#define TIM4_CCER			TIM_CCER(TIM4_BASE)
#define TIM5_CCER			TIM_CCER(TIM5_BASE)	/* (**) */
#define TIM9_CCER			TIM_CCER(TIM9_BASE)
#define TIM10_CCER			TIM_CCER(TIM10_BASE)
#define TIM11_CCER			TIM_CCER(TIM11_BASE)

/* TIMx_CNT */
/* x=2..7,9..11 */
#define TIM_CNT(tim_base)		MMIO32((tim_base) + 0x24)
#define TIM2_CNT			TIM_CNT(TIM2_BASE)
#define TIM3_CNT			TIM_CNT(TIM3_BASE)
#define TIM4_CNT			TIM_CNT(TIM4_BASE)
#define TIM5_CNT			TIM_CNT(TIM5_BASE)	/* (**) 32bit */
#define TIM6_CNT			TIM_CNT(TIM6_BASE)
#define TIM7_CNT			TIM_CNT(TIM7_BASE)
#define TIM9_CNT			TIM_CNT(TIM9_BASE)
#define TIM10_CNT			TIM_CNT(TIM10_BASE)
#define TIM11_CNT			TIM_CNT(TIM11_BASE)

/* TIMx_PSC */
/* x=2..7,9..11 */
#define TIM_PSC(tim_base)		MMIO32((tim_base) + 0x28)
#define TIM2_PSC			TIM_PSC(TIM2_BASE)
#define TIM3_PSC			TIM_PSC(TIM3_BASE)
#define TIM4_PSC			TIM_PSC(TIM4_BASE)
#define TIM5_PSC			TIM_PSC(TIM5_BASE)	/* (**) */
#define TIM6_PSC			TIM_PSC(TIM6_BASE)
#define TIM7_PSC			TIM_PSC(TIM7_BASE)
#define TIM9_PSC			TIM_PSC(TIM9_BASE)
#define TIM10_PSC			TIM_PSC(TIM10_BASE)
#define TIM11_PSC			TIM_PSC(TIM11_BASE)

/* TIMx_ARR */
/* x=2..7,9..11 */
#define TIM_ARR(tim_base)		MMIO32((tim_base) + 0x2C)
#define TIM2_ARR			TIM_ARR(TIM2_BASE)
#define TIM3_ARR			TIM_ARR(TIM3_BASE)
#define TIM4_ARR			TIM_ARR(TIM4_BASE)
#define TIM5_ARR			TIM_ARR(TIM5_BASE)	/* (**) 32bit */
#define TIM6_ARR			TIM_ARR(TIM6_BASE)
#define TIM7_ARR			TIM_ARR(TIM7_BASE)
#define TIM9_ARR			TIM_ARR(TIM9_BASE)
#define TIM10_ARR			TIM_ARR(TIM10_BASE)
#define TIM11_ARR			TIM_ARR(TIM11_BASE)

/* TIMx_CCR1 */
/* x=2..5,9..11 */
#define TIM_CCR1(tim_base)		MMIO32((tim_base) + 0x34)
#define TIM2_CCR1			TIM_CCR1(TIM2_BASE)
#define TIM3_CCR1			TIM_CCR1(TIM3_BASE)
#define TIM4_CCR1			TIM_CCR1(TIM4_BASE)
#define TIM5_CCR1			TIM_CCR1(TIM5_BASE)	/* (**) 32bit */
#define TIM9_CCR1			TIM_CCR1(TIM9_BASE)
#define TIM10_CCR1			TIM_CCR1(TIM10_BASE)
#define TIM11_CCR1			TIM_CCR1(TIM11_BASE)

/* TIMx_CCR2 */
/* x=2..5,9 */
#define TIM_CCR2(tim_base)		MMIO32((tim_base) + 0x38)
#define TIM2_CCR2			TIM_CCR2(TIM2_BASE)
#define TIM3_CCR2			TIM_CCR2(TIM3_BASE)
#define TIM4_CCR2			TIM_CCR2(TIM4_BASE)
#define TIM5_CCR2			TIM_CCR2(TIM5_BASE)	/* (**) 32bit */
#define TIM9_CCR2			TIM_CCR2(TIM9_BASE)

/* TIMx_CCR3 */
/* x=2..5 */
#define TIM_CCR3(tim_base)		MMIO32((tim_base) + 0x3C)
#define TIM2_CCR3			TIM_CCR3(TIM2_BASE)
#define TIM3_CCR3			TIM_CCR3(TIM3_BASE)
#define TIM4_CCR3			TIM_CCR3(TIM4_BASE)
#define TIM5_CCR3			TIM_CCR3(TIM5_BASE)	/* (**) 32bit */

/* TIMx_CCR4 */
/* x=2..5 */
#define TIM_CCR4(tim_base)		MMIO32((tim_base) + 0x40)
#define TIM2_CCR4			TIM_CCR4(TIM2_BASE)
#define TIM3_CCR4			TIM_CCR4(TIM3_BASE)
#define TIM4_CCR4			TIM_CCR4(TIM4_BASE)
#define TIM5_CCR4			TIM_CCR4(TIM5_BASE)	/* (**) 32bit */

/* TIMx_DCR */
/* x=2..5 */
#define TIM_DCR(tim_base)		MMIO32((tim_base) + 0x48)
#define TIM2_DCR			TIM_DCR(TIM2_BASE)
#define TIM3_DCR			TIM_DCR(TIM3_BASE)
#define TIM4_DCR			TIM_DCR(TIM4_BASE)
#define TIM5_DCR			TIM_DCR(TIM5_BASE)	/* (**) */

/* TIMx_DMAR */
/* x=2..5 */
#define TIM_DMAR(tim_base)		MMIO32((tim_base) + 0x4C)
#define TIM2_DMAR			TIM_DMAR(TIM2_BASE)
#define TIM3_DMAR			TIM_DMAR(TIM3_BASE)
#define TIM4_DMAR			TIM_DMAR(TIM4_BASE)
#define TIM5_DMAR			TIM_DMAR(TIM5_BASE)	/* (**) */

/* TIMx_OR */
/* x=2,3,9..11 */
#define TIM_OR(tim_base)		MMIO32((tim_base) + 0x50)
#define TIM2_OR				TIM_OR(TIM2_BASE)	/* (**) */
#define TIM3_OR				TIM_OR(TIM3_BASE)	/* (**) */
#define TIM9_OR				TIM_OR(TIM9_BASE)
#define TIM10_OR			TIM_OR(TIM10_BASE)
#define TIM11_OR			TIM_OR(TIM11_BASE)

/* --- TIMx_CR1 values ----------------------------------------------------- */

#define TIM_CR1_CKD1			(1 << 9)	/* (x=2..5,9..11) */
#define TIM_CR1_CKD0			(1 << 8)	/* (x=2..5,9..11) */
#define TIM_CR1_ARPE			(1 << 7)	/* (x=2..7,9..11) */
#define TIM_CR1_CMS1			(1 << 6)	/* (x=2..5,9(**)) */
#define TIM_CR1_CMS0			(1 << 5)	/* (x=2..5,9(**)) */
#define TIM_CR1_DIR			(1 << 4)	/* (x=2..5,9(**)) */
#define TIM_CR1_OPM			(1 << 3)	/* (x=2..7,9) */
#define TIM_CR1_URS			(1 << 2)	/* (x=2..7,9..11) */
#define TIM_CR1_UDIS			(1 << 1)	/* (x=2..7,9..11) */
#define TIM_CR1_CEN			(1 << 0)	/* (x=2..7,9..11) */

/* Clock division */
#define TIM_CR1_CKD_CK_INT		(0 << 8)
#define TIM_CR1_CKD_CK_INT_2		(1 << 8)
#define TIM_CR1_CKD_CK_INT_4		(2 << 8)

/* Center-aligned mode selection */
#define TIM_CR1_CMS_EDGE		(0 << 5)
#define TIM_CR1_CMS_CENTER_1		(1 << 5)
#define TIM_CR1_CMS_CENTER_2		(2 << 5)
#define TIM_CR1_CMS_CENTER_3		(3 << 5)

/* --- TIMx_CR2 values ----------------------------------------------------- */

#define TIM_CR2_TI1S			(1 << 7)	/* (x=2..5) */
#define TIM_CR2_MMS2			(1 << 6)	/* (x=2..7,9) */
#define TIM_CR2_MMS1			(1 << 5)	/* (x=2..7,9) */
#define TIM_CR2_MMS0			(1 << 4)	/* (x=2..7,9) */
#define TIM_CR2_CCDS			(1 << 3)	/* (x=2..5) */

/* Master mode selection */
#define TIM_CR2_MMS_RESET		(0 << 4)	/* (x=2..7,9) */
#define TIM_CR2_MMS_ENABLE		(1 << 4)	/* (x=2..7,9) */
#define TIM_CR2_MMS_UPDATE		(2 << 4)	/* (x=2..7,9) */
#define TIM_CR2_MMS_COMPARE_PULSE	(3 << 4)	/* (x=2..5,9) */
#define TIM_CR2_MMS_COMPARE_OC1REF	(4 << 4)	/* (x=2..5,9) */
#define TIM_CR2_MMS_COMPARE_OC2REF	(5 << 4)	/* (x=2..5,9) */
#define TIM_CR2_MMS_COMPARE_OC3REF	(6 << 4)	/* (x=2..5) */
#define TIM_CR2_MMS_COMPARE_OC4REF	(7 << 4)	/* (x=2..5) */

/* --- TIMx_SMCR values ---------------------------------------------------- */

#define TIM_SMCR_ETP			(1 << 15)	/* (x=2..5,9..11) */
#define TIM_SMCR_ECE			(1 << 14)	/* (x=2..5,9..11) */
#define TIM_SMCR_ETPS1			(1 << 13)	/* (x=2..5,9..11) */
#define TIM_SMCR_ETPS0			(1 << 12)	/* (x=2..5,9..11) */
#define TIM_SMCR_ETF3			(1 << 11)	/* (x=2..5,9..11) */
#define TIM_SMCR_ETF2			(1 << 10)	/* (x=2..5,9..11) */
#define TIM_SMCR_ETF1			(1 << 9)	/* (x=2..5,9..11) */
#define TIM_SMCR_ETF0			(1 << 8)	/* (x=2..5,9..11) */
#define TIM_SMCR_MSM			(1 << 7)	/* (x=2..5,9) */
#define TIM_SMCR_TS2			(1 << 6)	/* (x=2..5,9) */
#define TIM_SMCR_TS1			(1 << 5)	/* (x=2..5,9) */
#define TIM_SMCR_TS0			(1 << 4)	/* (x=2..5,9) */
#define TIM_SMCR_OCCS			(1 << 3)	/* (x=2..5) */
#define TIM_SMCR_SMS2			(1 << 2)	/* (x=2..5,9) */
#define TIM_SMCR_SMS1			(1 << 1)	/* (x=2..5,9) */
#define TIM_SMCR_SMS0			(1 << 0)	/* (x=2..5,9) */

/* External trigger prescaler */
#define TIM_SMCR_ETPS_OFF		(0 << 12)
#define TIM_SMCR_ETPS_ETRP_2		(1 << 12)
#define TIM_SMCR_ETPS_ETRP_4		(2 << 12)
#define TIM_SMCR_ETPS_ETRP_8		(3 << 12)

/* External trigger filter */
#define TIM_SMCR_ETF_OFF		(0 << 8)
#define TIM_SMCR_ETF_CK_INT_N_2		(1 << 8)
#define TIM_SMCR_ETF_CK_INT_N_4		(2 << 8)
#define TIM_SMCR_ETF_CK_INT_N_8		(3 << 8)
#define TIM_SMCR_ETF_DTS_2_N_6		(4 << 8)
#define TIM_SMCR_ETF_DTS_2_N_8		(5 << 8)
#define TIM_SMCR_ETF_DTS_4_N_6		(6 << 8)
#define TIM_SMCR_ETF_DTS_4_N_8		(7 << 8)
#define TIM_SMCR_ETF_DTS_8_N_6		(8 << 8)
#define TIM_SMCR_ETF_DTS_8_N_8		(9 << 8)
#define TIM_SMCR_ETF_DTS_16_N_5		(10 << 8)
#define TIM_SMCR_ETF_DTS_16_N_6		(11 << 8)
#define TIM_SMCR_ETF_DTS_16_N_8		(12 << 8)
#define TIM_SMCR_ETF_DTS_32_N_5		(13 << 8)
#define TIM_SMCR_ETF_DTS_32_N_6		(14 << 8)
#define TIM_SMCR_ETF_DTS_32_N_8		(15 << 8)

/* Trigger selection */
#define TIM_SMCR_TS_ITR0		(0 << 4)	/* (x=2..5,9) */
#define TIM_SMCR_TS_ITR1		(1 << 4)	/* (x=2..5,9) */
#define TIM_SMCR_TS_ITR2		(2 << 4)	/* (x=2..5,9) */
#define TIM_SMCR_TS_ITR3		(3 << 4)	/* (x=2..5,9) */
#define TIM_SMCR_TS_TI1F_ED		(4 << 4)	/* (x=2..5,9) */
#define TIM_SMCR_TS_TI1FP1		(5 << 4)	/* (x=2..5,9) */
#define TIM_SMCR_TS_TI1FP2		(6 << 4)	/* (x=2..5,9) */
#define TIM_SMCR_TS_ETRF		(7 << 4)	/* (x=2..5) */

/* Slave mode selection */
#define TIM_SMCR_SMS_OFF		(0 << 0)
#define TIM_SMCR_SMS_ENCODER_1		(1 << 0)
#define TIM_SMCR_SMS_ENCODER_2		(2 << 0)
#define TIM_SMCR_SMS_ENCODER_3		(3 << 0)
#define TIM_SMCR_SMS_RESET		(4 << 0)
#define TIM_SMCR_SMS_GATED		(5 << 0)
#define TIM_SMCR_SMS_TRIGGER		(6 << 0)
#define TIM_SMCR_SMS_EXTERNAL_CLOCK_1	(7 << 0)

/* --- TIMx_DIER values ---------------------------------------------------- */

#define TIM_DIER_TDE			(1 << 14)	/* (x=2..5) */
#define TIM_DIER_COMDE			(1 << 13)	/* (x=5?) */
#define TIM_DIER_CC4DE			(1 << 12)	/* (x=2..5) */
#define TIM_DIER_CC3DE			(1 << 11)	/* (x=2..5) */
#define TIM_DIER_CC2DE			(1 << 10)	/* (x=2..5) */
#define TIM_DIER_CC1DE			(1 << 9)	/* (x=2..5) */
#define TIM_DIER_UDE			(1 << 8)	/* (x=2..7) */
#define TIM_DIER_TIE			(1 << 6)	/* (x=2..5,9) */
#define TIM_DIER_CC4IE			(1 << 4)	/* (x=2..5) */
#define TIM_DIER_CC3IE			(1 << 3)	/* (x=2..5) */
#define TIM_DIER_CC2IE			(1 << 2)	/* (x=2..5,9) */
#define TIM_DIER_CC1IE			(1 << 1)	/* (x=2..5,9..11) */
#define TIM_DIER_UIE			(1 << 0)	/* (x=2..7,9..11) */

/* --- TIMx_SR values ------------------------------------------------------ */

#define TIM_SR_CC4OF			(1 << 12)	/* (x=2..5) */
#define TIM_SR_CC3OF			(1 << 11)	/* (x=2..5) */
#define TIM_SR_CC2OF			(1 << 10)	/* (x=2..5,9) */
#define TIM_SR_CC1OF			(1 << 9)	/* (x=2..5,9..11) */
#define TIM_SR_TIF			(1 << 6)	/* (x=2..5,9) */
#define TIM_SR_CC4IF			(1 << 4)	/* (x=2..5) */
#define TIM_SR_CC3IF			(1 << 3)	/* (x=2..5) */
#define TIM_SR_CC2IF			(1 << 2)	/* (x=2..5,9) */
#define TIM_SR_CC1IF			(1 << 1)	/* (x=2..5,9..11) */
#define TIM_SR_UIF			(1 << 0)	/* (x=2..7,9..11) */

/* --- TIMx_EGR values ----------------------------------------------------- */

#define TIM_EGR_TG			(1 << 6)	/* (x=2..5,9) */
#define TIM_EGR_CC4G			(1 << 4)	/* (x=2..5) */
#define TIM_EGR_CC3G			(1 << 3)	/* (x=2..5) */
#define TIM_EGR_CC2G			(1 << 2)	/* (x=2..5,9) */
#define TIM_EGR_CC1G			(1 << 1)	/* (x=2..5,9..11) */
#define TIM_EGR_UG			(1 << 0)	/* (x=2..7,9..11) */

/* --- TIMx_CCMR1 values --------------------------------------------------- */

/* --- Output compare mode --- */

#define TIM_CCMR1_OC2CE			(1 << 15)	/* (x=2..5,9) */
#define TIM_CCMR1_OC2M2			(1 << 14)	/* (x=2..5,9) */
#define TIM_CCMR1_OC2M1			(1 << 13)	/* (x=2..5,9) */
#define TIM_CCMR1_OC2M0			(1 << 12)	/* (x=2..5,9) */
#define TIM_CCMR1_OC2PE			(1 << 11)	/* (x=2..5,9) */
#define TIM_CCMR1_OC2FE			(1 << 10)	/* (x=2..5,9) */
#define TIM_CCMR1_CC2S1			(1 << 9)	/* (x=2..5,9) */
#define TIM_CCMR1_CC2S0			(1 << 8)	/* (x=2..5,9) */
#define TIM_CCMR1_OC1CE			(1 << 7)	/* (x=2..5,9..11) */
#define TIM_CCMR1_OC1M2			(1 << 6)	/* (x=2..5,9..11) */
#define TIM_CCMR1_OC1M1			(1 << 5)	/* (x=2..5,9..11) */
#define TIM_CCMR1_OC1M0			(1 << 4)	/* (x=2..5,9..11) */
#define TIM_CCMR1_OC1PE			(1 << 3)	/* (x=2..5,9..11) */
#define TIM_CCMR1_OC1FE			(1 << 2)	/* (x=2..5,9..11) */
#define TIM_CCMR1_CC1S1			(1 << 1)	/* (x=2..5,9..11) */
#define TIM_CCMR1_CC1S0			(1 << 0)	/* (x=2..5,9..11) */

/* Output compare 2 mode */
#define TIM_CCMR1_OC2M_FROZEN		(0 << 12)
#define TIM_CCMR1_OC2M_ACTIVE		(1 << 12)
#define TIM_CCMR1_OC2M_INACTIVE		(2 << 12)
#define TIM_CCMR1_OC2M_TOGGLE		(3 << 12)
#define TIM_CCMR1_OC2M_FORCE_LOW	(4 << 12)
#define TIM_CCMR1_OC2M_FORCE_HIGH	(5 << 12)
#define TIM_CCMR1_OC2M_PWM1		(6 << 12)
#define TIM_CCMR1_OC2M_PWM2		(7 << 12)

/* Capture/compare 2 selection */
#define TIM_CCMR1_CC2S_OUT		(0 << 8)
#define TIM_CCMR1_CC2S_IN_TI2		(1 << 8)
#define TIM_CCMR1_CC2S_IN_TI1		(2 << 8)
#define TIM_CCMR1_CC2S_IN_TRC		(3 << 8)

/* Output compare 1 mode */
#define TIM_CCMR1_OC1M_FROZEN		(0 << 4)
#define TIM_CCMR1_OC1M_ACTIVE		(1 << 4)
#define TIM_CCMR1_OC1M_INACTIVE		(2 << 4)
#define TIM_CCMR1_OC1M_TOGGLE		(3 << 4)
#define TIM_CCMR1_OC1M_FORCE_LOW	(4 << 4)
#define TIM_CCMR1_OC1M_FORCE_HIGH	(5 << 4)
#define TIM_CCMR1_OC1M_PWM1		(6 << 4)
#define TIM_CCMR1_OC1M_PWM2		(7 << 4)

/* Capture/compare 1 selection */
#define TIM_CCMR1_CC1S_OUT		(0 << 0)
#define TIM_CCMR1_CC1S_IN_TI1		(1 << 0)
#define TIM_CCMR1_CC1S_IN_TI2		(2 << 0)
#define TIM_CCMR1_CC1S_IN_TRC		(3 << 0)

/* --- Input capture mode --- */

#define TIM_CCMR1_IC2F3			(1 << 15)	/* (x=2..5,9) */
#define TIM_CCMR1_IC2F2			(1 << 14)	/* (x=2..5,9) */
#define TIM_CCMR1_IC2F1			(1 << 13)	/* (x=2..5,9) */
#define TIM_CCMR1_IC2F0			(1 << 12)	/* (x=2..5,9) */
#define TIM_CCMR1_IC2PSC1		(1 << 11)	/* (x=2..5,9) */
#define TIM_CCMR1_IC2PSC0		(1 << 10)	/* (x=2..5,9) */
#define TIM_CCMR1_IC1F3			(1 << 7)	/* (x=2..5,9..11) */
#define TIM_CCMR1_IC1F2			(1 << 6)	/* (x=2..5,9..11) */
#define TIM_CCMR1_IC1F1			(1 << 5)	/* (x=2..5,9..11) */
#define TIM_CCMR1_IC1F0			(1 << 4)	/* (x=2..5,9..11) */
#define TIM_CCMR1_IC1PSC1		(1 << 3)	/* (x=2..5,9..11) */
#define TIM_CCMR1_IC1PSC0		(1 << 2)	/* (x=2..5,9..11) */

/* Input capture 2 filter */
#define TIM_CCMR1_IC2F_OFF		(0 << 12)
#define TIM_CCMR1_IC2F_CK_INT_N_2	(1 << 12)
#define TIM_CCMR1_IC2F_CK_INT_N_4	(2 << 12)
#define TIM_CCMR1_IC2F_CK_INT_N_8	(3 << 12)
#define TIM_CCMR1_IC2F_DTS_2_N_6	(4 << 12)
#define TIM_CCMR1_IC2F_DTS_2_N_8	(5 << 12)
#define TIM_CCMR1_IC2F_DTS_4_N_6	(6 << 12)
#define TIM_CCMR1_IC2F_DTS_4_N_8	(7 << 12)
#define TIM_CCMR1_IC2F_DTS_8_N_6	(8 << 12)
#define TIM_CCMR1_IC2F_DTS_8_N_8	(9 << 12)
#define TIM_CCMR1_IC2F_DTS_16_N_5	(10 << 12)
#define TIM_CCMR1_IC2F_DTS_16_N_6	(11 << 12)
#define TIM_CCMR1_IC2F_DTS_16_N_8	(12 << 12)
#define TIM_CCMR1_IC2F_DTS_32_N_5	(13 << 12)
#define TIM_CCMR1_IC2F_DTS_32_N_6	(14 << 12)
#define TIM_CCMR1_IC2F_DTS_32_N_8	(15 << 12)

/* Input capture 2 prescaler */
#define TIM_CCMR1_IC2PSC_OFF		(0 << 10)
#define TIM_CCMR1_IC2PSC_2		(1 << 10)
#define TIM_CCMR1_IC2PSC_4		(2 << 10)
#define TIM_CCMR1_IC2PSC_8		(3 << 10)

/* Input capture 1 filter */
#define TIM_CCMR1_IC1F_OFF		(0 << 4)
#define TIM_CCMR1_IC1F_CK_INT_N_2	(1 << 4)
#define TIM_CCMR1_IC1F_CK_INT_N_4	(2 << 4)
#define TIM_CCMR1_IC1F_CK_INT_N_8	(3 << 4)
#define TIM_CCMR1_IC1F_DTS_2_N_6	(4 << 4)
#define TIM_CCMR1_IC1F_DTS_2_N_8	(5 << 4)
#define TIM_CCMR1_IC1F_DTS_4_N_6	(6 << 4)
#define TIM_CCMR1_IC1F_DTS_4_N_8	(7 << 4)
#define TIM_CCMR1_IC1F_DTS_8_N_6	(8 << 4)
#define TIM_CCMR1_IC1F_DTS_8_N_8	(9 << 4)
#define TIM_CCMR1_IC1F_DTS_16_N_5	(10 << 4)
#define TIM_CCMR1_IC1F_DTS_16_N_6	(11 << 4)
#define TIM_CCMR1_IC1F_DTS_16_N_8	(12 << 4)
#define TIM_CCMR1_IC1F_DTS_32_N_5	(13 << 4)
#define TIM_CCMR1_IC1F_DTS_32_N_6	(14 << 4)
#define TIM_CCMR1_IC1F_DTS_32_N_8	(15 << 4)

/* Input capture 1 prescaler */
#define TIM_CCMR1_IC1PSC_OFF		(0 << 2)
#define TIM_CCMR1_IC1PSC_2		(1 << 2)
#define TIM_CCMR1_IC1PSC_4		(2 << 2)
#define TIM_CCMR1_IC1PSC_8		(3 << 2)

/* --- TIMx_CCMR2 values --------------------------------------------------- */

/* --- Output compare mode --- */

#define TIM_CCMR2_OC4CE			(1 << 15)
#define TIM_CCMR2_OC4M2			(1 << 14)
#define TIM_CCMR2_OC4M1			(1 << 13)
#define TIM_CCMR2_OC4M0			(1 << 12)
#define TIM_CCMR2_OC4PE			(1 << 11)
#define TIM_CCMR2_OC4FE			(1 << 10)
#define TIM_CCMR2_CC4S1			(1 << 9)
#define TIM_CCMR2_CC4S0			(1 << 8)
#define TIM_CCMR2_OC3CE			(1 << 7)
#define TIM_CCMR2_OC3M2			(1 << 6)
#define TIM_CCMR2_OC3M1			(1 << 5)
#define TIM_CCMR2_OC3M0			(1 << 4)
#define TIM_CCMR2_OC3PE			(1 << 3)
#define TIM_CCMR2_OC3FE			(1 << 2)
#define TIM_CCMR2_CC3S1			(1 << 1)
#define TIM_CCMR2_CC3S0			(1 << 0)

/* Output compare 4 mode */
#define TIM_CCMR2_OC4M_FROZEN		(0 << 12)
#define TIM_CCMR2_OC4M_ACTIVE		(1 << 12)
#define TIM_CCMR2_OC4M_INACTIVE		(2 << 12)
#define TIM_CCMR2_OC4M_TOGGLE		(3 << 12)
#define TIM_CCMR2_OC4M_FORCE_LOW	(4 << 12)
#define TIM_CCMR2_OC4M_FORCE_HIGH	(5 << 12)
#define TIM_CCMR2_OC4M_PWM1		(6 << 12)
#define TIM_CCMR2_OC4M_PWM2		(7 << 12)

/* Capture/compare 4 selection */
#define TIM_CCMR2_CC4S_OUT		(0 << 8)
#define TIM_CCMR2_CC4S_IN_TI4		(1 << 8)
#define TIM_CCMR2_CC4S_IN_TI3		(2 << 8)
#define TIM_CCMR2_CC4S_IN_TRC		(3 << 8)

/* Output compare 3 mode */
#define TIM_CCMR2_OC3M_FROZEN		(0 << 4)
#define TIM_CCMR2_OC3M_ACTIVE		(1 << 4)
#define TIM_CCMR2_OC3M_INACTIVE		(2 << 4)
#define TIM_CCMR2_OC3M_TOGGLE		(3 << 4)
#define TIM_CCMR2_OC3M_FORCE_LOW	(4 << 4)
#define TIM_CCMR2_OC3M_FORCE_HIGH	(5 << 4)
#define TIM_CCMR2_OC3M_PWM1		(6 << 4)
#define TIM_CCMR2_OC3M_PWM2		(7 << 4)

/* Capture/compare 3 selection */
#define TIM_CCMR2_CC3S_OUT		(0 << 0)
#define TIM_CCMR2_CC3S_IN_TI3		(1 << 0)
#define TIM_CCMR2_CC3S_IN_TI4		(2 << 0)
#define TIM_CCMR2_CC3S_IN_TRC		(3 << 0)

/* --- Input capture mode --- */

#define TIM_CCMR2_IC4F3			(1 << 15)
#define TIM_CCMR2_IC4F2			(1 << 14)
#define TIM_CCMR2_IC4F1			(1 << 13)
#define TIM_CCMR2_IC4F0			(1 << 12)
#define TIM_CCMR2_IC4PSC1		(1 << 11)
#define TIM_CCMR2_IC4PSC0		(1 << 10)
#define TIM_CCMR2_IC3F3			(1 << 7)
#define TIM_CCMR2_IC3F2			(1 << 6)
#define TIM_CCMR2_IC3F1			(1 << 5)
#define TIM_CCMR2_IC3F0			(1 << 4)
#define TIM_CCMR2_IC3PSC1		(1 << 3)
#define TIM_CCMR2_IC3PSC0		(1 << 2)

/* Input capture 4 filter */
#define TIM_CCMR2_IC4F_OFF		(0 << 12)
#define TIM_CCMR2_IC4F_CK_INT_N_2	(1 << 12)
#define TIM_CCMR2_IC4F_CK_INT_N_4	(2 << 12)
#define TIM_CCMR2_IC4F_CK_INT_N_8	(3 << 12)
#define TIM_CCMR2_IC4F_DTS_2_N_6	(4 << 12)
#define TIM_CCMR2_IC4F_DTS_2_N_8	(5 << 12)
#define TIM_CCMR2_IC4F_DTS_4_N_6	(6 << 12)
#define TIM_CCMR2_IC4F_DTS_4_N_8	(7 << 12)
#define TIM_CCMR2_IC4F_DTS_8_N_6	(8 << 12)
#define TIM_CCMR2_IC4F_DTS_8_N_8	(9 << 12)
#define TIM_CCMR2_IC4F_DTS_16_N_5	(10 << 12)
#define TIM_CCMR2_IC4F_DTS_16_N_6	(11 << 12)
#define TIM_CCMR2_IC4F_DTS_16_N_8	(12 << 12)
#define TIM_CCMR2_IC4F_DTS_32_N_5	(13 << 12)
#define TIM_CCMR2_IC4F_DTS_32_N_6	(14 << 12)
#define TIM_CCMR2_IC4F_DTS_32_N_8	(15 << 12)

/* Input capture 4 prescaler */
#define TIM_CCMR2_IC4PSC_OFF		(0 << 10)
#define TIM_CCMR2_IC4PSC_2		(1 << 10)
#define TIM_CCMR2_IC4PSC_4		(2 << 10)
#define TIM_CCMR2_IC4PSC_8		(3 << 10)

/* Input capture 3 filter */
#define TIM_CCMR2_IC3F_OFF		(0 << 4)
#define TIM_CCMR2_IC3F_CK_INT_N_2	(1 << 4)
#define TIM_CCMR2_IC3F_CK_INT_N_4	(2 << 4)
#define TIM_CCMR2_IC3F_CK_INT_N_8	(3 << 4)
#define TIM_CCMR2_IC3F_DTS_2_N_6	(4 << 4)
#define TIM_CCMR2_IC3F_DTS_2_N_8	(5 << 4)
#define TIM_CCMR2_IC3F_DTS_4_N_6	(6 << 4)
#define TIM_CCMR2_IC3F_DTS_4_N_8	(7 << 4)
#define TIM_CCMR2_IC3F_DTS_8_N_6	(8 << 4)
#define TIM_CCMR2_IC3F_DTS_8_N_8	(9 << 4)
#define TIM_CCMR2_IC3F_DTS_16_N_5	(10 << 4)
#define TIM_CCMR2_IC3F_DTS_16_N_6	(11 << 4)
#define TIM_CCMR2_IC3F_DTS_16_N_8	(12 << 4)
#define TIM_CCMR2_IC3F_DTS_32_N_5	(13 << 4)
#define TIM_CCMR2_IC3F_DTS_32_N_6	(14 << 4)
#define TIM_CCMR2_IC3F_DTS_32_N_8	(15 << 4)

/* Input capture 3 prescaler */
#define TIM_CCMR2_IC3PSC_OFF		(0 << 2)
#define TIM_CCMR2_IC3PSC_2		(1 << 2)
#define TIM_CCMR2_IC3PSC_4		(2 << 2)
#define TIM_CCMR2_IC3PSC_8		(3 << 2)

/* --- TIMx_CCER values ---------------------------------------------------- */

#define TIM_CCER_CC4NP			(1 << 15)	/* (x=2..5) */
#define TIM_CCER_CC4P			(1 << 13)	/* (x=2..5) */
#define TIM_CCER_CC4E			(1 << 12)	/* (x=2..5) */
#define TIM_CCER_CC3NP			(1 << 11)	/* (x=2..5) */
#define TIM_CCER_CC3P			(1 << 9)	/* (x=2..5) */
#define TIM_CCER_CC3E			(1 << 8)	/* (x=2..5) */
#define TIM_CCER_CC2NP			(1 << 7)	/* (x=2..5,9) */
#define TIM_CCER_CC2P			(1 << 5)	/* (x=2..5,9) */
#define TIM_CCER_CC2E			(1 << 4)	/* (x=2..5,9) */
#define TIM_CCER_CC1NP			(1 << 3)	/* (x=2..5,9..11) */
#define TIM_CCER_CC1P			(1 << 1)	/* (x=2..5,9..11) */
#define TIM_CCER_CC1E			(1 << 0)	/* (x=2..5,9..11) */

#define TIM_CCER_TIXFP1_RISING		0
#define TIM_CCER_TIXFP1_FALLING		TIM_CCER_CC1P
#define TIM_CCER_TIXFP1_BOTH		(TIM_CCER_CC1NP | TIM_CCER_CC1P)
#define TIM_CCER_TIXFP2_RISING		0
#define TIM_CCER_TIXFP2_FALLING		TIM_CCER_CC2P
#define TIM_CCER_TIXFP2_BOTH		(TIM_CCER_CC2NP | TIM_CCER_CC2P)
#define TIM_CCER_TIXFP3_RISING		0
#define TIM_CCER_TIXFP3_FALLING		TIM_CCER_CC3P
#define TIM_CCER_TIXFP3_BOTH		(TIM_CCER_CC3NP | TIM_CCER_CC3P)
#define TIM_CCER_TIXFP4_RISING		0
#define TIM_CCER_TIXFP4_FALLING		TIM_CCER_CC4P
#define TIM_CCER_TIXFP4_BOTH		(TIM_CCER_CC4NP | TIM_CCER_CC4P)

/* --- TIMx_CNT values ----------------------------------------------------- */

/* CNT[15:0]: Counter value (x=2..4,6,7,9..11) */
/* CNT[31:0]: Counter value (x=5) */

/* --- TIMx_PSC values ----------------------------------------------------- */

/* PSC[15:0]: Prescaler value */

/* --- TIMx_ARR values ----------------------------------------------------- */

/* ARR[15:0]: Prescaler value (x=2..4,6,7,9..11) */
/* ARR[31:0]: Prescaler value (x=5) */

/* --- TIMx_CCR1 values ---------------------------------------------------- */

/* CCR1[15:0]: Capture/compare 1 value (x=2..4,9..11) */
/* CCR1[31:0]: Capture/compare 1 value (x=5) */

/* --- TIMx_CCR2 values ---------------------------------------------------- */

/* CCR2[15:0]: Capture/compare 2 value (x=2..4,9) */
/* CCR2[31:0]: Capture/compare 2 value (x=5) */

/* --- TIMx_CCR3 values ---------------------------------------------------- */

/* CCR3[15:0]: Capture/compare 3 value (x=2..4) */
/* CCR3[31:0]: Capture/compare 3 value (x=5) */

/* --- TIMx_CCR4 values ---------------------------------------------------- */

/* CCR4[15:0]: Capture/compare 4 value (x=2..4) */
/* CCR4[31:0]: Capture/compare 4 value (x=5) */

/* --- TIMx_DCR values ----------------------------------------------------- */

#define TIM_DCR_DBL4			(1 << 12)
#define TIM_DCR_DBL3			(1 << 11)
#define TIM_DCR_DBL2			(1 << 10)
#define TIM_DCR_DBL1			(1 << 9)
#define TIM_DCR_DBL0			(1 << 8)
#define TIM_DCR_DBA4			(1 << 4)
#define TIM_DCR_DBA3			(1 << 3)
#define TIM_DCR_DBA2			(1 << 2)
#define TIM_DCR_DBA1			(1 << 1)
#define TIM_DCR_DBA0			(1 << 0)

/* --- TIMx_DMAR values ---------------------------------------------------- */

/* DMAB[15:0]: DMA register for burst accesses */

/* --- TIMx_OR values ------------------------------------------------------ */

#define TIM_OR_TI1_RMP_RI		(1 << 3)	/* (x=10,11)(**) */
#define TIM_OR_ETR_RMP			(1 << 2)	/* (x=10,11)(**) */
#define TIM9_OR_ITR1_RMP		(1 << 2)	/* (x=9)(**) */
#define TIM_OR_TI1_RMP1			(1 << 1)	/* (x=9..11) */
#define TIM_OR_TI1_RMP0			(1 << 0)	/* (x=9..11) */
#define TIM2_OR_ITR1_RMP		(1 << 0)	/* (x=2) */
#define TIM3_OR_ITR2_RMP		(1 << 0)	/* (x=3) */

/* TIM9 Input 1 remapping capability */
#define TIM9_OR_TI1_RMP_GPIO		(0 << 0)
#define TIM9_OR_TI1_RMP_LSE		(1 << 0)

/* TIM10 Input 1 remapping capability */
#define TIM10_OR_TI1_RMP_GPIO		(0 << 0)
#define TIM10_OR_TI1_RMP_LSI		(1 << 0)
#define TIM10_OR_TI1_RMP_LSE		(2 << 0)
#define TIM10_OR_TI1_RMP_RTC		(3 << 0)

/* TIM11 Input 1 remapping capability */
#define TIM11_OR_TI1_RMP_GPIO		(0 << 0)
#define TIM11_OR_TI1_RMP_MSI		(1 << 0)
#define TIM11_OR_TI1_RMP_HSE_RTC	(2 << 0)

/* --- Function prototypes ------------------------------------------------- */

#define MAXTIM				9

/* Device number */
typedef enum {
	TIM2 = 8,
	TIM3 = 12,
	TIM4 = 16,
	TIM5 = 20,	/* (**) */
	TIM6 = 24,
	TIM7 = 28,
	TIM9 = 36,
	TIM10 = 40,
	TIM11 = 44
} tim_t;

/* Interrupt */
enum {
	TIM_UPDATE = (1 << 0),
	TIM_CC1 = (1 << 1),
	TIM_CC2 = (1 << 2),
	TIM_CC3 = (1 << 3),
	TIM_CC4 = (1 << 4),
	TIM_TRIGGER = (1 << 6),
	TIM_CC1_OVERCAPTURE = (1 << 9),
	TIM_CC2_OVERCAPTURE = (1 << 10),
	TIM_CC3_OVERCAPTURE = (1 << 11),
	TIM_CC4_OVERCAPTURE = (1 << 12)
};

/* DMA */
enum {
	TIM_DMA_UPDATE = (1 << 8),
	TIM_DMA_CC1 = (1 << 9),
	TIM_DMA_CC2 = (1 << 10),
	TIM_DMA_CC3 = (1 << 11),
	TIM_DMA_CC4 = (1 << 12),
	TIM_DMA_COM = (1 << 13), /* ??? */
	TIM_DMA_TRIGGER = (1 << 14)
};

/* Center-aligned mode */
typedef enum {
	TIM_EDGE_ALIGNED_MODE = (0 << 5),
	TIM_MODE1 = (1 << 5),
	TIM_MODE2 = (2 << 5),
	TIM_MODE3 = (3 << 5),
} tim_center_aligned_mode_t;

/* Channel */
typedef enum {
	TIM2_CC1 = 8,
	TIM2_CC2 = 9,
	TIM2_CC3 = 10,
	TIM2_CC4 = 11,
	TIM3_CC1 = 12,
	TIM3_CC2 = 13,
	TIM3_CC3 = 14,
	TIM3_CC4 = 15,
	TIM4_CC1 = 16,
	TIM4_CC2 = 17,
	TIM4_CC3 = 18,
	TIM4_CC4 = 19,
	TIM5_CC1 = 20,	/* (**) */
	TIM5_CC2 = 21,	/* (**) */
	TIM5_CC3 = 22,	/* (**) */
	TIM5_CC4 = 23,	/* (**) */
	TIM9_CC1 = 36,
	TIM9_CC2 = 37,
	TIM10_CC1 = 40,
	TIM11_CC1 = 44
} tim_cc_t;

/* Capture compare mode */
enum {
	TIM_CC_OUTPUT = 0,
	TIM_CC_INPUT1 = 1,
	TIM_CC_INPUT2 = 2,
	TIM_CC_INPUT3 = 3,

	TIM_OC_FAST = (1 << 2),
	TIM_OC_PRELOAD = (1 << 3),

	TIM_OC_FROZEN = (0 << 4),
	TIM_OC_MATCH_ACTIVE = (1 << 4),
	TIM_OC_MATCH_INACTIVE = (2 << 4),
	TIM_OC_MATCH_TOGGLE = (3 << 4),
	TIM_OC_FORCE_INACTIVE = (4 << 4),
	TIM_OC_FORCE_ACTIVE = (5 << 4),
	TIM_OC_PWM1 = (6 << 4),
	TIM_OC_PWM2 = (7 << 4),

	TIM_OC_CLEAR = (1 << 7),

	TIM_IC_PRESCALER_1 = (0 << 2),
	TIM_IC_PRESCALER_2 = (1 << 2),
	TIM_IC_PRESCALER_4 = (2 << 2),
	TIM_IC_PRESCALER_8 = (3 << 2),

	TIM_IC_DTS_N_1 = (0 << 4),
	TIM_IC_CK_INT_N_2 = (1 << 4),
	TIM_IC_CK_INT_N_4 = (2 << 4),
	TIM_IC_CK_INT_N_8 = (3 << 4),
	TIM_IC_DTS_2_N_6 = (4 << 4),
	TIM_IC_DTS_2_N_8 = (5 << 4),
	TIM_IC_DTS_4_N_6 = (6 << 4),
	TIM_IC_DTS_4_N_8 = (7 << 4),
	TIM_IC_DTS_8_N_6 = (8 << 4),
	TIM_IC_DTS_8_N_8 = (9 << 4),
	TIM_IC_DTS_16_N_5 = (10 << 4),
	TIM_IC_DTS_16_N_6 = (11 << 4),
	TIM_IC_DTS_16_N_8 = (12 << 4),
	TIM_IC_DTS_32_N_5 = (13 << 4),
	TIM_IC_DTS_32_N_6 = (14 << 4),
	TIM_IC_DTS_32_N_8 = (15 << 4),

	TIM_CC_ENABLE = (1 << 8),
	TIM_OC_ACTIVE_LOW = (1 << 9),
	TIM_OC_ACTIVE_HIGH = 0,

	TIM_IC_NONINVERTED_RISING = 0,
	TIM_IC_INVERTED_FALLING = (1 << 9),
	TIM_IC_NONINVERTED_BOTH = (5 << 9)
};

/* tDTS */
typedef enum {
	TIM_NODIV = (0 << 8),
	TIM_DIV2 = (1 << 8),
	TIM_DIV4 = (2 << 8)
} tim_clock_div_t;

/* Master mode */
typedef enum {
	TIM_TRGO_RESET = (0 << 4),
	TIM_TRGO_ENABLE = (1 << 4),
	TIM_TRGO_UPDATE = (2 << 4),
	TIM_TRGO_COMPARE_PULSE = (3 << 4),
	TIM_TRGO_COMPARE1 = (4 << 4),
	TIM_TRGO_COMPARE2 = (5 << 4),
	TIM_TRGO_COMPARE3 = (6 << 4),
	TIM_TRGO_COMPARE4 = (7 << 4)
} tim_master_mode_t;

/* Slave mode */
enum {
	TIM_SLAVE_DISABLE = 0,
	TIM_ENCODER_MODE1 = 1,
	TIM_ENCODER_MODE2 = 2,
	TIM_ENCODER_MODE3 = 3,
	TIM_TRGI_RESET = 4,
	TIM_TRGI_GATED = 5,
	TIM_TRGI_TRIGGER = 6,
	TIM_EXTERNAL_CLOCK_MODE1 = 7,

	TIM_OCREF_ETRF = (1 << 3),

	TIM_ITR0 = (0 << 4),
	TIM_ITR1 = (1 << 4),
	TIM_ITR2 = (2 << 4),
	TIM_ITR3 = (3 << 4),
	TIM_TI1F_ED = (4 << 4),
	TIM_TI1FP1 = (5 << 4),
	TIM_TI2FP2 = (6 << 4),
	TIM_ETRF = (7 << 4),

	TIM_MASTER_SLAVE = (1 << 7),

	TIM_ET_DTS_N_1 = (0 << 8),
	TIM_ET_CK_INT_N_2 = (1 << 8),
	TIM_ET_CK_INT_N_4 = (2 << 8),
	TIM_ET_CK_INT_N_8 = (3 << 8),
	TIM_ET_DTS_2_N_6 = (4 << 8),
	TIM_ET_DTS_2_N_8 = (5 << 8),
	TIM_ET_DTS_4_N_6 = (6 << 8),
	TIM_ET_DTS_4_N_8 = (7 << 8),
	TIM_ET_DTS_8_N_6 = (8 << 8),
	TIM_ET_DTS_8_N_8 = (9 << 8),
	TIM_ET_DTS_16_N_5 = (10 << 8),
	TIM_ET_DTS_16_N_6 = (11 << 8),
	TIM_ET_DTS_16_N_8 = (12 << 8),
	TIM_ET_DTS_32_N_5 = (13 << 8),
	TIM_ET_DTS_32_N_6 = (14 << 8),
	TIM_ET_DTS_32_N_8 = (15 << 8),

	TIM_ETRP_2 = (1 << 12),
	TIM_ETRP_4 = (2 << 12),
	TIM_ETRP_8 = (3 << 12),

	TIM_EXTERNAL_CLOCK_MODE2 = (1 << 14),

	TIM_ETR_INVERTED = (1 << 15)
};

void tim_setup_counter(tim_t tim, int prescaler, int autoreload);
void tim_load_prescaler_value(tim_t tim, int prescaler);
void tim_set_autoreload_value(tim_t tim, int autoreload);
void tim_set_counter(tim_t tim, int count);
int tim_get_counter(tim_t tim);
void tim_enable_counter(tim_t tim);
void tim_disable_counter(tim_t tim);
void tim_enable_preload(tim_t tim);
void tim_disable_preload(tim_t tim);
void tim_enable_interrupt(tim_t tim, int interrupt);
void tim_disable_interrupt(tim_t tim, int interrupt);
int tim_get_interrupt_mask(tim_t tim, int interrupt);
int tim_get_interrupt_status(tim_t tim, int interrupt);
void tim_clear_interrupt(tim_t tim, int interrupt);
void tim_generate_event(tim_t tim, int event);
#define tim_enable_dma tim_enable_interrupt
#define tim_disable_dma tim_disable_interrupt
void tim_set_upcounting_mode(tim_t tim);
void tim_set_downcounting_mode(tim_t tim);
void tim_set_center_aligned_mode(tim_t tim, tim_center_aligned_mode_t mode);
void tim_disable_update_event(tim_t tim);
void tim_enable_update_event(tim_t tim);
void tim_disable_update_interrupt_on_any(tim_t tim);
void tim_enable_update_interrupt_on_any(tim_t tim);
void tim_set_capture_compare_mode(tim_cc_t tim_cc, int mode);
void tim_start_capture_compare(tim_t tim);
void tim_set_dts_division(tim_t tim, tim_clock_div_t div);
void tim_set_capture_compare_value(tim_cc_t tim_cc, int value);
int tim_get_capture_compare_value(tim_cc_t tim_cc);
void tim_enable_one_pulse_mode(tim_t tim);
void tim_disable_one_pulse_mode(tim_t tim);
void tim_enable_ti1_ch123_xor(tim_t tim);
void tim_disable_ti1_ch123_xor(tim_t tim);
void tim_set_master_mode(tim_t tim, tim_master_mode_t mode);
void tim_set_slave_mode(tim_t tim, int mode);
void tim_enable_ccx_dma_on_update_event(tim_t tim);
void tim_disable_ccx_dma_on_update_event(tim_t tim);
void tim_setup_dma(tim_t tim, int dba, int dbl);
