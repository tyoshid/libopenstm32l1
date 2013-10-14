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

/* Definitions of Device Firmware Upgrade from
 * "Universal Serial Bus Class Definitions for Device Firmware Upgrade
 * Version 1.1"
 */

#define USB_CLASS_APPLICATION			0xfe

#define USB_APP_SUBCLASS_DFU			0x01

#define USB_DFU_PROTO_RUNTIME			0x01
#define USB_DFU_PROTO_DFU			0x02

/* Table 3.2: DFU Class-Specific Request Values */
#define USB_DFU_REQ_DETACH			0
#define USB_DFU_REQ_DNLOAD			1
#define USB_DFU_REQ_UPLOAD			2
#define USB_DFU_REQ_GETSTATUS			3
#define USB_DFU_REQ_CLRSTATUS			4
#define USB_DFU_REQ_GETSTATE			5
#define USB_DFU_REQ_ABORT			6

/* Table 4.2: DFU Functional Descriptor */
struct usb_dfu_descriptor {
	u8 bLength;
	u8 bDescriptorType;
	u8 bmAttributes;
	u16 wDetachTimeout;
	u16 wTransferSize;
	u16 bcdDFUVersion;
} __attribute__ ((packed));

#define USB_DT_DFU_FUNCTIONAL			0x21

/* bmAttributes */
#define USB_DFU_CAN_DOWNLOAD			0x01
#define USB_DFU_CAN_UPLOAD			0x02
#define USB_DFU_MANIFEST_TOLERANT		0x04
#define USB_DFU_WILL_DETACH			0x08

/* DFU_GETSTATUS response */
struct usb_dfu_status {
	u8 bStatus;
	u8 bwPollTimeout[3];
	u8 bState;
	u8 iString;
} __attribute__ ((packed));

/* Device Status */
typedef enum {
	USB_DFU_STATUS_OK,
	USB_DFU_STATUS_ERR_TARGET,
	USB_DFU_STATUS_ERR_FILE,
	USB_DFU_STATUS_ERR_WRITE,
	USB_DFU_STATUS_ERR_ERASE,
	USB_DFU_STATUS_ERR_CHECK_ERASED,
	USB_DFU_STATUS_ERR_PROG,
	USB_DFU_STATUS_ERR_VERIFY,
	USB_DFU_STATUS_ERR_ADDRESS,
	USB_DFU_STATUS_ERR_NOTDONE,
	USB_DFU_STATUS_ERR_FIRMWARE,
	USB_DFU_STATUS_ERR_VENDOR,
	USB_DFU_STATUS_ERR_USBR,
	USB_DFU_STATUS_ERR_POR,
	USB_DFU_STATUS_ERR_UNKNOWN,
	USB_DFU_STATUS_ERR_STALLEDPKT
} usb_dfu_status_t;

/* Device State */
typedef enum {
	USB_DFU_STATE_APP_IDLE,
	USB_DFU_STATE_APP_DETACH,
	USB_DFU_STATE_DFU_IDLE,
	USB_DFU_STATE_DFU_DNLOAD_SYNC,
	USB_DFU_STATE_DFU_DNBUSY,
	USB_DFU_STATE_DFU_DNLOAD_IDLE,
	USB_DFU_STATE_DFU_MANIFEST_SYNC,
	USB_DFU_STATE_DFU_MANIFEST,
	USB_DFU_STATE_DFU_MANIFEST_WAIT_RESET,
	USB_DFU_STATE_DFU_UPLOAD_IDLE,
	USB_DFU_STATE_DFU_ERROR
} usb_dfu_state_t;
