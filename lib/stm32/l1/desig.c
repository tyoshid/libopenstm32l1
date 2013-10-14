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

#include <stm32/l1/desig.h>

int desig_get_flash_size(u16 dev_id)
{
	switch (dev_id) {
	case 0x416:
		return DESIG_FLASH_SIZE * 1024;
	case 0x427:
		return DESIG_H_FLASH_SIZE * 1024;
	case 0x436:
		if (DESIG_H_FLASH_SIZE)
			return 256 * 1024;
		else
			return 384 * 1024;
	default:
		break;
	}
	return 0;
}

int desig_get_unique_id(u16 dev_id, u32 *result)
{
	if (dev_id == 0x416) {
		*result++ = DESIG_UID_31_0;
		*result++ = DESIG_UID_63_32;
		*result = DESIG_UID_95_64;
	} else if (dev_id == 0x436 || dev_id == 427) {
		*result++ = DESIG_H_UID_31_0;
		*result++ = DESIG_H_UID_63_32;
		*result = DESIG_H_UID_95_64;
	} else {
		return -1;
	}
	return 0;
}
