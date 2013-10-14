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

#include <stm32/l1/iwdg.h>

void iwdg_set_timeout(int prescaler, int reload)
{
	int i;

	for (i = 0; i < 7 && prescaler > (1 << (i + 2)); i++)
		;
	IWDG_KR = IWDG_KR_UNLOCK;
	IWDG_PR = (i & 7);
	IWDG_RLR = reload;
}

void iwdg_start(void)
{
	IWDG_KR = IWDG_KR_START;
}

void iwdg_reset(void)
{
	IWDG_KR = IWDG_KR_RESET;
}

int iwdg_get_status(int status)
{
	return IWDG_SR & status;
}
