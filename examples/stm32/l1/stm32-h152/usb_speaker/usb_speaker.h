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
#define MAXPACKETSIZE0		32

/* Maximum OUT data length */
#define MAX_DATA_OUT		2

/* Maximum IN data length */
#define MAX_DATA_IN		64

/* Maximun interface number */
#define MAXINTERFACE		2

/* Maximum endpoint number */
#define MAXENDPOINT		2

/* Audio Control Interface */
#define INTERFACE_AC		0

/* Audio Stream Interface */
#define INTERFACE_AS		1
/* AS endpoint address */
#define AS_ENUM			1
/* Maximum packet size for data endpoint */
#define AS_SIZE			192
/* Interval for polling endpoint */
#define AS_INTERVAL		1

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

/* Volume */
#define MIN_VOLUME		(-63 * 256)
#define MAX_VOLUME		(0 * 256)
#define RES_VOLUME		(1 * 256)
#define DEFAULT_VOLUME		(-10 * 256)
#define MUTE_VOLUME		(-64 * 256)


void volume_set(s16 ch1, s16 ch2);
