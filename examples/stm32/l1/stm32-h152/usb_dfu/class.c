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

#include "usb_dfu.h"
#include "class.h"

static volatile usb_dfu_status_t dfu_status = USB_DFU_STATUS_OK;
static volatile usb_dfu_state_t dfu_state = USB_DFU_STATE_DFU_IDLE;
static volatile int block_num;
static volatile int block_len;
static u32 *block_buf;
static volatile u32 addr_pointer = APP_ADDRESS;

/* --- Class-specific Requests --------------------------------------------- */

/*
 * DFU_DETACH
 *
 * | bmRequestType | bRequest | wValue   | wIndex    | wLength
 * | 00100001B     | 00h      | wTimeout | Interface | Zero
 *
 */
#if 0
static bool dfu_dnload_error(struct usb_setup_data *req)
{
	/* bmRequestType = 00100001B */
	if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Interface number */
	if (req->wIndex)
		return true;

	/* Not specified. */
	if (req->wValue || req->wLength)
		return true;

	return false;
}
#endif
		
/*
 * DFU_DNLOAD
 *
 * | bmRequestType | bRequest | wValue    | wIndex    | wLength
 * | 00100001B     | 01h      | wBlockNum | Interface | Length
 * |               |          |           |           | of data
 *
 */
static bool dfu_dnload_error(struct usb_setup_data *req)
{
	/* bmRequestType = 00100001B */
	if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Interface number */
	if (req->wIndex)
		return true;

	/* Length of data */
	if (req->wLength > MAXTRANSFERSIZE)
		return true;

	/* DFU state */
	if (!((dfu_state == USB_DFU_STATE_DFU_IDLE && req->wLength) ||
	      dfu_state == USB_DFU_STATE_DFU_DNLOAD_IDLE))
			return true;

	return false;
}

/*
 * DFU_UPLOAD
 *
 * | bmRequestType | bRequest | wValue    | wIndex    | wLength
 * | 10100001B     | 02h      | wBlockNum | Interface | Length
 * |               |          |           |           | of data
 *
 */
static bool dfu_upload_error(struct usb_setup_data *req)
{
	/* bmRequestType = 10100001B */
	if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Interface number */
	if (req->wIndex)
		return true;

	/* Length of data */
	if (req->wLength > MAXTRANSFERSIZE)
		return true;

	/* DFU state */
	if (dfu_state != USB_DFU_STATE_DFU_IDLE &&
	    dfu_state != USB_DFU_STATE_DFU_UPLOAD_IDLE)
		return true;

	return false;
}

/*
 * DFU_GETSTATUS
 *
 * | bmRequestType | bRequest | wValue | wIndex    | wLength
 * | 10100001B     | 03h      | Zero   | Interface | Size of
 * |               |          |        |           | Structure
 *
 */
static bool dfu_getstatus_error(struct usb_setup_data *req)
{
	/* bmRequestType = 10100001B */
	if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
			return true;

	/* Interface number */
	if (req->wIndex)
		return true;

	/* Not specified. */
	if (req->wValue ||
	    req->wLength != sizeof(struct usb_dfu_status))
		return true;

	/* DFU state */
	if (dfu_state == USB_DFU_STATE_DFU_DNBUSY ||
	    dfu_state == USB_DFU_STATE_DFU_MANIFEST ||
	    dfu_state == USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET)
		return true;

	return false;
}

/*
 * DFU_CLRSTATUS
 *
 * | bmRequestType | bRequest | wValue | wIndex    | wLength
 * | 00100001B     | 04h      | Zero   | Interface | Zero
 *
 */
static bool dfu_clrstatus_error(struct usb_setup_data *req)
{
	/* bmRequestType = 00100001B */
	if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Interface number */
	if (req->wIndex)
		return true;

	/* Not specified. */
	if (req->wValue || req->wLength)
		return true;

	/* DFU state */
	if (dfu_state != USB_DFU_STATE_DFU_ERROR)
		return true;

	return false;
}

/*
 * DFU_GETSTATE
 *
 * | bmRequestType | bRequest | wValue | wIndex    | wLength
 * | 10100001B     | 05h      | Zero   | Interface | 1
 *
 */
static bool dfu_getstate_error(struct usb_setup_data *req)
{
	/* bmRequestType = 10100001B */
	if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Interface number */
	if (req->wIndex)
		return true;

	/* Not specified. */
	if (req->wValue || req->wLength != 1)
		return true;

	/* DFU state */
	if (dfu_state == USB_DFU_STATE_DFU_DNBUSY ||
	    dfu_state == USB_DFU_STATE_DFU_MANIFEST ||
	    dfu_state == USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET)
		return true;

	return false;
}

/*
 * DFU_ABORT
 *
 * | bmRequestType | bRequest | wValue | wIndex    | wLength
 * | 00100001B     | 06h      | Zero   | Interface | Zero
 *
 */
static bool dfu_abort_error(struct usb_setup_data *req)
{
	/* bmRequestType = 00100001B */
	if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Interface number */
	if (req->wIndex)
		return true;

	/* Not specified. */
	if (req->wValue || req->wLength)
		return true;

	/* DFU state */
	if (!(dfu_state == USB_DFU_STATE_DFU_IDLE ||
	      dfu_state == USB_DFU_STATE_DFU_DNLOAD_SYNC ||
	      dfu_state == USB_DFU_STATE_DFU_DNLOAD_IDLE ||
	      dfu_state == USB_DFU_STATE_DFU_MANIFEST_SYNC ||
	      dfu_state == USB_DFU_STATE_DFU_UPLOAD_IDLE))
		return true;

	return false;
}

bool class_request_error(struct usb_setup_data *req)
{
	bool r = false;

	if (dfu_state == USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET)
		return true;

	switch (req->bRequest) {
	case USB_DFU_REQ_DETACH:
		/* Not supported here. */
		r = true;
		break;
	case USB_DFU_REQ_DNLOAD:
		r = dfu_dnload_error(req);
		break;
	case USB_DFU_REQ_UPLOAD:
		r = dfu_upload_error(req);
		break;
	case USB_DFU_REQ_GETSTATUS:
		r = dfu_getstatus_error(req);
		break;
	case USB_DFU_REQ_CLRSTATUS:
		r = dfu_clrstatus_error(req);
		break;
	case USB_DFU_REQ_GETSTATE:
		r = dfu_getstate_error(req);
		break;
	case USB_DFU_REQ_ABORT:
		r = dfu_abort_error(req);
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

/* DFU_DNLOAD */
static int request_dfu_dnload(struct usb_setup_data *req, u8 *buf)
{
	block_num = req->wValue;
	block_len = req->wLength;
	block_buf = (u32 *)buf;
	if (dfu_state == USB_DFU_STATE_DFU_IDLE) {
		dfu_state = USB_DFU_STATE_DFU_DNLOAD_SYNC;
	} else if (dfu_state == USB_DFU_STATE_DFU_DNLOAD_IDLE) {
		if (req->wLength)
			dfu_state = USB_DFU_STATE_DFU_DNLOAD_SYNC;
		else
			dfu_state = USB_DFU_STATE_DFU_MANIFEST_SYNC;
	}
	return 0;
}

/* DFU_UPLOAD */
static int request_dfu_upload(struct usb_setup_data *req, u8 **data)
{
	int len;

	if (program_addr_error(addr_pointer)) {
		len = 0;
		addr_pointer = APP_ADDRESS;
		dfu_state = USB_DFU_STATE_DFU_IDLE;
	} else {
		*data = (u8 *)addr_pointer;
		len = req->wLength;
		addr_pointer += MAXTRANSFERSIZE;
		dfu_state = USB_DFU_STATE_DFU_UPLOAD_IDLE;
	}

	return len;
}

/* DFU_GETSTATUS */
static int request_dfu_getstatus(u8 *buf, u8 **data)
{
	static bool block_complete;
	struct usb_dfu_status *p;
	int t;

	if (dfu_state == USB_DFU_STATE_DFU_DNLOAD_SYNC) {
		if (!block_complete) {
			block_complete = true;
			dfu_state = USB_DFU_STATE_DFU_DNBUSY;
		} else {
			block_complete = false;
			dfu_state = USB_DFU_STATE_DFU_DNLOAD_IDLE;
		}
	} else if (dfu_state == USB_DFU_STATE_DFU_MANIFEST_SYNC) {
		dfu_state = USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET;
	}

	p = (struct usb_dfu_status *)buf;
	p->bStatus = dfu_status;
	if (dfu_state == USB_DFU_STATE_DFU_DNBUSY) {
		t = program_time(block_len / 4 + 1);
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

	return sizeof(struct usb_dfu_status);
}

/* DFU_CLRSTATUS */
static int request_dfu_clrstatus(void)
{
	dfu_status = USB_DFU_STATUS_OK;
	dfu_state = USB_DFU_STATE_DFU_IDLE;
	addr_pointer = APP_ADDRESS;
	return 0;
}

/* DFU_GETSTATE */
static int request_dfu_getstate(u8 *buf, u8 **data)
{
	*buf = dfu_state;
	*data = buf;
	return 1;
}

/* DFU_ABORT */
static int request_dfu_abort(void)
{
	dfu_state = USB_DFU_STATE_DFU_IDLE;
	addr_pointer = APP_ADDRESS;
	return 0;
}

int class_request(struct usb_setup_data *req, u8 *buf, u8 **data)
{
	switch (req->bRequest) {
	// case USB_DFU_REQ_DETACH:

	case USB_DFU_REQ_DNLOAD:
		return request_dfu_dnload(req, buf);
	case USB_DFU_REQ_UPLOAD:
		return request_dfu_upload(req, data);
	case USB_DFU_REQ_GETSTATUS:
		return request_dfu_getstatus(buf, data);
	case USB_DFU_REQ_CLRSTATUS:
		return request_dfu_clrstatus();
	case USB_DFU_REQ_GETSTATE:
		return request_dfu_getstate(buf, data);
	case USB_DFU_REQ_ABORT:
		return request_dfu_abort();
	default:
		break;
	}

	return -1;
}

void class_reset(void)
{
	dfu_state = USB_DFU_STATE_DFU_IDLE;
	dfu_status = USB_DFU_STATUS_OK;
	addr_pointer = APP_ADDRESS;
}

static bool erase_error(u32 *p, int nword)
{
	int i;

	for (i = 0; i < nword; i++)
		if (*p++)
			return true;
	return false;
}

static bool verify_error(u32 *d, u32 *s, int nword)
{
	int i;

	for (i = 0; i < nword; i++)
		if (*d++ != *s++)
			return true;
	return false;
}

void block_transfer(void)
{
	int len;

	if (dfu_state != USB_DFU_STATE_DFU_DNBUSY)
		return;

	/* Address check */
	if (program_addr_error(addr_pointer)) {
		dfu_status = USB_DFU_STATUS_ERR_ADDRESS;
		dfu_state = USB_DFU_STATE_DFU_ERROR;
		return;
	}

	/* Erase */
	len = block_len / sizeof(u32);
	if (erase_error((u32 *)addr_pointer, len)) {
		program_erase(addr_pointer);
		/* Erase check */
		if (erase_error((u32 *)addr_pointer, len)) {
			dfu_status = USB_DFU_STATUS_ERR_CHECK_ERASED;
			dfu_state = USB_DFU_STATE_DFU_ERROR;
			return;
		}
	}

	/* Program */
	program_memory((u32 *)addr_pointer, block_buf, len);

	/* Verify */
	if (verify_error((u32 *)addr_pointer, block_buf, len)) {
		dfu_status = USB_DFU_STATUS_ERR_VERIFY;
		dfu_state = USB_DFU_STATE_DFU_ERROR;
		return;
	}

	addr_pointer += MAXTRANSFERSIZE;

	dfu_state = USB_DFU_STATE_DFU_DNLOAD_SYNC;
}

bool dfu_wait_reset(void)
{
	return (dfu_state == USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET);
}
