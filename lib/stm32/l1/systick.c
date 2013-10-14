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

#include <stm32/l1/systick.h>

void systick_set_reload(int value)
{
	STK_LOAD = value;
}

int systick_get_value(void)
{
	return STK_VAL;
}

void systick_set_clocksource(int clocksource)
{
	u32 reg32;

	reg32 = STK_CTRL;
	reg32 &= ~STK_CTRL_CLKSOURCE;
	STK_CTRL = (reg32 | clocksource);
}

void systick_enable_interrupt(void)
{
	STK_CTRL |= STK_CTRL_TICKINT;
}

void systick_disable_interrupt(void)
{
	STK_CTRL &= ~STK_CTRL_TICKINT;
}

void systick_enable_counter(void)
{
	STK_CTRL |= STK_CTRL_ENABLE;
}

void systick_disable_counter(void)
{
	STK_CTRL &= ~STK_CTRL_ENABLE;
}

bool systick_countflag(void)
{
	return STK_CTRL & STK_CTRL_COUNTFLAG;
}

int systick_get_calib(void)
{
	return STK_CALIB;
}
