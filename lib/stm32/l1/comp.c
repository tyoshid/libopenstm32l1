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

#include <stm32/l1/comp.h>

void comp_set_control(int control)
{
	if (control & COMP_CA)
		control = (control & ~COMP_CA) | COMP_CSR_CAIE;
	COMP_CSR = control;
}

void comp_enable_control(int control)
{
	if (control & COMP_CA)
		control = (control & ~COMP_CA) | COMP_CSR_CAIE;
	COMP_CSR |= control;
}

void comp_disable_control(int control)
{
	if (control & COMP_CA)
		control = (control & ~COMP_CA) | COMP_CSR_CAIE;
	COMP_CSR &= ~control;
}

int comp_get_status(int status)
{
	return COMP_CSR & status;
}
