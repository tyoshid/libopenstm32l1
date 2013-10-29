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
#include <usart.h>
#include <dma.h>
#include <nvic.h>
#include <spi.h>

#include <syscall.h>
#include <stdio.h>

#define TIMX_CLK_APB1		32000000
#define PCLK2			32000000

/* Freescale MPL115A1 - Miniature SPI Digital Barometer */
#define START_CONVERSIONS	0x24
#define READ_PRESSURE_MSB	0x80
#define READ_PRESSURE_LSB	0x82
#define READ_TEMPERATURE_MSB	0x84
#define READ_TEMPERATURE_LSB	0x86
#define READ_A0_MSB		0x88
#define READ_A0_LSB		0x8a
#define READ_B1_MSB		0x8c
#define READ_B1_LSB		0x8e
#define READ_B2_MSB		0x90
#define READ_B2_LSB		0x92
#define READ_C12_MSB		0x94
#define READ_C12_LSB		0x96
#define TC			3	/* Conversion Time: 3 msec */
#define TW			5	/* Wakeup Time: 5 msec */

volatile bool spi_busy;
volatile u32 spi_status;

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

static void usart_setup(void)
{
	/* Enable GPIOA clock. */
	rcc_enable_clock(RCC_GPIOA);

	/* Enable USART1 clock. */
	rcc_enable_clock(RCC_USART1);

	/* Setup GPIO pin PA9 as alternate function. */
	gpio_config_altfn(GPIO_USART1_3, GPIO_PUSHPULL, GPIO_10MHZ, GPIO_NOPUPD,
			  GPIO_PA_USART1_TX);

	/* Setup USART1. */
	usart_init(USART1, PCLK2, 115200, 8, USART_STOP_1,
		   USART_PARITY_NONE, USART_FLOW_NONE, USART_TX);
}

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(USART1, ptr[i]);
		return i;
	}

	errno = EIO;
	return -1;
}

static void dma_setup(void)
{
	/* Enable DMA1 clock. */
	rcc_enable_clock(DMA_RCC_SPI1);

	/* Enable DMA SPI1 RX(DMA1 channel2) interrupt. */
	nvic_enable_irq(DMA_SPI1_RX_IRQ);

	/* Enable DMA SPI TX(DMA1 channel3) interrupt. */
	nvic_enable_irq(DMA_SPI1_TX_IRQ);
}

/* SPI1 RX (DMA1 Channel2) interrupt */
void dma_spi1_rx_isr(void)
{
	if (dma_get_interrupt_mask(DMA_SPI1_RX, DMA_COMPLETE) &&
	    dma_get_interrupt_status(DMA_SPI1_RX, DMA_GLOBAL | DMA_COMPLETE) ==
	    (DMA_GLOBAL | DMA_COMPLETE)) {
		/* Disable DMA. */
		dma_disable(DMA_SPI1_RX);

		/* Clear flag. */
		spi_busy = false;

		/* Clear interrupt. */
		dma_clear_interrupt(DMA_SPI1_RX, DMA_GLOBAL | DMA_COMPLETE);
	}
}

/* SPI1 TX (DMA1 Channel3) interrupt */
void dma_spi1_tx_isr(void)
{
	if (dma_get_interrupt_mask(DMA_SPI1_TX, DMA_COMPLETE) &&
	    dma_get_interrupt_status(DMA_SPI1_TX, DMA_GLOBAL | DMA_COMPLETE) ==
	    (DMA_GLOBAL | DMA_COMPLETE)) {
		/* Disable DMA. */
		dma_disable(DMA_SPI1_TX);

		/* Clear interrupt. */
		dma_clear_interrupt(DMA_SPI1_TX, DMA_GLOBAL | DMA_COMPLETE);
	}
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

void spi1_isr(void)
{
	u32 r;

	// if ((SPI1_CR2 & SPI_CR2_ERRIE) && ((r = SPI1_SR) & SPI_SR_ERROR)) {
	if (spi_get_interrupt_mask(SPI1, SPI_ERROR) &&
	    (r = spi_get_interrupt_status(SPI1, SPI_ERROR))) {
		/* Disable DMA. */
		dma_disable(DMA_SPI1_RX);
		dma_disable(DMA_SPI1_TX);

		/* Save error status. */
		spi_status = r;

		/* Clear flag. */
		spi_busy = false;

		/* Disable interrupt. */
		// SPI1_CR2 &= ~SPI_CR2_ERRIE;
		spi_disable_interrupt(SPI1, SPI_ERROR);
	}
}

static int baro_transfer(const u32 *sndbuf, u32 *rcvbuf, int n)
{
	if (spi_status) {
		printf("ERROR: SPI_SR1 = 0x%04x\r\n", (unsigned int)spi_status);
		return -1;
	}

	/* Set flag. */
	spi_busy = true;

	/* Set RX DMA mode and enable DMA. */
	dma_setup_channel(DMA_SPI1_RX, (u32)rcvbuf, (u32)&SPI1_DR, n,
			  DMA_CCR_MSIZE_32BIT |
			  DMA_CCR_PSIZE_32BIT |
			  DMA_CCR_MINC |
			  DMA_CCR_TCIE |
			  DMA_CCR_EN);

	/* Set TX DMA mode and enable DMA. */
	dma_setup_channel(DMA_SPI1_TX, (u32)sndbuf, (u32)&SPI1_DR, n,
			  DMA_CCR_MSIZE_32BIT |
			  DMA_CCR_PSIZE_32BIT |
			  DMA_CCR_MINC |
			  DMA_CCR_DIR |
			  DMA_CCR_TCIE |
			  DMA_CCR_EN);

	/* 'nCS' Low */
	gpio_clear(GPIO_PD5);

	/* Enable DMA. */
	// SPI1_CR2 |= (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
	spi_enable_dma(SPI1, SPI_DMA_TX_RX);

	/* Wait until the completion of the transfer. */
	while (spi_busy)
		;

	/* 'nCS' High */
	gpio_set(GPIO_PD5);

	/* Disable DMA. */
	// SPI1_CR2 &= ~(SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
	spi_disable_dma(SPI1, SPI_DMA_TX_RX);

	if (spi_status) {
		printf("ERROR: SPI_SR1 = 0x%04x\r\n", (unsigned int)spi_status);
		return -1;
	}

	return 0;
}

/* Calculate the compensated pressure PComp value */
static s16 calc_pcomp(u16 padc, u16 tadc, s16 a0, s16 b1, s16 b2, s16 c12)
{
	s32 c12x2;
	s32 a1;
	s32 a1x1;
	s32 y1;
	s32 a2x2;
	s32 pcomp;

	/* PComp = a0 + (b1 + c12 * Tadc) * Padc + b2 * Tadc */

	/*
	 * c12 Signed, Integer Bits = 0, Fractional Bits = 13,
	 * dec pt zero pad = 9
	 */
	/* c12x2 = c12 * Tadc */
	c12x2 = ((s32)c12 * tadc) >> 11;

	/* b1 Signed, Integer Bits = 2, Fractional Bits = 13 */
	/* a1 = b1 + c12x2 */
	a1 = (s32)b1 + c12x2;

	/* a1x1 = a1 * Padc */
	a1x1 = a1 * padc;

	/* a0 Signed, Integer Bits = 12, Fractional Bits = 3 */
	/* y1 = a0 + a1x1 */
	y1 = ((s32)a0 << 10) + a1x1;

	/* b2 Signed, Integer Bits = 1, Fractional Bits = 14 */
	/* a2x2 = b2 * Tadc */
	a2x2 = ((s32)b2 * tadc) >> 1;

	/* Pcomp = y1 + a2x2 */
	/* Fractional Bits = 4 */
	pcomp = (y1 + a2x2) >> 9;

	return (s16)pcomp;
}

static void print_pressure(s32 pressure)
{
	int f;
	int i;
	static const u8 frac[16] = {0, 1, 1, 2, 3, 3, 4, 4,
				    5, 6, 6, 7, 8, 8, 9, 9};

	f = pressure & 0xf;
	i = pressure >> 4;
	printf("%d.%d kPa\r\n", i, frac[f]);
}

int main(void)
{
	u32 buf[32];
	u16 padc;
	u16 tadc;
	s16 a0;
	s16 b1;
	s16 b2;
	s16 c12;
	s16 pcomp;
	s32 pressure;
	static const u32 coeff[] = {
		READ_A0_MSB, 0,
		READ_A0_LSB, 0,
		READ_B1_MSB, 0,
		READ_B1_LSB, 0,
		READ_B2_MSB, 0,
		READ_B2_LSB, 0,
		READ_C12_MSB, 0,
		READ_C12_LSB, 0,
		0
	};
	static const u32 conv[] = {
		START_CONVERSIONS,
		0
	};
	static const u32 press[] = {
		READ_PRESSURE_MSB, 0,
		READ_PRESSURE_LSB, 0,
		READ_TEMPERATURE_MSB, 0,
		READ_TEMPERATURE_LSB, 0,
		0
	};

	clock_setup();
	gpio_setup();
	tim_setup();
	usart_setup();
	dma_setup();
	spi_setup();

	/* Wait for Wakeup Time */
	delay_ms(TW);

	/* Enable interrupt. */
	//SPI1_CR2 |= SPI_CR2_ERRIE;
	spi_enable_interrupt(SPI1, SPI_ERROR);

	/* Read Coefficients. */
	if (baro_transfer(coeff, buf, sizeof(coeff) / sizeof(u32)) < 0)
		goto spi_error;

	// printf("a0=0x%02x%02x b1=0x%02x%02x b2=0x%02x%02x "
	//        "c12=0x%02x%02x\r\n", (unsigned int)buf[1],
	//        (unsigned int)buf[3], (unsigned int)buf[5],
	//        (unsigned int)buf[7], (unsigned int)buf[9],
	//        (unsigned int)buf[11], (unsigned int)buf[13],
	//        (unsigned int)buf[15]);

	a0 = (buf[1] << 8) | buf[3];
	b1 = (buf[5] << 8) | buf[7];
	b2 = (buf[9] << 8) | buf[11];
	c12 = (buf[13] << 8) | buf[15];

	while (1) {
		/* LED(PE10) on/off */
		gpio_toggle(GPIO_PE10);

		/* Start Pressure and Temperature Conversion. */
		if (baro_transfer(conv, buf, sizeof(conv) / sizeof(u32)) < 0)
			goto spi_error;

		/* Wait for Conversion Time. */
		delay_ms(TC);

		/* Read raw Pressure. */
		if (baro_transfer(press, buf, sizeof(press) / sizeof(u32)) < 0)
			goto spi_error;

		padc = (buf[1] << 8) | buf[3];
		padc >>= 6;
		tadc = (buf[5] << 8) | buf[7];
		tadc >>= 6;

		// printf("padc=%d tadc=%d\r\n", padc, tadc);

		/* compensated pressure */
		pcomp = calc_pcomp(padc, tadc, a0, b1, b2, c12);

		/* Pressure = PComp * ((115.0 - 50.0) / 1023.0) + 50 */
		/* Pressure = ((PComp * 1041) >> 14) + 50 */
		/* Fractional Bits = 4 */
		pressure = (((s32)pcomp * 1041) >> 14) + 800;

		/* Print the result. */
		print_pressure(pressure);

		/* Wait 1 sec. */
		delay_ms(1000);
	}

spi_error:
	while (1) {
		/* LED(PE11) on/off */
		gpio_toggle(GPIO_PE11);

		/* Wait 500 msec. */
		delay_ms(500);
	}

	return 0;
}
