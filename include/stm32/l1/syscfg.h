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
 * 7 System configuration controller (SYSCFG) and routing interface (RI)
 */

/* --- SYSCFG registers ---------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		SYSCFG_MEMRMP	SYSCFG memory remap register
 * 0x04		SYSCFG_PMC	SYSCFG peripheral mode configuration register
 * 0x08		SYSCFG_EXTICR1	SYSCFG externel interrupt configuration
 *					register 1
 * 0x0c		SYSCFG_EXTICR2	SYSCFG externel interrupt configuration
 *					register 2
 * 0x10		SYSCFG_EXTICR3	SYSCFG externel interrupt configuration
 *					register 3
 * 0x14		SYSCFG_EXTICR4	SYSCFG externel interrupt configuration
 *					register 4
 */

#define SYSCFG_MEMRMP			MMIO32(SYSCFG_BASE + 0x00)
#define SYSCFG_PMC			MMIO32(SYSCFG_BASE + 0x04)
#define SYSCFG_EXTICR1			MMIO32(SYSCFG_BASE + 0x08)
#define SYSCFG_EXTICR2			MMIO32(SYSCFG_BASE + 0x0c)
#define SYSCFG_EXTICR3			MMIO32(SYSCFG_BASE + 0x10)
#define SYSCFG_EXTICR4			MMIO32(SYSCFG_BASE + 0x14)

/* --- SYSCFG_MEMRMP values ------------------------------------------------ */

#define SYSCFG_MEMRMP_BOOT_MODE1	(1 << 9)
#define SYSCFG_MEMRMP_BOOT_MODE0	(1 << 8)
#define SYSCFG_MEMRMP_MEM_MODE1		(1 << 1)
#define SYSCFG_MEMRMP_MEM_MODE0		(1 << 0)

/* Boot mode selected by the boot pins */
#define SYSCFG_MEMRMP_BOOT_MAIN_FLASH	(0 << 8)
#define SYSCFG_MEMRMP_BOOT_SYSTEM_FLASH	(1 << 8)
#define SYSCFG_MEMRMP_BOOT_SRAM		(3 << 8)

/* Memory mapping selection */
#define SYSCFG_MEMRMP_MEM_MAIN_FLASH	(0 << 0)
#define SYSCFG_MEMRMP_MEM_SYSTEM_FLASH	(1 << 0)
#define SYSCFG_MEMRMP_MEM_FSMC		(2 << 0)
#define SYSCFG_MEMRMP_MEM_SRAM		(3 << 0)

/* --- SYSCFG_PMC values --------------------------------------------------- */

#define SYSCFG_PMC_LCD_CAPA4		(1 << 5)
#define SYSCFG_PMC_LCD_CAPA3		(1 << 4)
#define SYSCFG_PMC_LCD_CAPA2		(1 << 3)
#define SYSCFG_PMC_LCD_CAPA1		(1 << 2)
#define SYSCFG_PMC_LCD_CAPA0		(1 << 1)
#define SYSCFG_PMC_USB_PU		(1 << 0)

/* --- SYSCFG_EXTICR1-4 values --------------------------------------------- */

/*
 * SYSCFG_EXTICRn[15:0]: EXTIx[3:0]: EXTI x configuration
 * (x = 4(n-1)..4(n-1)+3)
 */

#define SYSCFG_EXTICR_PA		0
#define SYSCFG_EXTICR_PB		1
#define SYSCFG_EXTICR_PC		2
#define SYSCFG_EXTICR_PD		3
#define SYSCFG_EXTICR_PE		4
#define SYSCFG_EXTICR_PH		5
#define SYSCFG_EXTICR_PF		6	/* (**) */
#define SYSCFG_EXTICR_PG		7	/* (**) */

/* --- Function prototypes ------------------------------------------------- */

/* Memory mapping */
typedef enum {
	SYSCFG_MAIN_FLASH = 0,
	SYSCFG_SYSTEM_FLASH = 1,
	SYSCFG_FSMC = 2,
	SYSCFG_SRAM = 3
} syscfg_mem_t;

/* LCD decoupling capacitance connection */
#if 0
enum {
	SYSCFG_CAP2_PB2 = (1 << 1),	/* VLCDrail2 */
	SYSCFG_CAP1_PB12 = (1 << 2),	/* VLCDrail1 */
	SYSCFG_CAP3_PB0 = (1 << 3),	/* VLCDrail3 */
	SYSCFG_CAP1_PE11 = (1 << 4),	/* VLCDrail1 */
	SYSCFG_CAP3_PE12 = (1 << 5)	/* VLCDrail3 */
};
#endif

/* EXTI configuration */
typedef enum {
	SYSCFG_PA,
	SYSCFG_PB,
	SYSCFG_PC,
	SYSCFG_PD,
	SYSCFG_PE,
	SYSCFG_PH,
	SYSCFG_PF,
	SYSCFG_PG
} syscfg_exti_t;

void syscfg_set_memory_mapping(syscfg_mem_t mapping);
int syscfg_get_memory_mapping(void);
void syscfg_enable_usb_pullup(void);
void syscfg_disable_usb_pullup(void);
void syscfg_select_exti_source(int exti, syscfg_exti_t config);
void syscfg_read_exti_source(syscfg_exti_t *config);
void syscfg_write_exti_source(syscfg_exti_t *config);
