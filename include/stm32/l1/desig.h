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
 * 30 Device electronic signature
 */

/* --- Device Electronic Signature registers ------------------------------- */
/*
 * Medium density devices
 * Base address			Register
 * 0x1ff8004c			Flash size register
 *
 * 0x1ff80050			Unique device ID registers
 * Offset
 * 0x00				U_ID(31:0)
 * 0x04				U_ID(63:32)
 * 0x14				U_ID(95:64)
 *
 *
 * Medium+ and high density devices
 * Base address			Register
 * 0x1ff800cc			Flash size register
 *
 * 0x1ff800d0			Unique device ID registers
 * Offset
 * 0x00				U_ID(31:0)
 * 0x04				U_ID(63:32)
 * 0x14				U_ID(95:64)
 */

#define DESIG_FLASH_SIZE		MMIO16(DESIG_FLASH_SIZE_BASE + 0x00)
#define DESIG_UID_31_0			MMIO32(DESIG_UNIQUE_ID_BASE + 0x00)
#define DESIG_UID_63_32			MMIO32(DESIG_UNIQUE_ID_BASE + 0x04)
#define DESIG_UID_95_64			MMIO32(DESIG_UNIQUE_ID_BASE + 0x14)

#define DESIG_H_FLASH_SIZE		MMIO16(DESIG_H_FLASH_SIZE_BASE + 0x00)
#define DESIG_H_UID_31_0		MMIO32(DESIG_H_UNIQUE_ID_BASE + 0x00)
#define DESIG_H_UID_63_32		MMIO32(DESIG_H_UNIQUE_ID_BASE + 0x04)
#define DESIG_H_UID_95_64		MMIO32(DESIG_H_UNIQUE_ID_BASE + 0x14)

/* --- Function prototypes ------------------------------------------------- */

int desig_get_flash_size(u16 dev_id);
int desig_get_unique_id(u16 dev_id, u32 *result);
