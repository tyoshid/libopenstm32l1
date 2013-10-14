/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Thomas Otto <tommi@viadmin.org>
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
 * 3 CRC calculation unit
 */

/* --- CRC registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		CRC_DR		Data register
 * 0x04		CRC_IDR		Independent data register
 * 0x08		CRC_CR		Control register
 */

#define CRC_DR				MMIO32(CRC_BASE + 0x00)
#define CRC_IDR				MMIO32(CRC_BASE + 0x04)
#define CRC_CR				MMIO32(CRC_BASE + 0x08)

/* --- CRC_DR values ------------------------------------------------------- */

/* CRC_DR[31:0]: Data register bits */

/* --- CRC_IDR values ------------------------------------------------------ */

/* CRC_IDR[7:0]: General-purpose 8-bit data register bits */

/* --- CRC_CR values ------------------------------------------------------- */

#define CRC_CR_RESET			(1 << 0)

/* --- Function prototypes ------------------------------------------------- */

void crc_reset(void);
u32 crc_calculate(u32 *data, int size);
