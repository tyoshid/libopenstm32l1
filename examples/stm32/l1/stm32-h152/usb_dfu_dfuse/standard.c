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
#include <usb/langid.h>

#include "usb_dfu_dfuse.h"
#include "standard.h"
#include "descriptor.h"

/* USB Device States */
usb_standard_state_t standard_state = USB_STATE_POWERED;

/* --- USB Device Requests ------------------------------------------------- */

/* Standard Device Requests */

/*
 * Get Descriptor
 *
 * | bmRequestType | bRequest | wValue | wIndex      | wLength
 * | 10000000B     | 6        | Type & | Zero or     | Length
 * |               |          | Index  | Language ID |
 *
 * Defaut state:	This request is valid.
 * Address state:	This request is valid.
 * Configured state:	This request is valid.
 */
static bool get_descriptor_error(struct usb_setup_data *req)
{
	int dt;
	int index;

	/* bmRequestType = 10000000B */
	if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |
				   USB_RECIP_DEVICE))
		return true;

	/* Descriptor Type */
	dt = (req->wValue >> 8);
	/* Descriptor Index */
	index = (req->wValue & 0xff);

	switch (dt) {
	case USB_DT_DEVICE:
		/* Device descriptor */
		if (index || req->wIndex)
			return true;
		break;
	case USB_DT_CONFIGURATION:
		/* Configuration descriptor */
		if (index || req->wIndex)
			return true;
		break;
	case USB_DT_STRING:
		/* String descriptor */
		if ((!index && req->wIndex) ||
		    (index && req->wIndex != LANGID_ENGLISH_US) ||
		    (index > MAX_STRING_INDEX))
			return true;
		break;
	default:
		return true;

	}
	return false;
}

/*
 * Get Configuration
 *
 * | bmRequestType | bRequest | wValue | wIndex | wLength
 * | 10000000B     | 8        | Zero   | Zero   | One
 *
 * Defaut state:	Not specified.
 * Address state:	This request is valid.
 * Configured state:	This request is valid.
 */
static bool get_configuration_error(struct usb_setup_data *req)
{
	/* bmRequestType = 10000000B */
	if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |
				   USB_RECIP_DEVICE))
		return true;

	/* Not specified. */
	if (req->wValue || req->wIndex || req->wLength != 1 ||
	    standard_state == USB_STATE_DEFAULT)
		return true;

	return false;
}

/*
 * Get Interface
 *
 * | bmRequestType | bRequest | wValue | wIndex    | wLength
 * | 10000001B     | 10       | Zero   | Interface | One
 *
 * Defaut state:	Not specified.
 * Address state:	Request Error.
 * Configured state:	This request is valid.
 */
static bool get_interface_error(struct usb_setup_data *req)
{
	/* bmRequestType = 10000001B */
	if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |
				   USB_RECIP_INTERFACE))
		return true;

	/* Request Error */
	if (req->wIndex >= MAXINTERFACE || standard_state == USB_STATE_ADDRESS)
		return true;

	/* Not specified. */
	if (req->wValue || req->wLength != 1 ||
	    standard_state == USB_STATE_DEFAULT)
		return true;

	return false;
}

/*
 * Get Status
 *
 * | bmRequestType | bRequest | wValue | wIndex    | wLength
 * | 10000000B     | 0        | Zero   | Zero      | Two
 * | 10000001B     |          |        | Interface |
 * | 10000010B     |          |        | Endpoint  |
 *
 * Default state:	Not specified.
 * Address state:	This request is valid.
 * Configured state:	This request is valid.
 */
static bool get_status_error(struct usb_setup_data *req)
{
	switch (req->bmRequestType) {
	/* bmRequestType = 10000000B */
	case (USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_DEVICE):
		/* Not specified */
		if (req->wIndex)
			return true;
		break;
	/* bmRequestType = 10000001B */
	case (USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_INTERFACE):
		/* Request Error */
		if (req->wIndex >= MAXINTERFACE ||
		    standard_state != USB_STATE_CONFIGURED)
			return true;
		break;
	/* bmRequestType = 10000010B */
	case (USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_ENDPOINT):
		/* Request Error */
		if (req->wIndex)
			return true;
		break;
	default:
		return true;
	}

	/* Not specified */
	if (req->wValue || req->wLength != 2 ||
	    standard_state == USB_STATE_DEFAULT)
		return true;

	return false;
}

/*
 * Set Address
 *
 * | bmRequestType | bRequest | wValue  | wIndex | wLength
 * | 00000000B     | 5        | Address | Zero   | Zero
 *
 * Defaut state:	This request is valid.
 * Address state:	This request is valid.
 * Configured state:	Not specified.
 */
static bool set_address_error(struct usb_setup_data *req)
{
	/* bmRequestType = 00000000B */
	if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_STANDARD |
				   USB_RECIP_DEVICE))
		return true;

	/* Not specified. */
	if (req->wValue > 127 || req->wIndex || req->wLength ||
	    standard_state == USB_STATE_CONFIGURED)
		return true;

	return false;
}

/*
 * Set Configuration
 *
 * | bmRequestType | bRequest | wValue  | wIndex | wLength
 * | 00000000B     | 9        | Value   | Zero   | Zero
 *
 * Defaut state:	Not specified.
 * Address state:	This request is valid.
 * Configured state:	This request is valid.
 */
static bool set_configuration_error(struct usb_setup_data *req)
{
	/* bmRequestType = 00000000B */
	if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_STANDARD |
				   USB_RECIP_DEVICE))
		return true;

	/* Configuration value */
	if (req->wValue && req->wValue != CONFIGURATION_VALUE)
		return true;

	/* Not specified. */
	if (req->wIndex || req->wLength || standard_state == USB_STATE_DEFAULT)
		return true;

	return false;
}

/*
 * Set Interface
 *
 * | bmRequestType | bRequest | wValue    | wIndex    | wLength
 * | 00000001B     | 11       | Alternate | Interface | Zero
 * |               |          | Setting   |           |
 *
 * Defaut state:	Not specified.
 * Address state:	Request Error.
 * Configured state:	This request is valid.
 */
static bool set_interface_error(struct usb_setup_data *req)
{
	/* bmRequestType = 00000001B */
	if (req->bmRequestType !=
	    (USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_INTERFACE))
		return true;

	/* Request Error */
	if (req->wValue || req->wIndex >= MAXINTERFACE ||
	    standard_state == USB_STATE_ADDRESS)
		return true;

	/* Not specified. */
	if (req->wLength || standard_state == USB_STATE_DEFAULT)
		return true;

	return false;
}

/*
 * Clear Feature
 *
 * | bmRequestType | bRequest | wValue   | wIndex    | wLength
 * | 00000000B     | 1        | Feature  | Zero      | Zero
 * | 00000001B     |          | Selector | Interface |
 * | 00000010B     |          |          | Endpoint  |
 *
 * Default state:	Not specified.
 * Address state:	This request is valid.
 * Configured state:	This request is valid.
 */
static bool clear_feature_error(struct usb_setup_data *req)
{
	switch (req->bmRequestType) {
	/* bmRequestType = 10000000B */
	case (USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_DEVICE):
		/* Not specified */
		if (req->wIndex)
			return true;
		break;
	/* bmRequestType = 10000001B */
	case (USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_INTERFACE):
		/* Request Error */
		if (req->wIndex >= MAXINTERFACE ||
		    standard_state != USB_STATE_CONFIGURED)
			return true;
		break;
	/* bmRequestType = 10000010B */
	case (USB_DIR_IN | USB_TYPE_STANDARD | USB_RECIP_ENDPOINT):
		/* Request Error */
		if (req->wIndex)
			return true;
		break;
	default:
		return true;
	}

	/* Not specified. */
	if (req->wLength || standard_state == USB_STATE_DEFAULT)
		return true;

	return false;
}

bool standard_request_error(struct usb_setup_data *req)
{
	switch (req->bRequest) {
	case USB_REQ_GET_DESCRIPTOR:
		return get_descriptor_error(req);
	case USB_REQ_GET_CONFIGURATION:
		return get_configuration_error(req);
	case USB_REQ_GET_INTERFACE:
		return get_interface_error(req);
	case USB_REQ_GET_STATUS:
		return get_status_error(req);
	case USB_REQ_SET_ADDRESS:
		return set_address_error(req);
	case USB_REQ_SET_CONFIGURATION:
		return set_configuration_error(req);
	case USB_REQ_SET_INTERFACE:
		return set_interface_error(req);
	case USB_REQ_CLEAR_FEATURE:
		return clear_feature_error(req);
	default:
		break;
	}

	return true;
}

/* Get Descriptor */
static int request_get_descriptor(struct usb_setup_data *req, u8 *buf,
				  u8 **data)
{
	int dt;
	int index;
	int len;

	/* Descriptor Type */
	dt = (req->wValue >> 8);

	/* Descriptor Index */
	index = (req->wValue & 0xff);

	/* Get descriptor. */
	len = get_descriptor(dt, index, buf, data);

	/* Set data size. */
	if (len > req->wLength)
		len = req->wLength;

	return len;
}

/* Get Configuration */
static int request_get_configuration(u8 *buf, u8 **data)
{
	if (standard_state == USB_STATE_ADDRESS)
		*buf = 0;
	else
		/* standard_state == USB_STATE_CONFIGURED */
		*buf = CONFIGURATION_VALUE;

	*data = buf;

	return 1;
}

/* Get Interface */
static int request_get_interface(u8 *buf, u8 **data)
{
	*buf = 0;
	*data = buf;
	return 1;
}

/* Get Status */
static int request_get_status(u8 *buf, u8 **data)
{
	u16 *p;

	p = (u16 *)buf;
	*p = 0;
	*data = buf;

	return sizeof(u16);
}

/* Set Address */
static int request_set_address(struct usb_setup_data *req)
{
	if (standard_state == USB_STATE_DEFAULT) {
		/*
		 * If the address specified is non-zero,
		 * then the device shall enter the Address state.
		 */
		if (req->wValue)
			standard_state = USB_STATE_ADDRESS;
	} else if (standard_state == USB_STATE_ADDRESS) {
		/*
		 * If the address specified is zero,
		 * then the device shall enter the Default state.
		 */
		if (!req->wValue)
			standard_state = USB_STATE_DEFAULT;
	}

	/* Set device address. */
	usbdevfs_set_device_address(req->wValue);

	return 0;
}

/* Set Configuration */
static int request_set_configuration(struct usb_setup_data *req)
{
	if (standard_state == USB_STATE_ADDRESS) {
		/*
		 * If the specified configuration value matches
		 * the configuration value from a configuration
		 * descriptor, then that configuratio is selected and
		 * the device enters the Configuration state.
		 */
		if (req->wValue)
			standard_state = USB_STATE_CONFIGURED;
	} else if (standard_state == USB_STATE_CONFIGURED) {
		/*
		 * If the specified configuration value is zero,
		 * then the device enters the Address state.
		 */
		if (!req->wValue)
			standard_state = USB_STATE_ADDRESS;
	}

	return 0;
}

int standard_request(struct usb_setup_data *req, u8 *buf, u8 **data)
{
	switch (req->bRequest) {
	case USB_REQ_GET_DESCRIPTOR:
		return request_get_descriptor(req, buf, data);
	case USB_REQ_GET_CONFIGURATION:
		return request_get_configuration(buf, data);
	case USB_REQ_GET_INTERFACE:
		return request_get_interface(buf, data);
	case USB_REQ_GET_STATUS:
		return request_get_status(buf, data);
	case USB_REQ_SET_ADDRESS:
		return request_set_address(req);
	case USB_REQ_SET_CONFIGURATION:
		return request_set_configuration(req);
	case USB_REQ_SET_INTERFACE:
		return 0;
	case USB_REQ_CLEAR_FEATURE:
		return 0;
	default:
		break;
	}

	return -1;
}

void standard_reset(void)
{
	/* Device state */
	standard_state = USB_STATE_DEFAULT;
}
