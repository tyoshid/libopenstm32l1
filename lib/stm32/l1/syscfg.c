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

#include <stm32/l1/syscfg.h>

void syscfg_set_memory_mapping(syscfg_mem_t mapping)
{
	SYSCFG_MEMRMP = mapping;
}

int syscfg_get_memory_mapping(void)
{
	return SYSCFG_MEMRMP;
}

#if 0
void syscfg_enable_lcd_cap(int cap)
{
	SYSCFG_PMC |= cap;
}
#endif

void syscfg_enable_usb_pullup(void)
{
	SYSCFG_PMC |= SYSCFG_PMC_USB_PU;
}

void syscfg_disable_usb_pullup(void)
{
	SYSCFG_PMC &= ~SYSCFG_PMC_USB_PU;
}

void syscfg_select_exti_source(int exti, syscfg_exti_t config)
{
	u32 reg32[4];
	int i;

	if (exti & 0xf)
		reg32[0] = SYSCFG_EXTICR1;
	if (exti & 0xf0)
		reg32[1] = SYSCFG_EXTICR2;
	if (exti & 0xf00)
		reg32[2] = SYSCFG_EXTICR3;
	if (exti & 0xf000)
		reg32[3] = SYSCFG_EXTICR4;

	for (i = 0; i < 16; i++) {
		if (exti & (1 << i)) {
			reg32[i / 4] &= ~(0xf << ((i % 4) * 4));
			reg32[i / 4] |= (config << ((i % 4) * 4));
		}
	}

	if (exti & 0xf)
		SYSCFG_EXTICR1 = reg32[0];
	if (exti & 0xf0)
		SYSCFG_EXTICR2 = reg32[1];
	if (exti & 0xf00)
		SYSCFG_EXTICR3 = reg32[2];
	if (exti & 0xf000)
		SYSCFG_EXTICR4 = reg32[3];
}

void syscfg_read_exti_source(syscfg_exti_t *config)
{
	u32 reg32[4];
	int i;

	reg32[0] = SYSCFG_EXTICR1;
	reg32[1] = SYSCFG_EXTICR2;
	reg32[2] = SYSCFG_EXTICR3;
	reg32[3] = SYSCFG_EXTICR4;

	for (i = 0; i < 16; i++)
		*config++ = (reg32[i / 4] >> ((i % 4) * 4)) & 0xf;
}

void syscfg_write_exti_source(syscfg_exti_t *config)
{
	u32 reg32[4] = {0, 0, 0, 0};
	int i;

	for (i = 0; i < 16; i++)
		reg32[i / 4] |= ((*config++) << ((i % 4) * 4));

	SYSCFG_EXTICR1 = reg32[0];
	SYSCFG_EXTICR2 = reg32[1];
	SYSCFG_EXTICR3 = reg32[2];
	SYSCFG_EXTICR4 = reg32[3];
}
