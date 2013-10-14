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
#include <rtc.h>
#include <iwdg.h>

#define TIMX_CLK_APB1 32000000

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

	/* Enable internal low-speed oscillator (30 - 60 kHz (37 kHz)). */
	rcc_enable_osc(RCC_LSI);
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

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM6);
	tim_enable_one_pulse_mode(TIM7);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM6);
	tim_disable_update_interrupt_on_any(TIM7);

	/* Load prescaler value (2MHz). */
	tim_load_prescaler_value(TIM6, TIMX_CLK_APB1 / 2000000 - 1);

	/* Load prescaler value (2kHz). */
	tim_load_prescaler_value(TIM7, TIMX_CLK_APB1 / 2000 - 1);
}

/* 1 - 32767 usec */
static void delay_us(u16 us)
{
	/* Set auto-reload value (us * 2). */
	tim_set_autoreload_value(TIM6, (us << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM6);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM6, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM6, TIM_UPDATE);
}

/* 1 - 32767 msec */
static void delay_ms(u16 ms)
{
	/* Set auto-reload value (ms * 2). */
	tim_set_autoreload_value(TIM7, (ms << 1) - 1);

	/* Enable counter. */
	tim_enable_counter(TIM7);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM7, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM7, TIM_UPDATE);
}

static void blink_led(int n)
{
	int i;

	for (i = 0; i < n; i++) {
		/* LED(PE11) on */
		gpio_clear(GPIO_PE11);

		/* Wait a bit */
		delay_ms(250);

		/* LED(PE11) off */
		gpio_set(GPIO_PE11);

		/* Wait a bit */
		delay_ms(250);
	}

	/* Wait a bit */
	delay_ms(250);
}


int main(void)
{
	int i;
	int j;

	clock_setup();
	gpio_setup();
	tim_setup();

	/* Enable write access to RTC backup register */
	pwr_disable_backup_write_protection();

	/* Check reset flags. */
	if (rcc_get_reset_flag(RCC_IWDG_RESET)) {
		/* Clear reset flags. */
		rcc_clear_reset_flag();

		/* BKP1R is state number. */
		if (RTC_BKP1R != 1)
			goto iwdg_error;

		/* BKP0R is test number. */
		if (RTC_BKP0R >= 3)
			RTC_BKP0R = 0;
		else
			RTC_BKP0R++;
	} else {
		/* Not IWDG reset (Power-on reset) */
		RTC_BKP0R = 0;
	}

	/* TEST 0 */
	if (RTC_BKP0R == 0) {
		/* STATE 0 */
		RTC_BKP1R = 0;

		/* LED(PE10) on */
		gpio_clear(GPIO_PE10);

		blink_led(1);

		/* Start IWDG (default: prescaler = 4, counter = 4095). */
		iwdg_start();

		for (i = 0; i < 5; i++) {
			/* 1 / LSI freq.(60kHz) * 4 * 4095 = 273 msec */
			delay_ms(270);

			/* Reset IWDG. */
			iwdg_reset();
		}

		/* STATE 1 */
		RTC_BKP1R = 1;

		/* LED(PE10) off */
		gpio_set(GPIO_PE10);

		/* 1 / LSI freq.(30kHz) * 4 * 4095 = 546 msec */
		delay_ms(550);

		/* STATE 2 */
		RTC_BKP1R = 2;

		/* LED(PE10) on */
		gpio_clear(GPIO_PE10);

		/* No IWDG reset */
		goto iwdg_error;
	}

	/* TEST 1 */
	if (RTC_BKP0R == 1) {
		/* STATE 0 */
		RTC_BKP1R = 0;

		/* LED(PE10) on */
		gpio_clear(GPIO_PE10);

		blink_led(2);

		/* prescaler = 4, counter = 1 */
		iwdg_set_timeout(4, 1);

		/* Start IWDG. */
		iwdg_start();

		/* Load RLR in the counter. */
		iwdg_reset();

		for (i = 0; i < 5; i++) {
			/* 1 / LSI freq.(60kHz) * 4 * 1 = 66.7 usec */
			delay_us(30);

			/* Reset IWDG. */
			iwdg_reset();
		}

		/* STATE 1 */
		RTC_BKP1R = 1;

		/* LED(PE10) off */
		gpio_set(GPIO_PE10);

		/* 1 / LSI freq.(30kHz) * 4 * 1 = 133 usec */
		/* with DELAY! */
		delay_us(300);

		/* STATE 2 */
		RTC_BKP1R = 2;

		/* LED(PE10) on */
		gpio_clear(GPIO_PE10);

		/* No IWDG reset. */
		goto iwdg_error;
	}

	/* TEST 2 */
	if (RTC_BKP0R == 2) {
		/* STATE 0 */
		RTC_BKP1R = 0;

		/* LED(PE10) on */
		gpio_clear(GPIO_PE10);

		blink_led(3);

		/* prescaler = 256, counter = 1 */
		iwdg_set_timeout(256, 1);

		/* Start IWDG. */
		iwdg_start();

		/* Load RLR in the counter. */
		iwdg_reset();

		for (i = 0; i < 5; i++) {
			/* 1 / LSI freq.(60kHz) * 256 * 1 = 4.27 msec */
			delay_ms(4);

			/* Reset IWDG. */
			iwdg_reset();
		}

		/* STATE 1 */
		RTC_BKP1R = 1;

		/* LED(PE10) off */
		gpio_set(GPIO_PE10);

		/* 1 / LSI freq.(30kHz) * 256 * 1 = 8.53 msec */
		delay_ms(9);

		/* STATE 2 */
		RTC_BKP1R = 2;

		/* LED(PE10) on */
		gpio_clear(GPIO_PE10);

		/* No IWDG reset. */
		goto iwdg_error;
	}

	/* TEST 3 */
	if (RTC_BKP0R == 3) {
		/* STATE 0 */
		RTC_BKP1R = 0;

		/* LED(PE10) on */
		gpio_clear(GPIO_PE10);

		blink_led(4);

		/* prescaler = 256, counter = 4095 */
		iwdg_set_timeout(256, 4095);

		/* Start IWDG. */
		iwdg_start();

		/* Load RLR in the counter. */
		iwdg_reset();

		for (i = 0; i < 5; i++) {
			/* 1 / LSI freq.(60kHz) * 256 * 4095 = 17.5 sec */
			for (j = 0; j < 34; j++) {
				delay_ms(500);

				/* LED(PE10) on/off */
				gpio_toggle(GPIO_PE10);
			}

			/* Reset IWDG. */
			iwdg_reset();
		}

		/* STATE 1 */
		RTC_BKP1R = 1;

		/* LED(PE10) off */
		gpio_set(GPIO_PE10);

		/* 1 / LSI freq.(30kHz) * 256 * 4095 = 34.9 sec */
		for (j = 0; j < 70; j++) {
			delay_ms(500);

			/* LED(PE10) on/off */
			gpio_toggle(GPIO_PE10);
		}

		/* STATE 2 */
		RTC_BKP1R = 2;

		/* LED(PE10) on */
		gpio_clear(GPIO_PE10);

		/* No IWDG reset. */
		goto iwdg_error;
	}

iwdg_error:
	while (1) {
		/* LED(PE11) on/off */
		gpio_toggle(GPIO_PE11);

		/* Wait a bit. */
		delay_ms(500);
	}

	return 0;
}
