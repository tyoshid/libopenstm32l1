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

#include <stm32/l1/memorymap.h>
#include <libopencm3.h>

/*
 * STMicroelectronics(www.st.com)
 *
 * RM0038: STM32L100xx, STM32L151xx, STM32L152xx and STM32L162xx
 *         advanced ARM-based 32-bit MCUs
 * (19-Apr-2013 Rev 8)
 *
 * 11 Analog-to-digital converter (ADC)
 *
 *
 * DS6876: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         16KB SRAM, 4KB EEPROM, LCD, USB, ADC, DAC
 * (07-Feb-2013 Rev 8)
 *
 * 3.10 ADC(analog-to-digital converter)
 *
 * Table 54. ADC characteristics
 *
 *
 * DS8890: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 256KB Flash,
 *         32KB SRAM, 8KB EEPROM, LCD, USB, ADC, DAC
 * (01-Feb-2013 Rev 3)
 *
 * 3.10 ADC(analog-to-digital converter)
 *
 * Table 55. ADC characteristics
 *
 *
 * DS8576: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F
 * (01-Feb-2013 Rev 6)
 *
 * 3.11 ADC(analog-to-digital converter)
 *
 * Table 64. ADC characteristics
 *
 *
 * DS8669: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F, AES
 * (06-Feb-2013 Rev 4)
 *
 * 3.11 ADC(analog-to-digital converter)
 *
 * Table 63. ADC characteristics
 *
 *
 * DS9496: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         10KB SRAM, 2KB EEPROM, LCD, USB, ADC, DAC
 * (21-Feb-2013 Rev 1)
 *
 * 3.10 ADC(analog-to-digital converter)
 *
 * Table 52. ADC characteristics
 */

/*
 * Note: Registers and bits marked (**) only exist in high and medium+ density
 * devices.
 */

/* --- ADC registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		ADC_SR		ADC status register
 * 0x04		ADC_CR1		ADC control register 1
 * 0x08		ADC_CR2		ADC control register 2
 * 0x0c		ADC_SMPR1	ADC sample time register 1
 * 0x10		ADC_SMPR2	ADC sample time register 2
 * 0x14		ADC_SMPR3	ADC sample time register 3
 * 0x18		ADC_JOFR1	ADC injected channel data offset register 1
 * 0x1c		ADC_JOFR2	ADC injected channel data offset register 2
 * 0x20		ADC_JOFR3	ADC injected channel data offset register 3
 * 0x24		ADC_JOFR4	ADC injected channel data offset register 4
 * 0x28		ADC_HTR		ADC watchdog higher threshold register
 * 0x2c		ADC_LTR		ADC watchdog lower threshold register
 * 0x30		ADC_SQR1	ADC regular sequence register 1
 * 0x34		ADC_SQR2	ADC regular sequence register 2
 * 0x38		ADC_SQR3	ADC regular sequence register 3
 * 0x3c		ADC_SQR4	ADC regular sequence register 4
 * 0x40		ADC_SQR5	ADC regular sequence register 5
 * 0x44		ADC_JSQR	ADC injected sequence register
 * 0x48		ADC_JDR1	ADC injected data register 1
 * 0x4c		ADC_JDR2	ADC injected data register 2
 * 0x50		ADC_JDR3	ADC injected data register 3
 * 0x54		ADC_JDR4	ADC injected data register 4
 * 0x58		ADC_DR		ADC regular data register
 * 0x5c		ADC_SMPR0	ADC sample time register 0
 *
 * 0x300			Common registers
 * 0x00		ADC_CSR		ADC common status register
 * 0x04		ADC_CCR		ADC common control register
 */

#define ADC_SR				MMIO32(ADC_BASE + 0x00)
#define ADC_CR1				MMIO32(ADC_BASE + 0x04)
#define ADC_CR2				MMIO32(ADC_BASE + 0x08)
#define ADC_SMPR1			MMIO32(ADC_BASE + 0x0c)
#define ADC_SMPR2			MMIO32(ADC_BASE + 0x10)
#define ADC_SMPR3			MMIO32(ADC_BASE + 0x14)
#define ADC_JOFR1			MMIO32(ADC_BASE + 0x18)
#define ADC_JOFR2			MMIO32(ADC_BASE + 0x1c)
#define ADC_JOFR3			MMIO32(ADC_BASE + 0x20)
#define ADC_JOFR4			MMIO32(ADC_BASE + 0x24)
#define ADC_HTR				MMIO32(ADC_BASE + 0x28)
#define ADC_LTR				MMIO32(ADC_BASE + 0x2c)
#define ADC_SQR1			MMIO32(ADC_BASE + 0x30)
#define ADC_SQR2			MMIO32(ADC_BASE + 0x34)
#define ADC_SQR3			MMIO32(ADC_BASE + 0x38)
#define ADC_SQR4			MMIO32(ADC_BASE + 0x3c)
#define ADC_SQR5			MMIO32(ADC_BASE + 0x40)
#define ADC_JSQR			MMIO32(ADC_BASE + 0x44)
#define ADC_JDR1			MMIO32(ADC_BASE + 0x48)
#define ADC_JDR2			MMIO32(ADC_BASE + 0x4c)
#define ADC_JDR3			MMIO32(ADC_BASE + 0x50)
#define ADC_JDR4			MMIO32(ADC_BASE + 0x54)
#define ADC_DR				MMIO32(ADC_BASE + 0x58)
#define ADC_SMPR0			MMIO32(ADC_BASE + 0x5c)	/* (**) */

#define ADC_CSR				MMIO32(ADC_BASE + 0x300)
#define ADC_CCR				MMIO32(ADC_BASE + 0x304)

/* medium density device */
#define ADC_VREFINT_CAL			MMIO16(SYSTEM_MEMORY_BASE + 0x80078)
#define ADC_TSENSE_CAL1			MMIO16(SYSTEM_MEMORY_BASE + 0x8007a)
#define ADC_TSENSE_CAL2			MMIO16(SYSTEM_MEMORY_BASE + 0x8007e)
/* medium+ and high density device */
#define ADC_H_VREFINT_CAL		MMIO16(SYSTEM_MEMORY_BASE + 0x800f8)
#define ADC_H_TSENSE_CAL1		MMIO16(SYSTEM_MEMORY_BASE + 0x800fa)
#define ADC_H_TSENSE_CAL2		MMIO16(SYSTEM_MEMORY_BASE + 0x800fe)

/* --- ADC_SR values ------------------------------------------------------- */

#define ADC_SR_JCNR			(1 << 9)
#define ADC_SR_RCNR			(1 << 8)
#define ADC_SR_ADONS			(1 << 6)
#define ADC_SR_OVR			(1 << 5)
#define ADC_SR_STRT			(1 << 4)
#define ADC_SR_JSTRT			(1 << 3)
#define ADC_SR_JEOC			(1 << 2)
#define ADC_SR_EOC			(1 << 1)
#define ADC_SR_AWD			(1 << 0)

/* --- ADC_CR1 values ------------------------------------------------------ */

#define ADC_CR1_OVRIE			(1 << 26)
#define ADC_CR1_RES1			(1 << 25)
#define ADC_CR1_RES0			(1 << 24)
#define ADC_CR1_AWDEN			(1 << 23)
#define ADC_CR1_JAWDEN			(1 << 22)
#define ADC_CR1_PDI			(1 << 17)
#define ADC_CR1_PDD			(1 << 16)
#define ADC_CR1_DISCNUM2		(1 << 15)
#define ADC_CR1_DISCNUM1		(1 << 14)
#define ADC_CR1_DISCNUM0		(1 << 13)
#define ADC_CR1_JDISCEN			(1 << 12)
#define ADC_CR1_DISCEN			(1 << 11)
#define ADC_CR1_JAUTO			(1 << 10)
#define ADC_CR1_AWDSGL			(1 << 9)
#define ADC_CR1_SCAN			(1 << 8)
#define ADC_CR1_JEOCIE			(1 << 7)
#define ADC_CR1_AWDIE			(1 << 6)
#define ADC_CR1_EOCIE			(1 << 5)
#define ADC_CR1_AWDCH4			(1 << 4)
#define ADC_CR1_AWDCH3			(1 << 3)
#define ADC_CR1_AWDCH2			(1 << 2)
#define ADC_CR1_AWDCH1			(1 << 1)
#define ADC_CR1_AWDCH0			(1 << 0)

/* Resolution */
#define ADC_CR1_RES_12BIT		(0 << 24)
#define ADC_CR1_RES_10BIT		(1 << 24)
#define ADC_CR1_RES_8BIT		(2 << 24)
#define ADC_CR1_RES_6BIT		(3 << 24)

/* Discontinous mode channel count. */
#define ADC_CR1_DISCNUM_1CHANNEL	(0 << 13)
#define ADC_CR1_DISCNUM_2CHANNELS       (1 << 13)
#define ADC_CR1_DISCNUM_3CHANNELS       (2 << 13)
#define ADC_CR1_DISCNUM_4CHANNELS       (3 << 13)
#define ADC_CR1_DISCNUM_5CHANNELS       (4 << 13)
#define ADC_CR1_DISCNUM_6CHANNELS       (5 << 13)
#define ADC_CR1_DISCNUM_7CHANNELS       (6 << 13)
#define ADC_CR1_DISCNUM_8CHANNELS       (7 << 13)

/* Analog watchdog channel select bits */
#define ADC_CR1_AWDCH_CHANNEL0		(0 << 0)
#define ADC_CR1_AWDCH_CHANNEL1		(1 << 0)
#define ADC_CR1_AWDCH_CHANNEL2		(2 << 0)
#define ADC_CR1_AWDCH_CHANNEL3		(3 << 0)
#define ADC_CR1_AWDCH_CHANNEL4		(4 << 0)
#define ADC_CR1_AWDCH_CHANNEL5		(5 << 0)
#define ADC_CR1_AWDCH_CHANNEL6		(6 << 0)
#define ADC_CR1_AWDCH_CHANNEL7		(7 << 0)
#define ADC_CR1_AWDCH_CHANNEL8		(8 << 0)
#define ADC_CR1_AWDCH_CHANNEL9		(9 << 0)
#define ADC_CR1_AWDCH_CHANNEL10		(10 << 0)
#define ADC_CR1_AWDCH_CHANNEL11		(11 << 0)
#define ADC_CR1_AWDCH_CHANNEL12		(12 << 0)
#define ADC_CR1_AWDCH_CHANNEL13		(13 << 0)
#define ADC_CR1_AWDCH_CHANNEL14		(14 << 0)
#define ADC_CR1_AWDCH_CHANNEL15		(15 << 0)
#define ADC_CR1_AWDCH_CHANNEL16		(16 << 0)
#define ADC_CR1_AWDCH_CHANNEL17		(17 << 0)
#define ADC_CR1_AWDCH_CHANNEL18		(18 << 0)
#define ADC_CR1_AWDCH_CHANNEL19		(19 << 0)
#define ADC_CR1_AWDCH_CHANNEL20		(20 << 0)
#define ADC_CR1_AWDCH_CHANNEL21		(21 << 0)
#define ADC_CR1_AWDCH_CHANNEL22		(22 << 0)
#define ADC_CR1_AWDCH_CHANNEL23		(23 << 0)
#define ADC_CR1_AWDCH_CHANNEL24		(24 << 0)
#define ADC_CR1_AWDCH_CHANNEL25		(25 << 0)
#define ADC_CR1_AWDCH_CHANNEL26		(26 << 0)

/* --- ADC_CR2 values ------------------------------------------------------ */

#define ADC_CR2_SWSTART			(1 << 30)
#define ADC_CR2_EXTEN1			(1 << 29)
#define ADC_CR2_EXTEN0			(1 << 28)
#define ADC_CR2_EXTSEL3			(1 << 27)
#define ADC_CR2_EXTSEL2			(1 << 26)
#define ADC_CR2_EXTSEL1			(1 << 25)
#define ADC_CR2_EXTSEL0			(1 << 24)
#define ADC_CR2_JSWSTART		(1 << 22)
#define ADC_CR2_JEXTEN1			(1 << 21)
#define ADC_CR2_JEXTEN0			(1 << 20)
#define ADC_CR2_JEXTSEL3		(1 << 19)
#define ADC_CR2_JEXTSEL2		(1 << 18)
#define ADC_CR2_JEXTSEL1		(1 << 17)
#define ADC_CR2_JEXTSEL0		(1 << 16)
#define ADC_CR2_ALIGN			(1 << 11)
#define ADC_CR2_EOCS			(1 << 10)
#define ADC_CR2_DDS			(1 << 9)
#define ADC_CR2_DMA			(1 << 8)
#define ADC_CR2_DELS2			(1 << 6)
#define ADC_CR2_DELS1			(1 << 5)
#define ADC_CR2_DELS0			(1 << 4)
#define ADC_CR2_ADC_CFG			(1 << 2)	/* (**) */
#define ADC_CR2_CONT			(1 << 1)
#define ADC_CR2_ADON			(1 << 0)

/* External trigger enable for regular channels */
#define ADC_CR2_EXTEN_DISABLE		(0 << 28)
#define ADC_CR2_EXTEN_RISING		(1 << 28)
#define ADC_CR2_EXTEN_FALLING		(2 << 28)
#define ADC_CR2_EXTEN_BOTH		(3 << 28)

/* External event select for regular group */
#define ADC_CR2_EXTSEL_TIM9_CC2		(0 << 24)
#define ADC_CR2_EXTSEL_TIM9_TRGO	(1 << 24)
#define ADC_CR2_EXTSEL_TIM2_CC3		(2 << 24)
#define ADC_CR2_EXTSEL_TIM2_CC2		(3 << 24)
#define ADC_CR2_EXTSEL_TIM3_TRGO	(4 << 24)
#define ADC_CR2_EXTSEL_TIM4_CC4		(5 << 24)
#define ADC_CR2_EXTSEL_TIM2_TRGO	(6 << 24)
#define ADC_CR2_EXTSEL_TIM3_CC1		(7 << 24)
#define ADC_CR2_EXTSEL_TIM3_CC3		(8 << 24)
#define ADC_CR2_EXTSEL_TIM4_TRGO	(9 << 24)
#define ADC_CR2_EXTSEL_TIM6_TRGO	(10 << 24)
#define ADC_CR2_EXTSEL_EXTI11		(15 << 24)

/* External trigger enable for injected channels */
#define ADC_CR2_JEXTEN_DISABLE		(0 << 20)
#define ADC_CR2_JEXTEN_RISING		(1 << 20)
#define ADC_CR2_JEXTEN_FALLING		(2 << 20)
#define ADC_CR2_JEXTEN_BOTH		(3 << 20)

/* External event select for injected group */
#define ADC_CR2_JEXTSEL_TIM9_CC1	(0 << 16)
#define ADC_CR2_JEXTSEL_TIM9_TRGO	(1 << 16)
#define ADC_CR2_JEXTSEL_TIM2_TRGO	(2 << 16)
#define ADC_CR2_JEXTSEL_TIM2_CC1	(3 << 16)
#define ADC_CR2_JEXTSEL_TIM3_CC4	(4 << 16)
#define ADC_CR2_JEXTSEL_TIM4_TRGO	(5 << 16)
#define ADC_CR2_JEXTSEL_TIM4_CC1	(6 << 16)
#define ADC_CR2_JEXTSEL_TIM4_CC2	(7 << 16)
#define ADC_CR2_JEXTSEL_TIM4_CC3	(8 << 16)
#define ADC_CR2_JEXTSEL_TIM10_CC1	(9 << 16)
#define ADC_CR2_JEXTSEL_TIM7_TRGO	(10 << 16)
#define ADC_CR2_JEXTSEL_EXTI15		(15 << 16)

/* Delay selection */
#define ADC_CR2_DELS_NONE		(0 << 4)
#define ADC_CR2_DELS_UNTIL_READ		(1 << 4)
#define ADC_CR2_DELS_7			(2 << 4)
#define ADC_CR2_DELS_15			(3 << 4)
#define ADC_CR2_DELS_31			(4 << 4)
#define ADC_CR2_DELS_63			(5 << 4)
#define ADC_CR2_DELS_127		(6 << 4)
#define ADC_CR2_DELS_255		(7 << 4)

/* --- ADC_SMPR1 values ---------------------------------------------------- */

/* ADC_SMPR1[3(x-20)+2:3(x-20)]: SMPx[2:0]: Channel x sampling time selection */
/* (x = 20..29) */

/* --- ADC_SMPR2 values ---------------------------------------------------- */

/* ADC_SMPR2[3(x-10)+2:3(x-10)]: SMPx[2:0]: Channel x sampling time selection */
/* (x = 10..19) */

/* --- ADC_SMPR3 values ---------------------------------------------------- */

/* ADC_SMPR3[3x+2:3x]: SMPx[2:0]: Channel x sampling time selection */
/* (x = 0..9) */

/* --- ADC_SMPRn generic values -------------------------------------------- */

#define ADC_SMPR_4			0
#define ADC_SMPR_9			1
#define ADC_SMPR_16			2
#define ADC_SMPR_24			3
#define ADC_SMPR_48			4
#define ADC_SMPR_96			5
#define ADC_SMPR_192			6
#define ADC_SMPR_384			7

/* --- ADC_JOFRx values ---------------------------------------------------- */

/* JOFFSETx[11:0]: Data offset for injected channel x (x = 1..4) */

/* --- ADC_HTR values ------------------------------------------------------ */

/* HT[11:0]: Analog watchdog higher threshold */

/* --- ADC_LTR values ------------------------------------------------------ */

/* LT[11:0]: Analog watchdog lower threshold */

/* --- ADC_SQR1 values ----------------------------------------------------- */

#define ADC_SQR1_L4			(1 << 24)
#define ADC_SQR1_L3			(1 << 23)
#define ADC_SQR1_L2			(1 << 22)
#define ADC_SQR1_L1			(1 << 21)
#define ADC_SQR1_L0			(1 << 20)

/* ADC_SQR1[5(n-25)+4:5(n-25)]: SQn[4:0]: n-th conversion in regular sequence */
/* (n = 25..28) */

/* Regular channel sequence length */
#define ADC_SQR1_L_1			(0 << 20)
#define ADC_SQR1_L_2			(1 << 20)
#define ADC_SQR1_L_3			(2 << 20)
#define ADC_SQR1_L_4			(3 << 20)
#define ADC_SQR1_L_5			(4 << 20)
#define ADC_SQR1_L_6			(5 << 20)
#define ADC_SQR1_L_7			(6 << 20)
#define ADC_SQR1_L_8			(7 << 20)
#define ADC_SQR1_L_9			(8 << 20)
#define ADC_SQR1_L_10			(9 << 20)
#define ADC_SQR1_L_11			(10 << 20)
#define ADC_SQR1_L_12			(11 << 20)
#define ADC_SQR1_L_13			(12 << 20)
#define ADC_SQR1_L_14			(13 << 20)
#define ADC_SQR1_L_15			(14 << 20)
#define ADC_SQR1_L_16			(15 << 20)
#define ADC_SQR1_L_17			(16 << 20)
#define ADC_SQR1_L_18			(17 << 20)
#define ADC_SQR1_L_19			(18 << 20)
#define ADC_SQR1_L_20			(19 << 20)
#define ADC_SQR1_L_21			(20 << 20)
#define ADC_SQR1_L_22			(21 << 20)
#define ADC_SQR1_L_23			(22 << 20)
#define ADC_SQR1_L_24			(23 << 20)
#define ADC_SQR1_L_25			(24 << 20)
#define ADC_SQR1_L_26			(25 << 20)
#define ADC_SQR1_L_27			(26 << 20)
#define ADC_SQR1_L_28			(27 << 20)	/* (**) */

/* --- ADC_SQR2 values ----------------------------------------------------- */

/* ADC_SQR2[5(n-19)+4:5(n-19)]: SQn[4:0]: n-th conversion in regular sequence */
/* (n = 19..24) */

/* --- ADC_SQR3 values ----------------------------------------------------- */

/* ADC_SQR3[5(n-13)+4:5(n-13)]: SQn[4:0]: n-th conversion in regular sequence */
/* (n = 13..18) */

/* --- ADC_SQR4 values ----------------------------------------------------- */

/* ADC_SQR4[5(n-7)+4:5(n-7)]: SQn[4:0]: n-th conversion in regular sequence */
/* (n = 7..12) */

/* --- ADC_SQR5 values ----------------------------------------------------- */

/* ADC_SQR4[5(n-1)+4:5(n-1)]: SQn[4:0]: n-th conversion in regular sequence */
/* (n = 1..6) */

/* --- ADC_JSQR values ----------------------------------------------------- */

#define ADC_SQR1_JL1			(1 << 21)
#define ADC_SQR1_JL0			(1 << 20)

/* ADC_JSQR[5(n-1)+4:5(n-1)]: JSQn[4:0]: n-th conversion in injected sequence */
/* (n = 1..4) */

/* Injected sequence length */
#define ADC_JSQR_JL_1			(0 << 20)
#define ADC_JSQR_JL_2			(1 << 20)
#define ADC_JSQR_JL_3			(2 << 20)
#define ADC_JSQR_JL_4			(3 << 20)

/* --- ADC_JDRx values ----------------------------------------------------- */

/* JDATA[15:0]: Injected data */

/* --- ADC_DR values ------------------------------------------------------- */

/* DATA[15:0]: Regular data */

/* --- ADC_SMPR0 values ---------------------------------------------------- */

/* ADC_SMPR0[3(x-30)+2:3(x-30)]: SMPx[2:0]: Channel x sampling time selection */
/* (x = 30..31) */

/* --- ADC_CSR values ------------------------------------------------------ */

#define ADC_CSR_ADONS1			(1 << 6)
#define ADC_CSR_OVR1			(1 << 5)
#define ADC_CSR_STRT1			(1 << 4)
#define ADC_CSR_JSTRT1			(1 << 3)
#define ADC_CSR_JEOC1			(1 << 2)
#define ADC_CSR_EOC1			(1 << 1)
#define ADC_CSR_AWD1			(1 << 0)

/* --- ADC_CCR values ------------------------------------------------------ */

#define ADC_CCR_TSVREFE			(1 << 23)
#define ADC_CCR_ADCPRE1			(1 << 17)
#define ADC_CCR_ADCPRE0			(1 << 16)

/* ADC prescaler */
#define ADC_CCR_ADCPRE_HSI_1		(0 << 16)
#define ADC_CCR_ADCPRE_HSI_2		(1 << 16)
#define ADC_CCR_ADCPRE_HSI_4		(2 << 16)

/* --- Power-up time ------------------------------------------------------- */

#define ADC_T_STAB			4	/* 4(3.5) usec */

/* --- Function prototypes ------------------------------------------------- */

/* External trigger edge */
typedef enum {
	ADC_TRIGGER_DISABLE,
	ADC_RISING,
	ADC_FALLING,
	ADC_BOTH
} adc_trigger_edge_t;

/* External trigger source */
typedef enum {
	ADC_TIM9_CC2,
	ADC_TIM9_TRGO,
	ADC_TIM2_CC3,
	ADC_TIM2_CC2,
	ADC_TIM3_TRGO,
	ADC_TIM4_CC4,
	ADC_TIM2_TRGO,
	ADC_TIM3_CC1,
	ADC_TIM3_CC3,
	ADC_TIM4_TRGO,
	ADC_TIM6_TRGO,
	ADC_EXTI11,

	ADC_TIM9_CC1,
	ADC_TIM2_CC1,
	ADC_TIM3_CC4,
	ADC_TIM4_CC1,
	ADC_TIM4_CC2,
	ADC_TIM4_CC3,
	ADC_TIM10_CC1,
	ADC_TIM7_TRGO,
	ADC_EXTI15
} adc_trigger_source_t;

/* Interrupt and status */
enum {
	ADC_ANALOG_WATCHDOG = (1 << 0),
	ADC_REGULAR_END = (1 << 1),
	ADC_INJECTED_END = (1 << 2),
	ADC_INJECTED_START = (1 << 3),
	ADC_REGULAR_START = (1 << 4),
	ADC_OVERRUN = (1 << 5),
	ADC_ON = (1 << 6),
	ADC_REGULAR_NOT_READY = (1 << 8),
	ADC_INJECTED_NOT_READY = (1 << 9)
};

/* Input */
enum {
	ADC_IN_PA0 = 0,
	ADC_IN_PA1 = 1,
	ADC_IN_PA2 = 2,
	ADC_IN_PA3 = 3,
	ADC_IN_PA4 = 4,
	ADC_IN_PA5 = 5,
	ADC_IN_PA6 = 6,
	ADC_IN_PA7 = 7,
	ADC_IN_PB0 = 8,
	ADC_IN_PB1 = 9,
	ADC_IN_PC0 = 10,
	ADC_IN_PC1 = 11,
	ADC_IN_PC2 = 12,
	ADC_IN_PC3 = 13,
	ADC_IN_PC4 = 14,
	ADC_IN_PC5 = 15,
	ADC_IN_TEMP = 16,
	ADC_IN_VREFINT = 17,
	ADC_IN_PB12 = 18,
	ADC_IN_PB13 = 19,
	ADC_IN_PB14 = 20,
	ADC_IN_PB15 = 21,
	ADC_IN_PE7 = 22,
	ADC_IN_PE8 = 23,
	ADC_IN_PE9 = 24,
	ADC_IN_PE10 = 25,

	ADC_IN_PF6 = 27,
	ADC_IN_PF7 = 28,
	ADC_IN_PF8 = 29,
	ADC_IN_PF9 = 30,
	ADC_IN_PF10 = 31,

	/* Bank B */
	ADC_IN_PB2 = 0,
	ADC_IN_PF11 = 1,
	ADC_IN_PF12 = 2,
	ADC_IN_PF13 = 3,

	ADC_IN_PF14 = 6,
	ADC_IN_PF15 = 7,
	ADC_IN_PG0 = 8,
	ADC_IN_PG1 = 9,
	ADC_IN_PG2 = 10,
	ADC_IN_PG3 = 11,
	ADC_IN_PG4 = 12
};

void adc_start_regular_conversion(void);
void adc_start_injected_conversion(void);
u16 adc_get_regular_data(void);
u16 adc_get_injected_data(int index);
void adc_config_power_down(bool idle, bool delay);
void adc_enable(void);
void adc_disable(void);
void adc_set_prescaler(int pre);
void adc_set_bank(int bank);
void adc_set_regular_channel(int channel);
void adc_set_regular_sequence(int length, int *channel);
void adc_set_injected_sequence(int length, int *channel);
void adc_enable_continuous(void);
void adc_disable_continuous(void);
void adc_set_analog_watchdog(int low, int high, bool regular, bool injected,
			     bool single, int channel);
void adc_enable_scan(bool channel_eoc);
void adc_disable_scan(void);
void adc_enable_auto_injected(void);
void adc_disable_auto_injected(void);
void adc_enable_regular_discontinuous(int num, bool channel_eoc);
void adc_disable_regular_discontinuous(void);
void adc_enable_injected_discontinuous(int num);
void adc_disable_injected_discontinuous(void);
void adc_set_right_alignment(void);
void adc_set_left_alignment(void);
void adc_set_offset(int index, int offset);
void adc_set_sampling(int channel, int sampling);
void adc_set_regular_ext(adc_trigger_edge_t edge, adc_trigger_source_t source);
void adc_set_injected_ext(adc_trigger_edge_t edge, adc_trigger_source_t source);
void adc_wait_for_regular_ready(void);
void adc_wait_for_injected_ready(void);
void adc_set_resolution(int res);
void adc_set_delay(int delay);
void adc_enable_dma(bool circular);
void adc_disable_dma(void);
void adc_enable_ts_vref(void);
void adc_disable_ts_vref(void);
void adc_enable_interrupt(int interrupt);
void adc_disable_interrupt(int interrupt);
int adc_get_interrupt_mask(int interrupt);
int adc_get_interrupt_status(int interrupt);
void adc_clear_interrupt(int interrupt);
