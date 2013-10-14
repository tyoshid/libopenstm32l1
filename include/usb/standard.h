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

/*
 * This file contains structure definitions for the USB control structures
 * defined in chapter 9 of the "Univeral Serial Bus Specification Revision 2.0"
 * Available from the USB Implementers Forum - http://www.usb.org/
 */

/* --- USB Device States --------------------------------------------------- */

/* USB Device States - Figure 9-1 */
typedef enum {
	USB_STATE_ATTACHED,
	USB_STATE_POWERED,
	USB_STATE_DEFAULT,
	USB_STATE_ADDRESS,
	USB_STATE_CONFIGURED,
	USB_STATE_SUSPENDED
} usb_standard_state_t;

/* --- USB Device Requests ------------------------------------------------- */

/* USB Setup Data structure - Table 9-2 */
struct usb_setup_data {
	u8 bmRequestType;
	u8 bRequest;
	u16 wValue;
	u16 wIndex;
	u16 wLength;
} __attribute__ ((packed));

/* bmRequestType bit definitions */
/* Data transfer direction */
#define USB_DIR_OUT				(0 << 7) /* Host-to-device */
#define USB_DIR_IN				(1 << 7) /* Device-to-host */
/* Type */
#define USB_TYPE_MASK				(3 << 5)
#define USB_TYPE_STANDARD			(0 << 5)
#define USB_TYPE_CLASS				(1 << 5)
#define USB_TYPE_VENDOR				(2 << 5)
/* Recipient */
#define USB_RECIP_MASK				(31 << 0)
#define USB_RECIP_DEVICE			(0 << 0)
#define USB_RECIP_INTERFACE			(1 << 0)
#define USB_RECIP_ENDPOINT			(2 << 0)
#define USB_RECIP_OTHER				(3 << 0)

/* USB Standard Request Codes - Table 9-4 */
#define USB_REQ_GET_STATUS			0
#define USB_REQ_CLEAR_FEATURE			1
/* Reserved for future use: 2 */
#define USB_REQ_SET_FEATURE			3
/* Reserved for future use: 3 */
#define USB_REQ_SET_ADDRESS			5
#define USB_REQ_GET_DESCRIPTOR			6
#define USB_REQ_SET_DESCRIPTOR			7
#define USB_REQ_GET_CONFIGURATION		8
#define USB_REQ_SET_CONFIGURATION		9
#define USB_REQ_GET_INTERFACE			10
#define USB_REQ_SET_INTERFACE			11
#define USB_REQ_SYNCH_FRAME			12

/* USB Descriptor Types - Table 9-5 */
#define USB_DT_DEVICE				1
#define USB_DT_CONFIGURATION			2
#define USB_DT_STRING				3
#define USB_DT_INTERFACE			4
#define USB_DT_ENDPOINT				5
#define USB_DT_DEVICE_QUALIFIER			6
#define USB_DT_OTHER_SPEED_CONFIGURATION	7
#define USB_DT_INTERFACE_POWER			8
/* From ECNs */
#define USB_DT_OTG				9
#define USB_DT_DEBUG				10
#define USB_DT_INTERFACE_ASSOCIATION		11

/* USB Standard Feature Selectors - Table 9-6 */
#define USB_FEAT_ENDPOINT_HALT			0
#define USB_FEAT_DEVICE_REMOTE_WAKEUP		1
#define USB_FEAT_TEST_MODE			2

/* Information Returned by a GetStatus() Request to a Device - Figure 9-4 */
#define USB_DEV_STATUS_SELF_POWERED		(1 << 0)
#define USB_DEV_STATUS_REMOTE_WAKEUP		(1 << 1)
/* Information Returned by a GetStatus() Request to an Endpoint - Figure 9-6 */
#define USB_EP_STATUS_HALT			(1 << 0)

/* Test Mode Selectors - Table 9-7 */
#define USB_TEST_J				0x01
#define USB_TEST_K				0x02
#define USB_TEST_SE0_NAK			0x03
#define USB_TEST_PACKET				0x04
#define USB_TEST_FORCE_ENABLE			0x05

/* --- Standard USB Descriptor Definitions --------------------------------- */

/* USB Standard Device Descriptor - Table 9-8 */
struct usb_device_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u16 idVendor;
	u16 idProduct;
	u16 bcdDevice;
	u8 iManufacturer;
	u8 iProduct;
	u8 iSerialNumber;
	u8 bNumConfigurations;
} __attribute__ ((packed));

/* Class Definition */
#define USB_CLASS_VENDOR            		0xFF

/* bMaxPacketSize0: only 8, 16, 32 or 64 are valid */
#define USB_MAX_MAXPACKETSIZE0			64

/* USB Device_Qualifier Descriptor - Table 9-9
 * Not used in this implementation.
 */
struct usb_device_qualifier_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u16 bcdUSB;
	u8 bDeviceClass;
	u8 bDeviceSubClass;
	u8 bDeviceProtocol;
	u8 bMaxPacketSize0;
	u8 bNumConfigurations;
	u8 bReserved;
} __attribute__ ((packed));

/* USB Standard Configuration Descriptor - Table 9-10 */
struct usb_config_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u16 wTotalLength;
	u8 bNumInterfaces;
	u8 bConfigurationValue;
	u8 iConfiguration;
	u8 bmAttributes;
	u8 bMaxPower;
} __attribute__ ((packed));

/* USB Configuration Descriptor bmAttributes bit definitions */
#define USB_CONFIG_ATTR_D7			(1 << 7) /* must be set */
#define USB_CONFIG_ATTR_SELF_POWERED		(1 << 6)
#define USB_CONFIG_ATTR_REMOTE_WAKEUP		(1 << 5)

/* Other Speed Configuration is the same as Configuration Descriptor.
 *  - Table 9-11
 */

/* USB Standard Interface Descriptor - Table 9-12 */
struct usb_interface_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bInterfaceNumber;
	u8 bAlternateSetting;
	u8 bNumEndpoints;
	u8 bInterfaceClass;
	u8 bInterfaceSubClass;
	u8 bInterfaceProtocol;
	u8 iInterface;
} __attribute__ ((packed));

/* USB Standard Endpoint Descriptor - Table 9-13 */
struct usb_endpoint_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bEndpointAddress;
	u8 bmAttributes;
	u16 wMaxPacketSize;
	u8 bInterval;
} __attribute__ ((packed));

/* USB Endpoint Descriptor bmAttributes bit definitions */
/* Transfer Type */
#define USB_ENDPOINT_TRANS_MASK			(3 << 0)
#define USB_ENDPOINT_TRANS_CONTROL		(0 << 0)
#define USB_ENDPOINT_TRANS_ISOCHRONOUS		(1 << 0)
#define USB_ENDPOINT_TRANS_BULK			(2 << 0)
#define USB_ENDPOINT_TRANS_INTERRUPT		(3 << 0)
/* Synchronization Type */
#define USB_ENDPOINT_SYNC_MASK			(3 << 2)
#define USB_ENDPOINT_SYNC_NOSYNC		(0 << 2)
#define USB_ENDPOINT_SYNC_ASYNC			(1 << 2)
#define USB_ENDPOINT_SYNC_ADAPTIVE		(2 << 2)
#define USB_ENDPOINT_SYNC_SYNC			(3 << 2)
/* Usage Type */
#define USB_ENDPOINT_USAGE_MASK			(3 << 4)
#define USB_ENDPOINT_USAGE_DATA			(0 << 4)
#define USB_ENDPOINT_USAGE_FEEDBACK		(1 << 4)
#define USB_ENDPOINT_USAGE_IMPLICIT_FEEDBACK	(2 << 4)

/* Table 9-15 specifies String Descriptor Zero.
 * Table 9-16 specified UNICODE String Descriptor.
 */
#define USB_STRING_DESCRIPTOR(n)		\
	struct usb_string_descriptor_##n {	\
		u8 bLength;			\
		u8 bDescriptorType;		\
		u16 wData[n];			\
	} __attribute__ ((packed))

/* From ECN: Interface Association Descriptors, Table 9-Z */
struct usb_iface_assoc_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bFirstInterface;
	u8 bInterfaceCount;
	u8 bFunctionClass;
	u8 bFunctionSubClass;
	u8 bFunctionProtocol;
	u8 iFunction;
} __attribute__ ((packed));

