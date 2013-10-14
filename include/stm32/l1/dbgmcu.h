/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2011 Gareth McMullin <gareth@blacksphere.co.nz>
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
 * 29 Debug support (DBG)
 */

/* Note: Bits marked (**) only exist in high and medium+ density devices. */

/* --- DBG registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		DBGMCU_IDCODE		MCU device ID code
 * 0x04		DBGMCU_CR		Debug MCU configuration register
 * 0x08		DBGMCU_APB1_FZ		Debug MCU APB1 freeze register
 * 0x0c		DBGMCU_APB2_FZ		Debug MCU APB2 freeze register
 */

#define DBGMCU_IDCODE				MMIO32(DBGMCU_BASE + 0x00)
#define DBGMCU_CR				MMIO32(DBGMCU_BASE + 0x04)
#define DBGMCU_APB1_FZ				MMIO32(DBGMCU_BASE + 0x08)
#define DBGMCU_APB2_FZ				MMIO32(DBGMCU_BASE + 0x0c)

/* --- DBGMCU_IDCODE values ------------------------------------------------ */

#define DBGMCU_IDCODE_REV_ID_MASK		0xffff0000
#define DBGMCU_IDCODE_DEV_ID_MASK		0x00000fff

/* Revision identifier */
#define DBGMCU_IDCODE_REV_ID_MEDIUM_REV_A	0x10000000
#define DBGMCU_IDCODE_REV_ID_MEDIUM_REV_Y	0x10080000
#define DBGMCU_IDCODE_REV_ID_MEDIUM_REV_W	0x10380000
#define DBGMCU_IDCODE_REV_ID_MEDIUM_REV_V	0x10780000
#define DBGMCU_IDCODE_REV_ID_MEDIUM_PLUS_REV_A	0x10180000
#define DBGMCU_IDCODE_REV_ID_HIGH_REV_A		0x10000000
#define DBGMCU_IDCODE_REV_ID_HIGH_REV_Z		0x10080000
#define DBGMCU_IDCODE_REV_ID_HIGH_REV_Y		0x10180000

/* Device identifier */
#define DBGMCU_IDCODE_DEV_ID_MEDIUM		0x416
#define DBGMCU_IDCODE_DEV_ID_MEDIUM_PLUS	0x427
#define DBGMCU_IDCODE_DEV_ID_HIGH		0x436

/* --- DBGMCU_CR values ---------------------------------------------------- */

#define DBGMCU_CR_TRACE_MODE1			(1 << 7)
#define DBGMCU_CR_TRACE_MODE0			(1 << 6)
#define DBGMCU_CR_TRACE_IOEN			(1 << 5)
#define DBGMCU_CR_DBG_STANDBY			(1 << 2)
#define DBGMCU_CR_DBG_STOP			(1 << 1)
#define DBGMCU_CR_DBG_SLEEP			(1 << 0)

/* Trace pin assignment control */
#define DBGMCU_CR_TRACE_MODE_ASYNC		(0 << 6)
#define DBGMCU_CR_TRACE_MODE_SYNC_1		(1 << 6)
#define DBGMCU_CR_TRACE_MODE_SYNC_2		(2 << 6)
#define DBGMCU_CR_TRACE_MODE_SYNC_4		(3 << 6)

/* --- DBGMCU_APB1_FZ values ----------------------------------------------- */

#define DBGMCU_APB1_FZ_DBG_I2C2_SMBUS_TIMEOUT	(1 << 22)
#define DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT	(1 << 21)
#define DBGMCU_APB1_FZ_DBG_IWDG_STOP		(1 << 12)
#define DBGMCU_APB1_FZ_DBG_WWDG_STOP		(1 << 11)
#define DBGMCU_APB1_FZ_DBG_RTC_STOP		(1 << 10)	/* (**) */
#define DBGMCU_APB1_FZ_DBG_TIM7_STOP		(1 << 5)
#define DBGMCU_APB1_FZ_DBG_TIM6_STOP		(1 << 4)
#define DBGMCU_APB1_FZ_DBG_TIM5_STOP		(1 << 3)	/* (**) */
#define DBGMCU_APB1_FZ_DBG_TIM4_STOP		(1 << 2)
#define DBGMCU_APB1_FZ_DBG_TIM3_STOP		(1 << 1)
#define DBGMCU_APB1_FZ_DBG_TIM2_STOP		(1 << 0)

/* --- DBGMCU_APB1_FZ values ----------------------------------------------- */

#define DBGMCU_APB2_FZ_DBG_TIM11_STOP		(1 << 4)
#define DBGMCU_APB2_FZ_DBG_TIM10_STOP		(1 << 3)
#define DBGMCU_APB2_FZ_DBG_TIM9_STOP		(1 << 2)

/* --- Function prototypes ------------------------------------------------- */

u32 dbgmcu_get_device_id(void);
