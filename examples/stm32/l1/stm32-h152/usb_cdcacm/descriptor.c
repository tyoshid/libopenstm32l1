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
#include <usb/cdc.h>

#include "usb_cdcacm.h"
#include "descriptor.h"

/* struct usb_cdc_union_descriptor_1 */
USB_CDC_UNION_DESCRIPTOR(1);

/* Communications Class Specific Interface Descriptor */
struct cdcacm_functional_descriptors {
	struct usb_cdc_header_descriptor header;
	struct usb_cdc_acm_descriptor acm;
	struct usb_cdc_union_descriptor_1 cdc_union;
} __attribute__ ((packed));

/* Configuration */
struct config_desc {
	struct usb_config_descriptor config;
	struct usb_interface_descriptor comm_if;
	struct cdcacm_functional_descriptors cdcacm_func;
	struct usb_endpoint_descriptor comm_endp;
	struct usb_interface_descriptor data_if;
	struct usb_endpoint_descriptor rx_endp;
	struct usb_endpoint_descriptor tx_endp;
} __attribute__ ((packed));

/* Device Descriptor */
const struct usb_device_descriptor dev_desc __attribute__ ((aligned(2))) = {
	.bLength = sizeof(struct usb_device_descriptor),
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = USB_CLASS_COMM,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = MAXPACKETSIZE0,
	.idVendor = 0x2975,
	.idProduct = 0x0003,
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
		.bNumInterfaces = MAXINTERFACE,
		.bConfigurationValue = CONFIGURATION_VALUE,
		.iConfiguration = 0,
		.bmAttributes = USB_CONFIG_ATTR_D7,
		.bMaxPower = 100 / 2,
	},
	/* Commucations Class Interface Descriptor */
	.comm_if = {
		.bLength = sizeof(struct usb_interface_descriptor),
		.bDescriptorType = USB_DT_INTERFACE,
		.bInterfaceNumber = INTERFACE_COMM,
		.bAlternateSetting = 0,
		.bNumEndpoints = 1,
		.bInterfaceClass = USB_CLASS_COMM,
		.bInterfaceSubClass = USB_COMM_SUBCLASS_ACM,
		.bInterfaceProtocol = USB_COMM_PROTO_AT_V25TER,
		.iInterface = 0,
	},
	/* Communications Class Specific Interface Descriptor */
	.cdcacm_func = {
		/* Header Functional Descriptor */
		.header = {
			.bFunctionLength =
			sizeof(struct usb_cdc_header_descriptor),
			.bDescriptorType = USB_DT_CS_INTERFACE,
			.bDescriptorSubtype = USB_COMM_TYPE_HEADER,
			.bcdCDC = 0x0120,
		},
		/* Abstract Control Management Functional Descriptor */
		.acm = {
			.bFunctionLength =
			sizeof(struct usb_cdc_acm_descriptor),
			.bDescriptorType = USB_DT_CS_INTERFACE,
			.bDescriptorSubtype = USB_COMM_TYPE_ACM,
			.bmCapabilities = (USB_CDC_ACM_CAP_LINE |
					   USB_CDC_ACM_CAP_BREAK),
		},
		/* Union Interface Functional Descriptor */
		.cdc_union = {
			.bFunctionLength =
			sizeof(struct usb_cdc_union_descriptor_1),
			.bDescriptorType = USB_DT_CS_INTERFACE,
			.bDescriptorSubtype = USB_COMM_TYPE_UNION,
			.bControlInterface = INTERFACE_COMM,
			.bSubordinateInterface[0] = INTERFACE_DATA,
		},
	},
	/* Notification endpoint */
	.comm_endp = {
		.bLength = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = NOTIFICATION_ENUM,
		.bmAttributes = USB_ENDPOINT_TRANS_INTERRUPT,
		.wMaxPacketSize = NOTIFICATION_SIZE,
		.bInterval = NOTIFICATION_INTERVAL,
	},
	/* Data Class Interface Descriptor */
	.data_if = {
		.bLength = sizeof(struct usb_interface_descriptor),
		.bDescriptorType = USB_DT_INTERFACE,
		.bInterfaceNumber = INTERFACE_DATA,
		.bAlternateSetting = 0,
		.bNumEndpoints = 2,
		.bInterfaceClass = USB_CLASS_DATA,
		.bInterfaceSubClass = 0,
		.bInterfaceProtocol = 0,
		.iInterface = 0,
	},
	/* Data rx endpoint */
	.rx_endp = {
		.bLength = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = DATA_RX_ENUM,
		.bmAttributes = USB_ENDPOINT_TRANS_BULK,
		.wMaxPacketSize = DATA_SIZE,
		.bInterval = DATA_INTERVAL,
	},
	/* Data tx endpoint */
	.tx_endp = {
		.bLength = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = DATA_TX_ENUM,
		.bmAttributes = USB_ENDPOINT_TRANS_BULK,
		.wMaxPacketSize = DATA_SIZE,
		.bInterval = DATA_INTERVAL,
	}
};

/* Strings */
const u16 langid[] = {LANGID_ENGLISH_US, 0};
const u16 string1[] = L"MPC Research Ltd.";
const u16 string2[] = L"CDC-ACM";
u16 string3[25];
const u16 *string_english_us[MAX_STRING_INDEX] = {
	string1,
	string2,
	string3
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
