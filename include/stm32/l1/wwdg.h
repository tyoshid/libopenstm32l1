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
 * 21 Window watchdog (WWDG)
 */

/* --- WWDG registers ------------------------------------------------------ */
/*
 * Offset	Register
 * 0x00		WWDG_CR		Control register
 * 0x04		WWDG_CFR	Configuration register
 * 0x08		WWDG_SR		Status register
 */

#define WWDG_CR				MMIO32(WWDG_BASE + 0x00)
#define WWDG_CFR			MMIO32(WWDG_BASE + 0x04)
#define WWDG_SR				MMIO32(WWDG_BASE + 0x08)

/* --- WWDG_CR values ------------------------------------------------------ */

#define WWDG_CR_WDGA			(1 << 7)
#define WWDG_CR_T6			(1 << 6)
#define WWDG_CR_T5			(1 << 5)
#define WWDG_CR_T4			(1 << 4)
#define WWDG_CR_T3			(1 << 3)
#define WWDG_CR_T2			(1 << 2)
#define WWDG_CR_T1			(1 << 1)
#define WWDG_CR_T0			(1 << 0)

/* --- WWDG_CFR values ----------------------------------------------------- */

#define WWDG_CFR_EWI			(1 << 9)
#define WWDG_CFR_WDGTB1			(1 << 8)
#define WWDG_CFR_WDGTB0			(1 << 7)
#define WWDG_CFG_W6			(1 << 6)
#define WWDG_CFG_W5			(1 << 5)
#define WWDG_CFG_W4			(1 << 4)
#define WWDG_CFG_W3			(1 << 3)
#define WWDG_CFG_W2			(1 << 2)
#define WWDG_CFG_W1			(1 << 1)
#define WWDG_CFG_W0			(1 << 0)

/* Time base */
#define WWDG_CFR_WDGTB_CK_DIV1		(0 << 7)
#define WWDG_CFR_WDGTB_CK_DIV2		(1 << 7)
#define WWDG_CFR_WDGTB_CK_DIV4		(2 << 7)
#define WWDG_CFR_WDGTB_CK_DIV8		(3 << 7)

/* --- WWDG_SR values ------------------------------------------------------ */

#define WWDG_SR_EWIF			(1 << 0)

/* --- Funtion prototypes -------------------------------------------------- */

/* Interrupt */
enum {
	WWDG_EARLY_WAKEUP = (1 << 0)
};

void wwdg_set_window(int prescaler, int window);
void wwdg_reset(int count);
void wwdg_enable_interrupt(int interrupt);
int wwdg_get_interrupt_mask(int interrupt);
int wwdg_get_interrupt_status(int interrupt);
void wwdg_clear_interrupt(int interrupt);
