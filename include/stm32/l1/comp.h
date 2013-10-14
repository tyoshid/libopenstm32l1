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

#include <stm32/l1/memorymap.h>
#include <libopencm3.h>

/*
 * STMicroelectronics(www.st.com)
 *
 * RM0038: STM32L100xx, STM32L151xx, STM32L152xx and STM32L162xx
 *         advanced ARM-based 32-bit MCUs
 * (19-Apr-2013 Rev 8)
 *
 * 13 Comparators (COMP)
 *
 *
 * DS6876: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         16KB SRAM, 4KB EEPROM, LCD, USB, ADC, DAC
 * (07-Feb-2013 Rev 8)
 *
 * 6.3.19 Comparator
 *
 *
 * DS8890: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 256KB Flash,
 *         32KB SRAM, 8KB EEPROM, LCD, USB, ADC, DAC
 * (01-Feb-2013 Rev 3)
 *
 * 6.3.21 Comparator
 *
 *
 * DS8576: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F
 * (01-Feb-2013 Rev 6)
 *
 * 6.3.23 Comparator
 *
 *
 * DS8669: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F, AES
 * (06-Feb-2013 Rev 4)
 *
 * 6.3.23 Comparator
 *
 *
 * DS9496: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         10KB SRAM, 2KB EEPROM, LCD, USB, ADC, DAC
 * (21-Feb-2013 Rev 1)
 *
 * 6.3.18 Comparator
 */

/* Note: Bits marked (**) only exist in high and medium+ density devices. */

/* --- COMP register ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		COMP_CSR	COMP comparator control and status register
 */

#define COMP_CSR			MMIO32(COMP_BASE + 0x00)

/* --- COMP_CSR values ----------------------------------------------------- */

#define COMP_CSR_TSUSP			(1 << 31)	/* (**) */
#define COMP_CSR_CAIF			(1 << 30)	/* (**) */
#define COMP_CSR_CAIE			(1 << 29)	/* (**) */
#define COMP_CSR_RCH13			(1 << 28)	/* (**) */
#define COMP_CSR_FCH8			(1 << 27)	/* (**) */
#define COMP_CSR_FCH3			(1 << 26)	/* (**) */
#define COMP_CSR_OUTSEL2		(1 << 23)
#define COMP_CSR_OUTSEL1		(1 << 22)
#define COMP_CSR_OUTSEL0		(1 << 21)
#define COMP_CSR_INSEL2			(1 << 20)
#define COMP_CSR_INSEL1			(1 << 19)
#define COMP_CSR_INSEL0			(1 << 18)
#define COMP_CSR_WNDWE			(1 << 17)
#define COMP_CSR_VREFOUTEN		(1 << 16)
#define COMP_CSR_CMP2OUT		(1 << 13)
#define COMP_CSR_SPEED			(1 << 12)
#define COMP_CSR_CMP1OUT		(1 << 7)
#define COMP_CSR_SW1			(1 << 5)	/* (**) */
#define COMP_CSR_CMP1EN			(1 << 4)
#define COMP_CSR_400KPD			(1 << 3)
#define COMP_CSR_10KPD			(1 << 2)
#define COMP_CSR_400KPU			(1 << 1)
#define COMP_CSR_10KPU			(1 << 0)

/* Comparator 2 output selection */
#define COMP_CSR_OUTSEL_TIM2_IC4	(0 << 21)
#define COMP_CSR_OUTSEL_TIM2_OCREF_CLR	(1 << 21)
#define COMP_CSR_OUTSEL_TIM3_IC4	(2 << 21)
#define COMP_CSR_OUTSEL_TIM3_OCREF_CLR	(3 << 21)
#define COMP_CSR_OUTSEL_TIM4_IC4	(4 << 21)
#define COMP_CSR_OUTSEL_TIM4_OCREF_CLR	(5 << 21)
#define COMP_CSR_OUTSEL_TIM10_IC1	(6 << 21)

/* Inverted input selection */
#define COMP_CSR_INSEL_NONE		(0 << 18)
#define COMP_CSR_INSEL_EXTERNAL		(1 << 18)
#define COMP_CSR_INSEL_VREFINT		(2 << 18)
#define COMP_CSR_INSEL_3_4_VREFINT	(3 << 18)
#define COMP_CSR_INSEL_1_2_VREFINT	(4 << 18)
#define COMP_CSR_INSEL_1_4_VREFINT	(5 << 18)
#define COMP_CSR_INSEL_DAC_OUT1		(6 << 18)
#define COMP_CSR_INSEL_DAC_OUT2		(7 << 18)

/* --- Comparator startup time --------------------------------------------- */

#define COMP1_T_START			10	/* 10 usec */
#define COMP2_T_START_FAST		20	/* 20 usec */
#define COMP2_T_START_SLOW		25	/* 25 usec */

/* --- Function prototypes ------------------------------------------------- */

/* Output status */
enum {
	COMP1_OUT = (1 << 7),
	COMP2_OUT = (1 << 13),

	COMP_CA = (1 << 30)
};

/* Comparator 1 control */
enum {
	COMP1_ENABLE = (1 << 4),
	COMP1_400KPD = (1 << 3),
	COMP1_10KPD = (1 << 2),
	COMP1_400KPU = (1 << 1),
	COMP1_10KPU = (1 << 0),

	COMP1_SW1 = (1 << 5)
};

/* Comparator 2 control */
enum {
	COMP2_PB3 = (1 << 18),
	COMP2_VREFINT = (2 << 18),
	COMP2_3_4_VREFINT = (3 << 18),
	COMP2_1_2_VREFINT = (4 << 18),
	COMP2_1_4_VREFINT = (5 << 18),
	COMP2_DAC_OUT1 = (6 << 18),
	COMP2_DAC_OUT2 = (7 << 18),

	COMP2_TIM2_IC4 = (0 << 21),
	COMP2_TIM2_OCREF_CLR = (1 << 21),
	COMP2_TIM3_IC4 = (2 << 21),
	COMP2_TIM3_OCREF_CLR = (3 << 21),
	COMP2_TIM4_IC4 = (4 << 21),
	COMP2_TIM5_OCREF_CLR = (5 << 21),
	COMP2_TIM10_IC1 = (6 << 21),

	COMP2_FAST = (1 << 12)
};

/* Other control */
enum {
	COMP_WNDWE = (1 << 17),
	COMP_VREFOUTEN = (1 << 16),

	COMP_FCH3 = (1 << 26),
	COMP_FCH8 = (1 << 27),
	COMP_RCH13 = (1 << 28),
	COMP_TSUSP = (1 << 31)
	/* COMP_CA */
};

void comp_set_control(int control);
void comp_enable_control(int control);
void comp_disable_control(int control);
int comp_get_status(int status);
