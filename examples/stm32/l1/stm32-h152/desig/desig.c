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
#include <gpio.h>
#include <usart.h>
#include <dbgmcu.h>
#include <desig.h>

#include <syscall.h>
#include <stdio.h>

/* USART clock frequency */
#define PCLK1	2097000

static void usart_setup(void)
{
	/* Enable GPIOD clock. */
	rcc_enable_clock(RCC_GPIOD);

	/* Enable USART2 clock. */
	rcc_enable_clock(RCC_USART2);

	/* Setup GPIO pin PD5 as alternate function. */
	gpio_config_altfn(GPIO_USART1_3, GPIO_PUSHPULL, GPIO_10MHZ,
			  GPIO_NOPUPD, GPIO_PD_USART2_TX);

	/* Setup USART2. */
	usart_init(USART2, PCLK1, 38400, 8, USART_STOP_1,
		   USART_PARITY_NONE, USART_FLOW_NONE, USART_TX);
}

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(USART2, ptr[i]);
		return i;
	}

	errno = EIO;
	return -1;
}

int main(void)
{
	u32 id;
	int flash;
	u32 uid[3] = {0, 0, 0};

	usart_setup();

	id = dbgmcu_get_device_id();
	printf("Device ID = 0x%x ", (unsigned int)(id & 0xfff));
	switch (id & 0xfff) {
	case 0x416:
		printf("[Low and Medium density device]\r\n");
		break;
	case 0x427:
		printf("[Medium+ density device]\r\n");
		break;
	case 0x436:
		printf("[High and Medium+(CSP64, BGA132, LQFP144) density "
		       "device]\r\n");
		break;
	default:
		printf("[Unknown]\r\n");
		break;
	}

	flash = desig_get_flash_size(id & 0xfff);

	printf("Rivision ID = 0x%x ", (unsigned int)(id >> 16));
	switch (id & 0xfff) {
	case 0x416:
		switch (id >> 16) {
		case 0x1000:
			printf("[Rev A]\r\n");
			break;
		case 0x1008:
			printf("[Rev Y]\r\n");
			break;
		case 0x1038:
			printf("[Rev W]\r\n");
			break;
		case 0x1078:
			printf("[Rev V]\r\n");
			break;
		default:
			printf("[Unknown]\r\n");
			break;
		}
		break;
	case 0x427:
		switch (id >> 16) {
		case 0x1018:
			printf("[Rev A]\r\n");
			break;
		default:
			printf("[Unknown]\r\n");
			break;
		}
		break;
	case 0x436:
		if (flash == 256 * 1024)
			switch (id >> 16) {
			case 0x1018:
				printf("[Rev A]\r\n");
				break;
			default:
				printf("[Unknown]\r\n");
				break;
			}
		else
			switch (id >> 16) {
			case 0x1000:
				printf("[Rev A]\r\n");
				break;
			case 0x1008:
				printf("[Rev Z]\r\n");
				break;
			case 0x1018:
				printf("[Rev Y]\r\n");
				break;
			default:
				printf("[Unknown]\r\n");
				break;
			}
		break;
	}

	printf("Flash memory size = %d Kbytes\r\n", flash / 1024);

	desig_get_unique_id(id & 0xfff, uid);
	printf("Unique ID = %08x%08x%08x\r\n",
	       (unsigned int)uid[2], (unsigned int)uid[1],
	       (unsigned int)uid[0]);

	while (1)
		;

	return 0;
}
