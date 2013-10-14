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

#include "usb_speaker.h"
#include "control.h"
#include "standard.h"
#include "class.h"

/* Control transfer state */
static usbdevfs_control_state_t control_state;

/* Control transfer stages */
typedef enum {
	CONTROL_STAGE_SETUP,
	CONTROL_STAGE_DATA,
	CONTROL_STAGE_STATUS
} control_stage_t;

/* Current device request */
static struct usb_setup_data curreq;

/* IN data */
static u8 inbuf[MAX_DATA_IN] __attribute__ ((aligned(2)));
static u8 *inp;			/* IN data pointer */
static int inlen;		/* IN data length */

/* OUT data */
static u8 outbuf[MAX_DATA_OUT] __attribute__ ((aligned(2)));
static u8 *outp;		/* OUT data pointer */
static int outlen;		/* OUT data length */

/* Flag */
static bool in_zero_length_packet;

/* --- USB Control Transfer ------------------------------------------------ */

static int device_request(struct usb_setup_data *req, control_stage_t stage)
{
	if ((req->bmRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD) {
		/* Standard Device Request */
		switch (stage) {
		case CONTROL_STAGE_SETUP:
			/* Check error. */
			if (standard_request_error(req))
				return -1;

			/* GET request */
			if (req->bmRequestType & USB_DIR_IN) {
				inlen = standard_request(req, inbuf, &inp);
				return inlen;
			}
			break;
		case CONTROL_STAGE_STATUS:
			/* SET request */
			if (!(req->bmRequestType & USB_DIR_IN))
				return standard_request(req, 0, 0);
			break;
		default:
			break;
		}
	} else if ((req->bmRequestType & USB_TYPE_MASK) == USB_TYPE_CLASS &&
		   standard_state == USB_STATE_CONFIGURED) {
		/* Class-specific Request */
		switch (stage) {
		case CONTROL_STAGE_SETUP:
			/* Check error. */
			if (class_request_error(req))
				return -1;

			/* GET request */
			if (req->bmRequestType & USB_DIR_IN) {
				inlen =  class_request(req, inbuf, &inp);
				return inlen;
			}
			break;
		case CONTROL_STAGE_STATUS:
			/* SET request */
			if (!(req->bmRequestType & USB_DIR_IN))
				return class_request(req, outbuf, 0);
			break;
		default:
			break;
		}
	} else {
		return -1;
	}
	return 0;
}

/* OUT transaction */
void control_rx(void)
{
	int len;

	/* Read data packet. */
	len = usbdevfs_read(0, (u16 *)outp, outlen);

	switch (control_state) {
	case USBDEVFS_DATA_OUT:
		/* Illegal packet size */
		if (len != MAXPACKETSIZE0) {
			control_state = USBDEVFS_STALL;
			break;
		}

		/* Set buffer pointer and length. */
		outp += len;
		outlen -= len;

		/* Next State */
		if (outlen <= MAXPACKETSIZE0)
			control_state = USBDEVFS_LAST_DATA_OUT;
		else
			control_state = USBDEVFS_DATA_OUT;
		break;
	case USBDEVFS_LAST_DATA_OUT:
		/* Illegal packet size */
		if (len != outlen) {
			control_state = USBDEVFS_STALL;
			break;
		}

		/* Send zero length packet. */
		usbdevfs_write(0, 0, 0);

		/* Next State */
		control_state = USBDEVFS_STATUS_IN;
		break;
	case USBDEVFS_STATUS_OUT:
		/* Next State */
		control_state = USBDEVFS_STALL;
		break;
	default:
		/* ??? */
		control_state = USBDEVFS_STALL;
		break;
	}
	usbdevfs_set_control_state(0, control_state);
}

/* IN transaction */
void control_tx(void)
{
	int n;

	switch (control_state) {
	case USBDEVFS_DATA_IN:
		/* Send next packet. */
		if (inlen < MAXPACKETSIZE0)
			n = inlen;
		else
			n = MAXPACKETSIZE0;
		usbdevfs_write(0, (u16 *)inp, n);

		/* Set buffer pointer and length. */
		inp += n;
		inlen -= n;

		/* Next State */
		if (n < MAXPACKETSIZE0 ||
		    (inlen == 0 && !in_zero_length_packet))
			control_state = USBDEVFS_LAST_DATA_IN;
		else
			control_state = USBDEVFS_DATA_IN;
		break;
	case USBDEVFS_LAST_DATA_IN:
		/* Next State */
		control_state = USBDEVFS_STATUS_OUT;
		break;
	case USBDEVFS_STATUS_IN:
		/* Execcute SET request. */
		device_request(&curreq, CONTROL_STAGE_STATUS);

		/* Next State */
		control_state = USBDEVFS_STALL;
		break;
	default:
		/* ??? */
		control_state = USBDEVFS_STALL;
		break;
	}
	usbdevfs_set_control_state(0, control_state);
}

/* SETUP transaction */
void control_setup(void)
{
	int len;
	int n;

	/* Read setup packet. */
	len = usbdevfs_read(0, (u16 *)&curreq, sizeof(curreq));

	/* Unknown packet */
	if (len != sizeof(struct usb_setup_data)) {
		control_state = USBDEVFS_STALL;
		usbdevfs_set_control_state(0, control_state);
		return;
	}

	/* Device Request */
	if (device_request(&curreq, CONTROL_STAGE_SETUP) < 0) {
		/* Request Error */
		control_state = USBDEVFS_STALL;
		usbdevfs_set_control_state(0, control_state);
		return;
	}

	if (curreq.bmRequestType & USB_DIR_IN) {
		/* IN transaction */

		/* Zero-length packet */
		in_zero_length_packet = (inlen < curreq.wLength &&
					 inlen % MAXPACKETSIZE0 == 0);

		/* Send data packet. */
		if (inlen < MAXPACKETSIZE0)
			n = inlen;
		else
			n = MAXPACKETSIZE0;
		usbdevfs_write(0, (u16 *)inp, n);

		/* Set data buffer pointer and length. */
		inp += n;
		inlen -= n;

		/* Next State */
		if (n < MAXPACKETSIZE0 ||
		    (inlen == 0 && !in_zero_length_packet))
			control_state = USBDEVFS_LAST_DATA_IN;
		else
			control_state = USBDEVFS_DATA_IN;
		usbdevfs_set_control_state(0, control_state);
	} else {
		/* OUT transaction */

		if (curreq.wLength) {
			/* Prepare buffer */
			outp = outbuf;
			outlen = curreq.wLength;

			/* Data stage */
			if (curreq.wLength < MAXPACKETSIZE0)
				control_state = USBDEVFS_LAST_DATA_OUT;
			else
				control_state = USBDEVFS_DATA_OUT;
			usbdevfs_set_control_state(0, control_state);
		} else {
			/* No data satge */

			/* Send zero length packet. */
			usbdevfs_write(0, 0, 0);

			/* Status Stage */
			control_state = USBDEVFS_STATUS_IN;
			usbdevfs_set_control_state(0, control_state);
		}
	}
}

void control_reset(void)
{
	/* Reset standard settings. */
	standard_reset();

	/* Reset class settings. */
	class_reset();

	/* Set control transfer state. */
	control_state = USBDEVFS_STALL;
	usbdevfs_set_control_state(0, control_state);
}
