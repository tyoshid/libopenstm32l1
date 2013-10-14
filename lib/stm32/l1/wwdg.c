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

#include <stm32/l1/wwdg.h>

void wwdg_set_window(int prescaler, int window)
{
	int i;

	for (i = 0; i < 4 && prescaler > (1 << i); i++)
		;
	WWDG_CFR = ((i & 3) << 7) | window;
}

void wwdg_reset(int count)
{
	WWDG_CR = WWDG_CR_WDGA | count;
}

void wwdg_enable_interrupt(int interrupt)
{
	if (interrupt & WWDG_EARLY_WAKEUP)
		WWDG_CFR |= WWDG_CFR_EWI;
}

int wwdg_get_interrupt_mask(int interrupt)
{
	int r = 0;

	if (interrupt & WWDG_EARLY_WAKEUP) {
		if (WWDG_CFR & WWDG_CFR_EWI)
			r |= WWDG_EARLY_WAKEUP;
	}
	return r;
}

int wwdg_get_interrupt_status(int interrupt)
{
	return WWDG_SR & interrupt;
}

void wwdg_clear_interrupt(int interrupt)
{
	WWDG_SR &= ~interrupt;
}
