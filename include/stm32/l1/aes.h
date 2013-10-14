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
 * 22 Advanced encryption standard hardware accelerator (AES)
 */

/* --- AES registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		AES_CR		AES control register
 * 0x04		AES_SR		AES status register
 * 0x08		AES_DINR	AES data input register
 * 0x0c		AES_DOUTR	AES data output register
 * 0x10		AES_KEYR0	AES key register 0
 * 0x14		AES_KEYR1	AES key register 1
 * 0x18		AES_KEYR2	AES key register 2
 * 0x1c		AES_KEYR3	AES key register 3
 * 0x20		AES_IVR0	AES initialization vector register 0
 * 0x24		AES_IVR1	AES initialization vector register 1
 * 0x28		AES_IVR2	AES initialization vector register 2
 * 0x2c		AES_IVR3	AES initialization vector register 3
 */

#define AES_CR				MMIO32(AES_BASE + 0x00)
#define AES_SR				MMIO32(AES_BASE + 0x04)
#define AES_DINR			MMIO32(AES_BASE + 0x08)
#define AES_DOUTR			MMIO32(AES_BASE + 0x0c)
#define AES_KEYR0			MMIO32(AES_BASE + 0x10)
#define AES_KEYR1			MMIO32(AES_BASE + 0x14)
#define AES_KEYR2			MMIO32(AES_BASE + 0x18)
#define AES_KEYR3			MMIO32(AES_BASE + 0x1c)
#define AES_IVR0			MMIO32(AES_BASE + 0x20)
#define AES_IVR1			MMIO32(AES_BASE + 0x24)
#define AES_IVR2			MMIO32(AES_BASE + 0x28)
#define AES_IVR3			MMIO32(AES_BASE + 0x2c)

/* --- AES_CR values ------------------------------------------------------- */

#define AES_CR_DMAOUTEN			(1 << 12)
#define AES_CR_DMAINEN			(1 << 11)
#define AES_CR_ERRIE			(1 << 10)
#define AES_CR_CCFIE			(1 << 9)
#define AES_CR_ERRC			(1 << 8)
#define AES_CR_CCFC			(1 << 7)
#define AES_CR_CHMOD1			(1 << 6)
#define AES_CR_CHMOD0			(1 << 5)
#define AES_CR_MODE1			(1 << 4)
#define AES_CR_MODE0			(1 << 3)
#define AES_CR_DATATYPE1		(1 << 2)
#define AES_CR_DATATYPE0		(1 << 1)
#define AES_CR_EN			(1 << 0)

/* AES chaining mode */
#define AES_CR_CHMOD_EBC		(0 << 5)
#define AES_CR_CHMOD_CBC		(1 << 5)
#define AES_CR_CHMOD_CTR		(2 << 5)

/* AES operation mode */
#define AES_CR_MODE_ENCRYPTION			(0 << 3)
#define AES_CR_MODE_KEY_DERIVATION		(1 << 3)
#define AES_CR_MODE_DECRYPTION			(2 << 3)
#define AES_CR_MODE_KEY_DERIVATION_DECRYPTION	(3 << 3)

/* Data type selection */
#define AES_CR_DATATYPE_32BIT		(0 << 1)
#define AES_CR_DATATYPE_16BIT		(1 << 1)
#define AES_CR_DATATYPE_8BIT		(2 << 1)
#define AES_CR_DATATYPE_BITDATA		(3 << 1)

/* --- AES_SR values ------------------------------------------------------- */

#define AES_SR_WRERR			(1 << 2)
#define AES_SR_RDERR			(1 << 1)
#define AES_SR_CCF			(1 << 0)

/* --- AES_DINR values ----------------------------------------------------- */

/* DINR[31:0]: Data input register */

/* --- AES_DOUTR values ---------------------------------------------------- */

/* DOUTR[31:0]: Data output register */

/* --- AES_KEYR0 values ---------------------------------------------------- */

/* KEYR0[31:0]: AES key register (key[31:0]) */

/* --- AES_KEYR1 values ---------------------------------------------------- */

/* KEYR1[31:0]: AES key register (key[63:32]) */

/* --- AES_KEYR2 values ---------------------------------------------------- */

/* KEYR2[31:0]: AES key register (key[95:64]) */

/* --- AES_KEYR3 values ---------------------------------------------------- */

/* KEYR3[31:0]: AES key register (key[127:96]) */

/* --- AES_IVR0 values ----------------------------------------------------- */

/* IVR0[31:0]: Initialization vector register (IVR[31:0]) */

/* --- AES_IVR1 values ----------------------------------------------------- */

/* IVR1[31:0]: Initialization vector register (IVR[63:32]) */

/* --- AES_IVR2 values ----------------------------------------------------- */

/* IVR2[31:0]: Initialization vector register (IVR[95:64]) */

/* --- AES_IVR3 values ----------------------------------------------------- */

/* IVR3[31:0]: Initialization vector register (IVR[127:96]) */
