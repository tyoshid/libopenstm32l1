/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Uwe Hermann <uwe@hermann-uwe.de>
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
 * 24 Flexible static memory controller (FSMC)
 */

/* --- FSMC registers ------------------------------------------------------ */
/*
 * Offset	Register
 * 0x0000	FSMC_BCR1	SRAM/NOR-Flash chip-select control registers 1
 * 0x0004	FSMC_BTR1	SRAM/NOR-Flash chip-select timing registers 1
 * 0x0008	FSMC_BCR2	SRAM/NOR-Flash chip-select control registers 2
 * 0x000c	FSMC_BTR2	SRAM/NOR-Flash chip-select timing registers 2
 * 0x0010	FSMC_BCR3	SRAM/NOR-Flash chip-select control registers 3
 * 0x0014	FSMC_BTR3	SRAM/NOR-Flash chip-select timing registers 3
 * 0x0018	FSMC_BCR4	SRAM/NOR-Flash chip-select control registers 4
 * 0x001c	FSMC_BTR4	SRAM/NOR-Flash chip-select timing registers 4
 * 0x0104	FSMC_BWTR1	SRAM/NOR-Flash write timing registers 1
 * 0x010c	FSMC_BWTR2	SRAM/NOR-Flash write timing registers 2
 * 0x0114	FSMC_BWTR3	SRAM/NOR-Flash write timing registers 3
 * 0x011c	FSMC_BWTR4	SRAM/NOR-Flash write timing registers 4
 */

/* FSMC_BCRx */
#define FSMC_BCR(x)			MMIO32(FSMC_BASE + 0x00 + 8 * (x))
#define FSMC_BCR1			FSMC_BCR(0)
#define FSMC_BCR2			FSMC_BCR(1)
#define FSMC_BCR3			FSMC_BCR(2)
#define FSMC_BCR4			FSMC_BCR(3)

/* FSMC_BTRx */
#define FSMC_BTR(x)			MMIO32(FSMC_BASE + 0x04 + 8 * (x))
#define FSMC_BTR1			FSMC_BTR(0)
#define FSMC_BTR2			FSMC_BTR(1)
#define FSMC_BTR3			FSMC_BTR(2)
#define FSMC_BTR4			FSMC_BTR(3)

/* FSMC_BWTRx */
#define FSMC_BWTR(x)			MMIO32(FSMC_BASE + 0x104 + 8 * (x))
#define FSMC_BWTR1			FSMC_BWTR(0)
#define FSMC_BWTR2			FSMC_BWTR(1)
#define FSMC_BWTR3			FSMC_BWTR(2)
#define FSMC_BWTR4			FSMC_BWTR(3)

/* --- FSMC_BCRx values ---------------------------------------------------- */

#define FSMC_BCR_CBURSTRW		(1 << 19)
#define FSMC_BCR_ASYNCWAIT		(1 << 15)
#define FSMC_BCR_EXTMOD			(1 << 14)
#define FSMC_BCR_WAITEN			(1 << 13)
#define FSMC_BCR_WREN			(1 << 12)
#define FSMC_BCR_WAITCFG		(1 << 11)
#define FSMC_BCR_WRAPMOD		(1 << 10)
#define FSMC_BCR_WAITPOL		(1 << 9)
#define FSMC_BCR_BURSTEN		(1 << 8)
#define FSMC_BCR_FACCEN			(1 << 6)
#define FSMC_BCR_MWID1			(1 << 5)
#define FSMC_BCR_MWID0			(1 << 4)
#define FSMC_BCR_MTYP1			(1 << 3)
#define FSMC_BCR_MTYP0			(1 << 2)
#define FSMC_BCR_MUXEN			(1 << 1)
#define FSMC_BCR_MBKEN			(1 << 0)

/* Memory databus width */
#define FSMC_BCR_MWID_8BITS		(0 << 4)
#define FSMC_BCR_MWID_16BITS		(1 << 4)

/* Memory type */
#define FSMC_BCR_MTYP_SRAM_ROM		(0 << 2)
#define FSMC_BCR_MTYP_PSRAM		(1 << 2)
#define FSMC_BCR_MTYP_NOR_ONENAND_FLASH	(2 << 2)

/* --- FSMC_BTRx values ---------------------------------------------------- */

/*
 * FSMC_BTR[29:28]: ACCMOD
 * FSMC_BTR[27:24]: DATLAT
 * FSMC_BTR[23:20]: CLKDIV
 * FSMC_BTR[19:16]: BUSTURN
 * FSMC_BTR[15:8]:  DATAST
 * FSMC_BTR[7:4]:   ADDHLD
 * FSMC_BTR[3:0]:   ADDSET
 */

/* Access mode */
#define FSMC_BTR_ACCMOD(x)		((x) << 28)
#define FSMC_BTR_ACCMOD_A		FSMC_BTR_ACCMOD(0)
#define FSMC_BTR_ACCMOD_B		FSMC_BTR_ACCMOD(1)
#define FSMC_BTR_ACCMOD_C		FSMC_BTR_ACCMOD(2)
#define FSMC_BTR_ACCMOD_D		FSMC_BTR_ACCMOD(3)

/* Data latency for synchronous burst NOR flash memory */
/*
 * x = 0: Data latency of 2 CLK clock cycles for first burst access
 * ...
 * x = 15: Data latency of 17 CLK clock cycles for first burst access
 */
#define FSMC_BTR_DATLAT(x)		((x) << 24)

/* Clock divide ratio (for CLK signal) */
/*
 * x = 0: Reserved
 * x = 1: 2 * HCLK periods
 * ...
 * x = 15: 16 * HCLK periods
 */
#define FSMC_BTR_CLKDIV(x)		((x) << 20)

/* Bus turnaround phase duration */
/*
 * x = 0: 0 * HCLK clock cycle added
 * ...
 * x = 15: 15 * HCLK clock cycles added
 */
#define FSMC_BTR_BUSTURN(x)		((x) << 16)

/* Data-phase duration */
/*
 * x = 0: Reserved
 * x = 1: 1 * HCLK clock cycle
 * ...
 * x = 255: 255 * HCLK clock cycles
 */
#define FSMC_BTR_DATAST(x)		((x) << 8)

/* Address-hold phase duration */
/*
 * x = 0: Reserved
 * x = 1: 1 * HCLK clock cycle
 * ...
 * x = 15: 15 * HCLK clock cycles
 */
#define FSMC_BTR_ADDHLD(x)		((x) << 4)

/* Address setup phase duration */
/*
 * x = 0: 0 * HCLK clock cycle
 * ...
 * x = 15: 15 * HCLK clock cycles
 */
#define FSMC_BTR_ADDSET(x)		((x) << 0)

/* --- FSMC_BWTRx values --------------------------------------------------- */

/*
 * FSMC_BWTR[29:28]: ACCMOD
 * FSMC_BWTR[27:24]: DATLAT
 * FSMC_BWTR[23:20]: CLKDIV
 * FSMC_BWTR[19:16]: BUSTURN
 * FSMC_BWTR[15:8]:  DATAST
 * FSMC_BWTR[7:4]:   ADDHLD
 * FSMC_BWTR[3:0]:   ADDSET
 */

/* Access mode */
#define FSMC_BWTR_ACCMOD(x)		((x) << 28)
#define FSMC_BWTR_ACCMOD_A		FSMC_BWTR_ACCMOD(0)
#define FSMC_BWTR_ACCMOD_B		FSMC_BWTR_ACCMOD(1)
#define FSMC_BWTR_ACCMOD_C		FSMC_BWTR_ACCMOD(2)
#define FSMC_BWTR_ACCMOD_D		FSMC_BWTR_ACCMOD(3)

/* Data latency for synchronous burst NOR flash memory */
/*
 * x = 0: Data latency of 2 CLK clock cycles for first burst access
 * ...
 * x = 15: Data latency of 17 CLK clock cycles for first burst access
 */
#define FSMC_BWTR_DATLAT(x)		((x) << 24)

/* Clock divide ratio (for CLK signal) */
/*
 * x = 0: Reserved
 * x = 1: 2 * HCLK periods
 * ...
 * x = 15: 16 * HCLK periods
 */
#define FSMC_BWTR_CLKDIV(x)		((x) << 20)

/* Bus turnaround phase duration */
/*
 * x = 0: 0 * HCLK clock cycle added
 * ...
 * x = 15: 15 * HCLK clock cycles added
 */
#define FSMC_BWTR_BUSTURN(x)		((x) << 16)

/* Data-phase duration */
/*
 * x = 0: Reserved
 * x = 1: 1 * HCLK clock cycle
 * ...
 * x = 255: 255 * HCLK clock cycles
 */
#define FSMC_BWTR_DATAST(x)		((x) << 8)

/* Address-hold phase duration */
/*
 * x = 0: Reserved
 * x = 1: 1 * HCLK clock cycle
 * ...
 * x = 15: 15 * HCLK clock cycles
 */
#define FSMC_BWTR_ADDHLD(x)		((x) << 4)

/* Address setup phase duration */
/*
 * x = 0: 0 * HCLK clock cycle
 * ...
 * x = 15: 15 * HCLK clock cycles
 */
#define FSMC_BWTR_ADDSET(x)		((x) << 0)

/* --- Bank ---------------------------------------------------------------- */

#define FSMC_BANK1	0
#define FSMC_BANK2	1
#define FSMC_BANK3	2
#define FSMC_BANK4	3

#define FSMC_BANK1_BASE	FSMC_EXT_MEMORY_BASE + 0x00000000 /* NOR / PSRAM */
#define FSMC_BANK2_BASE	FSMC_EXT_MEMORY_BASE + 0x04000000 /* NOR / PSRAM */
#define FSMC_BANK3_BASE	FSMC_EXT_MEMORY_BASE + 0x08000000 /* NOR / PSRAM */
#define FSMC_BANK4_BASE	FSMC_EXT_MEMORY_BASE + 0x0c000000 /* NOR / PSRAM */
