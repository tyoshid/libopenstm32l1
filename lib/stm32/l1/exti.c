/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Mark Butler <mbutler@physics.otago.ac.nz>
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

#include <stm32/l1/exti.h>

void exti_set_trigger(int extis, exti_trigger_t trig)
{
	switch (trig) {
	case EXTI_TRIGGER_NONE:
		EXTI_RTSR &= ~extis;
		EXTI_FTSR &= ~extis;
		break;
	case EXTI_RISING:
		EXTI_FTSR &= ~extis;
		EXTI_RTSR |= extis;
		break;
	case EXTI_FALLING:
		EXTI_RTSR &= ~extis;
		EXTI_FTSR |= extis;
		break;
	case EXTI_BOTH:
		EXTI_RTSR |= extis;
		EXTI_FTSR |= extis;
		break;
	default:
		break;
	}
}

exti_trigger_t exti_get_trigger(int extis)
{
	u32 r;
	u32 f;

	r = EXTI_RTSR & extis;
	f = EXTI_FTSR & extis;
	if (r == 0 && f == 0)
		return EXTI_TRIGGER_NONE;
	else if (r == (u32)extis && f == 0)
		return EXTI_RISING;
	else if (r == 0 && f == (u32)extis)
		return EXTI_FALLING;
	else if (r == (u32)extis && f == (u32)extis)
		return EXTI_BOTH;
	else
		return EXTI_TRIGGER_UNKNOWN;
}

void exti_enable_interrupt(int extis)
{
	EXTI_IMR |= extis;
}

void exti_disable_interrupt(int extis)
{
	EXTI_IMR &= ~extis;
}

int exti_get_interrupt_mask(int extis)
{
	return EXTI_IMR & extis;
}

int exti_get_interrupt_status(int extis)
{
	return EXTI_PR & extis;
}

void exti_clear_interrupt(int extis)
{
	EXTI_PR = extis;
}

void exti_enable_event(int extis)
{
	EXTI_EMR |= extis;
}

void exti_disable_event(int extis)
{
	EXTI_EMR &= ~extis;
}

int exti_get_event_mask(int extis)
{
	return EXTI_EMR & extis;
}

void exti_set_software_interrupt(int extis)
{
	EXTI_SWIER = extis;
}
