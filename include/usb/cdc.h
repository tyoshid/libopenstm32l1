/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Gareth McMullin <gareth@blacksphere.co.nz>
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

#include <libopencm3.h>

/* Definitions of Communications Device Class from
 * "Universal Serial Bus Class Definitions for Communications Devices
 * Revision 1.2"
 */

/* Table 2: Communications Device Class Code */
/* Table 3: Communications Interface Class Code */
#define USB_CLASS_COMM			0x02

/* Table 4: Class Subclass Code */
#define USB_COMM_SUBCLASS_DLCM		0x01
#define USB_COMM_SUBCLASS_ACM		0x02
#define USB_COMM_SUBCLASS_TCM		0x03
#define USB_COMM_SUBCLASS_MCCM		0x04
#define USB_COMM_SUBCLASS_CAPICM	0x05
#define USB_COMM_SUBCLASS_ECM		0x06
#define USB_COMM_SUBCLASS_ATM		0x07
#define USB_COMM_SUBCLASS_WHCM		0x08
#define USB_COMM_SUBCLASS_DMM		0x09
#define USB_COMM_SUBCLASS_MDLM		0x0a
#define USB_COMM_SUBCLASS_OBEX		0x0b
#define USB_COMM_SUBCLASS_EEM		0x0c
#define USB_COMM_SUBCLASS_NCM		0x0d

/* Table 5: Communications Interface Class Control Protocol Codes */
#define USB_COMM_PROTO_NONE		0x00
#define USB_COMM_PROTO_AT_V25TER	0x01
#define USB_COMM_PROTO_AT_PCCA101	0x02
#define USB_COMM_PROTO_AT_PCCA101_WAKE	0x03
#define USB_COMM_PROTO_AT_GSM		0x04
#define USB_COMM_PROTO_AT_3G		0x05
#define USB_COMM_PROTO_AT_CDMA		0x06
#define USB_COMM_PROTO_EEM		0x07
#define USB_COMM_PROTO_EXTERNAL		0xfe
#define USB_COMM_PROTO_VENDOR		0xff

/* Table 6: Data Interface Class Code */
#define USB_CLASS_DATA			0x0a

/* Data Interface Class SubClass Codes */
/* un-used (0x00) */

/* Table 7: Data Interface Class Protocol Codes */
#define USB_DATA_PROTO_NONE		0x00
#define USB_DATA_PROTO_NTB		0x01
#define USB_DATA_PROTO_I430		0x30
#define USB_DATA_PROTO_HDLC		0x31
#define USB_DATA_PROTO_TRANSPARENT	0x32
#define USB_DATA_PROTO_Q921M		0x50
#define USB_DATA_PROTO_Q921		0x51
#define USB_DATA_PROTO_Q921TM		0x52
#define USB_DATA_PROTO_V42BIS		0x90
#define USB_DATA_PROTO_EURO_ISDN	0x91
#define USB_DATA_PROTO_V120		0x92
#define USB_DATA_PROTO_CAPI		0x93
#define USB_DATA_PROTO_HOST		0xfd
#define USB_DATA_PROTO_CDC		0xfe
#define USB_DATA_PROTO_VENDOR		0xff

/* Table 12: Type Values for the bDescriptorType Field */
#define USB_DT_CS_INTERFACE		0x24
#define USB_DT_CS_ENDPOINT		0x25

/* Table 13: bDescriptorSubType in Communications Class Functional
 * Descriptors */
#define USB_COMM_TYPE_HEADER		0x00
#define USB_COMM_TYPE_CALL_MANAGEMENT	0x01
#define USB_COMM_TYPE_ACM		0x02
#define USB_COMM_TYPE_DLM		0x03
#define USB_COMM_TYPE_TELEPHONE_RINGER	0x04
#define USB_COMM_TYPE_TELEPHONE_CALL	0x05
#define USB_COMM_TYPE_UNION		0x06
#define USB_COMM_TYPE_COUNTRY		0x07
#define USB_COMM_TYPE_TOM		0x08
#define USB_COMM_TYPE_USB_TERMINAL	0x09
#define USB_COMM_TYPE_NETWORK_TERMINAL	0x0a
#define USB_COMM_TYPE_PROTOCOL_UNIT	0x0b
#define USB_COMM_TYPE_EXTENSION_UNIT	0x0c
#define USB_COMM_TYPE_MCM		0x0d
#define USB_COMM_TYPE_CAPICM		0x0e
#define USB_COMM_TYPE_ETHERNET		0x0f
#define USB_COMM_TYPE_ATM		0x10
#define USB_COMM_TYPE_WHCM		0x11
#define USB_COMM_TYPE_MDLM		0x12
#define USB_COMM_TYPE_MDLM_DETAIL	0x13
#define USB_COMM_TYPE_DMM		0x14
#define USB_COMM_TYPE_OBEX		0x15
#define USB_COMM_TYPE_CMD_SET		0x16
#define USB_COMM_TYPE_CMD_SET_DETAIL	0x17
#define USB_COMM_TYPE_TCM		0x18
#define USB_COMM_TYPE_OBEX_SERVICE_ID	0x19
#define USB_COMM_TYPE_NCM		0x1a

/* Table 14: bDescriptorSubType in Data Class Functional Descriptors */
#define USB_DATA_TYPE_HEADER		0x00

/* Table 15: Class-Specific Descriptor Header Format */
struct usb_cdc_header_descriptor {
	u8 bFunctionLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u16 bcdCDC;
} __attribute__ ((packed));

/* Table 16: Union Interface Functional Descriptor */
#define USB_CDC_UNION_DESCRIPTOR(n)		\
	struct usb_cdc_union_descriptor_##n {	\
		u8 bFunctionLength;		\
		u8 bDescriptorType;		\
		u8 bDescriptorSubtype;		\
		u8 bControlInterface;		\
		u8 bSubordinateInterface[n];	\
	} __attribute__ ((packed))

/* Table 17: Country Selection Functional Descriptor */
#define USB_CDC_COUNTRY_DESCRIPTOR(n)		\
	struct usb_cdc_country_descriptor_##n {	\
		u8 bFunctionLength;		\
		u8 bDescriptorType;		\
		u8 bDescriptorSubtype;		\
		u8 iCountryCodeRelDate;		\
		u16 wCountryCode[n];		\
	} __attribute__ ((packed))

/* Table 19: Class-Specific Request Codes */
#define USB_CDC_REQ_SEND_ENCAPSULATED_COMMAND		0x00
#define USB_CDC_REQ_GET_ENCAPSULATED_RESPONSE		0x01
#define USB_CDC_REQ_SET_COMM_FEATURE			0x02
#define USB_CDC_REQ_GET_COMM_FEATURE			0x03
#define USB_CDC_REQ_CLEAR_COMM_FEATURE			0x04
#define USB_CDC_REQ_SET_AUX_LINE_STATE			0x10
#define USB_CDC_REQ_SET_HOOK_STATE			0x11
#define USB_CDC_REQ_PULSE_SETUP				0x12
#define USB_CDC_REQ_SEND_PULSE				0x13
#define USB_CDC_REQ_SET_PULSE_TIME			0x14
#define USB_CDC_REQ_RING_AUX_JACK			0x15
#define USB_CDC_REQ_SET_LINE_CODING			0x20
#define USB_CDC_REQ_GET_LINE_CODING			0x21
#define USB_CDC_REQ_SET_CONTROL_LINE_STATE		0x22
#define USB_CDC_REQ_SEND_BREAK				0x23
#define USB_CDC_REQ_SET_RINGER_PARMS			0x30
#define USB_CDC_REQ_GET_RINGER_PARMS			0x31
#define USB_CDC_REQ_SET_OPERATION_PARMS			0x32
#define USB_CDC_REQ_GET_OPERATION_PARMS			0x33
#define USB_CDC_REQ_SET_LINE_PARMS			0x34
#define USB_CDC_REQ_GET_LINE_PARMS			0x35
#define USB_CDC_REQ_DIAL_DIGITS				0x36
#define USB_CDC_REQ_SET_UNIT_PARAMETER			0x37
#define USB_CDC_REQ_GET_UNIT_PARAMETER			0x38
#define USB_CDC_REQ_CLEAR_UNIT_PARAMETER		0x39
#define USB_CDC_REQ_GET_PROFILE				0x3a
#define USB_CDC_REQ_SET_ETHERNET_MULTICAST_FILTERS	0x40
#define USB_CDC_REQ_SET_ETHERNET_PM_PATTERN_FILTER	0x41
#define USB_CDC_REQ_GET_ETHERNET_PM_PATTERN_FILTER	0x42
#define USB_CDC_REQ_SET_ETHERNET_PACKET_FILTER		0x43
#define USB_CDC_REQ_GET_ETHERNET_STATISTIC		0x44
#define USB_CDC_REQ_SET_ATM_DATA_FORMAT			0x50
#define USB_CDC_REQ_GET_ATM_DEVICE_STATISTICS		0x51
#define USB_CDC_REQ_SET_ATM_DEFAULT_VC			0x52
#define USB_CDC_REQ_GET_ATM_VC_STATISTICS		0x53
#define USB_CDC_REQ_GET_NTB_PARAMETERS			0x80
#define USB_CDC_REQ_GET_NET_ADDRESS			0x81
#define USB_CDC_REQ_SET_NET_ADDRESS			0x82
#define USB_CDC_REQ_GET_NTB_FORMAT			0x83
#define USB_CDC_REQ_SET_NTB_FORMAT			0x84
#define USB_CDC_REQ_GET_NTB_INPUT_SIZE			0x85
#define USB_CDC_REQ_SET_NTB_INPUT_SIZE			0x86
#define USB_CDC_REQ_GET_MAX_DATAGRAM_SIZE		0x87
#define USB_CDC_REQ_SET_MAX_DATAGRAM_SIZE		0x88
#define USB_CDC_REQ_GET_CRC_MODE			0x89
#define USB_CDC_REQ_SET_CRC_MODE			0x8a

/* Table 20: Class-Specific Notification Codes */
#define USB_CDC_NOTIFY_NETWORK_CONNECTION		0x00
#define USB_CDC_NOTIFY_RESPONSE_AVAILABLE		0x01
#define USB_CDC_NOTIFY_AUX_JACK_HOOK_STATE		0x08
#define USB_CDC_NOTIFY_RING_DETECT			0x09
#define USB_CDC_NOTIFY_SERIAL_STATE			0x20
#define USB_CDC_NOTIFY_CALL_STATE_CHANGE		0x28
#define USB_CDC_NOTIFY_LINE_STATE_CHANGE		0x29
#define USB_CDC_NOTIFY_CONNECTION_SPEED_CHANGE		0x2a

/* Definitions for Abstract Control Model devices from:
 * "Universal Serial Bus Communications Class Subclass Specification for
 * PSTN Devices"
 */

/* Table 3: Call Management Functional Descriptor */
struct usb_cdc_call_management_descriptor {
	u8 bFunctionLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bmCapabilities;
	u8 bDataInterface;
} __attribute__ ((packed));

#define USB_CDC_CALL_MGMT_CAP_CALL_MGMT		0x01
#define USB_CDC_CALL_MGMT_CAP_DATA_INTF		0x02

/* Table 4: Abstract Control Management Functional Descriptor */
struct usb_cdc_acm_descriptor {
	u8 bFunctionLength;
	u8 bDescriptorType;
	u8 bDescriptorSubtype;
	u8 bmCapabilities;
} __attribute__ ((packed));

#define USB_CDC_ACM_CAP_COMM_FEATURE		0x01
#define USB_CDC_ACM_CAP_LINE			0x02
#define USB_CDC_ACM_CAP_BREAK			0x04
#define USB_CDC_ACM_CAP_NOTIFY			0x08

/* Table 17: Line Coding Structure */
struct usb_cdc_line_coding {
	u32 dwDTERate;
	u8 bCharFormat;
	u8 bParityType;
	u8 bDataBits;
} __attribute__ ((packed));

#define USB_CDC_LINE_CODING_CHARFORMAT_1_STOP_BIT	0
#define USB_CDC_LINE_CODING_CHARFORMAT_1_5_STOP_BITS	1
#define USB_CDC_LINE_CODING_CHARFORMAT_2_STOP_BITS	2

#define USB_CDC_LINE_CODING_PARITYTYPE_NONE		0
#define USB_CDC_LINE_CODING_PARITYTYPE_ODD		1
#define USB_CDC_LINE_CODING_PARITYTYPE_EVEN		2
#define USB_CDC_LINE_CODING_PARITYTYPE_MARK		3
#define USB_CDC_LINE_CODING_PARITYTYPE_SPACE		4

/* Table 18: Control Signal Bitmap Values for SetControlLineState */
#define USB_CDC_CONTROL_LINE_STATE_DTR			0x01
#define USB_CDC_CONTROL_LINE_STATE_RTS			0x02

/* Notification Structure */
struct usb_cdc_notification {
	u8 bmRequestType;
	u8 bNotification;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
} __attribute__ ((packed));

/* Table 31: UART State Bitmap Values */
#define USB_CDC_SERIAL_STATE_RX_CARRIER		0x0001 /* DCD */
#define USB_CDC_SERIAL_STATE_TX_CARRIER		0x0002 /* DSR */
#define USB_CDC_SERIAL_STATE_BREAK		0x0004
#define USB_CDC_SERIAL_STATE_RING_SIGNAL	0x0008
#define USB_CDC_SERIAL_STATE_FRAMING		0x0010
#define USB_CDC_SERIAL_STATE_PARITY		0x0020
#define USB_CDC_SERIAL_STATE_OVERRUN		0x0040
