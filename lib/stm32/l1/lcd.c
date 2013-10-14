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

#include <stm32/l1/lcd.h>

void lcd_set_seg_pin_multiplex(bool enable)
{
	if (enable)
		LCD_CR |= LCD_CR_MUX_SEG;
	else
		LCD_CR &= ~LCD_CR_MUX_SEG;
}

void lcd_set_duty_bias(lcd_duty_t duty, lcd_bias_t bias)
{
	u32 reg32;

	reg32 = LCD_CR;
	reg32 &= ~(LCD_CR_BIAS1 | LCD_CR_BIAS0 |
		   LCD_CR_DUTY2 | LCD_CR_DUTY1 | LCD_CR_DUTY0);
	LCD_CR = (reg32 | bias | duty);
}

void lcd_set_voltage_source(bool external)
{
	if (external)
		LCD_CR |= LCD_CR_VSEL;
	else
		LCD_CR &= ~LCD_CR_VSEL;
}

void lcd_enable(void)
{
	LCD_CR |= LCD_CR_LCDEN;
}

void lcd_disable(void)
{
	LCD_CR &= ~LCD_CR_LCDEN;
}

/*
 * fframe = fck_div * duty
 * fck_div = fLCDCLK / (prescaler * divider)
 * prescaler = 1, 2, 4, 8, ... 32768
 * divider = 16, 17, 18, ... 31
 */
void lcd_set_ck_div(int prescaler, int divider)
{
	int i;
	u32 ps;
	u32 div;
	u32 reg32;

	for (i = 0; i < 16 && prescaler > (1 << i); i++)
		;
	ps = (i & 0xf) << 22;
	div = ((divider - 16) & 0xf) << 18;

	reg32 = LCD_FCR;
	reg32 &= ~(LCD_FCR_PS3 | LCD_FCR_PS2 | LCD_FCR_PS1 | LCD_FCR_PS0 |
		   LCD_FCR_DIV3 | LCD_FCR_DIV2 | LCD_FCR_DIV1 | LCD_FCR_DIV0);
	LCD_FCR = (reg32 | ps | div);
}

void lcd_set_blink_mode(lcd_blink_mode_t mode)
{
	u32 reg32;

	reg32 = LCD_FCR;
	reg32 &= ~(LCD_FCR_BLINK1 | LCD_FCR_BLINK0);
	LCD_FCR = (reg32 | mode);
}

/*
 * fblink = fck_div / divider
 * divider = 8, 16, 32, ... 1024
 */
void lcd_set_blink_frequency(int divider)
{
	int i;
	u32 freq;
	u32 reg32;

	for (i = 0; i < 8 && divider > (1 << i) * 8; i++)
		;
	freq = (i & 7) << 13;

	reg32 = LCD_FCR;
	reg32 &= ~(LCD_FCR_BLINKF2 | LCD_FCR_BLINKF1 | LCD_FCR_BLINKF0);
	LCD_FCR = (reg32 | freq);
}

/* contrast = 0 - 7 */
void lcd_set_contrast(int contrast)
{
	u32 cc;
	u32 reg32;

	cc = (contrast & 7) << 10;

	reg32 = LCD_FCR;
	reg32 &= ~(LCD_FCR_CC2 | LCD_FCR_CC1 | LCD_FCR_CC0);
	LCD_FCR = (reg32 | cc);
}

/* dead_time = 0 - 7 */
void lcd_set_dead_time(int dead_time)
{
	u32 dead;
	u32 reg32;

	dead = (dead_time & 7) << 7;

	reg32 = LCD_FCR;
	reg32 &= ~(LCD_FCR_DEAD2 | LCD_FCR_DEAD1 | LCD_FCR_DEAD0);
	LCD_FCR = (reg32 | dead);
}

/* duration = 0 - 8 (8: permanent high drive) */
void lcd_set_pulse_on_duration(int duration)
{
	u32 pon;
	u32 reg32;

	if (duration == 8)
		pon = (LCD_FCR_PON0 | LCD_FCR_HD);
	else
		pon = (duration & 7) << 4;

	reg32 = LCD_FCR;
	reg32 &= ~(LCD_FCR_PON2 | LCD_FCR_PON1 | LCD_FCR_PON0 | LCD_FCR_HD);
	LCD_FCR = (reg32 | pon);
}


void lcd_wait_for_sync(void)
{
	while (!(LCD_SR & LCD_SR_FCRSF))
		;
}

void lcd_wait_for_ready(void)
{
	while (!(LCD_SR & LCD_SR_RDY))
		;
}

void lcd_update(void)
{
	if (LCD_SR & LCD_SR_UDD)
		LCD_CLR = LCD_CLR_UDDC;
	LCD_SR = LCD_SR_UDR;
	while (!(LCD_SR & LCD_SR_UDD))
		;
	LCD_CLR = LCD_CLR_UDDC;
}

void lcd_enable_interrupt(int interrupt)
{
	LCD_FCR |= interrupt;
}

void lcd_disable_interrupt(int interrupt)
{
	LCD_FCR &= ~interrupt;
}

int lcd_get_interrupt_mask(int interrupt)
{
	return LCD_FCR & interrupt;
}

int lcd_get_interrupt_status(int interrupt)
{
	return LCD_SR & interrupt;
}

void lcd_clear_interrupt(int interrupt)
{
	LCD_CLR = interrupt;
}
