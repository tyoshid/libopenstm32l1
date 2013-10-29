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

#include <usbdevfs.h>

#include "usb_cdcacm.h"
#include "bulk.h"
#include "v25ter.h"

/* Endpoint state */
extern ep_state_t endpoint_state[MAXENDPOINT];

/* USART Tx queue */
extern u8 tx_queue[TXQUEUESIZE];
extern volatile int tx_head;
extern volatile int tx_tail;

/* USART Rx queue */
extern u8 rx_queue[RXQUEUESIZE];
extern volatile int rx_head;
extern volatile int rx_tail;

extern volatile escape_state_t escape_state;
extern volatile bool escape_timer_enable;

/* Command buffer */
static u8 command_buf[DATA_SIZE] __attribute__ ((aligned(2)));
static volatile int command_len;
static bool busy;

/* Escape character buffer */
static u8 escape_buf[3] __attribute__ ((aligned(2)));
static volatile int escape_len;
static volatile u8 *escape_p;

/* Data buffer */
static u8 data_buf[DATA_SIZE] __attribute__ ((aligned(2)));
static volatile int data_len;
static volatile u8 *data_p;

int bulk_puts(char *string1, char *string2)
{
	int i;

	i = 0;
	if (string1) {
		while (*string1)
			command_buf[i++] = *string1++;
	}
	if (string2) {
		while (*string2)
			command_buf[i++] = *string2++;
	}
	command_len = i;

	if (endpoint_state[3] == EP_STATE_ENABLE && !busy) {
		/* Write packet. */
		usbdevfs_write(3, (u16 *)command_buf, command_len);

		/* TX NAK->VALID */
		usbdevfs_start_endpoint_tx(3);

		/* Data is sent. */
		command_len = 0;

		/* Set flag. */
		busy = true;

		return 0;
	}

	return -1;
}

int bulk_puts_cont(void)
{
	/* No data */
	if (!command_len) {
		busy = false;
		return -1;
	}

	/* Write packet. */
	usbdevfs_write(3, (u16 *)command_buf, command_len);

	/* TX NAK->VALID */
	usbdevfs_start_endpoint_tx(3);

	/* Data is sent. */
	command_len = 0;

	if (endpoint_state[2] == EP_STATE_ENABLE)
		/* Rx NAK -> VALID */
		usbdevfs_enable_endpoint_rx(2);

	return 0;
}

int bulk_tx(void)
{
	int i;
	u8 buf[DATA_SIZE] __attribute__ ((aligned(2)));

	if (endpoint_state[3] != EP_STATE_ENABLE || busy)
		return -1;

	/* Copy data from Rx queue to buffer. */
	i = 0;
	while (rx_head != rx_tail) {
		buf[i++] = rx_queue[rx_head++];
		rx_head %= RXQUEUESIZE;
	}

	/* Write packet. */
	usbdevfs_write(3, (u16 *)buf, i);

	/* TX NAK->VALID */
	usbdevfs_start_endpoint_tx(3);

	/* Set flag. */
	busy = true;

	return 0;
}

int bulk_tx_cont(void)
{
	int i;
	u8 buf[DATA_SIZE] __attribute__ ((aligned(2)));

	/* Empty */
	if (rx_head == rx_tail) {
		busy = false;
		return -1;
	}

	/* Copy data from Rx queue to buffer. */
	i = 0;
	while (rx_head != rx_tail) {
		buf[i++] = rx_queue[rx_head++];
		rx_head %= RXQUEUESIZE;
	}

	/* Write packet. */
	usbdevfs_write(3, (u16 *)buf, i);

	/* TX NAK->VALID */
	usbdevfs_start_endpoint_tx(3);

	return 0;
}

static int rx_command(u8 *buf, int len)
{
	int i;
	char *r;
	char echo[8];
	int v = 0;

	for (i = 0; i < len; i++) {
		/* V.25 ter command line input */
		r = v25ter_input(buf[i], echo);

		v = bulk_puts(echo, r);

		/* Ignore the remainder of the command line */
		if (r)
			break;
	}
	return v;
}

void bulk_rx_command(void)
{
	u8 buf[DATA_SIZE] __attribute__ ((aligned(2)));
	int len;

	/* Read command. */
	len = usbdevfs_read(2, (u16 *)buf, sizeof(buf));

	/* Execute command */
	if (rx_command(buf, len))
		/* Clear USB interrupt */
		usbdevfs_clear_endpoint_interrupt(2);
	else
		/* Rx NAK -> VALID */
		usbdevfs_enable_endpoint_rx(2);
}

static int escape_string(char *s)
{
	int i;

	i = 0;
	while (*s)
		escape_buf[i++] = *s++;
	escape_len = i;
	return i;
}

static int check_escape(int c)
{
	switch (escape_state) {
	case ESCAPE_STATE0:
		if (c == '+' && !escape_timer_enable) {
			escape_state = ESCAPE_STATE1;
			start_escape_timer();
			return -1;
		} else {
			start_escape_timer();
		}
		break;
	case ESCAPE_STATE1:
		if (c == '+' && escape_timer_enable) {
			escape_state = ESCAPE_STATE2;
			return -1;
		} else {
			escape_state = ESCAPE_STATE0;
			start_escape_timer();
			return escape_string("+");
		}
	case ESCAPE_STATE2:
		if (c == '+' && escape_timer_enable) {
			escape_state = ESCAPE_STATE3;
			start_escape_timer();
			return -1;
		} else {
			escape_state = ESCAPE_STATE0;
			start_escape_timer();
			return escape_string("++");
		}
	case ESCAPE_STATE3:
		escape_state = ESCAPE_STATE0;
		start_escape_timer();
		return escape_string("+++");
	default:
		break;
	}
	return 0;
}

static void rx_data(void)
{
	int r;

	while (data_len) {
		/* Check escape sequence ("+++"). */
		r = check_escape(*data_p);

		/* Return escape character. */
		if (r > 0) {
			escape_p = escape_buf;
			while (escape_len) {
				/* Full */
				if ((tx_tail + 1) % TXQUEUESIZE == tx_head)
					break;

				/* Add data. */
				tx_queue[tx_tail++] = *escape_p++;
				tx_tail %= TXQUEUESIZE;

				escape_len--;
			}
		}

		/* Enqueue the character */
		if (r >= 0) {
			/* Full */
			if ((tx_tail + 1) % TXQUEUESIZE == tx_head)
				break;

			/* Add data. */
			tx_queue[tx_tail++] = *data_p;
			tx_tail %= TXQUEUESIZE;
		}

		/* Next character */
		data_p++;
		data_len--;
	}
}

void bulk_rx_data(void)
{
	/* Read data packet. */
	data_len = usbdevfs_read(2, (u16 *)data_buf, sizeof(data_buf));

	/* Set data pointer. */
	data_p = data_buf;

	/* Enqueue */
	rx_data();

	if (data_len)
		/* Clear USB interrupt */
		usbdevfs_clear_endpoint_interrupt(2);
	else
		/* Rx NAK -> VALID */
		usbdevfs_enable_endpoint_rx(2);
}

void bulk_rx_data_cont(void)
{
	int prev_len;

	/* Buffered escape character */
	while (escape_len) {
		/* Full */
		if ((tx_tail + 1) % TXQUEUESIZE == tx_head)
			break;

		/* Add data. */
		tx_queue[tx_tail++] = *escape_p++;
		tx_tail %= TXQUEUESIZE;
		escape_len--;
	}

	/* Save data_len. */
	prev_len = data_len;

	while (data_len) {
		/* Full */
		if ((tx_tail + 1) % TXQUEUESIZE == tx_head)
			break;

		/* Add data. */
		tx_queue[tx_tail++] = *data_p++;
		tx_tail %= TXQUEUESIZE;
		data_len--;
	}

	if (prev_len && !data_len)
		/* Rx NAK -> VALID */
		usbdevfs_enable_endpoint_rx(2);
}

void bulk_escape_timeout(void)
{
	if (escape_state == ESCAPE_STATE1)
		escape_string("+");
	else			/* ESCAPE_STATE2 */
		escape_string("++");

	/* Enqueue escape characters */
	escape_p = escape_buf;
	while (escape_len) {
		/* Full */
		if ((tx_tail + 1) % TXQUEUESIZE == tx_head)
			break;

		/* Add data. */
		tx_queue[tx_tail++] = *escape_p++;
		tx_tail %= TXQUEUESIZE;

		escape_len--;
	}
}

void bulk_reset(void)
{
	/* Clear flag. */
	busy = false;

	/* Discard the remaining data. */
	command_len = 0;
	escape_len = 0;
	data_len = 0;
}
