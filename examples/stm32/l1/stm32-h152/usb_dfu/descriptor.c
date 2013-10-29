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
#include <usb/langid.h>
#include <usb/dfu.h>

#include "usb_dfu.h"
#include "descriptor.h"

/* Configuration */
struct config_desc {
	struct usb_config_descriptor config;
	struct usb_interface_descriptor iface;
	struct usb_dfu_descriptor dfu_func;
} __attribute__ ((packed));

/* Device Descriptor */
const struct usb_device_descriptor dev_desc __attribute__ ((aligned(2))) = {
	.bLength = sizeof(struct usb_device_descriptor),
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = 0,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = MAXPACKETSIZE0,
	.idVendor = 0x2975,
	.idProduct = 0x0002,
	.bcdDevice = 0x0010,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 3,
	.bNumConfigurations = 1,
};

const struct config_desc config_desc __attribute__ ((aligned(2))) = {
	/* Configuration Descriptor */
	.config = {
		.bLength = sizeof(struct usb_config_descriptor),
		.bDescriptorType = USB_DT_CONFIGURATION,
		.wTotalLength = sizeof(struct config_desc),
		.bNumInterfaces = 1,
		.bConfigurationValue = CONFIGURATION_VALUE,
		.iConfiguration = 0,
		.bmAttributes = USB_CONFIG_ATTR_D7,
		.bMaxPower = 100 / 2,
	},
	/* Interface Descriptor */
	.iface = {
		.bLength = sizeof(struct usb_interface_descriptor),
		.bDescriptorType = USB_DT_INTERFACE,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 0,
		.bInterfaceClass = USB_CLASS_APPLICATION,
		.bInterfaceSubClass = USB_APP_SUBCLASS_DFU,
		.bInterfaceProtocol = USB_DFU_PROTO_DFU,
		.iInterface = 4,
	},
	/* DFU Functional Descriptor */
	.dfu_func = {
		.bLength = sizeof(struct usb_dfu_descriptor),
		.bDescriptorType = USB_DT_DFU_FUNCTIONAL,
		.bmAttributes = USB_DFU_CAN_DOWNLOAD | USB_DFU_CAN_UPLOAD,
		.wDetachTimeout = 0,
		.wTransferSize = MAXTRANSFERSIZE,
		.bcdDFUVersion = 0x0110,
	}
};

/* Strings */
const u16 langid[] = {LANGID_ENGLISH_US, 0};
const u16 string1[] = L"MPC Research Ltd.";
const u16 string2[] = L"DFU";
u16 string3[25];		/* Serial number (unique device ID) */
const u16 string4[] = L"STM32L1";
const u16 *string_english_us[MAX_STRING_INDEX] = {
	string1,
	string2,
	string3,
	string4
};

void set_serial_number(u32 *uid)
{
	int i;
	int j;
	int d;

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 8; j++) {
			d = (*uid >> (4 * j)) & 0xf;
			if (d < 10)
				string3[23 - (i * 8 + j)] = L'0' + d;
			else
				string3[23 - (i * 8 + j)] = L'A' + (d - 10);
		}
		uid++;
	}
	string3[24] = L'\0';
}

/* Make string descriptor from NULL-terminated string. */
static int make_string_descriptor(int index, u16 *buf)
{
	int i;
	const u16 *p;
	u8 *s;

	if (index == 0)
		p = langid;	/* LANGID */
	else
		p = string_english_us[index - 1]; /* UNICODE encoded string */

	/* Copy data (not include EOS(0)). */
	for (i = 1; *p; i++)
		*(buf + i) = *p++;

	/* Add bLength and bDescriptorType. */
	s = (u8 *)buf;
	*s++ = i * sizeof(u16);
	*s = USB_DT_STRING;

	/* Return descriptor size. */
	return i * (int)sizeof(u16);
}

int get_descriptor(int type, int index, u8 *buf, u8 **data)
{
	int len;

	switch (type) {
	case USB_DT_DEVICE:
		len = sizeof(dev_desc);
		*data = (u8 *)&dev_desc;
		break;
	case USB_DT_CONFIGURATION:
		len = sizeof(config_desc);
		*data = (u8 *)&config_desc;
		break;
	case USB_DT_STRING:
		len = make_string_descriptor(index, (u16 *)buf);
		*data = buf;
		break;
	default:
		len = 0;
		break;
	}

	return len;
}
