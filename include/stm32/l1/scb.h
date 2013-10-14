/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
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

#include <stm32/l1/memorymap.h>
#include <libopencm3.h>

/*
 * STMicroelectronics(www.st.com)
 *
 * PM0056: STM32F10xxx/20xxx/21xxx/L1xxxx Cortex-M3 programming manual
 * (27-May-2013 Rev 5)
 *
 * 4.4 System control block (SCB)
 */

/* --- SCB registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0xe000e008	SCB_ACTLR	Auxiliary control register
 *
 * 0x00		SCB_CPUID	CPUID base register
 * 0x04		SCB_ICSR	Interrupt control and status register
 * 0x08		SCB_VTOR	Vector table offset register
 * 0x0c		SCB_AIRCR	Application interrupt and reset control
 *					register
 * 0x10		SCB_SCR		System control register
 * 0x14		SCB_CCR		Configuration and control register
 * 0x18		SCB_SHPR1	System handler priority register 1
 * 0x1c		SCB_SHPR2	System handler priority register 2
 * 0x20		SCB_SHPR3	System handler priority register 3
 * 0x24		SCB_SHCSR	System handler control and state register
 * 0x28		SCB_CFSR	Configurable fault status register
 * 0x2c		SCB_HFSR	Hard fault status register
 * 0x34		SCB_MMFAR	Memory management fault address register
 * 0x38		SCB_BFAR	Bus fault address register
 */

#define SCB_ACTLR				MMIO32(SCS_BASE + 0x08)
#define SCB_CPUID				MMIO32(SCB_BASE + 0x00)
#define SCB_ICSR				MMIO32(SCB_BASE + 0x04)
#define SCB_VTOR				MMIO32(SCB_BASE + 0x08)
#define SCB_AIRCR				MMIO32(SCB_BASE + 0x0C)
#define SCB_SCR					MMIO32(SCB_BASE + 0x10)
#define SCB_CCR					MMIO32(SCB_BASE + 0x14)
#define SCB_SHPR1				MMIO32(SCB_BASE + 0x18)
#define SCB_SHPR2				MMIO32(SCB_BASE + 0x1c)
#define SCB_SHPR3				MMIO32(SCB_BASE + 0x20)
#define SCB_SHCSR				MMIO32(SCB_BASE + 0x24)
#define SCB_CFSR				MMIO32(SCB_BASE + 0x28)
#define SCB_HFSR				MMIO32(SCB_BASE + 0x2C)
#define SCB_MMFAR				MMIO32(SCB_BASE + 0x34)
#define SCB_BFAR				MMIO32(SCB_BASE + 0x38)

/* --- SCB_ACTLR values ---------------------------------------------------- */

#define SCB_ACTLR_DISFOLD			(1 << 2)
#define SCB_ACTLR_DISDEFWBUF			(1 << 1)
#define SCB_ACTLR_DISMCYCINT			(1 << 0)

/* --- SCB_CPUID values ---------------------------------------------------- */

/* SCB_CPUID[31:24]: Implementer: Implementer code */
/* SCB_CPUID[23:20]: Variant: Variant number */
/* SCB_CPUID[19:16]: Constant: Reads as 0xF */
/* SCB_CPUID[15:4]: PartNo: Part number of the processor */
/* SCB_CPUID[3:0]: Revision: Revision number */

/* --- SCB_ICSR values ----------------------------------------------------- */

#define SCB_ICSR_NMIPENDSET			(1 << 31)
#define SCB_ICSR_PENDSVSET			(1 << 28)
#define SCB_ICSR_PENDSVCLR			(1 << 27)
#define SCB_ICSR_PENDSTSET			(1 << 26)
#define SCB_ICSR_PENDSTCLR			(1 << 25)
#define SCB_ICSR_ISRPENDING			(1 << 22)
#define SCB_ICSR_VECTPENDING9			(1 << 21)
#define SCB_ICSR_VECTPENDING8			(1 << 20)
#define SCB_ICSR_VECTPENDING7			(1 << 19)
#define SCB_ICSR_VECTPENDING6			(1 << 18)
#define SCB_ICSR_VECTPENDING5			(1 << 17)
#define SCB_ICSR_VECTPENDING4			(1 << 16)
#define SCB_ICSR_VECTPENDING3			(1 << 15)
#define SCB_ICSR_VECTPENDING2			(1 << 14)
#define SCB_ICSR_VECTPENDING1			(1 << 13)
#define SCB_ICSR_VECTPENDING0			(1 << 12)
#define SCB_ICSR_RETOBASE			(1 << 11)
#define SCB_ICSR_VECTACTIVE8			(1 << 8)
#define SCB_ICSR_VECTACTIVE7			(1 << 7)
#define SCB_ICSR_VECTACTIVE6			(1 << 6)
#define SCB_ICSR_VECTACTIVE5			(1 << 5)
#define SCB_ICSR_VECTACTIVE4			(1 << 4)
#define SCB_ICSR_VECTACTIVE3			(1 << 3)
#define SCB_ICSR_VECTACTIVE2			(1 << 2)
#define SCB_ICSR_VECTACTIVE1			(1 << 1)
#define SCB_ICSR_VECTACTIVE0			(1 << 0)

/* --- SCB_VTOR values ----------------------------------------------------- */

/* SCB_VTOR[29:9]: TBLOFF[29:9]: Vector table base offset field */

/* --- SCB_AIRCR values ---------------------------------------------------- */

#define SCB_AIRCR_VECTKEY15			(1 << 31)
#define SCB_AIRCR_VECTKEY14			(1 << 30)
#define SCB_AIRCR_VECTKEY13			(1 << 29)
#define SCB_AIRCR_VECTKEY12			(1 << 28)
#define SCB_AIRCR_VECTKEY11			(1 << 27)
#define SCB_AIRCR_VECTKEY10			(1 << 26)
#define SCB_AIRCR_VECTKEY9			(1 << 25)
#define SCB_AIRCR_VECTKEY8			(1 << 24)
#define SCB_AIRCR_VECTKEY7			(1 << 23)
#define SCB_AIRCR_VECTKEY6			(1 << 22)
#define SCB_AIRCR_VECTKEY5			(1 << 21)
#define SCB_AIRCR_VECTKEY4			(1 << 20)
#define SCB_AIRCR_VECTKEY3			(1 << 19)
#define SCB_AIRCR_VECTKEY2			(1 << 18)
#define SCB_AIRCR_VECTKEY1			(1 << 17)
#define SCB_AIRCR_VECTKEY0			(1 << 16)
#define SCB_AIRCR_ENDIANESS			(1 << 15)
#define SCB_AIRCR_PRIGROUP2			(1 << 10)
#define SCB_AIRCR_PRIGROUP1			(1 << 9)
#define SCB_AIRCR_PRIGROUP0			(1 << 8)
#define SCB_AIRCR_SYSRESETREQ			(1 << 2)
#define SCB_AIRCR_VECTCLRACTIVE			(1 << 1)
#define SCB_AIRCR_VECTRESET			(1 << 0)

/* Register key */
#define SCB_AIRCR_VECTKEY			(0x05fa << 16)
#define SCB_AIRCR_VECTKEYSTAT			(0xfa05 << 16)

/* Interrupt priority grouping field */
#define SCB_AIRCR_PRIGROUP_GROUP16_NOSUB	(3 << 8)
#define SCB_AIRCR_PRIGROUP_GROUP8_SUB2		(4 << 8)
#define SCB_AIRCR_PRIGROUP_GROUP4_SUB4		(5 << 8)
#define SCB_AIRCR_PRIGROUP_GROUP2_SUB8		(6 << 8)
#define SCB_AIRCR_PRIGROUP_NOGROUP_SUB16	(7 << 8)

/* --- SCB_SCR values ------------------------------------------------------ */

#define SCB_SCR_SEVEONPEND			(1 << 4)
#define SCB_SCR_SLEEPDEEP			(1 << 2)
#define SCB_SCR_SLEEPONEXIT			(1 << 1)

/* --- SCB_CCR values ------------------------------------------------------ */

#define SCB_CCR_STKALIGN			(1 << 9)
#define SCB_CCR_BFHFNMIGN			(1 << 8)
#define SCB_CCR_DIV_0_TRP			(1 << 4)
#define SCB_CCR_UNALIGN_TRP			(1 << 3)
#define SCB_CCR_USERSETMPEND			(1 << 1)
#define SCB_CCR_NONBASETHRDENA			(1 << 0)

/* --- SCB_SHPR1 values ---------------------------------------------------- */

/* SCB_SHPR1[23:16]: PRI_6[7:0]: Priority of system handler 6, usage fault */
/* SCB_SHPR1[15:8]:  PRI_5[7:0]: Priority of system handler 5, bus fault */
/*
 * SCB_SHPR1[7:0]:   PRI_4[7:0]: Priority of system handler 4,
 *				 memory management fault
 */

/* --- SCB_SHPR2 values ---------------------------------------------------- */

/* SCB_SHPR2[31:24]: PRI_11[7:0]: Priority of system handler 11, SVCall */

/* --- SCB_SHPR3 values ---------------------------------------------------- */

/*
 * SCB_SHPR3[31:24]: PRI_15[7:0]: Priority of system handler 15,
 *				  SysTick exception
 */
/* SCB_SHPR3[23:16]: PRI_14[7:0]: Priority of system handler 14, PendSV */

/* --- SCB_SHCSR values ---------------------------------------------------- */

#define SCB_SHCSR_USGFAULTENA			(1 << 18)
#define SCB_SHCSR_BUSFAULTENA			(1 << 17)
#define SCB_SHCSR_MEMFAULTENA			(1 << 16)
#define SCB_SHCSR_SVCALLPENDED			(1 << 15)
#define SCB_SHCSR_BUSFAULTPENDED		(1 << 14)
#define SCB_SHCSR_MEMFAULTPENDED		(1 << 13)
#define SCB_SHCSR_USGFAULTPENDED		(1 << 12)
#define SCB_SHCSR_SYSTICKACT			(1 << 11)
#define SCB_SHCSR_PENDSVACT			(1 << 10)
#define SCB_SHCSR_MONITORACT			(1 << 8)
#define SCB_SHCSR_SVCALLACT			(1 << 7)
#define SCB_SHCSR_USGFAULTACT			(1 << 3)
#define SCB_SHCSR_BUSFAULTACT			(1 << 1)
#define SCB_SHCSR_MEMFAULTACT			(1 << 0)

/* --- SCB_CFSR values ----------------------------------------------------- */

#define SCB_CFSR_DIVBYZERO			(1 << 25)
#define SCB_CFSR_UNALIGNED			(1 << 24)
#define SCB_CFSR_NOCP				(1 << 19)
#define SCB_CFSR_INVPC				(1 << 18)
#define SCB_CFSR_INVSTATE			(1 << 17)
#define SCB_CFSR_UNDEFINSTR			(1 << 16)
#define SCB_CFSR_BFARVALID			(1 << 15)
#define SCB_CFSR_STKERR				(1 << 12)
#define SCB_CFSR_UNSTKERR			(1 << 11)
#define SCB_CFSR_IMPRECISERR			(1 << 10)
#define SCB_CFSR_PRECISERR			(1 << 9)
#define SCB_CFSR_IBUSERR			(1 << 8)
#define SCB_CFSR_MMARVALID			(1 << 7)
#define SCB_CFSR_MSTKERR			(1 << 4)
#define SCB_CFSR_MUNSTKERR			(1 << 3)
#define SCB_CFSR_DACCVIOL			(1 << 1)
#define SCB_CFSR_IACCVIOL			(1 << 0)

/* --- SCB_HFSR values ----------------------------------------------------- */

#define SCB_HFSR_DEBUG_VT			(1 << 31)
#define SCB_HFSR_FORCED				(1 << 30)
#define SCB_HFSR_VECTTBL			(1 << 1)

/* --- SCB_MMFAR values ---------------------------------------------------- */

/* SCB_MMFAR[31:0]: MMFAR[31:0]: Memory management fault address */

/* --- SCB_BFAR values ----------------------------------------------------- */

/* SCB_BFAR[31:0]: BFAR[31:0]: Bus fault address */

/* --- Function Prototypes ------------------------------------------------- */

/* Priority grouping */
typedef enum {
	SCB_PRIGROUP_7_4 = (3 << 8),
	SCB_PRIGROUP_7_5 = (4 << 8),
	SCB_PRIGROUP_7_6 = (5 << 8),
	SCB_PRIGROUP_7 = (6 << 8),
	SCB_PRIGROUP_NONE = (7 << 8)
} scb_prigroup_t;

/* Sleep */
enum {
	SCB_SLEEP_ON_EXIT = (1 << 1),
	SCB_SLEEP_DEEP = (1 << 2)
};

void scb_reset_system(void);
void scb_set_priority_grouping(scb_prigroup_t prigroup);
void scb_set_sleep(int sleep);
int scb_get_sleep(void);
void scb_set_vector_table_offset(u32 tbloff);
