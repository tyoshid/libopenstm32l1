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
#include <usb/hid.h>

#include "usb_radio.h"
#include "class.h"

unsigned int idle_duration;

/* --- Audio Class-specific Requests --------------------------------------- */

static bool audio_class_request_error(struct usb_setup_data *req)
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
	if (req->wIndex != ((FEATURE_UNIT_ID << 8) | INTERFACE_AC))
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

static int audio_class_request(struct usb_setup_data *req, u8 *buf, u8 **data)
{
	int cs;
	s16 *p;
	int len = 0;

	cs = (req->wValue >> 8);
	switch (req->bmRequestType) {
	case (USB_DIR_OUT | USB_TYPE_CLASS | USB_RECIP_INTERFACE):
		switch (cs) {
		case USB_AUDIO_MUTE_CONTROL:
			/* Nothing to do */
			break;
		case USB_AUDIO_VOLUME_CONTROL:
			/* Nothing to do */
			break;
		default:
			return -1;
		}
		break;
	case (USB_DIR_IN | USB_TYPE_CLASS | USB_RECIP_INTERFACE):
		switch (cs) {
		case USB_AUDIO_MUTE_CONTROL:
			*buf = 0;
			*data = buf;
			len = 1;
			break;
		case USB_AUDIO_VOLUME_CONTROL:
			p = (s16 *)buf;
			switch (req->bRequest) {
			case USB_AUDIO_GET_CUR:
				*p = 0;
				break;
			case USB_AUDIO_GET_MIN:
				*p = 0;
				break;
			case USB_AUDIO_GET_MAX:
				*p = 0;
				break;
			case USB_AUDIO_GET_RES:
				*p = 0;
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

/* --- HID Class-specific Requests ----------------------------------------- */

/*
 * Get_Report Request
 *
 * | bmRequestType | bRequest   | wValue          | wIndex    | wLength
 * | 10100001B     | GET_REPORT | Report Type and | Interface | Report Length
 * |               |            | Report ID       |           |
 *
 */
static bool get_report_error(struct usb_setup_data *req)
{
	int id;
	// int type;

	/* bmRequestType = 10100001B */
	if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Report ID */
	id = (req->wValue & 0xff);
	if (id < 1 || id > 20 || id == 19)
		return true;

#if 0
	/* Report Type */
	type = (req->wValue >> 8);
	if (id == 18) {
		if (type != USB_HID_REPORT_TYPE_INPUT)
			return true;
	} else {
		if (type != USB_HID_REPORT_TYPE_FEATURE)
			return true;
	}
#endif

	/* Interface Number */
	if (req->wIndex != INTERFACE_HID)
		return true;

	return false;
}

/*
 * Set_Report Request
 *
 * | bmRequestType | bRequest   | wValue          | wIndex    | wLength
 * | 00100001B     | SET_REPORT | Report Type and | Interface | Report Length
 * |               |            | Report ID       |           |
 *
 */
static bool set_report_error(struct usb_setup_data *req)
{
	int id;
	// int type;

	/* bmRequestType = 00100001B */
	if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Report ID */
	id = (req->wValue & 0xff);
	if (id < 1 || id > 19 || id == 18)
		return true;

#if 0
	/* Report Type */
	type = (req->wValue >> 8);
	if (type != USB_HID_REPORT_TYPE_FEATURE)
		return true;
#endif

	/* Interface Number */
	if (req->wIndex != INTERFACE_HID)
		return true;

	/* Report Length */
	if (id == 17) {
		if (req->wLength != 33)
			return true;
	} else {
		if (req->wLength != 3)
			return true;
	}

	return false;
}

static int set_report_check_data(struct usb_setup_data *req, u8 *buf)
{
	int id;

	/* Report ID */
	id = (req->wValue & 0xff);
	if (id != *buf)
		return -1;

	if (id == 19)
		return 0;

	if (devreq == DEVICE_REQUEST_NONE)
		return -WAIT_STATE_DATA_DEVICE;

	return -WAIT_STATE_DATA_BUSY;
}

/*
 * Get_Idle Request
 *
 * | bmRequestType | bRequest | wValue    | wIndex    | wLength
 * | 10100001B     | GET_IDLE | 0 and     | Interface | 1
 * |               |          | Report ID |           |
 *
 */
static bool get_idle_error(struct usb_setup_data *req)
{
	/* bmRequestType = 10100001B */
	if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Report ID */
	if (req->wValue != 18)
		return true;

	/* Interface Number */
	if (req->wIndex != INTERFACE_HID)
		return true;

	/* Length */
	if (req->wLength != 1)
		return true;

	return false;
}

/*
 * Set_Idle Request
 *
 * | bmRequestType | bRequest | wValue       | wIndex    | wLength
 * | 00100001B     | SET_IDLE | Duration and | Interface | 0
 * |               |          | Report ID    |           |
 *
 */
static bool set_idle_error(struct usb_setup_data *req)
{
	int d;

	/* bmRequestType = 00100001B */
	if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_CLASS |
				   USB_RECIP_INTERFACE))
		return true;

	/* Report ID */
	if ((req->wValue & 0xff) != 18)
		return true;

	/* Duration */
	d = req->wValue >> 8;
	if (d < INTERRUPT_INTERVAL / 2)
		return true;

	/* Interface Number */
	if (req->wIndex != INTERFACE_HID)
		return true;

	/* Length */
	if (req->wLength != 0)
		return true;

	return false;
}

static bool hid_class_request_error(struct usb_setup_data *req)
{
	switch (req->bRequest) {
	case USB_HID_GET_REPORT:
		return get_report_error(req);
	case USB_HID_SET_REPORT:
		return set_report_error(req);
	case USB_HID_GET_IDLE:
		return get_idle_error(req);
	case USB_HID_SET_IDLE:
		return set_idle_error(req);
	default:
		break;
	}
	return true;
}

static int hid_class_request_check_data(struct usb_setup_data *req, u8 *buf)
{
	if (req->bRequest == USB_HID_SET_REPORT)
		return set_report_check_data(req, buf);

	return 0;
}

/* Get_Report */
static int request_get_report(struct usb_setup_data *req, u8 *buf, u8 **data,
			      wait_state_t state)
{
	int id;
	int i;
	int len;

	/* Report ID */
	id = req->wValue & 0xff;

	if (state == WAIT_STATE_NONE || state == WAIT_STATE_SETUP_BUSY) {
		if (id != 20) {
			if (devreq == DEVICE_REQUEST_NONE) {
				devreq = DEVICE_REQUEST_READ;
				devreq_id = id;
				return -WAIT_STATE_SETUP_DEVICE;
			}
			return -WAIT_STATE_SETUP_BUSY;
		}
	}

	if (id < 17) {
		*data = buf;
		*buf++ = id;
		*buf++ = si4703_reg[id - 1] >> 8;
		*buf = si4703_reg[id - 1] & 0xff;
		len = 3;
	} else if (id == 17) {
		*data = buf;
		*buf++ = id;
		for (i = 0; i < 16; i++) {
			*buf++ = si4703_reg[i] >> 8;
			*buf++ = si4703_reg[i] & 0xff;
		}
		len = 33;
	} else if (id == 18) {
		*data = buf;
		*buf++ = id;
		for (i = 10; i < 16; i++) {
			*buf++ = si4703_reg[i] >> 8;
			*buf++ = si4703_reg[i] & 0xff;
		}
		len = 13;
	} else {		/* id == 20 */
		*data = buf;
		*buf++ = id;
		*buf++ = 7;	/* Software version */
		*buf++ = 1;	/* Hardware version */
		for (i = 0; i < 61; i++)
			*buf++ = 0;
		len = 64;
	}

	/* Set data size. */
	if (len > req->wLength)
		len = req->wLength;

	return len;
}

/* Set_Report */
static int request_set_report(struct usb_setup_data *req, u8 *buf)
{
	int id;
	int i;

	/* Report ID */
	id = (req->wValue & 0xff);
	buf++;

	devreq = DEVICE_REQUEST_WRITE;
	devreq_id = id;
	if (id < 17) {
		devreq_reg[id - 1] = *buf++ << 8;
		devreq_reg[id - 1] |= *buf;
	} else if (id == 17) {
		for (i = 0; i < 16; i++) {
			devreq_reg[i] = *buf++ << 8;
			devreq_reg[i] |= *buf++;
		}
	}

	return 0;
}

/* Get_Idle */
static int request_get_idle(struct usb_setup_data *req, u8 *buf, u8 **data)
{
	(void)req;
	*data = buf;
	*buf = idle_duration;
	return 1;
}

/* Set_Idle */
static int request_set_idle(struct usb_setup_data *req)
{
	idle_duration = req->wValue >> 8;
	return 0;
}

static int hid_class_request(struct usb_setup_data *req, u8 *buf, u8 **data,
			     wait_state_t state)
{
	switch (req->bRequest) {
	case USB_HID_GET_REPORT:
		return request_get_report(req, buf, data, state);
	case USB_HID_SET_REPORT:
		return request_set_report(req, buf);
	case USB_HID_GET_IDLE:
		return request_get_idle(req, buf, data);
	case USB_HID_SET_IDLE:
		return request_set_idle(req);
	default:
		break;
	}
	return -1;
}

static void hid_class_reset(void)
{
	idle_duration = INTERRUPT_INTERVAL / 2;
}

/* ------------------------------------------------------------------------- */

bool class_request_error(struct usb_setup_data *req)
{
	if ((req->wIndex & 0xff) == INTERFACE_AC)
		return audio_class_request_error(req);
	else if ((req->wIndex & 0xff) == INTERFACE_HID)
		return hid_class_request_error(req);
	else
		return true;
}

int class_request_check_data(struct usb_setup_data *req, u8 *buf)
{
	if ((req->wIndex & 0xff) == INTERFACE_HID)
		return hid_class_request_check_data(req, buf);

	return 0;
}

int class_request(struct usb_setup_data *req, u8 *buf, u8 **data,
		  wait_state_t state)
{
	if ((req->wIndex & 0xff) == INTERFACE_AC)
		return audio_class_request(req, buf, data);
	else if ((req->wIndex & 0xff) == INTERFACE_HID)
		return hid_class_request(req, buf, data, state);
	else
		return -1;
}

void class_reset(void)
{
	hid_class_reset();
}
