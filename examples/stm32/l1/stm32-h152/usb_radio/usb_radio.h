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

/* Configuration value */
/* Value to use as an argument to the USB_REQ_SET_CONFIGURATION request */
#define CONFIGURATION_VALUE	1

/* Maximum packet size for endpoint zero */
#define MAXPACKETSIZE0		16

/* Maximum OUT data length */
#define MAX_DATA_OUT		64

/* Maximum IN data length */
#define MAX_DATA_IN		128

/* Maximun interface number */
#define MAXINTERFACE		3

/* Maximum endpoint number */
#define MAXENDPOINT		3

/* Audio Control Interface */
#define INTERFACE_AC		0

/* Audio Stream Interface */
#define INTERFACE_AS		1
/* AS endpoint address */
#define AS_ENUM			0x83
/* Maximum packet size for data endpoint */
#define AS_SIZE			192
/* Interval for polling endpoint */
#define AS_INTERVAL		1

/* HID Interface */
#define INTERFACE_HID		2
/* Interrupt endpoint address */
#define INTERRUPT_ENUM		0x81
/* Maximum packet size for interrupt endpoint */
#define INTERRUPT_SIZE		16
/* Interval for polling endpoint */
#define INTERRUPT_INTERVAL	10

/* Maximum string index */
#define MAX_STRING_INDEX	2

/* Terminal ID */
#define INPUT_TERMINAL_ID	1
#define FEATURE_UNIT_ID		2
#define OUTPUT_TERMINAL_ID	3

/* Bit Resolution */
#define SAMPLING_BITS		16
/* Sampling Frequency */
#define SAMPLING_FREQ		48000


/* Endpoint state */
typedef enum {
	EP_STATE_DISABLE,
	EP_STATE_ENABLE,
	EP_STATE_HALT
} ep_state_t;

extern ep_state_t endpoint_state[MAXENDPOINT];

/* Wait state */
typedef enum {
	WAIT_STATE_NONE = 0,
	WAIT_STATE_SETUP_BUSY = 2,
	WAIT_STATE_SETUP_DEVICE = 3,
	WAIT_STATE_DATA_BUSY = 4,
	WAIT_STATE_DATA_DEVICE = 5
} wait_state_t;

extern wait_state_t wait_state;

/* Si4703 device request */
typedef enum {
	DEVICE_REQUEST_NONE,
	DEVICE_REQUEST_READ,
	DEVICE_REQUEST_WRITE
} device_request_t;

extern volatile device_request_t devreq;
extern volatile int devreq_id;
extern volatile u16 devreq_reg[];
extern volatile u16 si4703_reg[];

extern unsigned int idle_duration;
