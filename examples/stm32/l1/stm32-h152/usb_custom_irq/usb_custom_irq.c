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

#include <rcc.h>
#include <pwr.h>
#include <flash.h>
#include <gpio.h>
#include <tim.h>
#include <syscfg.h>
#include <usbdevfs.h>
#include <nvic.h>
#include <exti.h>
#include <scb.h>

#include <usb/standard.h>
#include <usb/langid.h>

/*
 * This USB device has GPIOE(LED) read/write vendor requests.
 * GPIO10 is STAT1 LED, GPIO11 is STAT2 LED.
 * If you set bit 10 (0x0400), STAT1 LED is turned off.
 *
 * | bmRequestType | bRequest | wValue      | wIndex | wLength | Data
 * | 11000000B     | 0(read)  | Zero        | Zero   | Two     | GPIOE Value
 * | 01000000B     | 1(write) | GPIOE Value | Zero   | Zero    | None
 *
 */

/* Timer clock frequency */
#define TIMX_CLK_APB1		32000000

/* Packet buffer memory */
#define BUFFER_TABLE_ADDRESS	0
#define PACKET_MEMORY_START	(BUFFER_TABLE_ADDRESS + \
				 USBDEVFS_BUFFER_TABLE_SIZE)

/* Configuration value */
/* Value to use as an argument to the USB_REQ_SET_CONFIGURATION request */
#define CONFIGURATION_VALUE	1

/* Maximum packet size for endpoint zero */
#define MAXPACKETSIZE0		64

/* USB Device States */
usb_standard_state_t standard_state = USB_STATE_POWERED;

/* Control transfer states */
usbdevfs_control_state_t control_state;

/* OUT request */
struct usb_setup_data data_out_req;

/* statistics */
unsigned int usb_overrun;
unsigned int usb_error;

/* GPIOE data */
u16 led_state;

/* status */
u16 device_status;
u16 interface_status;
u16 endpoint_status;

/* --- USB Descriptors ----------------------------------------------------- */

/* Standard Device Descriptor */
const struct usb_device_descriptor dev_desc __attribute__ ((aligned(2))) = {
	.bLength = sizeof(struct usb_device_descriptor),
	.bDescriptorType = USB_DT_DEVICE,
	.bcdUSB = 0x0200,
	.bDeviceClass = USB_CLASS_VENDOR,
	.bDeviceSubClass = 0,
	.bDeviceProtocol = 0,
	.bMaxPacketSize0 = MAXPACKETSIZE0,
	.idVendor = 0x2975,
	.idProduct = 0x0001,
	.bcdDevice = 0x0020,
	.iManufacturer = 1,
	.iProduct = 2,
	.iSerialNumber = 0,
	.bNumConfigurations = 1,
};

/* Standard Configuration Descriptor and Standard Interface Descriptor */
struct config_desc {
	struct usb_config_descriptor config;
	struct usb_interface_descriptor interface;
} __attribute__ ((packed));

const struct config_desc config_desc __attribute__ ((aligned(2))) = {
	.config = {
		.bLength = sizeof(struct usb_config_descriptor),
		.bDescriptorType = USB_DT_CONFIGURATION,
		.wTotalLength = sizeof(struct config_desc),
		.bNumInterfaces = 1,
		.bConfigurationValue = CONFIGURATION_VALUE,
		.iConfiguration = 0,
		.bmAttributes = (USB_CONFIG_ATTR_D7 |
				 USB_CONFIG_ATTR_REMOTE_WAKEUP),
		.bMaxPower = 100 / 2,
	},
	.interface = {
		.bLength = sizeof(struct usb_interface_descriptor),
		.bDescriptorType = USB_DT_INTERFACE,
		.bInterfaceNumber = 0,
		.bAlternateSetting = 0,
		.bNumEndpoints = 0,
		.bInterfaceClass = 0,
		.bInterfaceSubClass = 0,
		.bInterfaceProtocol = 0,
		.iInterface = 0,
	},
};

/* Strings */
const u16 langid[] = {LANGID_ENGLISH_US, 0};
const u16 string1[] = L"MPC Research Ltd.";
const u16 string2[] = L"Custom Device";
const u16 *string_english_us[] = {
	string1,
	string2
};

/* --- USB Device Requests ------------------------------------------------- */

/* Vendor-specific Device Requests */
static bool vendor_request_error(struct usb_setup_data *req)
{
	switch (req->bRequest) {
	case 0:
		/*
		 * GET_GPIOE
		 *
		 * | bmRequestType | bRequest | wValue      | wIndex | wLength
		 * | 11000000B     | 0        | Zero        | Zero   | Two
		 */

		/* bmRequestType = 11000000B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_VENDOR |
					   USB_RECIP_DEVICE))
			return true;

		/* wLength = 2 */
		if (req->wLength != 2)
			return true;

		/* Not specified. */
		if (req->wValue || req->wIndex)
			return true;
		break;
	case 1:
		/*
		 * SET_GPIOE
		 *
		 * | bmRequestType | bRequest | wValue      | wIndex | wLength
		 * | 01000000B     | 1        | GPIOE Value | Zero   | Zero
		 */

		/* bmRequestType = 01000000B */
		if (req->bmRequestType != (USB_DIR_OUT | USB_TYPE_VENDOR |
					   USB_RECIP_DEVICE))
			return true;

		/* Not specified. */
		if (req->wLength || req->wIndex)
			return true;
		break;
	default:
		return true;
	}
	return false;
}

static int vendor_request(struct usb_setup_data *req)
{
	u16 data;

	switch (req->bRequest) {
	case 0:			/* GET_GPIOE */
		/* Read GPIOE */
		data = gpio_port_read(GPIOE);
		/* Write packet. */
		usbdevfs_write(0, &data, sizeof(data));
		break;
	case 1:			/* SET_GPIOE */
		/* Write GPIOE */
		gpio_port_write(GPIOE, req->wValue);
		break;
	default:
		return -1;
	}
	return 0;
}

/* Standard Device Requests */
static bool standard_request_error(struct usb_setup_data *req)
{
	int dt;
	int index;

	/* ENDPOINT_HALT */
	if (endpoint_status & USB_EP_STATUS_HALT) {
		if (req->bRequest != USB_REQ_GET_STATUS &&
		    req->bRequest != USB_REQ_SET_FEATURE &&
		    req->bRequest != USB_REQ_CLEAR_FEATURE)
			return true;
	}

	switch (req->bRequest) {
	case USB_REQ_GET_DESCRIPTOR:
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

		/* bmRequestType = 10000000B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |
					   USB_RECIP_DEVICE))
			return true;

		/* Descriptor Type */
		dt = (req->wValue >> 8);
		/* Descriptor Index */
		index = (req->wValue & 0xff);

		/* Descriptor type is device, configuration or string. */
		if (dt != USB_DT_DEVICE && dt != USB_DT_CONFIGURATION &&
		    dt != USB_DT_STRING)
			return true;

		/* Device descriptor */
		if (dt == USB_DT_DEVICE && (index || req->wIndex))
			return true;

		/* Configuration descriptor */
		if (dt == USB_DT_CONFIGURATION && (index || req->wIndex))
			return true;

		/* String descriptor */
		if (dt == USB_DT_STRING &&
		    ((!index && req->wIndex) ||
		     (index && req->wIndex != LANGID_ENGLISH_US) ||
		     (index > (int)(sizeof(string_english_us) / sizeof(u16 *)))
			    ))
			return true;
		break;
	case USB_REQ_GET_CONFIGURATION:
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

		/* bmRequestType = 10000000B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |
					   USB_RECIP_DEVICE))
			return true;

		/* Not specified. */
		if (req->wValue || req->wIndex || req->wLength != 1 ||
		    standard_state == USB_STATE_DEFAULT)
			return true;
		break;
	case USB_REQ_GET_INTERFACE:
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

		/* bmRequestType = 10000001B */
		if (req->bmRequestType != (USB_DIR_IN | USB_TYPE_STANDARD |
					   USB_RECIP_INTERFACE))
			return true;

		/* Request Error */
		if (req->wIndex || standard_state == USB_STATE_ADDRESS)
			return true;

		/* Not specified. */
		if (req->wValue || req->wLength != 1 ||
		    standard_state == USB_STATE_DEFAULT)
			return true;
		break;
	case USB_REQ_GET_STATUS:
		/*
		 * Get Status
		 *
		 * | bmRequestType | bRequest | wValue | wIndex    | wLength
		 * | 10000000B     | 0        | Zero   | Zero      | Two
		 * | 10000001B     |          |        | Interface |
		 * | 10000010B     |          |        | Endpoint  |
		 *
		 * Defaut state:	Not specified.
		 * Address state:	This request is valid.
		 * Configured state:	This request is valid.
		 */

		switch (req->bmRequestType) {
		/* bmRequestType = 10000000B */
		case (USB_DIR_IN | USB_TYPE_STANDARD |
		      USB_RECIP_DEVICE):
			/* Not specified. */
			if (req->wIndex)
				return true;
			break;
		/* bmRequestType = 10000001B */
		case (USB_DIR_IN | USB_TYPE_STANDARD |
		      USB_RECIP_INTERFACE):
			/* Request Error */
			if (req->wIndex || standard_state == USB_STATE_ADDRESS)
				return true;
			break;
		/* bmRequestType = 10000010B */
		case (USB_DIR_IN | USB_TYPE_STANDARD |
		      USB_RECIP_ENDPOINT):
			/* Request Error */
			if (req->wIndex)
				return true;
			break;
		default:
			return true;
		}

		/* Not specified. */
		if (req->wValue || req->wLength != 2 ||
		    standard_state == USB_STATE_DEFAULT)
			return true;
		break;
	case USB_REQ_SET_ADDRESS:
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

		/* bmRequestType = 00000000B */
		if (req->bmRequestType !=
		    (USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_DEVICE))
			return true;

		/* Not specified. */
		if (req->wValue > 127 || req->wIndex || req->wLength ||
		    standard_state == USB_STATE_CONFIGURED)
			return true;
		break;
	case USB_REQ_SET_CONFIGURATION:
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

		/* bmRequestType = 00000000B */
		if (req->bmRequestType !=
		    (USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_DEVICE))
			return true;

		/* Configuration value */
		if (req->wValue && req->wValue != CONFIGURATION_VALUE)
			return true;

		/* Not specified. */
		if (req->wIndex || req->wLength ||
		    standard_state == USB_STATE_DEFAULT)
			return true;
		break;
	case USB_REQ_SET_INTERFACE:
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

		/* bmRequestType = 00000001B */
		if (req->bmRequestType !=
		    (USB_DIR_OUT | USB_TYPE_STANDARD | USB_RECIP_INTERFACE))
			return true;

		/* Request Error */
		if (req->wValue || req->wIndex ||
		    standard_state == USB_STATE_ADDRESS)
			return true;

		/* Not specified. */
		if (req->wLength ||
		    standard_state == USB_STATE_DEFAULT)
			return true;
		break;
	case USB_REQ_SET_FEATURE:
		/*
		 * Set Feature
		 *
		 * | bmRequestType | bRequest | wValue   | wIndex    | wLength
		 * | 00000000B     | 3        | Feature  | Zero      | Zero
		 * | 00000001B     |          | Selector | Interface |
		 * | 00000010B     |          |          | Endpoint  |
		 *
		 * Defaut state:	Not specified.
		 * Address state:	This request is valid.
		 * Configured state:	This request is valid.
		 */

		switch (req->bmRequestType) {
		/* bmRequestType = 00000000B */
		case (USB_DIR_OUT | USB_TYPE_STANDARD |
		      USB_RECIP_DEVICE):
			/* TEST_MODE feature is not supported. */
			if (req->wValue != USB_FEAT_DEVICE_REMOTE_WAKEUP)
				return true;
			/* Not specified. */
			if (req->wIndex)
				return true;
			break;
		/* bmRequestType = 00000010B */
		case (USB_DIR_OUT | USB_TYPE_STANDARD |
		      USB_RECIP_ENDPOINT):
			if (req->wValue != USB_FEAT_ENDPOINT_HALT)
				return true;
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
		break;
	case USB_REQ_CLEAR_FEATURE:
		/*
		 * Clear Feature
		 *
		 * | bmRequestType | bRequest | wValue   | wIndex    | wLength
		 * | 00000000B     | 1        | Feature  | Zero      | Zero
		 * | 00000001B     |          | Selector | Interface |
		 * | 00000010B     |          |          | Endpoint  |
		 *
		 * Defaut state:	Not specified.
		 * Address state:	This request is valid.
		 * Configured state:	This request is valid.
		 */

		switch (req->bmRequestType) {
		/* bmRequestType = 00000000B */
		case (USB_DIR_OUT | USB_TYPE_STANDARD |
		      USB_RECIP_DEVICE):
			if (req->wValue != USB_FEAT_DEVICE_REMOTE_WAKEUP)
				return true;
			/* Not specified. */
			if (req->wIndex)
				return true;
			break;
		/* bmRequestType = 00000010B */
		case (USB_DIR_OUT | USB_TYPE_STANDARD |
		      USB_RECIP_ENDPOINT):
			if (req->wValue != USB_FEAT_ENDPOINT_HALT)
				return true;
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
		break;
	default:
		return true;
	}
	return false;
}

/* Make string descriptor from NULL-terminated string. */
static int make_string_descriptor(int index, u16 *buf, int buflen)
{
	int i;
	const u16 *p;
	u8 *s;

	if (index == 0)
		p = langid;	/* LANGID */
	else
		p = string_english_us[index - 1]; /* UNICODE encoded string */

	/* Copy data (not include EOS(0)). */
	for (i = 1; *p && buflen >= (i + 1) * (int)sizeof(u16); i++)
		*(buf + i) = *p++;

	/* Add bLength and bDescriptorType. */
	s = (u8 *)buf;
	*s++ = i * sizeof(u16);
	*s = USB_DT_STRING;

	/* Return descriptor size. */
	return i * (int)sizeof(u16);
}

static int standard_request(struct usb_setup_data *req)
{
	int dt;
	int index;
	int n;
	int len;
	u8 buf[MAXPACKETSIZE0] __attribute__ ((aligned(2)));

	switch (req->bRequest) {
	case USB_REQ_GET_DESCRIPTOR:
		/* Get Descriptor */

		/* Descriptor Type */
		dt = (req->wValue >> 8);
		/* Descriptor Index */
		index = (req->wValue & 0xff);
		/* Descriptor Length */
		n = req->wLength;
		if (dt == USB_DT_DEVICE) {
			/* Device Descriptor */
			if (sizeof(dev_desc) < req->wLength)
				n = sizeof(dev_desc);
			/* Write descriptor to packet memory. */
			usbdevfs_write(0, (u16 *)&dev_desc, n);
		} else if (dt == USB_DT_CONFIGURATION) {
			/* Configuration Descriptor */
			if (sizeof(config_desc) < req->wLength)
				n = sizeof(config_desc);
			/* Write descriptor to packet memory. */
			usbdevfs_write(0, (u16 *)&config_desc, n);
		} else if (dt == USB_DT_STRING) {
			/* String Descriptor */
			len = make_string_descriptor(index, (u16 *)buf,
						     sizeof(buf));
			if (len < req->wLength)
				n = len;
			/* Write descriptor to packet memory. */
			usbdevfs_write(0, (u16 *)buf, n);
		}
		break;
	case USB_REQ_GET_CONFIGURATION:
		/* Get Configuration */

		/* Return configuration value */
		if (standard_state == USB_STATE_ADDRESS)
			buf[0] = 0;
		else
			/* standard_state == USB_STATE_CONFIGURED */
			buf[0] = CONFIGURATION_VALUE;
		/* Write descriptor to packet memory. */
		usbdevfs_write(0, (u16 *)buf, 1);
		break;
	case USB_REQ_GET_INTERFACE:
		/* Get Interface */

		/* Return alternate setting */
		buf[0] = 0;
		/* Write descriptor to packet memory. */
		usbdevfs_write(0, (u16 *)buf, 1);
		break;
	case USB_REQ_GET_STATUS:
		/* Get Status */

		switch (req->bmRequestType & USB_RECIP_MASK) {
		case USB_RECIP_DEVICE:
			/* Write descriptor to packet memory. */
			usbdevfs_write(0, &device_status, 2);
			break;
		case USB_RECIP_INTERFACE:
			/* Write descriptor to packet memory. */
			usbdevfs_write(0, &interface_status, 2);
			break;
		case USB_RECIP_ENDPOINT:
			/* Write descriptor to packet memory. */
			usbdevfs_write(0, &endpoint_status, 2);
			break;
		default:
			break;
		}
		break;
	case USB_REQ_SET_ADDRESS:
		/* Set Address */

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

		break;
	case USB_REQ_SET_CONFIGURATION:
		/* Set Configuration */

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

		/* Clear status */
		interface_status = 0;
		endpoint_status = 0;
		break;
	case USB_REQ_SET_INTERFACE:
		/* Set Interface */

		/* Clear status */
		interface_status = 0;
		endpoint_status = 0;
		break;
	case USB_REQ_SET_FEATURE:
		/* Set Feature */

		switch (req->bmRequestType & USB_RECIP_MASK) {
		case USB_RECIP_DEVICE:
			device_status |= USB_DEV_STATUS_REMOTE_WAKEUP;
			break;
		case USB_RECIP_ENDPOINT:
			endpoint_status |= USB_EP_STATUS_HALT;
			break;
		default:
			break;
		}
		break;
	case USB_REQ_CLEAR_FEATURE:
		/* Clear Feature */

		switch (req->bmRequestType & USB_RECIP_MASK) {
		case USB_RECIP_DEVICE:
			device_status &= ~USB_DEV_STATUS_REMOTE_WAKEUP;
			break;
		case USB_RECIP_ENDPOINT:
			endpoint_status &= ~USB_EP_STATUS_HALT;
			break;
		default:
			break;
		}
		break;
	default:
		return -1;
	}
	return 0;
}

static int usb_device_request(struct usb_setup_data *req, int setup)
{
	if ((req->bmRequestType & USB_TYPE_MASK) == USB_TYPE_STANDARD) {
		/* Standard Device Request */
		if (setup) {
			/* Check error. */
			if (standard_request_error(req))
				return -1;

			/* GET request */
			if (req->bmRequestType & USB_DIR_IN)
				return standard_request(req);
		} else if (!(req->bmRequestType & USB_DIR_IN)) {
			/* SET request */
			return standard_request(req);
		}
	} else if ((req->bmRequestType & USB_TYPE_MASK) == USB_TYPE_VENDOR &&
		   standard_state == USB_STATE_CONFIGURED) {
		/* Vendor(Custom) Device Request */
		if (setup) {
			/* Check error. */
			if (vendor_request_error(req))
				return -1;

			/* GET request */
			if (req->bmRequestType & USB_DIR_IN)
				return vendor_request(req);
		} else if (!(req->bmRequestType & USB_DIR_IN)) {
			/* SET request */
			return vendor_request(req);
		}
	} else {
		return -1;
	}
	return 0;
}

/* --- USB Control Transfer ------------------------------------------------ */

/* OUT transaction */
static void control_rx(void)
{
	switch (control_state) {
	case USBDEVFS_DATA_OUT:
		/* No DATA_OUT transaction */
		control_state = USBDEVFS_STALL;
		break;
	case USBDEVFS_LAST_DATA_OUT:
		/* Send zero length packet */
		usbdevfs_write(0, 0, 0);
		control_state = USBDEVFS_STATUS_IN;
		break;
	case USBDEVFS_STATUS_OUT:
		control_state = USBDEVFS_STALL;
		break;
	default:
		control_state = USBDEVFS_STALL;
		break;
	}
	usbdevfs_set_control_state(0, control_state);
}

/* IN transaction */
static void control_tx(void)
{
	switch (control_state) {
	case USBDEVFS_DATA_IN:
		/* Only one data packet */
		control_state = USBDEVFS_LAST_DATA_IN;
		break;
	case USBDEVFS_LAST_DATA_IN:
		control_state = USBDEVFS_STATUS_OUT;
		break;
	case USBDEVFS_STATUS_IN:
		/* Execute saved request */
		usb_device_request(&data_out_req, 0);
		control_state = USBDEVFS_STALL;
		break;
	default:
		control_state = USBDEVFS_STALL;
		break;
	}
	usbdevfs_set_control_state(0, control_state);
}

/* SETUP transaction */
static void control_setup(u8 *buf, int len)
{
	struct usb_setup_data *req;

	if (len != sizeof(struct usb_setup_data)) {
		/* Unknown packet */
		control_state = USBDEVFS_STALL;
		usbdevfs_set_control_state(0, control_state);
		return;
	}
	req = (struct usb_setup_data *)buf;
	if (usb_device_request(req, 1)) {
		/* Request Error */
		control_state = USBDEVFS_STALL;
		usbdevfs_set_control_state(0, control_state);
		return;
	}
	if (req->bmRequestType & USB_DIR_IN) {
		/* 1 data packet */
		control_state = USBDEVFS_DATA_IN;
		control_tx();
	} else {
		/* Save the request */
		data_out_req = *req;
		/* No data stage */
		control_state = USBDEVFS_LAST_DATA_OUT;
		control_rx();
	}
}

/* --- USB Reset ----------------------------------------------------------- */

static void usb_reset(void)
{
	/* Disable button interrupt. */
	exti_disable_interrupt(EXTI0);

	/* Set endpoint type and address. */
	usbdevfs_setup_endpoint(0, USBDEVFS_CONTROL, 0);

	/* Set default address. */
	usbdevfs_set_device_address(0);

	/* Device state */
	standard_state = USB_STATE_DEFAULT;

	/* Clear status. */
	device_status = 0;
	interface_status = 0;
	endpoint_status = 0;

	/* Set control transfer state. */
	control_state = USBDEVFS_STALL;
	usbdevfs_set_control_state(0, control_state);
}

/* ------------------------------------------------------------------------- */

/* Set STM32 to 32 MHz. */
static void clock_setup(void)
{
	/* Enable PWR clock. */
	rcc_enable_clock(RCC_PWR);

	/* Set VCORE to 1.8V */
	pwr_set_vos(PWR_1_8_V);

	/* Set Flash memory latency (1WS). */
	flash_enable_64bit_access(1);

	/* Enable external high-speed oscillator 8MHz. */
	rcc_enable_osc(RCC_HSE);

	 /* Setup PLL (8MHz * 12 / 3 = 32MHz). */
	rcc_setup_pll(RCC_HSE, 12, 3);

	/* AHB, APB1 and APB2 prescaler value is default. */
	// rcc_set_prescaler(1, 1, 1);

	/* Enable PLL and wait for it to stabilize. */
	rcc_enable_osc(RCC_PLL);

	/* Select PLL as SYSCLK source. */
	rcc_set_sysclk_source(RCC_PLL);

	/* Disable internal multispeed oscillator. */
	rcc_disable_osc(RCC_MSI);

	/* Set the frequency range of MSI to 65 kHz. */
	/* (for low power run mode) */
	rcc_set_msi_range(RCC_MSI_65KHZ);

	/* Enable internal multispeed oscillator */
	rcc_enable_osc(RCC_MSI);
}

static void gpio_setup(void)
{
	/* Enable GPIO clock. */
	rcc_enable_clock(RCC_GPIOA);
	rcc_enable_clock(RCC_GPIOB);
	rcc_enable_clock(RCC_GPIOC);
	rcc_enable_clock(RCC_GPIOD);
	rcc_enable_clock(RCC_GPIOE);

	/* Set unused ports to analog input */
	gpio_config_analog(GPIO_PA(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 13, 14, 15));
	gpio_config_analog(GPIO_PB(0, 1, 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13,
				   14, 15));
	gpio_config_analog(GPIO_PC(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
				   13));
	gpio_config_analog(GPIO_PD_ALL);
	gpio_config_analog(GPIO_PE(0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 12, 13, 14,
				   15));

	/* Setup GPIO10 and GPIO11 (in GPIO port E) for LED use. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PE(10, 11));

	/* Set GPIO0 (in GPIO port A) to 'input float'. */
	gpio_config_input(GPIO_FLOAT, GPIO_PA0);
}

static void tim_setup(void)
{
	/* Enable TIM6 clock. */
	rcc_enable_clock(RCC_TIM6);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM6);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM6);

	/* Load prescaler value (2MHz). */
	tim_load_prescaler_value(TIM6, TIMX_CLK_APB1 / 2000000 - 1);
}

/* 1 - 32767 usec */
static void delay_us(u16 us)
{
	/* Set auto-reload value (us * 2). */
	tim_set_autoreload_value(TIM6, (us << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM6);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM6, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM6, TIM_UPDATE);
}

static void usb_setup(void)
{
	int offset;

	/* Enable USB and SYSCFG clock. */
	rcc_enable_clock(RCC_USB);
	rcc_enable_clock(RCC_SYSCFG);

	/* Enable USB Low priority interrupt. */
	nvic_enable_irq(NVIC_USB_LP_IRQ);

	/* Enable USB Device FS Wakeup interrupt. */
	nvic_enable_irq(NVIC_USB_FS_WKUP_IRQ);

	/* EXTI18 is connected to USB Device FS wakeup event. */
	exti_set_trigger(EXTI_USB_WAKEUP, EXTI_RISING);

	/* Enable EXTI interrupt. */
	exti_enable_interrupt(EXTI_USB_WAKEUP);

	/* Exit power down. */
	usbdevfs_disable_function(USBDEVFS_POWER_DOWN);

	/* Wait T_STATUP. */
	delay_us(USBDEVFS_T_STARTUP);

	/* Clear USB reset. */
	usbdevfs_disable_function(USBDEVFS_FORCE_RESET);

	/* Set buffer table address */
	usbdevfs_set_buffer_table_address(BUFFER_TABLE_ADDRESS);

	/* Assign packet memory to endpoint0 */
	offset = usbdevfs_assign_packet_memory_tx(0, PACKET_MEMORY_START,
						  MAXPACKETSIZE0);
	usbdevfs_assign_packet_memory_rx(0, offset, MAXPACKETSIZE0);
}

static void exti_setup(void)
{
	/* Enable EXTI0 interrupt. */
	nvic_enable_irq(NVIC_EXTI0_IRQ);

	/* Connect PA0 to EXTI0 */
	syscfg_select_exti_source(EXTI0, SYSCFG_PA);

	/* Set EXTI0 to rising edge trigger. */
	exti_set_trigger(EXTI0, EXTI_RISING);
}

/* USB wakeup interrupt */
void usb_fs_wkup_isr(void)
{
	if (exti_get_interrupt_mask(EXTI_USB_WAKEUP) &&
	    exti_get_interrupt_status(EXTI_USB_WAKEUP)) {
		if (usbdevfs_get_interrupt_mask(USBDEVFS_WAKEUP) &&
		    usbdevfs_get_interrupt_status(USBDEVFS_WAKEUP)) {
			/* Run mode */
			pwr_set_run_mode();
			pwr_wait_for_regulator_main_mode();
			scb_set_sleep(0); /* Reset SLEEPDEEP bit. */

			/* SYSCLK 65kHz -> 32MHz */
			pwr_set_vos(PWR_1_8_V);
			flash_enable_64bit_access(1);
			rcc_enable_osc(RCC_HSE);
			rcc_enable_osc(RCC_PLL);
			rcc_set_sysclk_source(RCC_PLL);

			/* Disable resume request. */
			usbdevfs_disable_function(USBDEVFS_RESUME);

			/* Restore LED state. */
			gpio_port_write(GPIOE, led_state);

			/* Exit low-power mode. */
			usbdevfs_disable_function(USBDEVFS_LOW_POWER_MODE);

			/* Exit suspend mode. */
			usbdevfs_disable_function(USBDEVFS_FORCE_SUSPEND);

			/* Clear interrupt. */
			usbdevfs_clear_interrupt(USBDEVFS_WAKEUP);
		}
		/* Clear EXTI18 interrupt. */
		exti_clear_interrupt(EXTI_USB_WAKEUP);
	}
}

/* USB (low priority) interrupt */
void usb_lp_isr(void)
{
	u16 mask;
	u16 status;
	int ep_id;
	u16 trans;
	u8 buf[MAXPACKETSIZE0] __attribute__ ((aligned(2)));
	int len;

	mask = usbdevfs_get_interrupt_mask(USBDEVFS_CORRECT_TRANSFER |
					   USBDEVFS_OVERRUN | USBDEVFS_ERROR |
					   USBDEVFS_SUSPEND | USBDEVFS_RESET);
	if (!mask)
		return;
	status = usbdevfs_get_interrupt_status(USBDEVFS_CORRECT_TRANSFER |
					       USBDEVFS_OVERRUN |
					       USBDEVFS_ERROR |
					       USBDEVFS_SUSPEND |
					       USBDEVFS_RESET |
					       USBDEVFS_DIR | USBDEVFS_EP_ID);
	if (!(status & (USBDEVFS_CORRECT_TRANSFER | USBDEVFS_OVERRUN |
			USBDEVFS_ERROR | USBDEVFS_SUSPEND | USBDEVFS_RESET)))
		return;

	/* Correct transfer */
	if (mask & status & USBDEVFS_CORRECT_TRANSFER) {
		ep_id = status & USBDEVFS_EP_ID;
		trans = usbdevfs_get_ep_status(ep_id, USBDEVFS_RX |
					       USBDEVFS_TX | USBDEVFS_SETUP);
		/* Rx (OUT/SETUP transaction) */
		if ((status & USBDEVFS_DIR) && (trans & USBDEVFS_RX)) {
			len = usbdevfs_read(ep_id, (u16 *)buf, sizeof(buf));
			if (trans & USBDEVFS_SETUP)
				control_setup(buf, len);
			else
				control_rx();
		}
		/* Tx (IN transaction) */
		if (!(status & USBDEVFS_DIR) && (trans & USBDEVFS_TX)) {
			control_tx();
		}
	}

	/* Packet memory area over/underrun */
	if (mask & status & USBDEVFS_OVERRUN) {
		usb_overrun++;
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_OVERRUN);
	}

	/* Error */
	if (mask & status & USBDEVFS_ERROR) {
		usb_error++;
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_ERROR);
	}

	/* Suspend mode request */
	/* (I can not decrease average current less than 500 uA.) */
	if (mask & status & USBDEVFS_SUSPEND) {
		/* Enter suspend mode. */
		usbdevfs_enable_function(USBDEVFS_FORCE_SUSPEND);

		/* Enter Low-power mode. */
		usbdevfs_enable_function(USBDEVFS_LOW_POWER_MODE);

		/* Save LED state. */
		led_state = gpio_get(GPIO_PE(11, 10));

		/* LED off */
		gpio_set(GPIO_PE(11, 10));

		/* Clear EXTI pending bits */
		exti_clear_interrupt(0xffffff);

		/* Enable button interrupt. */
		if (device_status & USB_DEV_STATUS_REMOTE_WAKEUP)
			exti_enable_interrupt(EXTI0);

		/* Slow down clock (32MHz -> 65kHz) */
		rcc_set_sysclk_source(RCC_MSI);
		flash_disable_64bit_access();

		/* VCORE range 2 */
		pwr_set_vos(PWR_1_5_V);

		/* Stop mode */
		scb_set_sleep(SCB_SCR_SLEEPDEEP);
		pwr_set_stop_mode();

		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_SUSPEND);
	}

	/* USB RESET */
	if (mask & status & USBDEVFS_RESET) {
		usb_reset();
		/* Clear interrupt. */
		usbdevfs_clear_interrupt(USBDEVFS_RESET);
	}
}

void exti0_isr(void)
{
	if (exti_get_interrupt_mask(EXTI0) &&
	    exti_get_interrupt_status(EXTI0)) {
		/* Enable resume request. */
		usbdevfs_enable_function(USBDEVFS_RESUME);

		/* Disable interrupt. */
		exti_disable_interrupt(EXTI0);

		/* Clear interrupt. */
		exti_clear_interrupt(EXTI0);
	}
}

int main(void)
{
	clock_setup();
	gpio_setup();
	tim_setup();
	usb_setup();
	exti_setup();

	/* Attach the device to USB. */
	syscfg_enable_usb_pullup();

	/* Clear interrupt. */
	usbdevfs_clear_interrupt(USBDEVFS_ALL_INTERRUPT);

	/* Enable interrupt. */
	usbdevfs_enable_interrupt(USBDEVFS_CORRECT_TRANSFER |
				  USBDEVFS_OVERRUN | USBDEVFS_ERROR |
				  USBDEVFS_WAKEUP | USBDEVFS_SUSPEND |
				  USBDEVFS_RESET);

	/* Sleep */
	while (1)
		__asm__ ("wfi");

	return 0;
}
