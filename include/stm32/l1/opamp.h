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
 * 14 Operational amplifiers (OPAMP)
 */

/* --- OPAMP registers ----------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		OPAMP_CSR	OPAMP control/status register
 * 0x04		OPAMP_OTR	OPAMP offset trimming register for normal mode
 * 0x08		OPAMP_LPOTR	OPAMP offset trimming register for low power
 *				mode
 */

#define OPAMP_CSR			MMIO32(OPAMP_BASE + 0x00)
#define OPAMP_OTR			MMIO32(OPAMP_BASE + 0x04)
#define OPAMP_LPOTR			MMIO32(OPAMP_BASE + 0x08)

/* --- OPAMP_CSR values ---------------------------------------------------- */

#define OPAMP_CSR_OPA3CALOUT		(1 << 31)
#define OPAMP_CSR_OPA2CALOUT		(1 << 30)
#define OPAMP_CSR_OPA1CALOUT		(1 << 29)
#define OPAMP_CSR_OPA_RANGE		(1 << 28)
#define OPAMP_CSR_S7SEL2		(1 << 27)
#define OPAMP_CSR_ANAWSEL3		(1 << 26)
#define OPAMP_CSR_ANAWSEL2		(1 << 25)
#define OPAMP_CSR_ANAWSEL1		(1 << 24)
#define OPAMP_CSR_OPA3LPM		(1 << 23)
#define OPAMP_CSR_OPA3CAL_H		(1 << 22)
#define OPAMP_CSR_OPA3CAL_L		(1 << 21)
#define OPAMP_CSR_S6SEL3		(1 << 20)
#define OPAMP_CSR_S5SEL3		(1 << 19)
#define OPAMP_CSR_S4SEL3		(1 << 18)
#define OPAMP_CSR_S3SEL3		(1 << 17)
#define OPAMP_CSR_OPA3PD		(1 << 16)
#define OPAMP_CSR_OPA2LPM		(1 << 15)
#define OPAMP_CSR_OPA2CAL_H		(1 << 14)
#define OPAMP_CSR_OPA2CAL_L		(1 << 13)
#define OPAMP_CSR_S6SEL2		(1 << 12)
#define OPAMP_CSR_S5SEL2		(1 << 11)
#define OPAMP_CSR_S4SEL2		(1 << 10)
#define OPAMP_CSR_S3SEL2		(1 << 9)
#define OPAMP_CSR_OPA2PD		(1 << 8)
#define OPAMP_CSR_OPA1LPM		(1 << 7)
#define OPAMP_CSR_OPA1CAL_H		(1 << 6)
#define OPAMP_CSR_OPA1CAL_L		(1 << 5)
#define OPAMP_CSR_S6SEL1		(1 << 4)
#define OPAMP_CSR_S5SEL1		(1 << 3)
#define OPAMP_CSR_S4SEL1		(1 << 2)
#define OPAMP_CSR_S3SEL1		(1 << 1)
#define OPAMP_CSR_OPA1PD		(1 << 0)

/* --- OPAMP_OTR values ---------------------------------------------------- */

#define OPAMP_OTR_OT_USER			(1 << 31)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_HIGH4	(1 << 29)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_HIGH3	(1 << 28)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_HIGH2	(1 << 27)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_HIGH1	(1 << 26)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_HIGH0	(1 << 25)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_LOW4	(1 << 24)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_LOW3	(1 << 23)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_LOW2	(1 << 22)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_LOW1	(1 << 21)
#define OPAMP_OTR_OA3_OPT_OFFSET_TRIM_LOW0	(1 << 20)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_HIGH4	(1 << 19)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_HIGH3	(1 << 18)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_HIGH2	(1 << 17)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_HIGH1	(1 << 16)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_HIGH0	(1 << 15)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_LOW4	(1 << 14)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_LOW3	(1 << 13)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_LOW2	(1 << 12)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_LOW1	(1 << 11)
#define OPAMP_OTR_OA2_OPT_OFFSET_TRIM_LOW0	(1 << 10)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_HIGH4	(1 << 9)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_HIGH3	(1 << 8)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_HIGH2	(1 << 7)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_HIGH1	(1 << 6)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_HIGH0	(1 << 5)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_LOW4	(1 << 4)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_LOW3	(1 << 3)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_LOW2	(1 << 2)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_LOW1	(1 << 1)
#define OPAMP_OTR_OA1_OPT_OFFSET_TRIM_LOW0	(1 << 0)

/* --- OPAMP_LPOTR values -------------------------------------------------- */

#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_HIGH4	(1 << 29)
#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_HIGH3	(1 << 28)
#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_HIGH2	(1 << 27)
#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_HIGH1	(1 << 26)
#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_HIGH0	(1 << 25)
#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_LOW4		(1 << 24)
#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_LOW3		(1 << 23)
#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_LOW2		(1 << 22)
#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_LOW1		(1 << 21)
#define OPAMP_LPOTR_OA3_OPT_OFFSET_TRIM_LP_LOW0		(1 << 20)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_HIGH4	(1 << 19)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_HIGH3	(1 << 18)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_HIGH2	(1 << 17)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_HIGH1	(1 << 16)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_HIGH0	(1 << 15)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_LOW4		(1 << 14)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_LOW3		(1 << 13)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_LOW2		(1 << 12)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_LOW1		(1 << 11)
#define OPAMP_LPOTR_OA2_OPT_OFFSET_TRIM_LP_LOW0		(1 << 10)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_HIGH4	(1 << 9)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_HIGH3	(1 << 8)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_HIGH2	(1 << 7)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_HIGH1	(1 << 6)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_HIGH0	(1 << 5)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_LOW4		(1 << 4)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_LOW3		(1 << 3)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_LOW2		(1 << 2)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_LOW1		(1 << 1)
#define OPAMP_LPOTR_OA1_OPT_OFFSET_TRIM_LP_LOW0		(1 << 0)
