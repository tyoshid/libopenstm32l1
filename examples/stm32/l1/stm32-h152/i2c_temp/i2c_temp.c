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
#include <nvic.h>
#include <i2c.h>
#include <usart.h>
#include <tim.h>

#include <syscall.h>
#include <stdio.h>

/* Clock frequency */
#define SYSCLK		32000000
#define PCLK1		32000000
#define TIMX_CLK_APB1	32000000

/*
 * TI TMP102
 * Low Power Digital Temperature Sensor
 * With SMBus/Two-Wire Serial Interface in SOT563
 *
 * Digital Temperature Sensor Breakout - TMP102
 * SEN-09418
 * SparkFun Electronics.
 */
#define TMP102_SLA	0x90	/* ADD0 -> GND */
#define CONVERSION_TIME	35	/* MAX 35msec */

/* I2C timing */
#define FASTMODE	1

#ifdef FASTMODE
#define MODE		I2C_FAST
#define SCL		400000	 /* 400kHz */
#define T_R_MAX		10	 /* tr.max(300ns) * PCLK1 = 10  */
#define START_TIMEOUT	25	 /* 10 / SCL = 25us */
#else
#define MODE		I2C_STANDARD
#define SCL		88000	 /* 88kHz (Errata) */
#define T_R_MAX		32	 /* tr.max(1us) * PCLK1 = 32 */
#define START_TIMEOUT	114	 /* 10 / SCL = 114us */
#endif

#define BUSY_TIMEOUT	(SYSCLK / SCL)
#define TIMEOUT		(10 * (SYSCLK / SCL))

volatile int sndbuflen;
volatile u8 sndbuf[3];
volatile int sndindex;
volatile int rcvbuflen;
volatile u8 rcvbuf[2];
volatile int rcvindex;

volatile bool i2c_busy;
volatile bool i2c_status;
volatile bool i2c_timeout;

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
	/* Enable TIM6 and TIM7 clock. */
	rcc_enable_clock(RCC_TIM6);
	rcc_enable_clock(RCC_TIM7);

	/* Enable TIM6 interrupt. */
	nvic_enable_irq(NVIC_TIM6_IRQ);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM6);
	tim_enable_one_pulse_mode(TIM7);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM6);
	tim_disable_update_interrupt_on_any(TIM7);

	/* Enable TIM6 interrupt. */
	tim_enable_interrupt(TIM6, TIM_UPDATE);
}

/* start-timeout (1 - 32767 usec) */
static void start_timer(u16 us)
{
	/* Load prescaler value (2MHz). */
	/* Set auto-reload value (us * 2). */
	tim_setup_counter(TIM6, TIMX_CLK_APB1 / 2000000 - 1, (us << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM6);
}

/* 1 - 32767 msec */
static void delay_ms(u16 ms)
{
	/* Load prescaler value (2kHz). */
	/* Set auto-reload value (ms * 2). */
	tim_setup_counter(TIM7, TIMX_CLK_APB1 / 2000 - 1, (ms << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM7);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM7, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM7, TIM_UPDATE);
}

void tim6_isr(void)
{
	if (tim_get_interrupt_mask(TIM6, TIM_UPDATE) &&
	    tim_get_interrupt_status(TIM6, TIM_UPDATE)) {
		/* Disable I2C interrupt */
		i2c_disable_interrupt(I2C2, I2C_BUFFER | I2C_EVENT | I2C_ERROR);

		/* LED(PE10 and PE11) on */
		gpio_clear(GPIO_PE(10, 11));

		/* Error */
		i2c_timeout = true;
		i2c_busy = false;

		/* Clear interrupt */
		tim_clear_interrupt(TIM6, TIM_UPDATE);
	}
}

static void usart_setup(void)
{
	/* Enable GPIOD clock. */
	rcc_enable_clock(RCC_GPIOD);

	/* Enable USART2 clock. */
	rcc_enable_clock(RCC_USART2);

	/* Setup GPIO pin PD5 as aloternate function. */
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

static void i2c_setup(void)
{
	/* Enable GPIOB clock. */
	rcc_enable_clock(RCC_GPIOB);

	/* Enable I2C2 clock. */
	rcc_enable_clock(RCC_I2C2);

	/* Enable I2C2 event interrupt. */
	nvic_enable_irq(NVIC_I2C2_EV_IRQ);

	/* Enable I2C2 error interrupt. */
	nvic_enable_irq(NVIC_I2C2_ER_IRQ);

	/* Set GPIO10 and GPIO11 (in GPIO port B) to 'altfn open-drain'. */
	gpio_config_altfn(GPIO_I2C1_2, GPIO_OPENDRAIN, GPIO_2MHZ, GPIO_NOPUPD,
			  GPIO_PB(I2C2_SCL, I2C2_SDA));
}

/* I2C Event interrupt */
void i2c2_ev_isr(void)
{
	u32 e;

	if (i2c_get_interrupt_mask(I2C2, I2C_EVENT) &&
	    (e = i2c_get_interrupt_status(I2C2, I2C_TXE | I2C_RXNE |
					  I2C_BTF | I2C_ADDR | I2C_START))) {
		switch (e) {
		case I2C_START: /* EV5 */
			/* Send I2C slave address. */
			if (sndbuflen > 0)
				i2c_put_data(I2C2, TMP102_SLA);
			else
				i2c_put_data(I2C2, TMP102_SLA | I2C_READ);
			/* Stop timer. */
			tim_disable_counter(TIM6);
			break;

		case I2C_ADDR: /* EV6 */
		case (I2C_TXE | I2C_ADDR): /* EV8_1 */
			/* Enable Acknowledge. */
			if (sndbuflen <= 0 && rcvbuflen > 1)
				i2c_enable_action(I2C2, I2C_ACK);

			/* Clear ADDR bit. */
			i2c_get_status(I2C2, 0);

			/* Generate Stop condition. */
			if (sndbuflen <= 0 && rcvbuflen == 1)
				i2c_enable_action(I2C2, I2C_STOP);

			/* Enable buffer interrupt */
			i2c_enable_interrupt(I2C2, I2C_BUFFER);
			break;

		case I2C_TXE: /* EV8, EV8_1 */
			/* Send data. */
			if (sndbuflen > 0) {
				i2c_put_data(I2C2, sndbuf[sndindex++]);
				sndbuflen--;
			}
			if (sndbuflen == 0) {
				/* Disable buffer interrupt */
				i2c_disable_interrupt(I2C2, I2C_BUFFER);

				/* Repeated Start */
				if (rcvbuflen > 0)
					i2c_enable_action(I2C2, I2C_START);
			}
			break;

		case I2C_RXNE: /* EV7, EV7_1 */
			/* Receive data. */
			rcvbuf[rcvindex++] = i2c_get_data(I2C2);
			rcvbuflen--;

			if (rcvbuflen == 1) {
				/* second last byte */
				/* Enable Acknowledge. */
				i2c_disable_action(I2C2, I2C_ACK);

				/* Generate Stop condition. */
				i2c_enable_action(I2C2, I2C_STOP);
			} else if (rcvbuflen == 0) {
				/* last byte */
				/* Disable interrupt. */
				i2c_disable_interrupt(I2C2, I2C_BUFFER |
						      I2C_EVENT | I2C_ERROR);
				/* Complete transfer. */
				i2c_busy = false;
			}
			break;

		case (I2C_TXE | I2C_BTF): /* EV8_2 */
			if (rcvbuflen <= 0) {
				/* last byte */
				/* Generate Stop condition. */
				i2c_enable_action(I2C2, I2C_STOP);
				/* Disable interrupt. */
				i2c_disable_interrupt(I2C2, I2C_BUFFER |
						      I2C_EVENT | I2C_ERROR);
				/* Complete transfer. */
				i2c_busy = false;
			}
			break;

		default:	/* unknown event */
			/* Disable interrupt. */
			i2c_disable_interrupt(I2C2, I2C_BUFFER |
					      I2C_EVENT | I2C_ERROR);
			/* Stop timer. */
			tim_disable_counter(TIM6);

			/* LED(PE10) on */
			gpio_clear(GPIO_PE10);

			/* Error */
			i2c_status = true;
			i2c_busy = false;
			break;
		}
	}
}

/* I2C Error interrupt */
void i2c2_er_isr(void)
{
	if (i2c_get_interrupt_mask(I2C2, I2C_ERROR) &&
	    i2c_get_interrupt_status(I2C2, I2C_ERROR)) {
		/* Disable interrupt. */
		i2c_disable_interrupt(I2C2, I2C_BUFFER |
				      I2C_EVENT | I2C_ERROR);

		/* Stop timer. */
		tim_disable_counter(TIM6);

		/* LED(PE11) on */
		gpio_clear(GPIO_PE11);

		/* Error */
		i2c_status = true;
		i2c_busy = false;
	}
}

static int temp_transfer(void)
{
	int i;

	/* Wait until STOP bit is cleared. */
	for (i = 0; i < TIMEOUT && i2c_get_action_status(I2C2, I2C_STOP); i++)
		;
	if (i >= TIMEOUT) {
		printf("STOP Timeout: I2C_CR1 = 0x%04x\r\n",
		       (unsigned int)i2c_get_action_status(I2C2, 0xffff));
		return -1;
	}

	/* Set flag. */
	i2c_busy = true;

	/* Enable interrupt. */
	i2c_enable_interrupt(I2C2, I2C_EVENT | I2C_ERROR);

	/* Start timer. */
	start_timer(START_TIMEOUT);

	/* Generate Start condition. */
	i2c_enable_action(I2C2, I2C_START);

	/* Wait until the completion of the transfer. */
	while (i2c_busy)
		;

	if (i2c_status) {
		printf("ERROR: I2C_SR1 = 0x%04x\r\n",
		       (unsigned int)i2c_get_interrupt_status(I2C2, 0xffff));
		return -1;
	}

	if (i2c_timeout) {
		printf("Timeout: No Start condition\r\n");
		return -1;
	}

	return 0;
}

static int temp_write(u8 pointer, u8 *buf, int nbyte)
{
	int i;

	sndbuf[0] = pointer;
	for (i = 0; i < nbyte; i++)
		sndbuf[i + 1] = *buf++;
	sndbuflen = nbyte + 1;
	sndindex = 0;
	rcvbuflen = 0;
	rcvindex = 0;

	if (temp_transfer() < 0)
		return -1;

	return 0;
}

static int temp_read(u8 pointer, u8 *buf, int nbyte)
{
	int i;

	if (pointer < 4) {
		sndbuf[0] = pointer;
		sndbuflen = 1;
		sndindex = 0;
	} else {
		sndbuflen = 0;
		sndindex = 0;
	}
	rcvbuflen = nbyte;
	rcvindex = 0;

	if (temp_transfer() < 0)
		return -1;

	for (i = 0; i < nbyte; i++)
		*buf++ = rcvbuf[i];

	return 0;
}

static void temp_display(u8 *buf)
{
	s16 t;
	int i;
	int f;
	char sign;
	static const u8 frac[16] = {0, 1, 1, 2, 3, 3, 4, 4,
				    5, 6, 6, 7, 8, 8, 9, 9};

	t = (buf[0] << 8) | buf[1];
	t >>= 4;
	if (t < 0) {
		t = -t;
		sign = '-';
	} else {
		sign = ' ';
	}
	f = t & 0xf;
	i = t >> 4;
	printf("%c%d.%d degrees Celsius\r\n", sign, i, frac[f]);
}

int main(void)
{
	int i;
	u8 buf[2];

	clock_setup();
	gpio_setup();
	tim_setup();
	usart_setup();
	i2c_setup();

	/* Reset I2C. */
	if (i2c_get_status(I2C2, I2C_BUSY)) {
		i2c_enable_action(I2C2, I2C_RESET);
		for (i = 0; i < BUSY_TIMEOUT &&
			     i2c_get_status(I2C2, I2C_BUSY); i++)
			;
		i2c_disable_action(I2C2, I2C_RESET);
		if (i >= BUSY_TIMEOUT) {
			printf("BUSY Timeout: Bus busy\r\n");
			goto i2c_error;
		}
	}

	/* Set I2C clock frequency. */
	i2c_set_clock(I2C2, PCLK1, MODE, SCL, T_R_MAX);

	/* Enable I2C. */
	i2c_set_bus_mode(I2C2, I2C_ENABLE);

	/* Read Configuration Register. */
	if (temp_read(1, buf, 2) < 0)
		goto i2c_error;
	printf("Configuration Register = 0x%02x%02x\r\n", buf[0], buf[1]);

	/* Read TLOW Register. */
	if (temp_read(2, buf, 2) < 0)
		goto i2c_error;
	printf("TLOW Register = 0x%02x%02x ", buf[0], buf[1]);
	temp_display(buf);

	/* Read THIGH Register. */
	if (temp_read(3, buf, 2) < 0)
		goto i2c_error;
	printf("THIGH Register = 0x%02x%02x ", buf[0], buf[1]);
	temp_display(buf);

	/* Write THIGH Register. */
	buf[0] = 85;
	buf[1] = 0x80;
	if (temp_write(3, buf, 2) < 0)
		goto i2c_error;
	printf("THIGH Register <- 85.5\r\n");

	/* Read THIGH Register. */
	if (temp_read(3, buf, 2) < 0)
		goto i2c_error;
	printf("THIGH Register = 0x%02x%02x ", buf[0], buf[1]);
	temp_display(buf);

	/* Write Pointer Register. */
	if (temp_write(0, NULL, 0) < 0)
		goto i2c_error;

	/* Wait for Conversion Time */
	delay_ms(CONVERSION_TIME);

	/* Read Temperature Register (16bit). */
	if (temp_read(-1, buf, 2) < 0)
		goto i2c_error;
	printf("Temperature Register = 0x%02x%02x ", buf[0], buf[1]);
	temp_display(buf);

	/* Read Temperature Register (8bit). */
	buf[1] = 0;
	if (temp_read(-1, buf, 1) < 0)
		goto i2c_error;
	printf("Temperature Register(8bit) = 0x%02x ", buf[0]);
	temp_display(buf);
	printf("\r\n");

	while (1) {
		/* Read Temperature Register (16bit). */
		if (temp_read(-1, buf, 2) < 0)
			goto i2c_error;
		temp_display(buf);

		/* Wait 1 sec */
		delay_ms(1000);
	}

i2c_error:
	while (1)
		;

	return 0;
}
