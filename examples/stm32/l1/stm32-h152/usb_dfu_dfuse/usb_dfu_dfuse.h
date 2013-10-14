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

/* Maximun transfer size (FLASH_PAGE_SIZE) */
#define MAXTRANSFERSIZE		256

/* Maximum OUT data length */
#define MAX_DATA_OUT		MAXTRANSFERSIZE

/* Maximum IN data length */
#define MAX_DATA_IN		MAXTRANSFERSIZE

/* Maximun interface number */
#define MAXINTERFACE		1

/* Maximum string index */
#define MAX_STRING_INDEX	3

/* Application start address */
#define APP_ADDRESS		0x08002000

bool program_addr_error(u32 addr);
int program_time(int n);
void program_erase(u32 addr);
void program_memory(u32 *d, u32 *s, int nword);

int block_transfer(void);
