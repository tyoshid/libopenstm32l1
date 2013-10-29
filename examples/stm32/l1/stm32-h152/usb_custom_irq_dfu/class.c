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

#include <gpio.h>
#include <scb.h>

#include <usb/standard.h>
#include <usb/dfu.h>

#include "usb_custom_irq_dfu.h"
#include "class.h"

/* --- Vendor-specific Requests -------------------------------------------- */

/*
 * This USB device has GPIOE(LED) read/write vendor requests.
 * GPIO10 is STAT1 LED, GPIO11 is STAT2 LED.
 * If you set bit 10 (0x0400), STAT1 LED is turned off.
 *
 * | bmRequestType | bRequest | wValue      | wIndex | wLength | Data
 * | 11000000B     | 0(read)  | Zero        | Zero   | Two     | GPIOE Value
 * | 01000000B     | 1(write) | GPIOE Value | Zero   | Zero    | None
 *
 */

bool vendor_request_error(struct usb_setup_data *req)
{
	switch (req->bRequest) {
	case 0:
		/*
		 * GET_GPIOE
		 *
		 * | bmRequestType | bRequest | wValue      | wIndex | wLength
		 * | 11000000B     | 0        | Zero        | Zero   | Two
		 */

		/* bmRequestType = 11000000B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_VENDOR |
					   USB_RECIP_DEVICE))
			return true;

		/* wLength = 2 */
		if (req->wLength != 2)
			return true;

		/* Not specified. */
		if (req->wValue || req->wIndex)
			return true;
		break;
	case 1:
		/*
		 * SET_GPIOE
		 *
		 * | bmRequestType | bRequest | wValue      | wIndex | wLength
		 * | 01000000B     | 1        | GPIOE Value | Zero   | Zero
		 */

		/* bmRequestType = 01000000B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_VENDOR |
					   USB_RECIP_DEVICE))
			return true;

		/* Not specified. */
		if (req->wLength || req->wIndex)
			return true;
		break;
	default:
		return true;
	}
	return false;
}

int vendor_request(struct usb_setup_data *req, u8 *buf, u8 **data)
{
	u16 *p;

	switch (req->bRequest) {
	case 0:			/* GET_GPIOE */
		p = (u16 *)buf;
		/* Read GPIOE */
		*p = gpio_port_read(GPIOE);
		*data = buf;
		return sizeof(u16);
	case 1:			/* SET_GPIOE */
		/* Write GPIOE */
		gpio_port_write(GPIOE, req->wValue);
		break;
	default:
		return -1;
	}
	return 0;
}

/* --- Class-specific Requests --------------------------------------------- */

bool class_request_error(struct usb_setup_data *req)
{
	switch (req->bRequest) {
	case USB_DFU_REQ_DETACH:
		/*
		 * | bmRequestType | bRequest | wValue   | wIndex    | wLength
		 * | 00100001B     | 00h      | wTimeout | Interface | Zero
		 *
		 */

		/* bmRequestType = 00100001B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
					   USB_RECIP_INTERFACE))
			return true;

		/* Interface number */
		if (req->wIndex != INTERFACE_DFU)
			return true;

		/* Not specified. */
		if (req->wLength)
			return true;

		break;
	case USB_DFU_REQ_GETSTATUS:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 10100001B     | 03h      | Zero   | Interface | Size of
		 * |               |          |        |           | Structure
		 *
		 */

		/* bmRequestType = 10100001B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
					   USB_RECIP_INTERFACE))
			return true;

		/* Interface number */
		if (req->wIndex != INTERFACE_DFU)
			return true;

		/* Not specified. */
		if (req->wValue ||
		    req->wLength != sizeof(struct usb_dfu_status))
			return true;

		break;
	case USB_DFU_REQ_GETSTATE:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 10100001B     | 05h      | Zero   | Interface | 1
		 *
		 */

		/* bmRequestType = 10100001B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
					   USB_RECIP_INTERFACE))
			return true;

		/* Interface number */
		if (req->wIndex != INTERFACE_DFU)
			return true;

		/* Not specified. */
		if (req->wValue || req->wLength != 1)
			return true;

		break;
	default:
		return true;
	}

	return false;
}

int class_request(struct usb_setup_data *req, u8 *buf, u8 **data)
{
	struct usb_dfu_status *p;
	int len;

	switch (req->bRequest) {
	case USB_DFU_REQ_DETACH:
		scb_reset_system(); /* Software reset */
		len = 0;
		break;
	case USB_DFU_REQ_GETSTATUS:
		p = (struct usb_dfu_status *)buf;
		p->bStatus = USB_DFU_STATUS_OK;
		p->bwPollTimeout[0] = 0;
		p->bwPollTimeout[1] = 0;
		p->bwPollTimeout[2] = 0;
		p->bState = USB_DFU_STATE_APP_IDLE;
		p->iString = 0;
		*data = buf;
		len = sizeof(struct usb_dfu_status);
		break;
	case USB_DFU_REQ_GETSTATE:
		*buf = USB_DFU_STATE_APP_IDLE;
		*data = buf;
		len = 1;
		break;
	default:
		return -1;
	}

	return len;
}
