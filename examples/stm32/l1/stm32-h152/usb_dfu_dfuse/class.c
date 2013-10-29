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

#include <usb/standard.h>
#include <usb/dfu.h>

#include "usb_dfu_dfuse.h"
#include "class.h"

#define CMD_SETADDR	0x21
#define CMD_ERASE	0x41

static volatile usb_dfu_status_t dfu_status = USB_DFU_STATUS_OK;
static volatile usb_dfu_state_t dfu_state = USB_DFU_STATE_DFU_IDLE;
static volatile int block_num;
static volatile int block_len;
static u8 *block_buf;
static volatile u32 addr_pointer = 0x08000000;

/* --- Class-specific Requests --------------------------------------------- */
bool class_request_error(struct usb_setup_data *req)
{
	bool r = false;

	switch (req->bRequest) {
	case USB_DFU_REQ_DETACH:
		/*
		 * | bmRequestType | bRequest | wValue   | wIndex    | wLength
		 * | 00100001B     | 00h      | wTimeout | Interface | Zero
		 *
		 */

		/* Not supported here. */
		r = true;
		break;
	case USB_DFU_REQ_DNLOAD:
		/*
		 * | bmRequestType | bRequest | wValue    | wIndex    | wLength
		 * | 00100001B     | 01h      | wBlockNum | Interface | Length
		 * |               |          |           |           | of data
		 *
		 */

		/* bmRequestType = 00100001B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
					   USB_RECIP_INTERFACE)) {
			r = true;
			break;
		}
		/* wBlockNum > 1 */
		if (req->wValue == 1) {
			r = true;
			break;
		}
		/* Interface number */
		if (req->wIndex) {
			r = true;
			break;
		}
		/* Length of data */
		if (req->wLength > MAXTRANSFERSIZE) {
			r = true;
			break;
		}
		/* DFU state */
		if (dfu_state != USB_DFU_STATE_DFU_IDLE &&
		    dfu_state != USB_DFU_STATE_DFU_DNLOAD_IDLE) {
			r = true;
			break;
		}
		break;
	case USB_DFU_REQ_UPLOAD:
		/*
		 * | bmRequestType | bRequest | wValue    | wIndex    | wLength
		 * | 10100001B     | 02h      | wBlockNum | Interface | Length
		 * |               |          |           |           | of data
		 *
		 */

		/* bmRequestType = 10100001B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
					   USB_RECIP_INTERFACE)) {
			r = true;
			break;
		}
		/* wBlockNum > 1 */
		if (req->wValue == 1) {
			r = true;
			break;
		}
		/* Interface number */
		if (req->wIndex) {
			r = true;
			break;
		}
		/* Length of data */
		if (req->wLength > MAXTRANSFERSIZE) {
			r = true;
			break;
		}
		/* DFU state */
		if (dfu_state != USB_DFU_STATE_DFU_IDLE &&
		    dfu_state != USB_DFU_STATE_DFU_UPLOAD_IDLE) {
			r = true;
			break;
		}
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
					   USB_RECIP_INTERFACE)) {
			r = true;
			break;
		}
		/* Interface number */
		if (req->wIndex) {
			r = true;
			break;
		}
		/* Not specified. */
		if (req->wValue ||
		    req->wLength != sizeof(struct usb_dfu_status)) {
			r = true;
			break;
		}
		/* DFU state */
		if (dfu_state == USB_DFU_STATE_DFU_DNBUSY ||
		    dfu_state == USB_DFU_STATE_DFU_MANIFEST ||
		    dfu_state == USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET) {
			r = true;
			break;
		}
		break;
	case USB_DFU_REQ_CLRSTATUS:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 00100001B     | 04h      | Zero   | Interface | Zero
		 *
		 */

		/* bmRequestType = 00100001B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
					   USB_RECIP_INTERFACE)) {
			r = true;
			break;
		}
		/* Interface number */
		if (req->wIndex) {
			r = true;
			break;
		}
		/* Not specified. */
		if (req->wValue || req->wLength) {
			r = true;
			break;
		}
		/* DFU state */
		if (dfu_state != USB_DFU_STATE_DFU_ERROR) {
			r = true;
			break;
		}
		break;
	case USB_DFU_REQ_GETSTATE:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 10100001B     | 05h      | Zero   | Interface | 1
		 *
		 */

		/* bmRequestType = 10100001B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
					   USB_RECIP_INTERFACE)) {
			r = true;
			break;
		}
		/* Interface number */
		if (req->wIndex) {
			r = true;
			break;
		}
		/* Not specified. */
		if (req->wValue || req->wLength != 1) {
			r = true;
		}
		/* DFU state */
		if (dfu_state == USB_DFU_STATE_DFU_DNBUSY ||
		    dfu_state == USB_DFU_STATE_DFU_MANIFEST ||
		    dfu_state == USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET) {
			r = true;
			break;
		}
		break;
	case USB_DFU_REQ_ABORT:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 00100001B     | 06h      | Zero   | Interface | Zero
		 *
		 */

		/* bmRequestType = 00100001B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
					   USB_RECIP_INTERFACE)) {
			r = true;
			break;
		}
		/* Interface number */
		if (req->wIndex) {
			r = true;
			break;
		}
		/* Not specified. */
		if (req->wValue || req->wLength) {
			r = true;
			break;
		}
		/* DFU state */
		if (!(dfu_state == USB_DFU_STATE_DFU_IDLE ||
		      dfu_state == USB_DFU_STATE_DFU_DNLOAD_SYNC ||
		      dfu_state == USB_DFU_STATE_DFU_DNLOAD_IDLE ||
		      dfu_state == USB_DFU_STATE_DFU_MANIFEST_SYNC ||
		      dfu_state == USB_DFU_STATE_DFU_UPLOAD_IDLE)) {
			r = true;
			break;
		}
		break;
	default:
		r = true;
		break;
	}
	if (r) {
		dfu_status = USB_DFU_STATUS_ERR_STALLEDPKT;
		dfu_state = USB_DFU_STATE_DFU_ERROR;
	}
	return r;
}

int class_request_check_data(struct usb_setup_data *req, u8 *buf)
{
	u8 cmd;

	if (req->bRequest != USB_DFU_REQ_DNLOAD)
		return -1;

	if (req->wValue == 0 && req->wLength) {
		cmd = buf[0];
		if (cmd != CMD_SETADDR && cmd != CMD_ERASE) {
			dfu_status = USB_DFU_STATUS_ERR_STALLEDPKT;
			dfu_state = USB_DFU_STATE_DFU_ERROR;
			return -1;
		}
		if (req->wLength != 5) {
			dfu_status = USB_DFU_STATUS_ERR_STALLEDPKT;
			dfu_state = USB_DFU_STATE_DFU_ERROR;
			return -1;
		}
	}
	return 0;
}

int class_request(struct usb_setup_data *req, u8 *buf, u8 **data)
{
	int len = 0;
	struct usb_dfu_status *p;
	int t;
	//static bool block_complete;

	switch (req->bRequest) {
	// case USB_DFU_REQ_DETACH:

	case USB_DFU_REQ_DNLOAD:
		block_num = req->wValue;
		block_len = req->wLength;
		block_buf = buf;
		if (dfu_state == USB_DFU_STATE_DFU_IDLE) {
			if (req->wLength)
				dfu_state = USB_DFU_STATE_DFU_DNLOAD_SYNC;
			else
				dfu_state = USB_DFU_STATE_DFU_MANIFEST_SYNC;
		} else if (dfu_state == USB_DFU_STATE_DFU_DNLOAD_IDLE) {
			if (req->wLength)
				dfu_state = USB_DFU_STATE_DFU_DNLOAD_SYNC;
			else
				dfu_state = USB_DFU_STATE_DFU_MANIFEST_SYNC;
		}
		break;
	case USB_DFU_REQ_UPLOAD:
		if (req->wValue == 0) {
			buf[0] = 0;
			buf[1] = CMD_SETADDR;
			buf[2] = CMD_ERASE;
			*data = buf;
			len = 3;
			if (len > req->wLength)
				len = req->wLength;
		} else {
			*data = (u8 *)(addr_pointer + (req->wValue - 2) *
				       MAXTRANSFERSIZE);
			len = req->wLength;
		}
		if (req->wLength < MAXTRANSFERSIZE)
			dfu_state = USB_DFU_STATE_DFU_IDLE;
		else
			dfu_state = USB_DFU_STATE_DFU_UPLOAD_IDLE;
		break;
	case USB_DFU_REQ_GETSTATUS:
		if (dfu_state == USB_DFU_STATE_DFU_DNLOAD_SYNC)
			dfu_state = USB_DFU_STATE_DFU_DNBUSY;
		else if (dfu_state == USB_DFU_STATE_DFU_MANIFEST_SYNC)
			dfu_state = USB_DFU_STATE_DFU_MANIFEST;

		p = (struct usb_dfu_status *)buf;
		p->bStatus = dfu_status;
		if (dfu_state == USB_DFU_STATE_DFU_DNBUSY) {
			if (block_num == 0) {
				if (block_buf[0] == CMD_ERASE)
					t = program_time(1);
				else
					t = 0;
			} else {
				t = program_time(block_len / 4);
			}
			p->bwPollTimeout[0] = (t & 0xff);
			p->bwPollTimeout[1] = ((t >> 8) & 0xff);
			p->bwPollTimeout[2] = ((t >> 16) & 0xff);
		} else {
			p->bwPollTimeout[0] = 0;
			p->bwPollTimeout[1] = 0;
			p->bwPollTimeout[2] = 0;
		}
		p->bState = dfu_state;
		p->iString = 0;
		*data = buf;
		len = sizeof(struct usb_dfu_status);
		break;
	case USB_DFU_REQ_CLRSTATUS:
		dfu_status = USB_DFU_STATUS_OK;
		dfu_state = USB_DFU_STATE_DFU_IDLE;
		break;
	case USB_DFU_REQ_GETSTATE:
		buf[0] = dfu_state;
		*data = buf;
		len = 1;
		break;
	case USB_DFU_REQ_ABORT:
		dfu_state = USB_DFU_STATE_DFU_IDLE;
		break;
	default:
		return -1;
	}
	return len;
}

void class_reset(void)
{
	dfu_status = USB_DFU_STATUS_OK;
	dfu_state = USB_DFU_STATE_DFU_IDLE;
	addr_pointer = 0x08000000;
}

static bool erase_error(volatile u32 *p, int nword)
{
	int i;

	for (i = 0; i < nword; i++)
		if (*p++)
			return true;
	return false;
}

static bool verify_error(volatile u32 *d, u32 *s, int nword)
{
	int i;

	for (i = 0; i < nword; i++)
		if (*d++ != *s++)
			return true;
	return false;
}

int block_transfer(void)
{
	int cmd;
	u32 a;
	int len;

	if (dfu_state != USB_DFU_STATE_DFU_DNBUSY)
		return dfu_state;

	if (block_num == 0) {
		cmd = block_buf[0];
		a = block_buf[1] | (block_buf[2] << 8) |
			(block_buf[3] << 16) | (block_buf[4] << 24);
		if (program_addr_error(a)) {
			dfu_status = USB_DFU_STATUS_ERR_TARGET;
			dfu_state = USB_DFU_STATE_DFU_ERROR;
			return dfu_state;
		}
		if (cmd == CMD_SETADDR)
			addr_pointer = a;
		else if (cmd == CMD_ERASE)
			program_erase(a);
	} else {
		a = addr_pointer + (block_num - 2) * MAXTRANSFERSIZE;
		if (program_addr_error(a) ||
		    program_addr_error(a + block_len - 1)) {
			dfu_status = USB_DFU_STATUS_ERR_TARGET;
			dfu_state = USB_DFU_STATE_DFU_ERROR;
			return dfu_state;
		}

		/* Erase check */
		len = block_len / sizeof(u32);
		if (erase_error((u32 *)a, len)) {
			dfu_status = USB_DFU_STATUS_ERR_CHECK_ERASED;
			dfu_state = USB_DFU_STATE_DFU_ERROR;
			return dfu_state;
		}

		/* Program */
		program_memory((u32 *)a, (u32 *)block_buf, len);

		/* Verify */
		if (verify_error((u32 *)a, (u32 *)block_buf, len)) {
			dfu_status = USB_DFU_STATUS_ERR_VERIFY;
			dfu_state = USB_DFU_STATE_DFU_ERROR;
			return dfu_state;
		}
	}

	dfu_state = USB_DFU_STATE_DFU_DNLOAD_IDLE;
	return dfu_state;
}
