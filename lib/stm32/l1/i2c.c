/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Thomas Otto <tommi@viadmin.org>
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

#include <stm32/l1/i2c.h>

static u32 base_addr(i2c_t i2c)
{
	switch (i2c) {
	case I2C1:
		return I2C1_BASE;
	case I2C2:
		return I2C2_BASE;
	default:
		break;
	}
	return 0;
}

void i2c_set_clock(i2c_t i2c, int apb1, i2c_mode_t mode, int scl, int t_r_max)
{
	u32 base;
	u32 reg32;

	base = base_addr(i2c);
	I2C_CR2(base) = apb1 / 1000000;
	switch (mode) {
	case I2C_STANDARD:
		reg32 = apb1 / scl / 2;
		if (apb1 % scl || apb1 / scl % 2)
			reg32++;
		break;
	case I2C_FAST:
		reg32 = apb1 / scl / 3;
		if (apb1 % scl || apb1 / scl % 3)
			reg32++;
		reg32 |= I2C_CCR_F_S;
		break;
	case I2C_FAST_DUTY:
		reg32 = apb1 / scl / 25;
		if (apb1 % scl || apb1 / scl % 25)
			reg32++;
		reg32 |= (I2C_CCR_F_S | I2C_CCR_DUTY);
		break;
	default:
		reg32 = 0;
		break;
	}
	I2C_CCR(base) = reg32;
	I2C_TRISE(base) = t_r_max + 1;
}

void i2c_set_bus_mode(i2c_t i2c, int mode)
{
	u32 reg32;

	reg32 = 0;
	if (mode & I2C_ENABLE)
		reg32 |= I2C_CR1_PE;
	if (mode & I2C_SMBUS)
		reg32 |= I2C_CR1_SMBUS;
	if (mode & I2C_SMBUS_HOST)
		reg32 |= I2C_CR1_SMBTYPE;
	if (mode & I2C_ARP)
		reg32 |= I2C_CR1_ENARP;
	if (mode & I2C_PEC)
		reg32 |= I2C_CR1_ENPEC;
	if (mode & I2C_GENCALL)
		reg32 |= I2C_CR1_ENGC;
	if (mode & I2C_NOSTRETCH)
		reg32 |= I2C_CR1_NOSTRETCH;
	I2C_CR1(base_addr(i2c)) = reg32;
}

void i2c_enable_action(i2c_t i2c, int action)
{
	u32 reg32;

	reg32 = 0;
	if (action & I2C_RESET)
		reg32 |= I2C_CR1_SWRST;
	if (action & I2C_SMBALERT)
		reg32 |= I2C_CR1_ALERT;
	if (action & I2C_PEC)
		reg32 |= I2C_CR1_PEC;
	if (action & I2C_POS)
		reg32 |= I2C_CR1_POS;
	if (action & I2C_ACK)
		reg32 |= I2C_CR1_ACK;
	if (action & I2C_STOP)
		reg32 |= I2C_CR1_STOP;
	if (action & I2C_START)
		reg32 |= I2C_CR1_START;

	I2C_CR1(base_addr(i2c)) |= reg32;
}

void i2c_disable_action(i2c_t i2c, int action)
{
	u32 reg32;

	reg32 = 0;
	if (action & I2C_RESET)
		reg32 |= I2C_CR1_SWRST;
	if (action & I2C_SMBALERT)
		reg32 |= I2C_CR1_ALERT;
	if (action & I2C_PEC)
		reg32 |= I2C_CR1_PEC;
	if (action & I2C_POS)
		reg32 |= I2C_CR1_POS;
	if (action & I2C_ACK)
		reg32 |= I2C_CR1_ACK;
	if (action & I2C_STOP)
		reg32 |= I2C_CR1_STOP;
	if (action & I2C_START)
		reg32 |= I2C_CR1_START;

	I2C_CR1(base_addr(i2c)) &= ~reg32;
}

int i2c_get_action_status(i2c_t i2c, int action)
{
	u32 reg32;
	int r;

	reg32 = I2C_CR1(base_addr(i2c));
	r = 0;
	if (reg32 & I2C_CR1_SWRST)
		r |= I2C_RESET;
	if (reg32 & I2C_CR1_ALERT)
		r |= I2C_SMBALERT;
	if (reg32 & I2C_CR1_PEC)
		r |= I2C_PEC;
	if (reg32 & I2C_CR1_POS)
		r |= I2C_POS;
	if (reg32 & I2C_CR1_ACK)
		r |= I2C_ACK;
	if (reg32 & I2C_CR1_STOP)
		r |= I2C_STOP;
	if (reg32 & I2C_CR1_START)
		r |= I2C_START;

	return r & action;
}

void i2c_enable_interrupt(i2c_t i2c, int interrupt)
{
	u32 reg32;

	reg32 = 0;
	if (interrupt & I2C_BUFFER)
		reg32 |= I2C_CR2_ITBUFEN;
	if (interrupt & I2C_EVENT)
		reg32 |= I2C_CR2_ITEVTEN;
	if (interrupt & I2C_ERROR)
		reg32 |= I2C_CR2_ITERREN;

	I2C_CR2(base_addr(i2c)) |= reg32;
}

void i2c_disable_interrupt(i2c_t i2c, int interrupt)
{
	u32 reg32;

	reg32 = 0;
	if (interrupt & I2C_BUFFER)
		reg32 |= I2C_CR2_ITBUFEN;
	if (interrupt & I2C_EVENT)
		reg32 |= I2C_CR2_ITEVTEN;
	if (interrupt & I2C_ERROR)
		reg32 |= I2C_CR2_ITERREN;

	I2C_CR2(base_addr(i2c)) &= ~reg32;
}

int i2c_get_interrupt_mask(i2c_t i2c, int interrupt)
{
	u32 reg32;
	int r;

	reg32 = I2C_CR2(base_addr(i2c));
	r = 0;
	if (reg32 & I2C_CR2_ITBUFEN)
		r |= I2C_BUFFER;
	if (reg32 & I2C_CR2_ITEVTEN)
		r |= I2C_EVENT;
	if (reg32 & I2C_CR2_ITERREN)
		r |= I2C_ERROR;
	return r & interrupt;
}

int i2c_get_interrupt_status(i2c_t i2c, int interrupt)
{
	return I2C_SR1(base_addr(i2c)) & interrupt;
}

void i2c_clear_interrupt(i2c_t i2c, int interrupt)
{
	I2C_SR1(base_addr(i2c)) &= ~interrupt;
}

int i2c_get_status(i2c_t i2c, int status)
{
	return I2C_SR2(base_addr(i2c)) & status;
}

void i2c_set_own_address(i2c_t i2c, u16 slave)
{
	I2C_OAR1(base_addr(i2c)) = slave;
}

void i2c_set_own_address2(i2c_t i2c, u8 slave)
{
	I2C_OAR2(base_addr(i2c)) = slave;
}

void i2c_put_data(i2c_t i2c, u8 data)
{
	I2C_DR(base_addr(i2c)) = data;
}

u8 i2c_get_data(i2c_t i2c)
{
	return I2C_DR(base_addr(i2c));
}

/* Polling mode functions */

int i2c_software_reset(i2c_t i2c, int timeout)
{
	u32 base;
	int i;

	base = base_addr(i2c);
	I2C_CR1(base) |= I2C_CR1_SWRST;
	for (i = 0; timeout < 0 ? 1 : i < timeout; i++)
		if (!(I2C_SR2(base) & I2C_SR2_BUSY))
			break;
	I2C_CR1(base) &= ~I2C_CR1_SWRST;
	if (I2C_SR2(base) & I2C_SR2_BUSY)
		return -I2C_ERROR_TIMEOUT;
	return 0;
}

int i2c_wait_previous_action(i2c_t i2c, int timeout)
{
	u32 base;
	int i;

	base = base_addr(i2c);
	for (i = 0; timeout < 0 ? 1 : i < timeout; i++) {
		if (!(I2C_CR1(base) & (I2C_CR1_STOP | I2C_CR1_START |
				       I2C_CR1_PEC)))
			break;
	}
	if (I2C_CR1(base) & (I2C_CR1_STOP | I2C_CR1_START | I2C_CR1_PEC))
		return -I2C_ERROR_TIMEOUT;
	return 0;
}

int i2c_start(i2c_t i2c, int timeout)
{
	u32 base;
	int i;

	base = base_addr(i2c);
	I2C_CR1(base) |= I2C_CR1_START;
	for (i = 0; timeout < 0 ? 1 : i < timeout; i++) {
		if (I2C_SR1(base) & (I2C_SR1_SB | I2C_SR1_ERROR))
			break;
	}
	if (I2C_SR1(base) & I2C_SR1_ERROR)
		return -I2C_ERROR_STATUS;
	if (!(I2C_SR1(base) & I2C_SR1_SB))
		return -I2C_ERROR_TIMEOUT;
	return 0;
}

int i2c_addr(i2c_t i2c, u8 sla)
{
	u32 base;

	base = base_addr(i2c);
	I2C_DR(base) = sla;
	while (!(I2C_SR1(base) & (I2C_SR1_ADDR | I2C_SR1_ERROR)))
		;
	if (I2C_SR1(base) & I2C_SR1_ERROR)
		return -I2C_ERROR_STATUS;
	I2C_SR2(base);		/* clear ADDR */
	return 0;
}

int i2c_write(i2c_t i2c, u8 *buf, int nbyte)
{
	u32 base;
	int i;

	base = base_addr(i2c);
	for (i = 0; i < nbyte; i++) {
		while (!(I2C_SR1(base) & (I2C_SR1_TXE | I2C_SR1_ERROR)))
			;
		if (I2C_SR1(base) & I2C_SR1_ERROR)
			return -I2C_ERROR_STATUS;
		I2C_DR(base) = *buf++;
	}

	while (!(I2C_SR1(base) & (I2C_SR1_BTF | I2C_SR1_ERROR)))
		;
	if (I2C_SR1(base) & I2C_SR1_ERROR)
		return -I2C_ERROR_STATUS;

	return nbyte;
}

int i2c_read(i2c_t i2c, u8 *buf, int nbyte)
{
	u32 base;
	u32 r;
	int i;

	base = base_addr(i2c);
	r = I2C_CR1(base);
	if (nbyte > 1)
		I2C_CR1(base) = r | I2C_CR1_ACK;
	for (i = 0; i < nbyte - 1; i++) {
		while (!(I2C_SR1(base) & (I2C_SR1_RXNE | I2C_SR1_ERROR)))
			;
		if (I2C_SR1(base) & I2C_SR1_ERROR)
			return -I2C_ERROR_STATUS;
		*buf++ = I2C_DR(base);
	}

	I2C_CR1(base) = r | I2C_CR1_STOP;

	while (!(I2C_SR1(base) & (I2C_SR1_RXNE | I2C_SR1_ERROR)))
		;
	if (I2C_SR1(base) & I2C_SR1_ERROR)
		return -I2C_ERROR_STATUS;
	*buf = I2C_DR(base);

	return nbyte;
}

int i2c_stop(i2c_t i2c, int timeout)
{
	u32 base;
	int i;

	base = base_addr(i2c);
	I2C_CR1(base) |= I2C_CR1_STOP;
	for (i = 0; timeout < 0 ? 1 : i < timeout; i++) {
		if (!(I2C_CR1(base) & I2C_CR1_STOP))
			break;
	}
	if (I2C_CR1(base) & I2C_CR1_STOP)
		return -I2C_ERROR_TIMEOUT;
	return 0;
}
