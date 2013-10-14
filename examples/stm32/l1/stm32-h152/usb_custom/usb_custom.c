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

#include <usb/standard.h>

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

/* Maximum packet size for endpoint zero */
#define MAXPACKETSIZE0		64

/* USB Device States */
usb_standard_state_t standard_state;

/* Control transfer states */
usbdevfs_control_state_t control_state;

/* OUT request */
struct usb_setup_data data_out_req;

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
	.bcdDevice = 0x0010,
	.iManufacturer = 0,
	.iProduct = 0,
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
		.bConfigurationValue = 1,
		.iConfiguration = 0,
		.bmAttributes = USB_CONFIG_ATTR_D7,
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

/* --- USB Device Requests ------------------------------------------------- */

/* Vendor-specific Device Requests */
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
static int standard_request(struct usb_setup_data *req)
{
	int dt;
	int n;

	switch (req->bRequest) {
	case USB_REQ_GET_DESCRIPTOR:
		/* Get Descriptor */

		/* Descriptor Type */
		dt = (req->wValue >> 8);
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
		} else {
			return -1;
		}
		break;
	case USB_REQ_SET_ADDRESS:
		/* Set Address */

		/* Set device address. */
		usbdevfs_set_device_address(req->wValue);

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
			if (req->wValue == 1)
				standard_state = USB_STATE_CONFIGURED;
		} else if (standard_state == USB_STATE_CONFIGURED) {
			/*
			 * If the specified configuration value is zero,
			 * then the device enters the Address state.
			 */
			if (!req->wValue)
				standard_state = USB_STATE_ADDRESS;
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
			/* GET request */
			if (req->bmRequestType & USB_DIR_IN)
				return standard_request(req);
		}
		else if (!(req->bmRequestType & USB_DIR_IN))
			/* SET request */
			return standard_request(req);
	} else if ((req->bmRequestType & USB_TYPE_MASK) == USB_TYPE_VENDOR &&
		   standard_state == USB_STATE_CONFIGURED) {
		/* Vendor(Custom) Device Request */
		if (setup) {
			/* GET request */
			if (req->bmRequestType & USB_DIR_IN)
				return vendor_request(req);
		}
		else if (!(req->bmRequestType & USB_DIR_IN))
			/* SET request */
			return vendor_request(req);
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
		/* Execcute saved request */
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
	/* Set endpoint type and address. */
	usbdevfs_setup_endpoint(0, USBDEVFS_CONTROL, 0);

	/* Set default address. */
	usbdevfs_set_device_address(0);

	/* Device state */
	standard_state = USB_STATE_DEFAULT;

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
}

static void gpio_setup(void)
{
	/* Enable GPIOE clock (for LED). */
	rcc_enable_clock(RCC_GPIOE);

	/* Setup GPIO10 and GPIO11 (in GPIO port E) for LED use. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PE(10, 11));
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

	/* Exit Power Down. */
	usbdevfs_disable_function(USBDEVFS_POWER_DOWN);

	/* Wait T_STARTUP. */
	delay_us(USBDEVFS_T_STARTUP);

	/* Clear USB reset. */
	usbdevfs_disable_function(USBDEVFS_FORCE_RESET);

	/* Set buffer table address */
	usbdevfs_set_buffer_table_address(BUFFER_TABLE_ADDRESS);

	/* Assign packet memory to endpoint 0 */
	offset = usbdevfs_assign_packet_memory_tx(0, PACKET_MEMORY_START,
						  MAXPACKETSIZE0);
	usbdevfs_assign_packet_memory_rx(0, offset, MAXPACKETSIZE0);
}

int main(void)
{
	u16 status;
	int ep_id;
	u16 trans;
	u8 buf[MAXPACKETSIZE0] __attribute__ ((aligned(2)));
	int len;

	clock_setup();
	gpio_setup();
	tim_setup();
	usb_setup();

	/* Attach the device to USB. */
	syscfg_enable_usb_pullup();

	standard_state = USB_STATE_POWERED;

	/* Clear interrupt. */
	usbdevfs_clear_interrupt(USBDEVFS_ALL_INTERRUPT);

	while (1) {
		status = usbdevfs_get_interrupt_status(
			USBDEVFS_CORRECT_TRANSFER | USBDEVFS_RESET |
			USBDEVFS_DIR | USBDEVFS_EP_ID);
		/* Correct transfer */
		if (status & USBDEVFS_CORRECT_TRANSFER) {
			/* Endpoint Identifier (must be 0) */
			ep_id = status & USBDEVFS_EP_ID;

			trans = usbdevfs_get_ep_status(ep_id, USBDEVFS_RX |
						       USBDEVFS_TX |
						       USBDEVFS_SETUP);
			/* Rx (OUT/SETUP transaction) */
			if ((status & USBDEVFS_DIR) &&
			    (trans & USBDEVFS_RX)) {
				/* Read packet */
				len = usbdevfs_read(ep_id, (u16 *)buf,
						    sizeof(buf));
				if (trans & USBDEVFS_SETUP)
					control_setup(buf, len);
				else
					control_rx();
			}
			/* Tx (IN transaction) */
			if (!(status & USBDEVFS_DIR) &&
			    (trans & USBDEVFS_TX)) {
				control_tx();
			}
		}
		/* USB RESET */
		if (status & USBDEVFS_RESET) {
			usb_reset();
			/* Clear interrupt. */
			usbdevfs_clear_interrupt(USBDEVFS_RESET);
		}
	}
	return 0;
}
