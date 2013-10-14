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

#include <stm32/l1/flash.h>

void flash_enable_64bit_access(int wait)
{
	u32 reg32;

	reg32 = FLASH_ACR;
	reg32 |= FLASH_ACR_ACC64;
	FLASH_ACR = reg32;
	if (wait >= 1)
		reg32 |= FLASH_ACR_LATENCY;
	else
		reg32 &= ~FLASH_ACR_LATENCY;
	reg32 |= FLASH_ACR_PRFTEN;
	FLASH_ACR = reg32;
}

void flash_disable_64bit_access(void)
{
	u32 reg32;

	reg32 = FLASH_ACR;
	reg32 &= ~(FLASH_ACR_LATENCY | FLASH_ACR_PRFTEN);
	FLASH_ACR = reg32;
	reg32 &= ~FLASH_ACR_ACC64;
	FLASH_ACR = reg32;
}

void flash_unlock_pecr(void)
{
	FLASH_PEKEYR = FLASH_PEKEY1;
	FLASH_PEKEYR = FLASH_PEKEY2;
}

void flash_lock_pecr(void)
{
	FLASH_PECR = FLASH_PECR_PELOCK;
}

void flash_unlock_program_memory(void)
{
	FLASH_PRGKEYR = FLASH_PRGKEY1;
	FLASH_PRGKEYR = FLASH_PRGKEY2;
}

void flash_lock_program_memory(void)
{
	FLASH_PECR = FLASH_PECR_PRGLOCK;
}

void flash_unlock_option_byte(void)
{
	FLASH_OPTKEYR = FLASH_OPTKEY1;
	FLASH_OPTKEYR = FLASH_OPTKEY2;
}

void flash_lock_option_byte(void)
{
	FLASH_PECR = FLASH_PECR_OPTLOCK;
}

void flash_unlock_power_down(void)
{
	FLASH_PDKEYR = FLASH_PDKEY1;
	FLASH_PDKEYR = FLASH_PDKEY2;
}

void flash_enable_interrupt(int interrupt)
{
	u32 reg32 = 0;

	if (interrupt & FLASH_ERROR)
		reg32 |= FLASH_PECR_ERRIE;
	if (interrupt & FLASH_EOP)
		reg32 |= FLASH_PECR_EOPIE;

	FLASH_PECR |= reg32;
}

void flash_disable_interrupt(int interrupt)
{
	u32 reg32 = 0;

	if (interrupt & FLASH_ERROR)
		reg32 |= FLASH_PECR_ERRIE;
	if (interrupt & FLASH_EOP)
		reg32 |= FLASH_PECR_EOPIE;

	FLASH_PECR &= ~reg32;
}

int flash_get_interrupt_mask(int interrupt)
{
	u32 reg32;
	int r = 0;

	reg32 = FLASH_PECR;
	if (reg32 & FLASH_PECR_ERRIE)
		r |= FLASH_ERROR;
	if (reg32 & FLASH_PECR_EOPIE)
		r |= FLASH_EOP;
	return r & interrupt;
}

int flash_get_interrupt_status(int interrupt)
{
	return FLASH_SR & interrupt;
}

void flash_clear_interrupt(int interrupt)
{
	FLASH_SR = interrupt;
}

void flash_enable_fixed_time_data_write(void)
{
	FLASH_PECR |= FLASH_PECR_FTDW;
}

void flash_disable_fixed_time_data_write(void)
{
	FLASH_PECR &= ~FLASH_PECR_FTDW;
}

void flash_wait_for_last_operation(void)
{
	/* Polling */
	while (FLASH_SR & FLASH_SR_BSY)
		;
}

/* Data EEPROM */
void flash_erase_double_word(u32 address)
{
	FLASH_PECR |= (FLASH_PECR_ERASE | FLASH_PECR_DATA);
	while (FLASH_SR & FLASH_SR_BSY)
		;
	*(u32 *)address = 0;
	*(u32 *)(address + 4) = 0;
}

/* Program memory */
void flash_erase_page(u32 page_address)
{
	FLASH_PECR |= (FLASH_PECR_ERASE | FLASH_PECR_PROG);
	while (FLASH_SR & FLASH_SR_BSY)
		;
	*(u32 *)page_address = 0;
}

/* Program memory */
void flash_program_half_page(u32 address, u32 *data)
{
	int i;
	u32 *p = (u32 *)address;

	FLASH_PECR |= (FLASH_PECR_FPRG | FLASH_PECR_PROG);
	while (FLASH_SR & FLASH_SR_BSY)
		;
	for (i = 0; i < 32; i++)
		*p++ = *data++;
}

/* Data EEPROM */
void flash_program_double_word(u32 address, u32 *data)
{
	FLASH_PECR |= (FLASH_PECR_FPRG | FLASH_PECR_DATA);
	while (FLASH_SR & FLASH_SR_BSY)
		;
	*(u32 *)address = *data++;
	*(u32 *)(address + 4) = *data;
}
