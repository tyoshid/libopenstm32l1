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
#include <nvic.h>
#include <wwdg.h>
#include <rtc.h>

/* Set STM32 to 16 MHz. */
static void clock_setup(void)
{
	/* Enable PWR clock. */
	rcc_enable_clock(RCC_PWR);

	/* Set VCORE to 1.8V */
	pwr_set_vos(PWR_1_8_V);

	/* Set Flash memory latency (0WS). */
	flash_enable_64bit_access(0);

	/* Enable internal high-speed oscillator (16MHz). */
	rcc_enable_osc(RCC_HSI);

	/* AHB, APB1 and APB2 prescaler value is default. */
	// rcc_set_prescaler(1, 1, 1);

	/* Select SYSCLK source. */
	rcc_set_sysclk_source(RCC_HSI);
}

static void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	rcc_enable_clock(RCC_GPIOB);

	/* Set GPIO6 and GPIO7 (in GPIO port B) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PB(6, 7));

	/* LED off */
	gpio_clear(GPIO_PB(6, 7));
}

static void tim_setup(void)
{
	/* Enable TIM6 clock. */
	rcc_enable_clock(RCC_TIM6);

	/* Enable one-pulse mode. */
	tim_enable_one_pulse_mode(TIM6);

	/* Generate update interrupt on counter overflow. */
	tim_disable_update_interrupt_on_any(TIM6);

	/* Load prescaler value. */
	tim_load_prescaler_value(TIM6, 4096 - 1);
}

static void delay_ck(u16 ck)
{
	/* Set auto-reload value. */
	tim_set_autoreload_value(TIM6, ck - 1);

	/* Enable counter. */
	tim_enable_counter(TIM6);

	/* Wait for update interrupt flag. */
	while (!tim_get_interrupt_status(TIM6, TIM_UPDATE))
		;

	/* Clear update interrupt flag. */
	tim_clear_interrupt(TIM6, TIM_UPDATE);
}

static void wwdg_setup(void)
{
	/* Enable WWDG clock. */
	rcc_enable_clock(RCC_WWDG);

	/* Enable WWDG interrupt. */
	nvic_enable_irq(NVIC_WWDG_IRQ);
}

void wwdg_isr(void)
{
	if (wwdg_get_interrupt_mask(WWDG_EARLY_WAKEUP) &&
	    wwdg_get_interrupt_status(WWDG_EARLY_WAKEUP)) {
		/* Set flag. */
		RTC_BKP2R = 0x55;

		/* Clear interrupt. */
		wwdg_clear_interrupt(WWDG_EARLY_WAKEUP);
	}
}

static void blink_led(int n)
{
	int i;

	for (i = 0; i < n; i++) {
		/* LED(PB7) on */
		gpio_set(GPIO_PB7);

		/* Wait a bit */
		delay_ck(977);	/* 250 msec */

		/* LED(PB7) off */
		gpio_clear(GPIO_PB7);

		/* Wait a bit */
		delay_ck(977);	/* 250 msec */
	}

	/* Wait a bit */
	delay_ck(977);	/* 250 msec */
}

int main(void)
{
	int i;

	clock_setup();
	gpio_setup();
	tim_setup();
	wwdg_setup();

	/* Enable write access to RTC backup register */
	pwr_disable_backup_write_protection();

	/* Check reset flags. */
	if (rcc_get_reset_flag(RCC_WWDG_RESET)) {
		/* Clear reset flags. */
		rcc_clear_reset_flag();

		/* BKP1R is state number. */
		if (RTC_BKP1R != 1)
			goto wwdg_error;

		/* BKP0R is test number. */
		if (RTC_BKP0R == 4) {
			/* Flag(BKP2R) is not set. */
			if (RTC_BKP2R != 0x55)
				goto wwdg_error;

			/* Clear flag. */
			RTC_BKP2R = 0;
		}

		/* next test */
		if (RTC_BKP0R >= 4)
			RTC_BKP0R = 0;
		else
			RTC_BKP0R++;
	} else {
		/* Not WWDG reset (Power-on reset) */
		RTC_BKP0R = 0;
		RTC_BKP2R = 0;
	}

	/* TEST 0 */
	if (RTC_BKP0R == 0) {
		/* STATE 0 */
		RTC_BKP1R = 0;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		blink_led(1);

		/* predcaler = 1, window value = 0x5f */
		/* tPCLK1 * 4096 * 1 * (0x7f - 0x5f) = 8.2 msec */
		wwdg_set_window(1, 0x5f);

		/* Enable WWDG (counter = 0x7f). */
		/* tPCLK1 * 4096 * 1 * 0x40 = 16.4 msec */
		wwdg_reset(0x7f);

		for (i = 0; i < 5; i++) {
			/* tPCLK1 * 4096 * 0x3e = 15.9 msec */
			delay_ck(0x3e);

			/* Reset WWDG. */
			wwdg_reset(0x7f);
		}

		/* STATE 1 */
		RTC_BKP1R = 1;

		/* LED(PB6) off */
		gpio_clear(GPIO_PB6);

		/* tPCLK1 * 4096 * 0x42 = 16.9 msec */
		delay_ck(0x42);

		/* STATE 2 */
		RTC_BKP1R = 2;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		/* No WWDG reset */
		goto wwdg_error;
	}

	/* TEST 1 */
	if (RTC_BKP0R == 1) {
		/* STATE 0 */
		RTC_BKP1R = 0;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		blink_led(2);

		/* predcaler = 1, window value = 0x5f */
		/* tPCLK1 * 4096 * 1 * (0x7f - 0x5f) = 8.2 msec */
		wwdg_set_window(1, 0x5f);

		/* Enable WWDG (counter = 0x7f). */
		/* tPCLK1 * 4096 * 1 * 0x40 = 16.4 msec */
		wwdg_reset(0x7f);

		for (i = 0; i < 5; i++) {
			/* tPCLK1 * 4096 * 0x22 = 8.7 msec */
			delay_ck(0x22);

			/* Reset WWDG. */
			wwdg_reset(0x7f);
		}

		/* STATE 1 */
		RTC_BKP1R = 1;

		/* LED(PB6) off */
		gpio_clear(GPIO_PB6);

		/* tPCLK1 * 4096 * 0x1e = 7.7 msec */
		delay_ck(0x1e);

		/* Reset WWDG. */
		wwdg_reset(0x7f);

		/* Wait a bit. */
		delay_ck(1);

		/* STATE 2 */
		RTC_BKP1R = 2;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		/* No WWDG reset */
		goto wwdg_error;
	}

	/* TEST 2 */
	if (RTC_BKP0R == 2) {
		/* STATE 0 */
		RTC_BKP1R = 0;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		blink_led(3);

		/* predcaler = 8, window value = 0x5f */
		/* tPCLK1 * 4096 * 8 * (0x7f - 0x5f) = 65.5 msec */
		wwdg_set_window(8, 0x5f);

		/* Enable WWDG (counter = 0x7f). */
		/* tPCLK1 * 4096 * 8 * 0x40 = 131.1 msec */
		wwdg_reset(0x7f);

		for (i = 0; i < 5; i++) {
			/* tPCLK1 * 4096 * 0x3e * 8 = 127.0 msec */
			delay_ck(0x3e << 3);

			/* Reset WWDG. */
			wwdg_reset(0x7f);
		}

		/* STATE 1 */
		RTC_BKP1R = 1;

		/* LED(PB6) off */
		gpio_clear(GPIO_PB6);

		/* tPCLK1 * 4096 * 0x42 * 8 = 135.2 msec */
		delay_ck(0x42 << 3);

		/* STATE 2 */
		RTC_BKP1R = 2;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		/* No WWDG reset */
		goto wwdg_error;
	}

	/* TEST 3 */
	if (RTC_BKP0R == 3) {
		/* STATE 0 */
		RTC_BKP1R = 0;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		blink_led(4);

		/* predcaler = 8, window value = 0x5f */
		/* tPCLK1 * 4096 * 8 * (0x7f - 0x5f) = 65.5 msec */
		wwdg_set_window(8, 0x5f);

		/* Enable WWDG (counter = 0x7f). */
		/* tPCLK1 * 4096 * 8 * 0x40 = 131.1 msec */
		wwdg_reset(0x7f);

		for (i = 0; i < 5; i++) {
			/* tPCLK1 * 4096 * 0x22 * 8 = 69.6 msec */
			delay_ck(0x22 << 3);

			/* Reset WWDG */
			wwdg_reset(0x7f);
		}

		/* STATE 1 */
		RTC_BKP1R = 1;

		/* LED(PB6) off */
		gpio_clear(GPIO_PB6);

		/* tPCLK1 * 4096 * 0x1e * 8 = 61.4 msec */
		delay_ck(0x1e << 3);

		/* Reset WWDG. */
		wwdg_reset(0x7f);

		/* Wait a bit. */
		delay_ck(1);

		/* STATE 2 */
		RTC_BKP1R = 2;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		/* No WWDG reset */
		goto wwdg_error;
	}

	/* TEST 4 */
	if (RTC_BKP0R == 4) {
		/* STATE 0 */
		RTC_BKP1R = 0;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		blink_led(5);

		/* Clear interrupt status. */
		wwdg_clear_interrupt(WWDG_EARLY_WAKEUP);

		/* Check flag. */
		if (RTC_BKP2R == 0x55)
			goto wwdg_error;

		/* Enable early wakeup interrupt. */
		/* predcaler = 1, window value = 0x5f */
		/* tPCLK1 * 4096 * 1 * (0x7f - 0x5f) = 8.2 msec */
		wwdg_set_window(1, 0x5f);
		wwdg_enable_interrupt(WWDG_EARLY_WAKEUP);

		/* Enable WWDG (counter = 0x7f). */
		/* tPCLK1 * 4096 * 1 * 0x40 = 16.4 msec */
		wwdg_reset(0x7f);

		for (i = 0; i < 5; i++) {
			/* tPCLK1 * 4096 * 0x3e = 15.9 msec */
			delay_ck(0x3e);

			/* Reset WWDG. */
			wwdg_reset(0x7f);
		}

		/* STATE 1 */
		RTC_BKP1R = 1;

		/* LED(PB6) off */
		gpio_clear(GPIO_PB6);

		/* tPCLK1 * 4096 * 0x42 = 16.9 msec */
		delay_ck(0x42);

		/* STATE 2 */
		RTC_BKP1R = 2;

		/* LED(PB6) on */
		gpio_set(GPIO_PB6);

		/* No WWDG reset */
		goto wwdg_error;
	}

wwdg_error:
	while (1) {
		/* LED(PB7) on/off */
		gpio_toggle(GPIO_PB7);

		/* Wait a bit. */
		delay_ck(1953);	/* 500 msec */
	}

	return 0;
}
