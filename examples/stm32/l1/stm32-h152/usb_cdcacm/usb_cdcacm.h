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
#define MAXPACKETSIZE0		64

/* Maximum OUT data length */
#define MAX_DATA_OUT		16

/* Maximum IN data length */
#define MAX_DATA_IN		64

/* Maximun interface number */
#define MAXINTERFACE		2

/* Maximum endpoint number */
#define MAXENDPOINT		4

/* Communications Class Interface */
#define INTERFACE_COMM		0
/* Notification endpoint */
#define NOTIFICATION_ENUM	0x81
/* Maximum packet size for notification endpoint */
#define NOTIFICATION_SIZE	16
/* Interval for polling endpoint */
#define NOTIFICATION_INTERVAL	128

/* Data Class Interface */
#define INTERFACE_DATA		1
/* Data rx endpoint */
#define DATA_RX_ENUM		0x02
/* Data tx endpoint */
#define DATA_TX_ENUM		0x83
/* Maximum packet size for data endpoint */
#define DATA_SIZE		64
/* Interval for polling endpoint */
#define DATA_INTERVAL		1

/* Maximum string index */
#define MAX_STRING_INDEX	3

/* USART queue size */
#define TXQUEUESIZE		(DATA_SIZE * 2)
#define RXQUEUESIZE		DATA_SIZE

/* Endpoint state */
typedef enum {
	EP_STATE_DISABLE,
	EP_STATE_ENABLE,
	EP_STATE_HALT
} ep_state_t;

typedef enum {
	ESCAPE_STATE0,
	ESCAPE_STATE1,		/* + */
	ESCAPE_STATE2,		/* ++ */
	ESCAPE_STATE3		/* +++ */
} escape_state_t;

void set_serial_number(u32 *uid);

void usart_set_param(int b, int d, int s, int p);
void usart_set_flow(bool rts, bool cts);
void usart_start_break(u16 period);
void usart_stop_break(void);

void start_escape_timer(void);
