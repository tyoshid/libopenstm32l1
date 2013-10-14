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

#include <stm32/l1/memorymap.h>
#include <libopencm3.h>

/*
 * STMicroelectronics(www.st.com)
 *
 * RM0038: STM32L100xx, STM32L151xx, STM32L152xx and STM32L162xx
 *         advanced ARM-based 32-bit MCUs
 * (19-Apr-2013 Rev 8)
 *
 * 9.2 External interrupt/event controller (EXTI)
 */

/* Note: Bits marked (**) only exist in high and medium+ density devices. */

/* --- EXTI registers ------------------------------------------------------ */
/*
 * Offset	Register
 * 0x00		EXTI_IMR	EXTI interrupt mask register
 * 0x04		EXTI_EMR	EXTI event mask register
 * 0x08		EXTI_RTSR	EXTI rising edge trigger selection register
 * 0x0c		EXTI_FTSR	Falling edge trigger selection register
 * 0x10		EXTI_SWIER	EXTI software interrupt event register
 * 0x14		EXTI_PR		EXTI pending register
 */

#define EXTI_IMR			MMIO32(EXTI_BASE + 0x00)
#define EXTI_EMR			MMIO32(EXTI_BASE + 0x04)
#define EXTI_RTSR			MMIO32(EXTI_BASE + 0x08)
#define EXTI_FTSR			MMIO32(EXTI_BASE + 0x0c)
#define EXTI_SWIER			MMIO32(EXTI_BASE + 0x10)
#define EXTI_PR				MMIO32(EXTI_BASE + 0x14)

/* --- EXTI_IMR values ----------------------------------------------------- */

/* EXTI_IMR[23:0]: MRx: Interrupt mask on line x */

/* --- EXTI_EMR values ----------------------------------------------------- */

/* EXTI_EMR[23:0]: MRx: Event mask on line x */

/* --- EXTI_RTSR values ---------------------------------------------------- */
/*
 * EXTI_RTSR[23:0]: TRx: Rising edge trigger event configuration bit of
 *                       line x
 */

/* --- EXTI_FTSR values ---------------------------------------------------- */
/*
 * EXTI_FTSR[23:0]: TRx: Falling edge trigger event configuration bit of
 *                       line x
 */

/* --- EXTI_SWIER values --------------------------------------------------- */

/* EXTI_SWIER[23:0]: SWIERx: Software interrupt on line x */

/* --- EXTI_PR values ------------------------------------------------------ */

/* EXTI_PR[23:0]: PRx: Pending bit */

/* --- Function prototypes ------------------------------------------------- */

/* EXTI number */
enum {
	EXTI0 = (1 << 0),
	EXTI1 = (1 << 1),
	EXTI2 = (1 << 2),
	EXTI3 = (1 << 3),
	EXTI4 = (1 << 4),
	EXTI5 = (1 << 5),
	EXTI6 = (1 << 6),
	EXTI7 = (1 << 7),
	EXTI8 = (1 << 8),
	EXTI9 = (1 << 9),
	EXTI10 = (1 << 10),
	EXTI11 = (1 << 11),
	EXTI12 = (1 << 12),
	EXTI13 = (1 << 13),
	EXTI14 = (1 << 14),
	EXTI15 = (1 << 15),
	EXTI16 = (1 << 16),
	EXTI17 = (1 << 17),
	EXTI18 = (1 << 18),
	EXTI19 = (1 << 19),
	EXTI20 = (1 << 20),
	EXTI21 = (1 << 21),
	EXTI22 = (1 << 22),
	EXTI23 = (1 << 23)	/* (**) */
};

#define	EXTI_PVD			EXTI16
#define EXTI_RTC_ALARM			EXTI17
#define EXTI_USB_WAKEUP			EXTI18
#define EXTI_RTC_TAMPER_TIMESTAMP	EXTI19
#define EXTI_RTC_WAKEUP			EXTI20
#define EXTI_COMP1_WAKEUP		EXTI21
#define EXTI_COMP2_WAKEUP		EXTI22
#define EXTI_COMP_CHANNEL_ACQUISITION	EXTI23	/* (**) */

/* Trigger types */
typedef enum {
	EXTI_TRIGGER_NONE,
	EXTI_RISING,
	EXTI_FALLING,
	EXTI_BOTH,
	EXTI_TRIGGER_UNKNOWN
} exti_trigger_t;

void exti_set_trigger(int extis, exti_trigger_t trig);
exti_trigger_t exti_get_trigger(int extis);
void exti_enable_interrupt(int extis);
void exti_disable_interrupt(int extis);
int exti_get_interrupt_mask(int extis);
int exti_get_interrupt_status(int extis);
void exti_clear_interrupt(int extis);
void exti_enable_event(int extis);
void exti_disable_event(int extis);
int exti_get_event_mask(int extis);
void exti_set_software_interrupt(int extis);
