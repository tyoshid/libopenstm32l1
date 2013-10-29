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
#include <usb/audio10.h>

#include "usb_speaker.h"
#include "class.h"

/* Multe */
static bool mute;

/* Volume */
static s16 volume[3];

/* --- Class-specific Requests --------------------------------------------- */

bool class_request_error(struct usb_setup_data *req)
{
	int cs;
	int cn;

	/* Feature Unit Control Requests */
	/*
	 * Set Feature Unit Control Request
	 *
	 * | bmRequestType | bRequest | wValue    | wIndex          | wLength
	 * | 00100001B     | SET_CUR  | CS and CN | Feature Unit ID | Length of
	 * |               | SET_MIN  |           | and             | parameter
	 * |               | SET_MAX  |           | Interface       | block
	 * |               | SET_RES  |           |                 |
	 *
	 */
	/*
	 * Get Feature Unit Control Request
	 *
	 * | bmRequestType | bRequest | wValue    | wIndex          | wLength
	 * | 10100001B     | GET_CUR  | CS and CN | Feature Unit ID | Length of
	 * |               | GET_MIN  |           | and             | parameter
	 * |               | GET_MAX  |           | Interface       | block
	 * |               | GET_RES  |           |                 |
	 *
	 */
	if (req->wIndex != (FEATURE_UNIT_ID << 8))
		return true;
	cs = (req->wValue >> 8);
	cn = (req->wValue & 0xff);
	switch (req->bmRequestType) {
	case (USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE):
		switch (cs) {
		case USB_AUDIO_MUTE_CONTROL:
			if (req->bRequest != USB_AUDIO_SET_CUR || cn ||
			    req->wLength != 1)
				return true;
			break;
		case USB_AUDIO_VOLUME_CONTROL:
			if (req->bRequest != USB_AUDIO_SET_CUR ||
			    (cn != 1 && cn != 2) ||
			    req->wLength != 2)
				return true;
			break;
		default:
			return true;
		}
		break;
	case (USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE):
		switch (cs) {
		case USB_AUDIO_MUTE_CONTROL:
			if (req->bRequest != USB_AUDIO_GET_CUR || cn)
				return true;
			break;
		case USB_AUDIO_VOLUME_CONTROL:
			if (req->bRequest != USB_AUDIO_GET_CUR &&
			    req->bRequest != USB_AUDIO_GET_MIN &&
			    req->bRequest != USB_AUDIO_GET_MAX &&
			    req->bRequest != USB_AUDIO_GET_RES)
				return true;
			if (cn != 1 && cn != 2)
				return true;
			break;
		default:
			return true;
		}
		break;
	default:
		return true;
	}
	return false;
}

int class_request(struct usb_setup_data *req, u8 *buf, u8 **data)
{
	int cs;
	int cn;
	s16 *p;
	int len = 0;

	cs = (req->wValue >> 8);
	cn = (req->wValue & 0xff);
	switch (req->bmRequestType) {
	case (USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE):
		switch (cs) {
		case USB_AUDIO_MUTE_CONTROL:
			if (*buf) {
				mute = true;
				volume_set(MUTE_VOLUME, MUTE_VOLUME);
			} else {
				mute = false;
				volume_set(volume[1], volume[2]);
			}
			break;
		case USB_AUDIO_VOLUME_CONTROL:
			p = (s16 *)buf;
			volume[cn] = *p;
			if (!mute)
				volume_set(volume[1], volume[2]);
			break;
		default:
			return -1;
		}
		break;
	case (USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE):
		switch (cs) {
		case USB_AUDIO_MUTE_CONTROL:
			*buf = mute;
			*data = buf;
			len = 1;
			break;
		case USB_AUDIO_VOLUME_CONTROL:
			p = (s16 *)buf;
			switch (req->bRequest) {
			case USB_AUDIO_GET_CUR:
				*p = volume[cn];
				break;
			case USB_AUDIO_GET_MIN:
				*p = MIN_VOLUME;
				break;
			case USB_AUDIO_GET_MAX:
				*p = MAX_VOLUME;
				break;
			case USB_AUDIO_GET_RES:
				*p = RES_VOLUME;
				break;
			default:
				return -1;
			}
			*data = buf;
			len = 2;
			break;
		default:
			return -1;
		}
		break;
	default:
		return -1;
	}
	return len;
}

void class_reset(void)
{
	/* Set volume */
	volume[1] = DEFAULT_VOLUME;
	volume[2] = DEFAULT_VOLUME;

	/* Set mute */
	mute = true;
	volume_set(MUTE_VOLUME, MUTE_VOLUME);
}
