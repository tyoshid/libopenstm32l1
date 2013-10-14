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
#include <i2c.h>

/* Microchip 24FC64 - 64Kbit I2C Serial EEPROM */
#define ROM_SIZE	8192	/* 8KByte */
#define PAGE_SIZE	32	/* 32Byte */
#define ROM_SLA		0xa0	/* |1|0|1|0|A2|A1|A0|R/W| */

/* I2C timing */
#define SYSCLK		32000000
#define PCLK1		32000000

#ifdef FASTMODE
#ifdef DUTY
#define MODE		I2C_FAST_DUTY
#else
#define MODE		I2C_FAST
#endif
#define SCL		400000	 /* 400kHz */
#define T_R_MAX		10	 /* 300ns(tr.max) * PCLK1 = 10 */
#else
#define MODE		I2C_STANDARD
#define SCL		88000	 /* 88kHz (Errata) */
#define T_R_MAX		32	 /* 1us(tr.max) * PCLK1 = 32 */
#endif

#define BUSY_TIMEOUT	(SYSCLK / SCL)
#define TIMEOUT		(10 * (SYSCLK / SCL))

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
	/* Enable GPIOE clock. */
	rcc_enable_clock(RCC_GPIOE);

	/* Set GPIO10 and GPIO11 (in GPIO port E) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PE(10, 11));

	/* LED off */
	gpio_set(GPIO_PE(10, 11));
}

static void i2c_setup(void)
{
	/* Enable GPIOB clock. */
	rcc_enable_clock(RCC_GPIOB);

	/* Enable I2C1 clock. */
	rcc_enable_clock(RCC_I2C1);

	/* Set GPIO6 and GPIO7 (in GPIO port B) to 'altfn open-drain'. */
	gpio_config_altfn(GPIO_I2C1_2, GPIO_OPENDRAIN, GPIO_2MHZ, GPIO_NOPUPD,
			  GPIO_PB(I2C1_SCL, I2C1_SDA));
}

/* Byte Write */
static int rom_write_byte(u16 addr, u8 data)
{
	int r;
	u8 buf[3];

	buf[0] = addr >> 8;	/* Address High byte */
	buf[1] = addr & 0xff;	/* Address Low byte */
	buf[2] = data;		/* Data */

	do {
		/* Genarate Start condition. */
		if ((r = i2c_start(I2C1, TIMEOUT)) < 0)
			return r;

		/* Send I2C slave address (Control Byte). */
		r = i2c_addr(I2C1, ROM_SLA);
		if (r == -I2C_ERROR_STATUS &&
		    i2c_get_interrupt_status(I2C1, I2C_ERROR) == I2C_AF)
			/* ROM returned NACK. */
			i2c_clear_interrupt(I2C1, I2C_AF);
		else if (r < 0)
			return r;
	} while (r < 0);

	/* Send ROM address and data. */
	if ((r = i2c_write(I2C1, buf, 3)) < 0)
		return r;

	/* Generate Stop condition. */
	if ((r = i2c_stop(I2C1, TIMEOUT)) < 0)
		return r;

	return 0;
}

/* Page Write */
static int rom_write_page(u8 *data)
{
	int r;

	do {
		/* Genarate Start condition. */
		if ((r = i2c_start(I2C1, TIMEOUT)) < 0)
			return r;

		/* Send I2C slave address (Control Byte). */
		r = i2c_addr(I2C1, ROM_SLA);
		if (r == -I2C_ERROR_STATUS &&
		    i2c_get_interrupt_status(I2C1, I2C_ERROR) == I2C_AF)
			/* ROM returned NACK. */
			i2c_clear_interrupt(I2C1, I2C_AF);
		else if (r < 0)
			return r;
	} while (r < 0);

	/* Send ROM address and data. */
	if ((r = i2c_write(I2C1, data, PAGE_SIZE + 2)) < 0)
		return r;

	/* Generate Stop condition. */
	if ((r = i2c_stop(I2C1, TIMEOUT)) < 0)
		return r;

	return 0;
}

/* Random Read　/ Sequential Read */
static int rom_read(u16 addr, u8 *data, int nbyte)
{
	int r;
	u8 buf[2];

	buf[0] = addr >> 8;	/* Address High byte */
	buf[1] = addr & 0xff;	/* Address Low byte */

	do {
		/* Genarate Start condition. */
		if ((r = i2c_start(I2C1, TIMEOUT)) < 0)
			return r;

		/* Send I2C slave address (Control Byte). */
		r = i2c_addr(I2C1, ROM_SLA);
		if (r == -I2C_ERROR_STATUS &&
		    i2c_get_interrupt_status(I2C1, I2C_ERROR) == I2C_AF)
			/* ROM returned NACK. */
			i2c_clear_interrupt(I2C1, I2C_AF);
		else if (r < 0)
			return r;
	} while (r < 0);

	/* Send ROM address. */
	if ((r = i2c_write(I2C1, buf, 2)) < 0)
		return r;

	/* Genarate Start condition. */
	if ((r = i2c_start(I2C1, TIMEOUT)) < 0)
		return r;

	/* Send I2C slave address (Control Byte). */
	if ((r = i2c_addr(I2C1, ROM_SLA | I2C_READ)) < 0)
		return r;

	/* Receive ROM data. */
	if ((r = i2c_read(I2C1, data, nbyte)) < 0)
		return r;

	/* Wait for Stop condition. */
	if ((r = i2c_wait_previous_action(I2C1, TIMEOUT)) < 0)
		return r;

	return 0;
}

int main(void)
{
	u16 addr;
	u8 data;
	int i;
	int j;
	u8 buf[8192];
	int n;
	int m;

	clock_setup();
	gpio_setup();
	i2c_setup();

	/* Reset I2C. */
	if (i2c_get_status(I2C1, I2C_BUSY))
		if (i2c_software_reset(I2C1, BUSY_TIMEOUT))
			goto i2c_error;

	/* Set I2C clock frequency. */
	i2c_set_clock(I2C1, PCLK1, MODE, SCL, T_R_MAX);

	/* Enable I2C. */
	i2c_set_bus_mode(I2C1, I2C_ENABLE);

	/* Write data. */
	addr = 0;
	data = 0x55;
	/* Byte */
	for (i = 0; i < PAGE_SIZE; i++) {
		if (rom_write_byte(addr++, data++) < 0)
			goto i2c_error;
	}
	/* Page */
	for (j = 0; j < ROM_SIZE / PAGE_SIZE - 1; j++) {
		buf[0] = addr >> 8;
		buf[1] = addr & 0xff;
		addr += PAGE_SIZE;
		for (i = 0; i < PAGE_SIZE; i++)
			buf[i + 2] = data++;
		if (rom_write_page(buf) < 0)
			goto i2c_error;
	}

	/* Read and check data. */
	while (1) {
		for (n = 1; n < (int)sizeof(buf); n++) {
			/* LED(PE10) on/off */
			gpio_toggle(GPIO_PE10);

			addr = 0;
			data = 0x55;
			for (j = 0; j < ROM_SIZE / n; j++) {
				if (rom_read(addr, buf, n) < 0)
					goto i2c_error;
				for (i = 0; i < n; i++) {
					if (buf[i] != data)
						goto i2c_error;
					data++;
				}
				addr += n;
			}
			m = ROM_SIZE % n;
			if (m) {
				if (rom_read(addr, buf, m) < 0)
					goto i2c_error;
				for (i = 0; i < m; i++) {
					if (buf[i] != data)
						goto i2c_error;
					data++;
				}
			}
		}
	}

i2c_error:
	while (1) {
		/* LED(PE11) on/off */
		gpio_toggle(GPIO_PE11);

		/* Wait a bit. */
		for (i = 0; i < 800000; i++)
			__asm__ ("nop");
	}

	return 0;
}
