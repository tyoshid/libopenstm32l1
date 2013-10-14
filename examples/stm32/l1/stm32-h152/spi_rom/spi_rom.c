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
#include <spi.h>

#define TIMX_CLK_APB1	32000000

/* Microchip 25LC640A - 64Kbit SPI Serial EEPROM */
#define ROM_SIZE	8192	/* 8KByte */
#define PAGE_SIZE	32	/* 32Byte */
#define COMMAND_READ	0x03
#define COMMAND_WRITE	0x02
#define COMMAND_WRDI	0x04
#define COMMAND_WREN	0x06
#define COMMAND_RDSR	0x05
#define COMMAND_WRSR	0x01
#define TWC		5	/* Internal write cycle time: 5 msec */

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

static void tim_setup(void)
{
	/* Enable TIM6 clock. */
	rcc_enable_clock(RCC_TIM6);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM6);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM6);

	/* Load prescaler value (2kHz). */
	tim_load_prescaler_value(TIM6, TIMX_CLK_APB1 / 2000 - 1);
}

/* 1 - 32767 msec */
static void delay_ms(u16 ms)
{
	/* Set auto-reload value (ms * 2). */
	tim_set_autoreload_value(TIM6, (ms << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM6);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM6, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM6, TIM_UPDATE);
}

static void spi_setup(void)
{
	/* Enable GPIOD clock. */
	rcc_enable_clock(RCC_GPIOD);

	/* Enable GPIOE clock. */
	// rcc_enable_clock(RCC_GPIOE);

	/* 'nCS' High */
	gpio_set(GPIO_PD5);

	/* Set GPIO5 (in GPIO port D) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_40MHZ, GPIO_NOPUPD, GPIO_PD5);

	/* Enable SPI1 clock. */
	rcc_enable_clock(RCC_SPI1);

	/* Set GPIO13-15 (in GPIO port E) to 'altfn push-pull'. */
	gpio_config_altfn(GPIO_SPI1_2, GPIO_PUSHPULL, GPIO_40MHZ, GPIO_NOPUPD,
			  GPIO_PE(SPI1_SCK, SPI1_MISO, SPI1_MOSI));

	// SPI1_CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_SPE |
	//	SPI_CR1_BR_FPCLK_DIV_4 | SPI_CR1_MSTR;
	spi_set_mode(SPI1, 4, SPI_NSS_SOFTWARE | SPI_NSS_HIGH | SPI_MASTER |
		     SPI_ENABLE);
}

/* Byte Write */
static int rom_write_byte(u16 addr, u8 data)
{
	int r;

	/* 'nCS' Low */
	gpio_clear(GPIO_PD5);

	/* Write Enable */
	if ((r = spi_transfer(SPI1, COMMAND_WREN)) < 0)
		return r;

	/* 'nCS' High */
	gpio_set(GPIO_PD5);


	/* 'nCS' Low */
	gpio_clear(GPIO_PD5);

	/* Instruction */
	if ((r = spi_transfer(SPI1, COMMAND_WRITE)) < 0)
		return r;

	/* Address (high byte) */
	if ((r = spi_transfer(SPI1, addr >> 8)) < 0)
		return r;

	/* Address (low byte) */
	if ((r = spi_transfer(SPI1, addr & 0xff)) < 0)
		return r;

	/* Data */
	if ((r = spi_transfer(SPI1, data)) < 0)
		return r;

	/* 'nCS' High */
	gpio_set(GPIO_PD5);

	return 0;
}

/* Page Write */
static int rom_write_page(u16 addr, u8 *data)
{
	int r;
	int i;

	/* 'nCS' Low */
	gpio_clear(GPIO_PD5);

	/* Write Enable */
	if ((r = spi_transfer(SPI1, COMMAND_WREN)) < 0)
		return r;

	/* 'nCS' High */
	gpio_set(GPIO_PD5);


	/* 'nCS' Low */
	gpio_clear(GPIO_PD5);

	/* Instruction */
	if ((r = spi_transfer(SPI1, COMMAND_WRITE)) < 0)
		return r;

	/* Address (high byte) */
	if ((spi_transfer(SPI1, addr >> 8)) < 0)
		return r;

	/* Address (low byte) */
	if ((r = spi_transfer(SPI1, addr & 0xff)) < 0)
		return r;

	/* Data */
	for (i = 0; i < PAGE_SIZE; i++) {
		if ((r = spi_transfer(SPI1, *data++)) < 0)
			return r;
	}

	/* 'nCS' High */
	gpio_set(GPIO_PD5);

	return 0;
}

/* Read */
static int rom_read(u16 addr, u8 *data, int nbyte)
{
	int r;
	int i;

	/* 'nCS' Low */
	gpio_clear(GPIO_PD5);

	/* Instruction */
	if ((r = spi_transfer(SPI1, COMMAND_READ)) < 0)
		return r;

	/* Address (high byte) */
	if ((r = spi_transfer(SPI1, addr >> 8)) < 0)
		return r;

	/* Address (low byte) */
	if ((r = spi_transfer(SPI1, addr & 0xff)) < 0)
		return r;

	/* Data */
	for (i = 0; i < nbyte; i++) {
		if ((r = spi_transfer(SPI1, 0)) < 0)
			return r;
		*data++ = r;
	}

	/* 'nCS' High */
	gpio_set(GPIO_PD5);

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
	tim_setup();
	spi_setup();

	/* Write data. */

	addr = 0;
	data = 0x55;
	/* Byte */
	for (i = 0; i < PAGE_SIZE; i++) {
		if (rom_write_byte(addr++, data++) < 0)
			goto spi_error;
	}

	/* Wait for internal write cycle time */
	delay_ms(TWC);

	/* Page */
	for (j = 0; j < ROM_SIZE / PAGE_SIZE - 1; j++) {
		/* Setup data. */
		for (i = 0; i < PAGE_SIZE; i++)
			buf[i] = data++;

		/* Page Write */
		if (rom_write_page(addr, buf) < 0)
			goto spi_error;

		/* Increment address. */
		addr += PAGE_SIZE;

		/* Wait for internal write cycle time */
		delay_ms(TWC);
	}

	/* Read and check data. */
	while (1) {
		for (n = 1; n <= (int)sizeof(buf); n++) {
			/* LED(PE10) on/off */
			gpio_toggle(GPIO_PE10);

			addr = 0;
			data = 0x55;
			for (j = 0; j < ROM_SIZE / n; j++) {
				/* Read data. */
				if (rom_read(addr, buf, n) < 0)
					goto spi_error;

				/* Check data. */
				for (i = 0; i < n; i++) {
					if (buf[i] != data)
						goto spi_error;
					data++;
				}

				/* Increment address. */
				addr += n;
			}

			/* remainder */
			m = ROM_SIZE % n;
			if (m) {
				/* Read data. */
				if (rom_read(addr, buf, m) < 0)
					goto spi_error;

				/* Check data. */
				for (i = 0; i < m; i++) {
					if (buf[i] != data)
						goto spi_error;
					data++;
				}
			}
		}
	}

spi_error:
	while (1) {
		/* LED(PE11) on/off */
		gpio_toggle(GPIO_PE11);

		/* Wait a bit. */
		delay_ms(500);
	}

	return 0;
}
