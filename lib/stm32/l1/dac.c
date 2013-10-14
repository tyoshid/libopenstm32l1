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

#include <stm32/l1/dac.h>

void dac_set_mode(int mode)
{
	DAC_CR = mode;
}

void dac_enable(dac_channel_t channel)
{
	u32 enable = 0;

	if (channel == DAC_CH1 || channel == DAC_DUAL)
		enable |= DAC_CR_EN1;
	if (channel == DAC_CH2 || channel == DAC_DUAL)
		enable |= DAC_CR_EN2;
	DAC_CR |= enable;
}

void dac_disable(dac_channel_t channel)
{
	u32 enable = 0;

	if (channel == DAC_CH1 || channel == DAC_DUAL)
		enable |= DAC_CR_EN1;
	if (channel == DAC_CH2 || channel == DAC_DUAL)
		enable |= DAC_CR_EN2;
	DAC_CR &= ~enable;
}

void dac_set_data(dac_format_t format, dac_channel_t channel, u32 data)
{
	switch (format) {
	case DAC_8R:
		switch (channel) {
		case DAC_CH1:
			DAC_DHR8R1 = data;
			break;
		case DAC_CH2:
			DAC_DHR8R2 = data;
			break;
		case DAC_DUAL:
			DAC_DHR8RD = data;
			break;
		default:
			break;
		}
		break;
	case DAC_12L:
		switch (channel) {
		case DAC_CH1:
			DAC_DHR12L1 = data;
			break;
		case DAC_CH2:
			DAC_DHR12L2 = data;
			break;
		case DAC_DUAL:
			DAC_DHR12LD = data;
			break;
		default:
			break;
		}
		break;
	case DAC_12R:
		switch (channel) {
		case DAC_CH1:
			DAC_DHR12R1 = data;
			break;
		case DAC_CH2:
			DAC_DHR12R2 = data;
			break;
		case DAC_DUAL:
			DAC_DHR12RD = data;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void dac_set_software_trigger(dac_channel_t channel)
{
	u32 trigger = 0;

	if (channel == DAC_CH1 || channel == DAC_DUAL)
		trigger |= DAC_SWTRIGR_SWTRIG1;
	if (channel == DAC_CH2 || channel == DAC_DUAL)
		trigger |= DAC_SWTRIGR_SWTRIG2;
	DAC_SWTRIGR = trigger;
}

void dac_enable_interrupt(int interrupt)
{
	DAC_CR |= interrupt;
}

void dac_disable_interrupt(int interrupt)
{
	DAC_CR &= ~interrupt;
}

int dac_get_interrupt_mask(int interrupt)
{
	return DAC_CR & interrupt;
}

int dac_get_interrupt_status(int interrupt)
{
	return DAC_SR & interrupt;
}

void dac_clear_interrupt(int interrupt)
{
	DAC_SR = interrupt;
}
