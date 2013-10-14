/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Thomas Otto <tommi@viadmin.org>
 * Copyright (C) 2010 Mark Butler <mbutler@physics.otago.ac.nz>
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
 * PM0062: STM32L151xx, STM32L152xx and STM32L162xx Flash and EEPROM
 *         programming
 * (05-Mar-2012 Rev 5)
 *
 *
 * DS6876: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         16KB SRAM, 4KB EEPROM, LCD, USB, ADC, DAC
 * (07-Feb-2013 Rev 8)
 *
 * Table 35. Flash memory and data EEPROM characteristics
 *
 *
 * DS8890: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 256KB Flash,
 *         32KB SRAM, 8KB EEPROM, LCD, USB, ADC, DAC
 * (01-Feb-2013 Rev 3)
 *
 * Table 35. Flash memory and data EEPROM characteristics
 *
 *
 * DS8576: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F
 * (01-Feb-2013 Rev 6)
 *
 * Table 35. Flash memory and data EEPROM characteristics
 *
 *
 * DS8669: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F, AES
 * (06-Feb-2013 Rev 4)
 *
 * Table 34. Flash memory and data EEPROM characteristics
 *
 *
 * DS9496: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         10KB SRAM, 2KB EEPROM, LCD, USB, ADC, DAC
 * (21-Feb-2013 Rev 1)
 *
 * Table 33. Flash memory and data EEPROM characteristics
 */

/* Note: Bits marked (**) only exist in high density devices. */

/* --- FLASH registers ----------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		FLASH_ACR	Access control register
 * 0x04		FLASH_PECR	Program/erase control register
 * 0x08		FLASH_PDKEYR	Power down key register
 * 0x0c		FLASH_PEKEYR	Program/erase key register
 * 0x10		FLASH_PRGKEYR	Program memory key register
 * 0x14		FLASH_OPTKEYR	Option byte key register
 * 0x18		FLASH_SR	Status register
 * 0x1c		FLASH_OBR	Option byte register
 * 0x20		FLASH_WRPR1	Write protection register 1
 * 0x80		FLASH_WRPR2	Write protection register 2	(**)
 * 0x84		FLASH_WRPR3	Write protection register 3	(***)
 */

#define FLASH_ACR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x00)
#define FLASH_PECR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x04)
#define FLASH_PDKEYR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x08)
#define FLASH_PEKEYR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x0c)
#define FLASH_PRGKEYR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x10)
#define FLASH_OPTKEYR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x14)
#define FLASH_SR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x18)
#define FLASH_OBR			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x1c)
#define FLASH_WRPR1			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x20)
#define FLASH_WRPR2			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x80)
#define FLASH_WRPR3			MMIO32(FLASH_MEM_INTERFACE_BASE + 0x84)

/* --- FLASH_ACR values ---------------------------------------------------- */

#define FLASH_ACR_RUN_PD		(1 << 4)
#define FLASH_ACR_SLEEP_PD		(1 << 3)
#define FLASH_ACR_ACC64			(1 << 2)
#define FLASH_ACR_PRFTEN		(1 << 1)
#define FLASH_ACR_LATENCY		(1 << 0)

/* --- FLASH_PECR values --------------------------------------------------- */

#define FLASH_PECR_OBL_LAUNCH		(1 << 18)
#define FLASH_PECR_ERRIE		(1 << 17)
#define FLASH_PECR_EOPIE		(1 << 16)
#define FLASH_PECR_PARALLBANK		(1 << 15)	/* (***) */
#define FLASH_PECR_FPRG			(1 << 10)
#define FLASH_PECR_ERASE		(1 << 9)
#define FLASH_PECR_FTDW			(1 << 8)
#define FLASH_PECR_DATA			(1 << 4)
#define FLASH_PECR_PROG			(1 << 3)
#define FLASH_PECR_OPTLOCK		(1 << 2)
#define FLASH_PECR_PRGLOCK		(1 << 1)
#define FLASH_PECR_PELOCK		(1 << 0)

/* --- FLASH_PDKEYR Keys --------------------------------------------------- */

#define FLASH_PDKEY1			((u32)0x04152637)
#define FLASH_PDKEY2			((u32)0xfafbfcfd)

/* --- FLASH_PEKEYR Keys --------------------------------------------------- */

#define FLASH_PEKEY1			((u32)0x89abcdef)
#define FLASH_PEKEY2			((u32)0x02030405)

/* --- FLASH_PRGKEYR Keys -------------------------------------------------- */

#define FLASH_PRGKEY1			((u32)0x8c9daebf)
#define FLASH_PRGKEY2			((u32)0x13141516)

/* --- FLASH_OPTKEYR Keys -------------------------------------------------- */

#define FLASH_OPTKEY1			((u32)0xfbead9c8)
#define FLASH_OPTKEY2			((u32)0x24252627)

/* --- FLASH_SR values ----------------------------------------------------- */

#define FLASH_SR_OPTVERRUSR		(1 << 12)	/* (***) */
#define FLASH_SR_OPTVERR		(1 << 11)
#define FLASH_SR_SIZERR			(1 << 10)
#define FLASH_SR_PGAERR			(1 << 9)
#define FLASH_SR_WRPERR			(1 << 8)
#define FLASH_SR_READY			(1 << 3)
#define FLASH_SR_ENDHV			(1 << 2)
#define FLASH_SR_EOP			(1 << 1)
#define FLASH_SR_BSY			(1 << 0)

#define FLASH_SR_ERROR			(FLASH_SR_OPTVERRUSR | \
					 FLASH_SR_OPTVERR | FLASH_SR_SIZERR | \
					 FLASH_SR_PGAERR | FLASH_SR_WRPERR)

/* --- FLASH_OBR values ---------------------------------------------------- */

#define FLASH_OBR_BFB2			(1 << 23)	/* (***) */
#define FLASH_OBR_NRST_STDBY		(1 << 22)
#define FLASH_OBR_NRST_STOP		(1 << 21)
#define FLASH_OBR_IWDG_SW		(1 << 20)
#define FLASH_OBR_BOR_LEV3		(1 << 19)
#define FLASH_OBR_BOR_LEV2		(1 << 18)
#define FLASH_OBR_BOR_LEV1		(1 << 17)
#define FLASH_OBR_BOR_LEV0		(1 << 16)
#define FLASH_OBR_RDPRT7		(1 << 7)
#define FLASH_OBR_RDPRT6		(1 << 6)
#define FLASH_OBR_RDPRT5		(1 << 5)
#define FLASH_OBR_RDPRT4		(1 << 4)
#define FLASH_OBR_RDPRT3		(1 << 3)
#define FLASH_OBR_RDPRT2		(1 << 2)
#define FLASH_OBR_RDPRT1		(1 << 1)
#define FLASH_OBR_RDPRT0		(1 << 0)

/* Brownout reset threshold level */
#define FLASH_OBR_BOR_LEV_BOR_OFF	(0x0 << 16) /* 1.45V-1.55V */
#define FLASH_OBR_BOR_LEV_BOR_LEVEL1	(0x8 << 16) /* 1.69V-1.8V */
#define FLASH_OBR_BOR_LEV_BOR_LEVEL2	(0x9 << 16) /* 1.94V-2.1V */
#define FLASH_OBR_BOR_LEV_BOR_LEVEL3	(0xa << 16) /* 2.3V-2.49V */
#define FLASH_OBR_BOR_LEV_BOR_LEVEL4	(0xb << 16) /* 2.54V-2.74V */
#define FLASH_OBR_BOR_LEV_BOR_LEVEL5	(0xc << 16) /* 2.77V-3.0V */

/* Read protection */
#define FLASH_OBR_RDPRT_LEVEL0		(0xaa << 0) /* not active */
#define FLASH_OBR_RDPRT_LEVEL1		(0x00 << 0) /* default */
#define FLASH_OBR_RDPRT_LEVEL2		(0xcc << 0) /* active */

/* --- FLASH_WRPRx values -------------------------------------------------- */
/*
 * FLASH_WRPRx[31:0]: WRP[n]: Write protection, where n is the number of
 *                            the concerned memory sector
 */

/* --- Program memory ------------------------------------------------------ */

#define FLASH_PAGE_SIZE			256
#define FLASH_SECTOR_SIZE		4096

/* Programming time for word or half-page */
#define FLASH_T_PROG			4	/* 4(3.94) msec */

/* --- Function prototypes ------------------------------------------------- */

/* Interrupt and status */
enum {
	FLASH_BSY = (1 << 0),
	FLASH_EOP = (1 << 1),
	FLASH_ENDHV = (1 << 2),
	FLASH_READY = (1 << 3),

	FLASH_WRPERR = (1 << 8),
	FLASH_PGAERR = (1 << 9),
	FLASH_SIZERR = (1 << 10),
	FLASH_OPTVERR = (1 << 11),
	FLASH_OPTVERRUSR = (1 << 12),

	FLASH_ERROR = (31 << 8)
};

void flash_enable_64bit_access(int wait);
void flash_disable_64bit_access(void);
void flash_unlock_pecr(void);
void flash_lock_pecr(void);
void flash_unlock_program_memory(void);
void flash_lock_program_memory(void);
void flash_unlock_option_byte(void);
void flash_lock_option_byte(void);
void flash_unlock_power_down(void);
void flash_enable_interrupt(int interrupt);
void flash_disable_interrupt(int interrupt);
int flash_get_interrupt_mask(int interrupt);
int flash_get_interrupt_status(int interrupt);
void flash_clear_interrupt(int interrupt);
void flash_enable_fixed_time_data_write(void);
void flash_disable_fixed_time_data_write(void);
void flash_wait_for_last_operation(void);
void flash_erase_double_word(u32 address);
void flash_erase_page(u32 page_address);
void flash_program_half_page(u32 address, u32 *data);
void flash_program_double_word(u32 address, u32 *data);
