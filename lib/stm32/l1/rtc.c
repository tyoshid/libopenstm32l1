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

#include <stm32/l1/rtc.h>

void rtc_unlock(void)
{
	RTC_WPR = 0xca;
	RTC_WPR = 0x53;
}

void rtc_lock(void)
{
	RTC_WPR = 0;
}

void rtc_init(int prediv_a, int prediv_s, u32 date, u32 time, bool fmt, u32 dc)
{
	RTC_ISR |= RTC_ISR_INIT;
	while (!(RTC_ISR & RTC_ISR_INITF))
		;
	RTC_PRER = prediv_s;
	RTC_PRER = (prediv_a << 16) | prediv_s;
	RTC_TR = time;
	RTC_DR = date;
	if (fmt)
		RTC_CR |= RTC_CR_FMT;
	else
		RTC_CR &= ~RTC_CR_FMT;
	RTC_CALIBR = dc;
	RTC_ISR &= ~RTC_ISR_INIT;
	while (RTC_ISR & RTC_ISR_INITF)
		;
}

void rtc_enable_daylight_saving_time(void)
{
	RTC_CR |= (RTC_CR_ADD1H | RTC_CR_BKP);
}

void rtc_disable_daylight_saving_time(void)
{
	u32 reg32;

	reg32 = RTC_CR;
	reg32 &= ~RTC_CR_BKP;
	RTC_CR = (reg32 | RTC_CR_SUB1H);
}

void rtc_enable_alarm_a(u32 time, u32 ss)
{
	u32 reg32;

	reg32 = RTC_CR;
	if (reg32 & RTC_CR_ALRAE)
		RTC_CR = (reg32 & ~RTC_CR_ALRAE);
	while (!(RTC_ISR & RTC_ISR_ALRAWF))
		;
	RTC_ALRMAR = time;
	if (ss != (u32)(-1))
		RTC_ALRMASSR = ss;
	RTC_CR = (reg32 | RTC_CR_ALRAE);
	while (RTC_ISR & RTC_ISR_ALRAWF)
		;
}

void rtc_enable_alarm_b(u32 time, u32 ss)
{
	u32 reg32;

	reg32 = RTC_CR;
	if (reg32 & RTC_CR_ALRBE)
		RTC_CR = (reg32 & ~RTC_CR_ALRBE);
	while (!(RTC_ISR & RTC_ISR_ALRBWF))
		;
	RTC_ALRMBR = time;
	if (ss != (u32)(-1))
		RTC_ALRMBSSR = ss;
	RTC_CR = (reg32 | RTC_CR_ALRBE);
	while (RTC_ISR & RTC_ISR_ALRBWF)
		;
}

void rtc_enable_wakeup_timer(rtc_wakeup_clock_t wucksel, int autoreload)
{
	u32 reg32;

	reg32 = RTC_CR;
	if (reg32 & RTC_CR_WUTE)
		RTC_CR = (reg32 & ~RTC_CR_WUTE);
	while (!(RTC_ISR & RTC_ISR_WUTWF))
		;
	RTC_WUTR = autoreload;
	reg32 &= ~(RTC_CR_WUCKSEL2 | RTC_CR_WUCKSEL1 | RTC_CR_WUCKSEL0);
	RTC_CR = (reg32 | wucksel | RTC_CR_WUTE);
	while (RTC_ISR & RTC_ISR_WUTWF)
		;
}

void rtc_get_calendar(u32 *date, u32 *time, u32 *ss)
{
	do {
		do {
			RTC_ISR &= ~RTC_ISR_RSF;
		} while (RTC_ISR & RTC_ISR_RSF);

		while (!(RTC_ISR & RTC_ISR_RSF))
			;

		RTC_ISR &= ~RTC_ISR_RSF;
		if (ss)
			*ss = RTC_SSR;
		*time = RTC_TR;
		*date = RTC_DR;
	} while (RTC_ISR & RTC_ISR_RSF);
}

void rtc_get_calendar_read_twice(u32 *date, u32 *time, u32 *ss)
{
	u32 ss1;
	u32 time1;
	u32 date1;
	u32 ss2;
	u32 time2;
	u32 date2;

	ss1 = ss2 = 0;
	do {
		if (ss)
			ss1 = RTC_SSR;
		time1 = RTC_TR;
		date1 = RTC_DR;
		if (ss)
			ss2 = RTC_SSR;
		time2 = RTC_TR;
		date2 = RTC_DR;
	} while ((ss && ss1 != ss2) || time1 != time2 || date1 != date2);
	if (ss)
		*ss = ss1;
	*time = time1;
	*date = date1;
}

void rtc_syncronize(u32 shift)
{
	RTC_SHIFTR = shift;
}

void rtc_enable_reference_clock_detection(void)
{
	RTC_CR |= RTC_CR_REFCKON;
}

void rtc_disable_reference_clock_detection(void)
{
	RTC_CR &= ~RTC_CR_REFCKON;
}

void rtc_set_smooth_digital_calibration(u32 cal)
{
	RTC_CALR = cal;
}

void rtc_get_timestamp(u32 *date, u32 *time, u32 *ss)
{
	if (ss)
		*ss = RTC_TSSSR;
	*time = RTC_TSTR;
	*date = RTC_TSDR;
}

void rtc_enable_interrupt(int interrupt)
{
	u32 cr = 0;
	u32 tafcr = 0;

	if (interrupt & RTC_ALARM_A)
		cr |= RTC_CR_ALRAIE;
	if (interrupt & RTC_ALARM_B)
		cr |= RTC_CR_ALRBIE;
	if (interrupt & RTC_WAKEUP_TIMER)
		cr |= RTC_CR_WUTIE;
	if (interrupt & RTC_TIMESTAMP)
		cr |= RTC_CR_TSIE;
	if (interrupt & (RTC_TAMPER1 | RTC_TAMPER2 | RTC_TAMPER3))
		tafcr |= RTC_TAFCR_TAMPIE;

	if (cr)
		RTC_CR |= cr;
	if (tafcr)
		RTC_TAFCR |= tafcr;
}

void rtc_disable_interrupt(int interrupt)
{
	u32 cr = 0;
	u32 tafcr = 0;

	if (interrupt & RTC_ALARM_A)
		cr |= RTC_CR_ALRAIE;
	if (interrupt & RTC_ALARM_B)
		cr |= RTC_CR_ALRBIE;
	if (interrupt & RTC_WAKEUP_TIMER)
		cr |= RTC_CR_WUTIE;
	if (interrupt & RTC_TIMESTAMP)
		cr |= RTC_CR_TSIE;
	if (interrupt & (RTC_TAMPER1 | RTC_TAMPER2 | RTC_TAMPER3))
		tafcr |= RTC_TAFCR_TAMPIE;

	if (cr)
		RTC_CR &= ~cr;
	if (tafcr)
		RTC_TAFCR &= ~tafcr;
}

int rtc_get_interrupt_mask(int interrupt)
{
	u32 reg32;
	u32 r = 0;

	if (interrupt & (RTC_ALARM_A | RTC_ALARM_B | RTC_WAKEUP_TIMER |
			 RTC_TIMESTAMP)) {
		reg32 = RTC_CR;
		if (reg32 & RTC_CR_ALRAIE)
			r |= (interrupt & RTC_ALARM_A);
		if (reg32 & RTC_CR_ALRBIE)
			r |= (interrupt & RTC_ALARM_B);
		if (reg32 & RTC_CR_WUTIE)
			r |= (interrupt & RTC_WAKEUP_TIMER);
		if (reg32 & RTC_CR_TSIE)
			r |= (interrupt & RTC_TIMESTAMP);
	}
	if (interrupt & (RTC_TAMPER1 | RTC_TAMPER2 | RTC_TAMPER3)) {
		reg32 = RTC_TAFCR;
		if (reg32 & RTC_TAFCR_TAMPIE) {
			if (reg32 & RTC_TAFCR_TAMP1E)
				r |= (interrupt & RTC_TAMPER1);
			if (reg32 & RTC_TAFCR_TAMP2E)
				r |= (interrupt & RTC_TAMPER2);
			if (reg32 & RTC_TAFCR_TAMP3E)
				r |= (interrupt & RTC_TAMPER3);
		}
	}
	return r;
}

int rtc_get_interrupt_status(int interrupt)
{
	return RTC_ISR & interrupt;
}

void rtc_clear_interrupt(int interrupt)
{
	RTC_ISR &= ~interrupt;
}

/* RTC_AF1 */
void rtc_set_afo_alarm(rtc_alarm_output_t osel, bool low_level, bool pushpull)
{
	u32 reg32;

	reg32 = RTC_CR;
	if (low_level) {
		reg32 &= ~(RTC_CR_OSEL1 | RTC_CR_OSEL0);
		reg32 |= (osel | RTC_CR_POL);
	} else {
		reg32 &= ~(RTC_CR_OSEL1 | RTC_CR_OSEL0 | RTC_CR_POL);
		reg32 |= osel;
	}
	RTC_CR = reg32;

	if (pushpull)
		RTC_TAFCR |= RTC_TAFCR_ALARMOUTTYPE;
	else
		RTC_TAFCR &= ~RTC_TAFCR_ALARMOUTTYPE;
}

void rtc_enable_afo_calib(rtc_calibration_output_t cosel)
{
	u32 reg32;

	reg32 = RTC_CR;
	if (cosel == RTC_512HZ)
		reg32 &= ~RTC_CR_COSEL;
	else if (cosel == RTC_1HZ)
		reg32 |= RTC_CR_COSEL;
	reg32 |= RTC_CR_COE;
	RTC_CR = reg32;
}

void rtc_disable_afo_calib(void)
{
	RTC_CR &= ~RTC_CR_COE;
}

void rtc_enable_timestamp(bool falling)
{
	u32 reg32;

	reg32 = RTC_CR;
	reg32 &= ~(RTC_CR_TSE | RTC_CR_TSEDGE);
	if (falling)
		reg32 |= RTC_CR_TSEDGE;
	RTC_CR = reg32;
	RTC_CR = (reg32 | RTC_CR_TSE);
}

void rtc_disable_timestamp(void)
{
	RTC_CR &= ~RTC_CR_TSE;
}

void rtc_set_tamper(u32 tamper)
{
	u32 reg32;

	reg32 = RTC_TAFCR;
	reg32 &= ~0xffff;
	reg32 |= (tamper & 0xffff);
	RTC_TAFCR = reg32;
}
