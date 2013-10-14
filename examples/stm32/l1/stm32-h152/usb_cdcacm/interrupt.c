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

#include <usbdevfs.h>

#include <usb/standard.h>

#include "usb_cdcacm.h"
#include "class.h"
#include "interrupt.h"

static volatile bool busy;

void interrupt_notify(void)
{
	extern ep_state_t endpoint_state[MAXENDPOINT];
	u8 buf[NOTIFICATION_SIZE] __attribute__ ((aligned(2)));
	int len;

	if (endpoint_state[1] == EP_STATE_ENABLE && !busy) {
		len = class_notify(buf);
		if (len > 0) {
			/* Write packet. */
			usbdevfs_write(1, (u16 *)buf, len);

			/* TX NAK->VALID */
			usbdevfs_start_endpoint_tx(1);

			/* Set flag. */
			busy = true;
		}
	}
}

void interrupt_notify_cont(void)
{
	u8 buf[NOTIFICATION_SIZE] __attribute__ ((aligned(2)));
	int len;

	len = class_notify(buf);
	if (len > 0) {
		/* Write packet. */
		usbdevfs_write(1, (u16 *)buf, len);

		/* TX NAK->VALID */
		usbdevfs_start_endpoint_tx(1);
	} else {
		/* Clear flag. */
		busy = false;
	}
}

void interrupt_reset(void)
{
	/* Clear flag. */
	busy = false;
}
