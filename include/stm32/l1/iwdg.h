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
 * 20 Independent watchdog (IWDG)
 */

/* --- IWDG registers ------------------------------------------------------ */
/*
 * Offset	Register
 * 0x00		IWDG_KR		Key register
 * 0x04		IWDG_PR		Prescaler register
 * 0x08		IWDG_RLR	Reload register
 * 0x0c		IWDG_SR		Status register
 */

#define IWDG_KR				MMIO32(IWDG_BASE + 0x00)
#define IWDG_PR				MMIO32(IWDG_BASE + 0x04)
#define IWDG_RLR			MMIO32(IWDG_BASE + 0x08)
#define IWDG_SR				MMIO32(IWDG_BASE + 0x0c)

/* --- IWDG_KR values ------------------------------------------------------ */

/* KEY[15:0]: Key value (write-only, reads as 0x0000) */
#define IWDG_KR_RESET			0xaaaa
#define IWDG_KR_UNLOCK			0x5555
#define IWDG_KR_START			0xcccc

/* --- IWDG_PR values ------------------------------------------------------ */

/* PR[2:0]: Prescaler divider */
#define IWDG_PR_DIV4			0
#define IWDG_PR_DIV8			1
#define IWDG_PR_DIV16			2
#define IWDG_PR_DIV32			3
#define IWDG_PR_DIV64			4
#define IWDG_PR_DIV128			5
#define IWDG_PR_DIV256			6

/* --- IWDG_RLR values ----------------------------------------------------- */

/* RL[11:0]: Watchdog counter reload value */

/* --- IWDG_SR values ------------------------------------------------------ */

#define IWDG_SR_RVU			(1 << 1)
#define IWDG_SR_PVU			(1 << 0)

/* --- Function prototypes ------------------------------------------------- */

/* Status */
enum {
	IWDG_PRESCALER_UPDATE = (1 << 0),
	IWDG_RELOAD_UPDATE = (1 << 1)
};

void iwdg_set_timeout(int prescaler, int reload);
void iwdg_start(void);
void iwdg_reset(void);
int iwdg_get_status(int status);
