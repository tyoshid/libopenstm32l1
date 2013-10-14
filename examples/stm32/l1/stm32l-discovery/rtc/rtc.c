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
#include <exti.h>
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

static void exti_setup(void)
{
	/* Enable RTC Wakeup interrupt. */
	nvic_enable_irq(NVIC_RTC_IRQ);

	/* Enable RTC Alarm interrupt. */
	nvic_enable_irq(NVIC_RTC_ALARM_IRQ);

	/* EXTI20 is connected to RTC Wakeup event. */
	/* EXTI17 is connected to RTC Alarm event. */
	exti_set_trigger(EXTI_RTC_WAKEUP | EXTI_RTC_ALARM, EXTI_RISING);

	/* Enable EXTI interrupt. */
	exti_enable_interrupt(EXTI_RTC_WAKEUP | EXTI_RTC_ALARM);
}

static void rtc_setup(void)
{
	/*
	 * Enable write access to RTC, RTC backup register and RCC CSR
	 * (LSEON, LSEBYP, RTCSEL and RTCEN)
	 */
	pwr_disable_backup_write_protection();

	/* Enable LSE. */
	rcc_enable_osc(RCC_LSE);

	/* RTCCLK = 32.768kHz */
	rcc_setup_rtc_lcd_clock(RCC_LSE, 0);

	/* Enable RTC clock. */
	rcc_enable_clock(RCC_RTC);

	/* Unlock write protection */
	rtc_unlock();

	/* Initialize RTC. */
	rtc_init(128 - 1, 256 - 1, 0x00132401, 0x00000000, false, 0);

	/* Set wakeup timer (1 sec). */
	rtc_enable_wakeup_timer(RTC_CK_SPRE, 1 - 1);

	/* Set alarm A (xx/xx xx:xx.10). */
	rtc_enable_alarm_a(0x80808010, (u32)(-1));

	/* Set alarm B (xx/xx xx:xx.11). */
	rtc_enable_alarm_b(0x80808011, (u32)(-1));

	/* Enable wakeup timer, alarm A and alarm B interrupt.*/
	rtc_enable_interrupt(RTC_WAKEUP_TIMER | RTC_ALARM_A | RTC_ALARM_B);

	/*
	 * Reactivate register write protection.
	 * except for RTC_ISR[13:8](TAMP1F, TSOVF, TSF, WUTF, ALRBF, ALRAF),
	 * RTC_TAFCR and RTC backup register
	 */
	rtc_lock();
}

void rtc_isr(void)
{
	if (exti_get_interrupt_mask(EXTI_RTC_WAKEUP) &&
	    exti_get_interrupt_status(EXTI_RTC_WAKEUP)) {
		if (rtc_get_interrupt_mask(RTC_WAKEUP_TIMER) &&
		    rtc_get_interrupt_status(RTC_WAKEUP_TIMER)) {
			/* LED(PB6) on/off */
			gpio_toggle(GPIO_PB6);

			/* Clear RTC wakeup timer interrupt. */
			rtc_clear_interrupt(RTC_WAKEUP_TIMER);
		}
		/* Clear EXTI20 interrupt. */
		exti_clear_interrupt(EXTI_RTC_WAKEUP);
	}
}

void rtc_alarm_isr(void)
{
	if (exti_get_interrupt_mask(EXTI_RTC_ALARM) &&
	    exti_get_interrupt_status(EXTI_RTC_ALARM)) {
		if (rtc_get_interrupt_mask(RTC_ALARM_A) &&
		    rtc_get_interrupt_status(RTC_ALARM_A)) {
			/* LED(PB7) on */
			gpio_set(GPIO_PB7);

			/* Clear RTC alarm A interrupt. */
			rtc_clear_interrupt(RTC_ALARM_A);
		}

		if (rtc_get_interrupt_mask(RTC_ALARM_B) &&
		    rtc_get_interrupt_status(RTC_ALARM_B)) {
			/* LED(PB7) off */
			gpio_clear(GPIO_PB7);

			/* Clear RTC alarm B interrupt. */
			rtc_clear_interrupt(RTC_ALARM_B);
		}

		/* Clear EXTI17 interrupt. */
		exti_clear_interrupt(EXTI_RTC_ALARM);
	}
}

int main(void)
{
	clock_setup();
	gpio_setup();
	exti_setup();
	rtc_setup();

	while (1)
		__asm__ ("nop");

	return 0;
}
