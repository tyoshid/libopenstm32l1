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

#include <stm32/l1/memorymap.h>
#include <libopencm3.h>

/*
 * STMicroelectronics(www.st.com)
 *
 * RM0038: STM32L100xx, STM32L151xx, STM32L152xx and STM32L162xx
 *         advanced ARM-based 32-bit MCUs
 * (19-Apr-2013 Rev 8)
 *
 * 15 Liquid crystal display controller (LCD)
 */

/* --- LCD registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		LCD_CR		LCD control register
 * 0x04		LCD_FCR		LCD frame control register
 * 0x08		LCD_SR		LCD status register
 * 0x0c		LCD_CLR		LCD clear register
 * 0x14		LCD_RAM(COM0)	LCD display memory
 * 0x18
 * 0x1c		LCD_RAM(COM1)
 * 0x20
 * 0x24		LCD_RAM(COM2)
 * 0x28
 * 0x2c		LCD_RAM(COM3)
 * 0x30
 * 0x34		LCD_RAM(COM4)
 * 0x38
 * 0x3c		LCD_RAM(COM5)
 * 0x40
 * 0x44		LCD_RAM(COM6)
 * 0x48
 * 0x4c		LCD_RAM(COM7)
 * 0x50
 */

#define LCD_CR				MMIO32(LCD_BASE + 0x00)
#define LCD_FCR				MMIO32(LCD_BASE + 0x04)
#define LCD_SR				MMIO32(LCD_BASE + 0x08)
#define LCD_CLR				MMIO32(LCD_BASE + 0x0c)

#define LCD_RAM_BASE			(LCD_BASE + 0x14)
#define LCD_RAM				((u32 *)LCD_RAM_BASE)
#define LCD_RAM_COM0			0
#define LCD_RAM_COM1			2
#define LCD_RAM_COM2			4
#define LCD_RAM_COM3			6
#define LCD_RAM_COM4			8
#define LCD_RAM_COM5			10
#define LCD_RAM_COM6			12
#define LCD_RAM_COM7			14

/* --- LCD_CR values ------------------------------------------------------- */

#define LCD_CR_MUX_SEG			(1 << 7)
#define LCD_CR_BIAS1			(1 << 6)
#define LCD_CR_BIAS0			(1 << 5)
#define LCD_CR_DUTY2			(1 << 4)
#define LCD_CR_DUTY1			(1 << 3)
#define LCD_CR_DUTY0			(1 << 2)
#define LCD_CR_VSEL			(1 << 1)
#define LCD_CR_LCDEN			(1 << 0)

/* Bias */
#define LCD_CR_BIAS_1_4			(0 << 5)
#define LCD_CR_BIAS_1_2			(1 << 5)
#define LCD_CR_BIAS_1_3			(2 << 5)

/* Duty cycle */
#define LCD_CR_DUTY_STATIC		(0 << 2)
#define LCD_CR_DUTY_1_2			(1 << 2)
#define LCD_CR_DUTY_1_3			(2 << 2)
#define LCD_CR_DUTY_1_4			(3 << 2)
#define LCD_CR_DUTY_1_8			(4 << 2)

/* --- LCD_FCR values ------------------------------------------------------ */

#define LCD_FCR_PS3			(1 << 25)
#define LCD_FCR_PS2			(1 << 24)
#define LCD_FCR_PS1			(1 << 23)
#define LCD_FCR_PS0			(1 << 22)
#define LCD_FCR_DIV3			(1 << 21)
#define LCD_FCR_DIV2			(1 << 20)
#define LCD_FCR_DIV1			(1 << 19)
#define LCD_FCR_DIV0			(1 << 18)
#define LCD_FCR_BLINK1			(1 << 17)
#define LCD_FCR_BLINK0			(1 << 16)
#define LCD_FCR_BLINKF2			(1 << 15)
#define LCD_FCR_BLINKF1			(1 << 14)
#define LCD_FCR_BLINKF0			(1 << 13)
#define LCD_FCR_CC2			(1 << 12)
#define LCD_FCR_CC1			(1 << 11)
#define LCD_FCR_CC0			(1 << 10)
#define LCD_FCR_DEAD2			(1 << 9)
#define LCD_FCR_DEAD1			(1 << 8)
#define LCD_FCR_DEAD0			(1 << 7)
#define LCD_FCR_PON2			(1 << 6)
#define LCD_FCR_PON1			(1 << 5)
#define LCD_FCR_PON0			(1 << 4)
#define LCD_FCR_UDDIE			(1 << 3)
#define LCD_FCR_SOFIE			(1 << 1)
#define LCD_FCR_HD			(1 << 0)

/* LCDCLK prescaler (ck_ps = LCDCLK / n) */
#define LCD_FCR_PS_1			(0 << 22)
#define LCD_FCR_PS_2			(1 << 22)
#define LCD_FCR_PS_4			(2 << 22)
#define LCD_FCR_PS_8			(3 << 22)
#define LCD_FCR_PS_16			(4 << 22)
#define LCD_FCR_PS_32			(5 << 22)
#define LCD_FCR_PS_64			(6 << 22)
#define LCD_FCR_PS_128			(7 << 22)
#define LCD_FCR_PS_256			(8 << 22)
#define LCD_FCR_PS_512			(9 << 22)
#define LCD_FCR_PS_1024			(10 << 22)
#define LCD_FCR_PS_2048			(11 << 22)
#define LCD_FCR_PS_4096			(12 << 22)
#define LCD_FCR_PS_8192			(13 << 22)
#define LCD_FCR_PS_16384		(14 << 22)
#define LCD_FCR_PS_32768		(15 << 22)

/* Clock divider (ck_div = ck_ps / n) */
#define LCD_FCR_DIV_16			(0 << 18)
#define LCD_FCR_DIV_17			(1 << 18)
#define LCD_FCR_DIV_18			(2 << 18)
#define LCD_FCR_DIV_19			(3 << 18)
#define LCD_FCR_DIV_20			(4 << 18)
#define LCD_FCR_DIV_21			(5 << 18)
#define LCD_FCR_DIV_22			(6 << 18)
#define LCD_FCR_DIV_23			(7 << 18)
#define LCD_FCR_DIV_24			(8 << 18)
#define LCD_FCR_DIV_25			(9 << 18)
#define LCD_FCR_DIV_26			(10 << 18)
#define LCD_FCR_DIV_27			(11 << 18)
#define LCD_FCR_DIV_28			(12 << 18)
#define LCD_FCR_DIV_29			(13 << 18)
#define LCD_FCR_DIV_30			(14 << 18)
#define LCD_FCR_DIV_31			(15 << 18)

/* Blink mode */
#define LCD_FCR_BLINK_DISABLE		(0 << 16)
#define LCD_FCR_BLINK_SEG0_COM0		(1 << 16)
#define LCD_FCR_BLINK_SEG0_ALL_COMS	(2 << 16)
#define LCD_FCR_BLINK_ALL_SEGS_ALL_COMS	(3 << 16)

/* Blink frequency (fck_div / n) */
#define LCD_FCR_BLINKF_8		(0 << 13)
#define LCD_FCR_BLINKF_16		(1 << 13)
#define LCD_FCR_BLINKF_32		(2 << 13)
#define LCD_FCR_BLINKF_64		(3 << 13)
#define LCD_FCR_BLINKF_128		(4 << 13)
#define LCD_FCR_BLINKF_256		(5 << 13)
#define LCD_FCR_BLINKF_512		(6 << 13)
#define LCD_FCR_BLINKF_1024		(7 << 13)

/* Contrast control */
#define LCD_FCR_CC_VLCD0		(0 << 10)
#define LCD_FCR_CC_VLCD1		(1 << 10)
#define LCD_FCR_CC_VLCD2		(2 << 10)
#define LCD_FCR_CC_VLCD3		(3 << 10)
#define LCD_FCR_CC_VLCD4		(4 << 10)
#define LCD_FCR_CC_VLCD5		(5 << 10)
#define LCD_FCR_CC_VLCD6		(6 << 10)
#define LCD_FCR_CC_VLCD7		(7 << 10)

/* Dead time duration */
#define LCD_FCR_DEAD_0			(0 << 7)
#define LCD_FCR_DEAD_1			(1 << 7)
#define LCD_FCR_DEAD_2			(2 << 7)
#define LCD_FCR_DEAD_3			(3 << 7)
#define LCD_FCR_DEAD_4			(4 << 7)
#define LCD_FCR_DEAD_5			(5 << 7)
#define LCD_FCR_DEAD_6			(6 << 7)
#define LCD_FCR_DEAD_7			(7 << 7)

/* Pulse ON duration (n / ck_ps) */
#define LCD_FCR_PON_0			(0 << 4)
#define LCD_FCR_PON_1			(1 << 4)
#define LCD_FCR_PON_2			(2 << 4)
#define LCD_FCR_PON_3			(3 << 4)
#define LCD_FCR_PON_4			(4 << 4)
#define LCD_FCR_PON_5			(5 << 4)
#define LCD_FCR_PON_6			(6 << 4)
#define LCD_FCR_PON_7			(7 << 4)
#define LCD_FCR_PON_PERMANENT		(1 << 4 | 1 << 0)

/* --- LCD_SR values ------------------------------------------------------- */

#define LCD_SR_FCRSF			(1 << 5)
#define LCD_SR_RDY			(1 << 4)
#define LCD_SR_UDD			(1 << 3)
#define LCD_SR_UDR			(1 << 2)
#define LCD_SR_SOF			(1 << 1)
#define LCD_SR_ENS			(1 << 0)

/* --- LCD_CLR values ------------------------------------------------------ */

#define LCD_CLR_UDDC			(1 << 3)
#define LCD_CLR_SOFC			(1 << 1)

/* --- Function prototypes ------------------------------------------------- */

/* Duty */
typedef enum {
	LCD_DUTY_STATIC = (0 << 2),
	LCD_DUTY_1_2 = (1 << 2),
	LCD_DUTY_1_3 = (2 << 2),
	LCD_DUTY_1_4 = (3 << 2),
	LCD_DUTY_1_8 = (4 << 2)
} lcd_duty_t;

/* Bias */
typedef enum {
	LCD_BIAS_1_4 = (0 << 5),
	LCD_BIAS_1_2 = (1 << 5),
	LCD_BIAS_1_3 = (2 << 5)
} lcd_bias_t;

/* Blink */
typedef enum {
	LCD_BLINK_DISABLE = (0 << 16),
	LCD_SEG0_COM0 = (1 << 16),
	LCD_SEG0_ALL_COMS = (2 << 16),
	LCD_ALL_SEGS_ALL_COMS = (3 << 16)
} lcd_blink_mode_t;

/* Interrupt */
enum {
	LCD_UDD = (1 << 3),
	LCD_SOF = (1 << 1)
};

void lcd_set_seg_pin_multiplex(bool enable);
void lcd_set_duty_bias(lcd_duty_t duty, lcd_bias_t bias);
void lcd_set_voltage_source(bool external);
void lcd_enable(void);
void lcd_disable(void);
void lcd_set_ck_div(int prescaler, int divider);
void lcd_set_blink_mode(lcd_blink_mode_t mode);
void lcd_set_blink_frequency(int divider);
void lcd_set_contrast(int contrast);
void lcd_set_dead_time(int dead_time);
void lcd_set_pulse_on_duration(int duration);
void lcd_wait_for_sync(void);
void lcd_wait_for_ready(void);
void lcd_update(void);
void lcd_enable_interrupt(int interrupt);
void lcd_disable_interrupt(int interrupt);
int lcd_get_interrupt_mask(int interrupt);
int lcd_get_interrupt_status(int interrupt);
void lcd_clear_interrupt(int interrupt);
