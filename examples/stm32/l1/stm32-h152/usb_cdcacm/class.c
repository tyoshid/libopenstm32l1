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
#include <usb/cdc.h>

#include "usb_cdcacm.h"
#include "class.h"
#include "v25ter.h"

/* Encapsulated response */
static u8 response[64] __attribute__ ((aligned(2)));
static int response_len;
static bool response_notify;

/* USART parameters */
static int baudrate;
static int stopbits;
static int parity;
static int databits;

/* Control Line State */
static int control_line_state;

/* --- Class-specific Requests --------------------------------------------- */
bool class_request_error(struct usb_setup_data *req)
{
	switch (req->bRequest) {
	case USB_CDC_REQ_SEND_ENCAPSULATED_COMMAND:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 00100001B     | 00h      | Zero   | Interface | Amount of
		 * |               |          |        |           | Data
		 *
		 */

		/* bmRequestType = 00100001B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
			return true;
		/* Interface number */
		if (req->wIndex != INTERFACE_COMM)
			return true;
		/* Not specified. */
		if (req->wValue || !req->wLength)
			return true;
		break;
	case USB_CDC_REQ_GET_ENCAPSULATED_RESPONSE:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 10100001B     | 01h      | Zero   | Interface | Amount of
		 * |               |          |        |           | Data
		 *
		 */

		/* bmRequestType = 10100001B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
			return true;
		/* Interface number */
		if (req->wIndex != INTERFACE_COMM)
			return true;
		/* Not specified. */
		if (req->wValue || !req->wLength)
			return true;
		break;
	case USB_CDC_REQ_SET_LINE_CODING:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 00100001B     | 20h      | Zero   | Interface | Size of
		 * |               |          |        |           | Structure
		 *
		 */

		/* bmRequestType = 00100001B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
			return true;
		/* Interface number */
		if (req->wIndex != INTERFACE_COMM)
			return true;
		/* Not specified. */
		if (req->wValue ||
		    req->wLength != sizeof(struct usb_cdc_line_coding))
			return true;
		break;
	case USB_CDC_REQ_GET_LINE_CODING:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 10100001B     | 21h      | Zero   | Interface | Size of
		 * |               |          |        |           | Structure
		 *
		 */

		/* bmRequestType = 10100001B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
			return true;
		/* Interface number */
		if (req->wIndex != INTERFACE_COMM)
			return true;
		/* Not specified. */
		if (req->wValue ||
		    req->wLength != sizeof(struct usb_cdc_line_coding))
			return true;
		break;
	case USB_CDC_REQ_SET_CONTROL_LINE_STATE:
		/*
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 00100001B     | 22h      | Bitmap | Interface | Zero
		 *
		 */

		/* bmRequestType = 00100001B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
			return true;
		/* Interface number */
		if (req->wIndex != INTERFACE_COMM)
			return true;
		/* Not specified. */
		if (req->wLength)
			return true;
		break;
	case USB_CDC_REQ_SEND_BREAK:
		/*
		 * | bmRequestType | bRequest | wValue   | wIndex    | wLength
		 * | 00100001B     | 23h      | Duration | Interface | Zero
		 *
		 */

		/* bmRequestType = 00100001B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
			return true;
		/* Interface number */
		if (req->wIndex != INTERFACE_COMM)
			return true;
		/* Not specified. */
		if (req->wLength)
			return true;
		break;
	default:
		return true;
	}
	return false;
}

bool class_request_check_data(struct usb_setup_data *req, u8 *buf)
{
	struct usb_cdc_line_coding *p;

	if (req->bRequest == USB_CDC_REQ_SET_LINE_CODING) {
		p = (struct usb_cdc_line_coding *)buf;
		if (p->bCharFormat > 2)
			return true;
		if (p->bParityType > 4)
			return true;
		if (!(p->bDataBits == 8 ||
		      (p->bDataBits == 7 &&
		       p->bParityType != USB_CDC_LINE_CODING_PARITYTYPE_NONE)))
			return true;
	}
	return false;
}

int class_request(struct usb_setup_data *req, u8 *buf, u8 **data)
{
	int i;
	char *r;
	struct usb_cdc_line_coding *p;
	int d;
	int len;

	switch (req->bRequest) {
	case USB_CDC_REQ_SEND_ENCAPSULATED_COMMAND:
		for (i = 0; i < req->wLength; i++) {
			/* V.25 ter command line input */
			r = v25ter_input_encap(*buf++);

			/* Response */
			if (r)
				break;
		}

		if (r) {
			/* Copy response message. */
			i = 0;
			while (*r)
				response[i++] = *r++;
			response_len = i;

			/* Set flag. */
			response_notify = true;
		}

		len = 0;
		break;
	case USB_CDC_REQ_GET_ENCAPSULATED_RESPONSE:
		/* Send response. */
		*data = response;
		len = response_len;

		if (len > req->wLength)
			len = req->wLength;
		break;
	case USB_CDC_REQ_SET_LINE_CODING:
		/* Save parameters. */
		p = (struct usb_cdc_line_coding *)buf;
		baudrate = p->dwDTERate;
		stopbits = p->bCharFormat;
		parity = p->bParityType;
		databits = p->bDataBits;
		if (databits == 7 ||
		    (databits == 8 &&
		     parity == USB_CDC_LINE_CODING_PARITYTYPE_NONE))
			d = 8;
		else
			d = 9;

		/* Set USART. */
		usart_set_param(baudrate, d, stopbits, parity);

		len = 0;
		break;
	case USB_CDC_REQ_GET_LINE_CODING:
		/* Send parameters. */
		p = (struct usb_cdc_line_coding *)buf;
		p->dwDTERate = baudrate;
		p->bCharFormat = stopbits;
		p->bParityType = parity;
		p->bDataBits = databits;
		*data = buf;
		len = sizeof(struct usb_cdc_line_coding);
		break;
	case USB_CDC_REQ_SET_CONTROL_LINE_STATE:
		/* Save state (nothing to do). */
		control_line_state = req->wValue;

		len = 0;
		break;
	case USB_CDC_REQ_SEND_BREAK:
		if (req->wValue) {
			/* Start break signal. */
			usart_start_break(req->wValue);
		} else {
			/* Stop break signal. */
			usart_stop_break();
		}

		len = 0;
		break;
	default:
		return -1;
	}
	return len;
}

/* --- USB CDC-ACM Functions ----------------------------------------------- */

/* Notification */
static int cdcacm_notify_response_available(u8 *buf)
{
	struct usb_cdc_notification *notif =
		(struct usb_cdc_notification *)buf;

	notif->bmRequestType = (USB_DIR_IN | USB_TYPE_CLASS |
			       USB_RECIP_INTERFACE);
	notif->bNotification = USB_CDC_NOTIFY_RESPONSE_AVAILABLE;
	notif->wValue = 0;
	notif->wIndex = INTERFACE_COMM;
	notif->wLength = 0;

	return sizeof(struct usb_cdc_notification);
}

static int cdcacm_notify_serial_state(u8 *buf, u32 status)
{
	struct usb_cdc_notification *notif =
		(struct usb_cdc_notification *)buf;
	int i;

	notif->bmRequestType = (USB_DIR_IN | USB_TYPE_CLASS |
				USB_RECIP_INTERFACE);
	notif->bNotification = USB_CDC_NOTIFY_SERIAL_STATE;
	notif->wValue = 0;
	notif->wIndex = INTERFACE_COMM;
	notif->wLength = 2;

	i = sizeof(struct usb_cdc_notification);
	buf[i] = 0;
	if (status & 2)		/* USART_FE */
		buf[i] |= USB_CDC_SERIAL_STATE_FRAMING;
	if (status & 1)		/* USART_PE */
		buf[i] |= USB_CDC_SERIAL_STATE_PARITY;
	if (status & 8)		/* USART_ORE */
		buf[i] |= USB_CDC_SERIAL_STATE_OVERRUN;
	i++;
	buf[i++] = 0;

	return i;
}

int class_notify(u8 *buf)
{
	extern volatile u32 serial_status;
	int len;

	if (response_notify) {
		len = cdcacm_notify_response_available(buf);

		response_notify = false;
	} else if (serial_status) {
		len = cdcacm_notify_serial_state(buf, serial_status);

		serial_status = 0;
	} else {
		return -1;
	}

	return len;
}

void class_reset(void)
{
	/* Clear flag. */
	response_notify = false;

	/* Reset V.25 ter command line input. */
	v25ter_reset();
}
