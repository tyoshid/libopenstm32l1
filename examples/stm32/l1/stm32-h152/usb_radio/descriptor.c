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
#include <usb/audio10.h>
#include <usb/hid.h>
#include <usb/hut.h>

#include "usb_radio.h"
#include "descriptor.h"

 /* struct usb_audio_ac_if_header_descriptor_1 */
USB_AUDIO_AC_IF_HEADER_DESCRIPTOR(1);
/* struct usb_audio_feature_unit_descriptor_2_2 */
USB_AUDIO_FEATURE_UNIT_DESCRIPTOR(2, 2);
/* struct usb_audio_type_i_format_type_descriptor_1 */
USB_AUDIO_TYPE_I_FORMAT_TYPE_DESCRIPTOR(1);
/* struct usb_hid_descriptor_1 */
USB_HID_DESCRIPTOR(1);

/* Class-Specific AC Interface Descriptor */
struct ac_interface_descriptor {
	struct usb_audio_ac_if_header_descriptor_1 header;
	struct usb_audio_input_terminal_descriptor it;
	struct usb_audio_feature_unit_descriptor_2_2 fu;
	struct usb_audio_output_terminal_descriptor ot;
} __attribute__ ((packed));

/* Configuration */
struct config_desc {
	struct usb_config_descriptor config;
	struct usb_interface_descriptor ac_if;
	struct ac_interface_descriptor cs_ac_if;
	struct usb_interface_descriptor as_if0;
	struct usb_interface_descriptor as_if1;
	struct usb_audio_as_interface_descriptor cs_as_if;
	struct usb_audio_type_i_format_type_descriptor_1 format;
	struct usb_audio_endpoint_descriptor as_endp;
	struct usb_audio_as_endpoint_descriptor cs_as_endp;
	struct usb_interface_descriptor hid_if;
	struct usb_hid_descriptor_1 hid;
	struct usb_endpoint_descriptor hid_endp;
} __attribute__ ((packed));

/* HID Report Descriptor */
const u8 report[] __attribute__ ((aligned(2))) = {
	/* Usage Page (Vendor Defined (0)) */
	USB_HID_USAGE_PAGE | USB_HID_SIZE_2,
	0, USB_HID_VENDOR_PAGE >> 8,
	/* Usage (Vendor Usage 1) */
	USB_HID_USAGE | USB_HID_SIZE_1,
	1,
	/* Collenction (Application) */
	USB_HID_COLLECTION | USB_HID_SIZE_1,
	USB_HID_APPLICATION,
	/* Logical Maximum (0xff) */
	USB_HID_LOGICAL_MAX | USB_HID_SIZE_1,
	0xff,
	/* Logical Minimum (0) */
	USB_HID_LOGICAL_MIN | USB_HID_SIZE_1,
	0,
	/* Report Size (8) */
	USB_HID_REPORT_SIZE | USB_HID_SIZE_1,
	8,
	/* Report Count (2) */
	USB_HID_REPORT_COUNT | USB_HID_SIZE_1,
	2,

	/*
	 * Report 1-16 give direct read/write access to th 16 Si4703 registers
	 * with the (Report ID - 1) corresponding to the register address
	 * across endpoint 0 using GET_REPORT and SET_REPORT
	 */
	/* Report ID (1) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	1,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (2) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	2,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (3) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	3,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (4) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	4,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (5) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	5,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (6) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	6,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (7) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	7,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (8) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	8,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (9) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	9,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (10) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	10,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (11) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	11,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (12) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	12,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (13) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	13,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (14) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	14,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (15) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	15,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report ID (16) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	16,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/*
	 * Report 17 gives direct read/write access to the entire Si4703
	 * register map across endpoint 0 using GET_REPORT and SET_REPORT
	 */
	/* Report ID (17) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	17,
	/* Report Count (32) */
	USB_HID_REPORT_COUNT | USB_HID_SIZE_1,
	32,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/*
	 * Report 18 is used to send the lowest 6 Si4703 register up the HID
	 * interrupt endpoint every 20 milliseconds for status
	 */
	/* Report ID (18) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	18,
	/* Report Count (12) */
	USB_HID_REPORT_COUNT | USB_HID_SIZE_1,
	12,
	/* Input (Data, Variable, Absolute) */
	USB_HID_INPUT | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report 19: LED state */
	/* Report ID (19) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	19,
	/* Report Count (3) */
	USB_HID_REPORT_COUNT | USB_HID_SIZE_1,
	3,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* Report 20: scratch */
	/* Report ID (20) */
	USB_HID_REPORT_ID | USB_HID_SIZE_1,
	20,
	/* Report Count (63) */
	USB_HID_REPORT_COUNT | USB_HID_SIZE_1,
	63,
	/* Feature (Data, Variable, Absolute) */
	USB_HID_FEATURE | USB_HID_SIZE_1,
	USB_HID_DATA | USB_HID_VARIABLE | USB_HID_ABSOLUTE,

	/* End Collection */
	USB_HID_END_COLLECTION
};

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
	.idProduct = 0x0005,
	.bcdDevice = 0x0010,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 0,
	.bNumConfigurations = 1,
};

const struct config_desc config_desc __attribute__ ((aligned(2))) = {
	/* Configuration Descriptor */
	.config = {
		.bLength = sizeof(struct usb_config_descriptor),
		.bDescriptorType = USB_DT_CONFIGURATION,
		.wTotalLength = sizeof(struct config_desc),
		.bNumInterfaces = 3,
		.bConfigurationValue = CONFIGURATION_VALUE,
		.iConfiguration = 0,
		.bmAttributes = USB_CONFIG_ATTR_D7,
		.bMaxPower = 100 / 2,
	},
	/* Standard AC Interface Descriptor */
	.ac_if = {
		.bLength = sizeof(struct usb_interface_descriptor),
		.bDescriptorType = USB_DT_INTERFACE,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 0,
		.bInterfaceClass = USB_CLASS_AUDIO,
		.bInterfaceSubClass = USB_AUDIO_SUBCLASS_AUDIOCONTROL,
		.bInterfaceProtocol = USB_AUDIO_PROTO_UNDEFINED,
		.iInterface = 0,
	},
	/* Class-Specific AC Interface Descriptor */
	.cs_ac_if = {
		/* Class-Specific AC Interfcae Header Descriptor */
		.header = {
			.bLength =
			sizeof(struct usb_audio_ac_if_header_descriptor_1),
			.bDescriptorType = USB_DT_CS_INTERFACE,
			.bDescriptorSubtype = USB_AUDIO_HEADER,
			.bcdADC = 0x0100,
			.wTotalLength =
			sizeof(struct ac_interface_descriptor),
			.bInCollection = 1,
			.baInterfaceNr[0] = 1,
		},
		/* Input Terminal Descriptor */
		.it = {
			.bLength =
			sizeof(struct usb_audio_input_terminal_descriptor),
			.bDescriptorType = USB_DT_CS_INTERFACE,
			.bDescriptorSubtype = USB_AUDIO_INPUT_TERMINAL,
			.bTerminalID = INPUT_TERMINAL_ID,
			.wTerminalType = USB_AUDIO_RADIO_RECEIVER,
			.bAssocTerminal = 0,
			.bNrChannels = 2,
			.wChannelConfig = (USB_AUDIO_LEFT_FRONT |
					   USB_AUDIO_RIGHT_FRONT),
			.iChannelNames = 0,
			.iTerminal = 0,
		},
		/* Feature Unit Descriptor */
		.fu = {
			.bLength =
			sizeof(struct usb_audio_feature_unit_descriptor_2_2),
			.bDescriptorType = USB_DT_CS_INTERFACE,
			.bDescriptorSubtype = USB_AUDIO_FEATURE_UNIT,
			.bUnitID = FEATURE_UNIT_ID,
			.bSourceID = INPUT_TERMINAL_ID,
			.bControlSize = 2,
			.bmaControls[0] = {USB_AUDIO_MUTE_CONTROL,},
			.bmaControls[1] = {USB_AUDIO_VOLUME_CONTROL,},
			.bmaControls[2] = {USB_AUDIO_VOLUME_CONTROL,},
			.iFeature = 0,
		},
		/* Output Terminal Descriptor */
		.ot = {
			.bLength =
			sizeof(struct usb_audio_output_terminal_descriptor),
			.bDescriptorType = USB_DT_CS_INTERFACE,
			.bDescriptorSubtype = USB_AUDIO_OUTPUT_TERMINAL,
			.bTerminalID = OUTPUT_TERMINAL_ID,
			.wTerminalType = USB_AUDIO_USB_STREAMING,
			.bAssocTerminal = 0,
			.bSourceID = FEATURE_UNIT_ID,
			.iTerminal = 0,
		},
	},
	/* Standard AS Interface Descriptor */
	.as_if0 = {
		.bLength = sizeof(struct usb_interface_descriptor),
		.bDescriptorType = USB_DT_INTERFACE,
		.bInterfaceNumber = 1,
		.bAlternateSetting = 0,
		.bNumEndpoints = 0,
		.bInterfaceClass = USB_CLASS_AUDIO,
		.bInterfaceSubClass = USB_AUDIO_SUBCLASS_AUDIOSTREAMING,
		.bInterfaceProtocol = USB_AUDIO_PROTO_UNDEFINED,
		.iInterface = 0,
	},
	.as_if1 = {
		.bLength = sizeof(struct usb_interface_descriptor),
		.bDescriptorType = USB_DT_INTERFACE,
		.bInterfaceNumber = 1,
		.bAlternateSetting = 1,
		.bNumEndpoints = 1,
		.bInterfaceClass = USB_CLASS_AUDIO,
		.bInterfaceSubClass = USB_AUDIO_SUBCLASS_AUDIOSTREAMING,
		.bInterfaceProtocol = USB_AUDIO_PROTO_UNDEFINED,
		.iInterface = 0,
	},
	/* Class-Specific AS Interface Descriptor */
	.cs_as_if = {
		.bLength = sizeof(struct usb_audio_as_interface_descriptor),
		.bDescriptorType = USB_DT_CS_INTERFACE,
		.bDescriptorSubtype = USB_AUDIO_AS_GENERAL,
		.bTerminalLink = OUTPUT_TERMINAL_ID,
		.bDelay = 0,
		.wFormatTag = USB_AUDIO_PCM,
	},
	/* Class-Specific AS Format Type Descriptor */
	.format = {
		.bLength =
		sizeof(struct usb_audio_type_i_format_type_descriptor_1),
		.bDescriptorType = USB_DT_CS_INTERFACE,
		.bDescriptorSubtype = USB_AUDIO_FORMAT_TYPE,
		.bFormatType = USB_AUDIO_FORMAT_TYPE_I,
		.bNrChannels = 2,
		.bSubFrameSize = 2,
		.bBitResolution = SAMPLING_BITS,
		.bSamFreqType = 1,
		.tSamFreq[0] = {(SAMPLING_FREQ & 0xff),
				((SAMPLING_FREQ >> 8) & 0xff),
				((SAMPLING_FREQ >> 16) & 0xff)},
	},
	/* Standard AS Isochronous Audio Data Endpoint Descriptor */
	.as_endp = {
		.bLength = sizeof(struct usb_audio_endpoint_descriptor),
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = AS_ENUM,
		.bmAttributes = (USB_ENDPOINT_SYNC_SYNC |
				 USB_ENDPOINT_TRANS_ISOCHRONOUS),
		.wMaxPacketSize = AS_SIZE,
		.bInterval = AS_INTERVAL,
		.bRefresh = 0,
		.bSynchAddress = 0,
	},
	/* Class-Specific AS Isochronous Audio Data Endpoint Descriptor */
	.cs_as_endp = {
		.bLength = sizeof(struct usb_audio_as_endpoint_descriptor),
		.bDescriptorType = USB_DT_CS_ENDPOINT,
		.bDescriptorSubtype = USB_AUDIO_EP_GENERAL,
		.bmAttributes = 0,
		.bLockDelayUnits = 0,
		.wLockDelay = 0,
	},
	/* Standard Interface Descriptor */
	.hid_if = {
		.bLength = sizeof(struct usb_interface_descriptor),
		.bDescriptorType = USB_DT_INTERFACE,
		.bInterfaceNumber = INTERFACE_HID,
		.bAlternateSetting = 0,
		.bNumEndpoints = 1,
		.bInterfaceClass = USB_CLASS_HID,
		.bInterfaceSubClass = USB_HID_SUBCLASS_UNDEFINED,
		.bInterfaceProtocol = USB_HID_PROTO_UNDEFINED,
		.iInterface = 0,
	},
	/* HID Descriptor */
	.hid = {
		.bLength = sizeof(struct usb_hid_descriptor_1),
		.bDescriptorType = USB_DT_HID,
		.bcdHID = 0x0111,
		.bCountryCode = 0,
		.bNumDescriptors = 1,
		.descriptor[0] = {USB_DT_REPORT, sizeof(report)},
	},
	/* Standard Endpoint Descriptor */
	.hid_endp = {
		.bLength = sizeof(struct usb_endpoint_descriptor),
		.bDescriptorType = USB_DT_ENDPOINT,
		.bEndpointAddress = INTERRUPT_ENUM,
		.bmAttributes = USB_ENDPOINT_TRANS_INTERRUPT,
		.wMaxPacketSize = INTERRUPT_SIZE,
		.bInterval = INTERRUPT_INTERVAL,
	},
};

/* Strings */
const u16 langid[] = {LANGID_ENGLISH_US, 0};
const u16 string1[] = L"MPC Research Ltd.";
const u16 string2[] = L"FM Radio";
const u16 *string_english_us[MAX_STRING_INDEX] = {
	string1,
	string2
};

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
	u8 *s;
	u8 *d;
	int i;

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
	case USB_DT_HID:
		len = sizeof(struct usb_hid_descriptor_1);
		s = (u8 *)&config_desc.hid;
		d = buf;
		for (i = 0; i < len; i++)
			*d++ = *s++;
		*data = buf;
		break;
	case USB_DT_REPORT:
		len = sizeof(report);
		*data = (u8 *)report;
		break;
	default:
		len = 0;
		break;
	}

	return len;
}
