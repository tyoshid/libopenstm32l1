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
 * 7 System configuration controller (SYSCFG) and routing interface (RI)
 */

/*
 * Note: Registers and bits marked (**) only exist in high and medium+ density
 * devices.
 */

/* --- RI registers -------------------------------------------------------- */
/*
 * Offset	Register
 * 0x04		RI_ICR		RI input capture register
 * 0x08		RI_ASCR1	RI analog switch control register
 * 0x0c		RI_ASCR2	RI analog switch control register 2
 * 0x10		RI_HYSCR1	RI Hysteresis control register
 * 0x14		RI_HYSCR2	RI Hysteresis control register
 * 0x18		RI_HYSCR3	RI Hysteresis control register
 * 0x1c		RI_HYSCR4	RI Hysteresis control register
 * 0x20		RI_ASMR1	Analog switch mode register
 * 0x24		RI_CMR1		Channel mask register
 * 0x28		RI_CICR1	Channel identification for capture register
 * 0x2c		RI_ASMR2	Analog switch mode register
 * 0x30		RI_CMR2		Channel mask register
 * 0x34		RI_CICR2	Channel identification for capture register
 * 0x38		RI_ASMR3	Analog switch mode register
 * 0x3c		RI_CMR3		Channel mask register
 * 0x40		RI_CICR3	Channel identification for capture register
 * 0x44		RI_ASMR4	Analog switch mode register
 * 0x48		RI_CMR4		Channel mask register
 * 0x4c		RI_CICR4	Channel identification for capture register
 * 0x50		RI_ASMR5	Analog switch mode register
 * 0x54		RI_CMR5		Channel mask register
 * 0x58		RI_CICR5	Channel identification for capture register
 */

#define RI_ICR				MMIO32(RI_BASE + 0x04)
#define RI_ASCR1			MMIO32(RI_BASE + 0x08)
#define RI_ASCR2			MMIO32(RI_BASE + 0x0c)
#define RI_HYSCR1			MMIO32(RI_BASE + 0x10)
#define RI_HYSCR2			MMIO32(RI_BASE + 0x14)
#define RI_HYSCR3			MMIO32(RI_BASE + 0x18)
#define RI_HYSCR4			MMIO32(RI_BASE + 0x1c)
#define RI_ASMR1			MMIO32(RI_BASE + 0x20)	/* (**) */
#define RI_CMR1				MMIO32(RI_BASE + 0x24)	/* (**) */
#define RI_CICR1			MMIO32(RI_BASE + 0x28)	/* (**) */
#define RI_ASMR2			MMIO32(RI_BASE + 0x2c)	/* (**) */
#define RI_CMR2				MMIO32(RI_BASE + 0x30)	/* (**) */
#define RI_CICR2			MMIO32(RI_BASE + 0x34)	/* (**) */
#define RI_ASMR3			MMIO32(RI_BASE + 0x38)	/* (**) */
#define RI_CMR3				MMIO32(RI_BASE + 0x3c)	/* (**) */
#define RI_CICR3			MMIO32(RI_BASE + 0x40)	/* (**) */
#define RI_ASMR4			MMIO32(RI_BASE + 0x44)	/* (**) */
#define RI_CMR4				MMIO32(RI_BASE + 0x48)	/* (**) */
#define RI_CICR4			MMIO32(RI_BASE + 0x4c)	/* (**) */
#define RI_ASMR5			MMIO32(RI_BASE + 0x50)	/* (**) */
#define RI_CMR5				MMIO32(RI_BASE + 0x54)	/* (**) */
#define RI_CICR5			MMIO32(RI_BASE + 0x58)	/* (**) */

/* --- RI_ICR values ------------------------------------------------------- */

#define RI_ICR_IC4			(1 << 21)
#define RI_ICR_IC3			(1 << 20)
#define RI_ICR_IC2			(1 << 19)
#define RI_ICR_IC1			(1 << 18)
#define RI_ICR_TIM1			(1 << 17)
#define RI_ICR_TIM0			(1 << 16)
#define RI_ICR_IC4IOS3			(1 << 15)
#define RI_ICR_IC4IOS2			(1 << 14)
#define RI_ICR_IC4IOS1			(1 << 13)
#define RI_ICR_IC4IOS0			(1 << 12)
#define RI_ICR_IC3IOS3			(1 << 11)
#define RI_ICR_IC3IOS2			(1 << 10)
#define RI_ICR_IC3IOS1			(1 << 9)
#define RI_ICR_IC3IOS0			(1 << 8)
#define RI_ICR_IC2IOS3			(1 << 7)
#define RI_ICR_IC2IOS2			(1 << 6)
#define RI_ICR_IC2IOS1			(1 << 5)
#define RI_ICR_IC2IOS0			(1 << 4)
#define RI_ICR_IC1IOS3			(1 << 3)
#define RI_ICR_IC1IOS2			(1 << 2)
#define RI_ICR_IC1IOS1			(1 << 1)
#define RI_ICR_IC1IOS0			(1 << 0)

/* Timer select bits */
#define RI_ICR_TIM_NO			(0 << 16)
#define RI_ICR_TIM_TIM2			(1 << 16)
#define RI_ICR_TIM_TIM3			(2 << 16)
#define RI_ICR_TIM_TIM4			(3 << 16)

/* Input capture 4 select bits */
#define RI_ICR_IC4IOS_PA3		(0 << 12)
#define RI_ICR_IC4IOS_PA7		(1 << 12)
#define RI_ICR_IC4IOS_PA11		(2 << 12)
#define RI_ICR_IC4IOS_PA15		(3 << 12)
#define RI_ICR_IC4IOS_PC3		(4 << 12)
#define RI_ICR_IC4IOS_PC7		(5 << 12)
#define RI_ICR_IC4IOS_PC11		(6 << 12)
#define RI_ICR_IC4IOS_PC15		(7 << 12)
#define RI_ICR_IC4IOS_PD3		(8 << 12)
#define RI_ICR_IC4IOS_PD7		(9 << 12)
#define RI_ICR_IC4IOS_PD11		(10 << 12)
#define RI_ICR_IC4IOS_PD15		(11 << 12)
#define RI_ICR_IC4IOS_PE3		(12 << 12)
#define RI_ICR_IC4IOS_PE7		(13 << 12)
#define RI_ICR_IC4IOS_PE11		(14 << 12)
#define RI_ICR_IC4IOS_PE15		(15 << 12)

/* Input capture 3 select bits */
#define RI_ICR_IC3IOS_PA2		(0 << 8)
#define RI_ICR_IC3IOS_PA6		(1 << 8)
#define RI_ICR_IC3IOS_PA10		(2 << 8)
#define RI_ICR_IC3IOS_PA14		(3 << 8)
#define RI_ICR_IC3IOS_PC2		(4 << 8)
#define RI_ICR_IC3IOS_PC6		(5 << 8)
#define RI_ICR_IC3IOS_PC10		(6 << 8)
#define RI_ICR_IC3IOS_PC14		(7 << 8)
#define RI_ICR_IC3IOS_PD2		(8 << 8)
#define RI_ICR_IC3IOS_PD6		(9 << 8)
#define RI_ICR_IC3IOS_PD10		(10 << 8)
#define RI_ICR_IC3IOS_PD14		(11 << 8)
#define RI_ICR_IC3IOS_PE2		(12 << 8)
#define RI_ICR_IC3IOS_PE6		(13 << 8)
#define RI_ICR_IC3IOS_PE10		(14 << 8)
#define RI_ICR_IC3IOS_PE14		(15 << 8)

/* Input capture 2 select bits */
#define RI_ICR_IC2IOS_PA1		(0 << 4)
#define RI_ICR_IC2IOS_PA5		(1 << 4)
#define RI_ICR_IC2IOS_PA9		(2 << 4)
#define RI_ICR_IC2IOS_PA13		(3 << 4)
#define RI_ICR_IC2IOS_PC1		(4 << 4)
#define RI_ICR_IC2IOS_PC5		(5 << 4)
#define RI_ICR_IC2IOS_PC9		(6 << 4)
#define RI_ICR_IC2IOS_PC13		(7 << 4)
#define RI_ICR_IC2IOS_PD1		(8 << 4)
#define RI_ICR_IC2IOS_PD5		(9 << 4)
#define RI_ICR_IC2IOS_PD9		(10 << 4)
#define RI_ICR_IC2IOS_PD13		(11 << 4)
#define RI_ICR_IC2IOS_PE1		(12 << 4)
#define RI_ICR_IC2IOS_PE5		(13 << 4)
#define RI_ICR_IC2IOS_PE9		(14 << 4)
#define RI_ICR_IC2IOS_PE13		(15 << 4)

/* Input capture 1 select bits */
#define RI_ICR_IC1IOS_PA0		(0 << 0)
#define RI_ICR_IC1IOS_PA4		(1 << 0)
#define RI_ICR_IC1IOS_PA8		(2 << 0)
#define RI_ICR_IC1IOS_PA12		(3 << 0)
#define RI_ICR_IC1IOS_PC0		(4 << 0)
#define RI_ICR_IC1IOS_PC4		(5 << 0)
#define RI_ICR_IC1IOS_PC8		(6 << 0)
#define RI_ICR_IC1IOS_PC12		(7 << 0)
#define RI_ICR_IC1IOS_PD0		(8 << 0)
#define RI_ICR_IC1IOS_PD4		(9 << 0)
#define RI_ICR_IC1IOS_PD8		(10 << 0)
#define RI_ICR_IC1IOS_PD12		(11 << 0)
#define RI_ICR_IC1IOS_PE0		(12 << 0)
#define RI_ICR_IC1IOS_PE4		(13 << 0)
#define RI_ICR_IC1IOS_PE8		(14 << 0)
#define RI_ICR_IC1IOS_PE12		(15 << 0)

/* --- RI_ASCR1 values ----------------------------------------------------- */

#define RI_ASCR1_SCM			(1 << 31)
#define RI_ASCR1_CH30_GR11_4		(1 << 30)	/* (**) */
#define RI_ASCR1_CH29_GR11_3		(1 << 29)	/* (**) */
#define RI_ASCR1_CH28_GR11_2		(1 << 28)	/* (**) */
#define RI_ASCR1_CH27_GR11_1		(1 << 27)	/* (**) */
#define RI_ASCR1_VCOMP			(1 << 26)
#define RI_ASCR1_CH25			(1 << 25)
#define RI_ASCR1_CH24			(1 << 24)
#define RI_ASCR1_CH23			(1 << 23)
#define RI_ASCR1_CH22			(1 << 22)
#define RI_ASCR1_CH21_GR7_4		(1 << 21)
#define RI_ASCR1_CH20_GR7_3		(1 << 20)
#define RI_ASCR1_CH19_GR7_2		(1 << 19)
#define RI_ASCR1_CH18_GR7_1		(1 << 18)
#define RI_ASCR1_CH31_GR11_5		(1 << 16)	/* (**) */
#define RI_ASCR1_CH15_GR9_2		(1 << 15)
#define RI_ASCR1_CH14_GR9_1		(1 << 14)
#define RI_ASCR1_CH13_GR8_4		(1 << 13)
#define RI_ASCR1_CH12_GR8_3		(1 << 12)
#define RI_ASCR1_CH11_GR8_2		(1 << 11)
#define RI_ASCR1_CH10_GR8_1		(1 << 10)
#define RI_ASCR1_CH9_GR3_2		(1 << 9)
#define RI_ASCR1_CH8_GR3_1		(1 << 8)
#define RI_ASCR1_CH7_GR2_2		(1 << 7)
#define RI_ASCR1_CH6_GR2_1		(1 << 6)
#define RI_ASCR1_CH5			(1 << 5)
#define RI_ASCR1_CH4			(1 << 4)
#define RI_ASCR1_CH3_GR1_4		(1 << 3)
#define RI_ASCR1_CH2_GR1_3		(1 << 2)
#define RI_ASCR1_CH1_GR1_2		(1 << 1)
#define RI_ASCR1_CH0_GR1_1		(1 << 0)

/* --- RI_ASCR2 values ----------------------------------------------------- */

#define RI_ASCR2_GR6_4			(1 << 28)	/* (**) */
#define RI_ASCR2_GR6_3			(1 << 27)	/* (**) */
#define RI_ASCR2_CH12B_GR7_7		(1 << 26)	/* (**) */
#define RI_ASCR2_CH11B_GR7_6		(1 << 25)	/* (**) */
#define RI_ASCR2_CH10B_GR7_5		(1 << 24)	/* (**) */
#define RI_ASCR2_CH9B_GR2_5		(1 << 23)	/* (**) */
#define RI_ASCR2_CH8B_GR2_4		(1 << 22)	/* (**) */
#define RI_ASCR2_CH7B_GR2_3		(1 << 21)	/* (**) */
#define RI_ASCR2_CH6B_GR9_4		(1 << 20)	/* (**) */
#define RI_ASCR2_CH3B_GR9_3		(1 << 19)	/* (**) */
#define RI_ASCR2_CH2B_GR3_5		(1 << 18)	/* (**) */
#define RI_ASCR2_CH1B_GR3_4		(1 << 17)	/* (**) */
#define RI_ASCR2_CH0B_GR3_3		(1 << 16)	/* (**) */
#define RI_ASCR2_GR4_3			(1 << 11)
#define RI_ASCR2_GR4_2			(1 << 10)
#define RI_ASCR2_GR4_1			(1 << 9)
#define RI_ASCR2_GR5_3			(1 << 8)
#define RI_ASCR2_GR5_2			(1 << 7)
#define RI_ASCR2_GR5_1			(1 << 6)
#define RI_ASCR2_GR6_2			(1 << 5)
#define RI_ASCR2_GR6_1			(1 << 4)
#define RI_ASCR2_GR10_4			(1 << 3)
#define RI_ASCR2_GR10_3			(1 << 2)
#define RI_ASCR2_GR10_2			(1 << 1)
#define RI_ASCR2_GR10_1			(1 << 0)

/* --- RI_HYSCR1 values ---------------------------------------------------- */

/* RI_HYSCR1[31:16]: PB[15:0]: Port B hysteresis control on/off */
/* RI_HYSCR1[15:0]: PA[15:0]: Port A hysteresis control on/off */

/* --- RI_HYSCR2 values ---------------------------------------------------- */

/* RI_HYSCR2[31:16]: PD[15:0]: Port D hysteresis control on/off */
/* RI_HYSCR2[15:0]: PC[15:0]: Port C hysteresis control on/off */

/* --- RI_HYSCR3 values ---------------------------------------------------- */

/* RI_HYSCR3[31:16]: PF[15:0]: Port F hysteresis control on/off (**) */
/* RI_HYSCR3[15:0]: PE[15:0]: Port E hysteresis control on/off */

/* --- RI_HYSCR4 values ---------------------------------------------------- */

/* PG[15:0]: Port G hysteresis control on/off (**) */

/* --- RI_ASMR1 values (**) ------------------------------------------------ */

/* PA[15:0]: Port A analog switch mode selection */

/* --- RI_CMR1 values (**) ------------------------------------------------- */

/* PA[15:0]: Port A channel masking */

/* --- RI_CICR1 values (**) ------------------------------------------------ */

/* PA[15:0]: Port A channel identification for capture */

/* --- RI_ASMR2 values (**) ------------------------------------------------ */

/* PB[15:0]: Port B analog switch mode selection */

/* --- RI_CMR2 values (**) ------------------------------------------------- */

/* PB[15:0]: Port B channel masking */

/* --- RI_CICR2 values (**) ------------------------------------------------ */

/* PB[15:0]: Port B channel identification for capture */

/* --- RI_ASMR3 values (**) ------------------------------------------------ */

/* PC[15:0]: Port C analog switch mode selection */

/* --- RI_CMR3 values (**) ------------------------------------------------- */

/* PC[15:0]: Port C channel masking */

/* --- RI_CICR3 values (**) ------------------------------------------------ */

/* PC[15:0]: Port C channel identification for capture */

/* --- RI_ASMR4 values (**) ------------------------------------------------ */

/* PF[15:0]: Port F analog switch mode selection */

/* --- RI_CMR4 values (**) ------------------------------------------------- */

/* PF[15:0]: Port F channel masking */

/* --- RI_CICR4 values (**) ------------------------------------------------ */

/* PF[15:0]: Port F channel identification for capture */

/* --- RI_ASMR5 values (**) ------------------------------------------------ */

/* PG[15:0]: Port G analog switch mode selection */

/* --- RI_CMR5 values (**) ------------------------------------------------- */

/* PG[15:0]: Port G channel masking */

/* --- RI_CICR5 values (**) ------------------------------------------------ */

/* PG[15:0]: Port G channel identification for capture */

/* --- Port definitions ---------------------------------------------------- */

enum {
	RI_PA = (0 << 16),
	RI_PB = (1 << 16),
	RI_PC = (2 << 16),
	RI_PD = (3 << 16),
	RI_PE = (4 << 16),
	RI_PF = (5 << 16),
	RI_PG = (6 << 16),
	RI_PH = (7 << 16),
	RI_OTHER = (8 << 16)
};

enum {
	RI_PA0 = RI_PA | (1 << 0),
	RI_PA1 = RI_PA | (1 << 1),
	RI_PA2 = RI_PA | (1 << 2),
	RI_PA3 = RI_PA | (1 << 3),
	RI_PA4 = RI_PA | (1 << 4),
	RI_PA5 = RI_PA | (1 << 5),
	RI_PA6 = RI_PA | (1 << 6),
	RI_PA7 = RI_PA | (1 << 7),
	RI_PA8 = RI_PA | (1 << 8),
	RI_PA9 = RI_PA | (1 << 9),
	RI_PA10 = RI_PA | (1 << 10),
	RI_PA11 = RI_PA | (1 << 11),
	RI_PA12 = RI_PA | (1 << 12),
	RI_PA13 = RI_PA | (1 << 13),
	RI_PA14 = RI_PA | (1 << 14),
	RI_PA15 = RI_PA | (1 << 15),
	RI_PA_ALL = RI_PA | 0xffff,
	RI_PANONE = 0
};

enum {
	RI_PB0 = RI_PB | (1 << 0),
	RI_PB1 = RI_PB | (1 << 1),
	RI_PB2 = RI_PB | (1 << 2),
	RI_PB3 = RI_PB | (1 << 3),
	RI_PB4 = RI_PB | (1 << 4),
	RI_PB5 = RI_PB | (1 << 5),
	RI_PB6 = RI_PB | (1 << 6),
	RI_PB7 = RI_PB | (1 << 7),
	RI_PB8 = RI_PB | (1 << 8),
	RI_PB9 = RI_PB | (1 << 9),
	RI_PB10 = RI_PB | (1 << 10),
	RI_PB11 = RI_PB | (1 << 11),
	RI_PB12 = RI_PB | (1 << 12),
	RI_PB13 = RI_PB | (1 << 13),
	RI_PB14 = RI_PB | (1 << 14),
	RI_PB15 = RI_PB | (1 << 15),
	RI_PB_ALL = RI_PB | 0xffff,
	RI_PBNONE = 0
};

enum {
	RI_PC0 = RI_PC | (1 << 0),
	RI_PC1 = RI_PC | (1 << 1),
	RI_PC2 = RI_PC | (1 << 2),
	RI_PC3 = RI_PC | (1 << 3),
	RI_PC4 = RI_PC | (1 << 4),
	RI_PC5 = RI_PC | (1 << 5),
	RI_PC6 = RI_PC | (1 << 6),
	RI_PC7 = RI_PC | (1 << 7),
	RI_PC8 = RI_PC | (1 << 8),
	RI_PC9 = RI_PC | (1 << 9),
	RI_PC10 = RI_PC | (1 << 10),
	RI_PC11 = RI_PC | (1 << 11),
	RI_PC12 = RI_PC | (1 << 12),
	RI_PC13 = RI_PC | (1 << 13),
	RI_PC14 = RI_PC | (1 << 14),
	RI_PC15 = RI_PC | (1 << 15),
	RI_PC_ALL = RI_PC | 0xffff,
	RI_PCNONE = 0
};

enum {
	RI_PD0 = RI_PD | (1 << 0),
	RI_PD1 = RI_PD | (1 << 1),
	RI_PD2 = RI_PD | (1 << 2),
	RI_PD3 = RI_PD | (1 << 3),
	RI_PD4 = RI_PD | (1 << 4),
	RI_PD5 = RI_PD | (1 << 5),
	RI_PD6 = RI_PD | (1 << 6),
	RI_PD7 = RI_PD | (1 << 7),
	RI_PD8 = RI_PD | (1 << 8),
	RI_PD9 = RI_PD | (1 << 9),
	RI_PD10 = RI_PD | (1 << 10),
	RI_PD11 = RI_PD | (1 << 11),
	RI_PD12 = RI_PD | (1 << 12),
	RI_PD13 = RI_PD | (1 << 13),
	RI_PD14 = RI_PD | (1 << 14),
	RI_PD15 = RI_PD | (1 << 15),
	RI_PD_ALL = RI_PD | 0xffff,
	RI_PDNONE = 0
};

enum {
	RI_PE0 = RI_PE | (1 << 0),
	RI_PE1 = RI_PE | (1 << 1),
	RI_PE2 = RI_PE | (1 << 2),
	RI_PE3 = RI_PE | (1 << 3),
	RI_PE4 = RI_PE | (1 << 4),
	RI_PE5 = RI_PE | (1 << 5),
	RI_PE6 = RI_PE | (1 << 6),
	RI_PE7 = RI_PE | (1 << 7),
	RI_PE8 = RI_PE | (1 << 8),
	RI_PE9 = RI_PE | (1 << 9),
	RI_PE10 = RI_PE | (1 << 10),
	RI_PE11 = RI_PE | (1 << 11),
	RI_PE12 = RI_PE | (1 << 12),
	RI_PE13 = RI_PE | (1 << 13),
	RI_PE14 = RI_PE | (1 << 14),
	RI_PE15 = RI_PE | (1 << 15),
	RI_PE_ALL = RI_PE | 0xffff,
	RI_PENONE = 0
};

enum {
	RI_PF0 = RI_PF | (1 << 0),
	RI_PF1 = RI_PF | (1 << 1),
	RI_PF2 = RI_PF | (1 << 2),
	RI_PF3 = RI_PF | (1 << 3),
	RI_PF4 = RI_PF | (1 << 4),
	RI_PF5 = RI_PF | (1 << 5),
	RI_PF6 = RI_PF | (1 << 6),
	RI_PF7 = RI_PF | (1 << 7),
	RI_PF8 = RI_PF | (1 << 8),
	RI_PF9 = RI_PF | (1 << 9),
	RI_PF10 = RI_PF | (1 << 10),
	RI_PF11 = RI_PF | (1 << 11),
	RI_PF12 = RI_PF | (1 << 12),
	RI_PF13 = RI_PF | (1 << 13),
	RI_PF14 = RI_PF | (1 << 14),
	RI_PF15 = RI_PF | (1 << 15),
	RI_PF_ALL = RI_PF | 0xffff,
	RI_PFNONE = 0
};

enum {
	RI_PG0 = RI_PG | (1 << 0),
	RI_PG1 = RI_PG | (1 << 1),
	RI_PG2 = RI_PG | (1 << 2),
	RI_PG3 = RI_PG | (1 << 3),
	RI_PG4 = RI_PG | (1 << 4),
	RI_PG5 = RI_PG | (1 << 5),
	RI_PG6 = RI_PG | (1 << 6),
	RI_PG7 = RI_PG | (1 << 7),
	RI_PG8 = RI_PG | (1 << 8),
	RI_PG9 = RI_PG | (1 << 9),
	RI_PG10 = RI_PG | (1 << 10),
	RI_PG11 = RI_PG | (1 << 11),
	RI_PG12 = RI_PG | (1 << 12),
	RI_PG13 = RI_PG | (1 << 13),
	RI_PG14 = RI_PG | (1 << 14),
	RI_PG15 = RI_PG | (1 << 15),
	RI_PG_ALL = RI_PG | 0xffff,
	RI_PGNONE = 0
};

#define RI_CONCAT(port, num) RI_P##port##num

#define RI_OR(port, b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11,	\
	      b12, b13, b14, b15, ...)					\
	(RI_CONCAT(port, b0) | RI_CONCAT(port, b1) |	\
	 RI_CONCAT(port, b2) | RI_CONCAT(port, b3) |	\
	 RI_CONCAT(port, b4) | RI_CONCAT(port, b5) |	\
	 RI_CONCAT(port, b6) | RI_CONCAT(port, b7) |	\
	 RI_CONCAT(port, b8) | RI_CONCAT(port, b9) |	\
	 RI_CONCAT(port, b10) | RI_CONCAT(port, b11) |	\
	 RI_CONCAT(port, b12) | RI_CONCAT(port, b13) |	\
	 RI_CONCAT(port, b14) | RI_CONCAT(port, b15))

#define RI_PA(...) RI_OR(A, __VA_ARGS__, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE)

#define RI_PB(...) RI_OR(B, __VA_ARGS__, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE)

#define RI_PC(...) RI_OR(C, __VA_ARGS__, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE)

#define RI_PD(...) RI_OR(D, __VA_ARGS__, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE)

#define RI_PE(...) RI_OR(E, __VA_ARGS__, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE)

#define RI_PF(...) RI_OR(F, __VA_ARGS__, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE)

#define RI_PG(...) RI_OR(G, __VA_ARGS__, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			 NONE, NONE, NONE, NONE)

/* --- Analog switch definitions ------------------------------------------- */

#define RI_CH0				RI_PA0
#define RI_CH1				RI_PA1
#define RI_CH2				RI_PA2
#define RI_CH3				RI_PA3
#define RI_CH4				RI_PA4
#define RI_CH5				RI_PA5
#define RI_CH6				RI_PA6
#define RI_CH7				RI_PA7
#define RI_CH8				RI_PB0
#define RI_CH9				RI_PB1
#define RI_CH10				RI_PC0
#define RI_CH11				RI_PC1
#define RI_CH12				RI_PC2
#define RI_CH13				RI_PC3
#define RI_CH14				RI_PC4
#define RI_CH15				RI_PC5

#define RI_CH18				RI_PB12
#define RI_CH19				RI_PB13
#define RI_CH20				RI_PB14
#define RI_CH21				RI_PB15
#define RI_CH22				RI_PE7
#define RI_CH23				RI_PE8
#define RI_CH24				RI_PE9
#define RI_CH25				RI_PE10

#define RI_CH27				RI_PF6
#define RI_CH28				RI_PF7
#define RI_CH29				RI_PF8
#define RI_CH30				RI_PF9
#define RI_CH31				RI_PF10

#define RI_GR1_1			RI_PA0
#define RI_GR1_2			RI_PA1
#define RI_GR1_3			RI_PA2
#define RI_GR1_4			RI_PA3

#define RI_GR2_1			RI_PA6
#define RI_GR2_2			RI_PA7
#define RI_GR3_1			RI_PB0
#define RI_GR3_2			RI_PB1
#define RI_GR8_1			RI_PC0
#define RI_GR8_2			RI_PC1
#define RI_GR8_3			RI_PC2
#define RI_GR8_4			RI_PC3
#define RI_GR9_1			RI_PC4
#define RI_CH9_2			RI_PC5

#define RI_CH0B				RI_PB2
#define RI_CH1B				RI_PF11
#define RI_CH2B				RI_PF12
#define RI_CH3B				RI_PF13
#define RI_CH6B				RI_PF14
#define RI_CH7B				RI_PF15
#define RI_CH8B				RI_PG0
#define RI_CH9B				RI_PG1
#define RI_CH10B			RI_PG2
#define RI_CH11B			RI_PG3
#define RI_CH12B			RI_PG4

#define RI_GR7_1			RI_PB12
#define RI_GR7_2			RI_PB13
#define RI_GR7_3			RI_PB14
#define RI_GR7_4			RI_PB15

#define RI_GR11_1			RI_PF6
#define RI_GR11_2			RI_PF7
#define RI_GR11_3			RI_PF8
#define RI_GR11_4			RI_PF9
#define RI_GR11_5			RI_PF10

#define RI_GR10_1			RI_PC6
#define RI_GR10_2			RI_PC7
#define RI_GR10_3			RI_PC8
#define RI_GR10_4			RI_PC9
#define RI_GR6_1			RI_PB4
#define RI_GR6_2			RI_PB5
#define RI_GR5_1			RI_PA13
#define RI_GR5_2			RI_PA14
#define RI_GR5_3			RI_PA15
#define RI_GR4_1			RI_PA8
#define RI_GR4_2			RI_PA9
#define RI_GR4_3			RI_PA10

#define RI_GR3_3			RI_PB2
#define RI_GR3_4			RI_PF11
#define RI_GR3_5			RI_PF12
#define RI_GR9_3			RI_PF13
#define RI_GR9_4			RI_PF14
#define RI_GR2_3			RI_PF15
#define RI_GR2_4			RI_PG0
#define RI_GR2_5			RI_PG1
#define RI_GR7_5			RI_PG2
#define RI_GR7_6			RI_PG3
#define RI_GR7_7			RI_PG4
#define RI_GR6_3			RI_PB6
#define RI_GR6_4			RI_PB7

enum {
	RI_VCOMP = RI_OTHER | (1 << 0),
	RI_ADC = RI_OTHER | (1 << 1)
};

/* --- Function prototypes ------------------------------------------------- */

/* Timer */
typedef enum {
	RI_TIM_NONE,
	RI_TIM2,
	RI_TIM3,
	RI_TIM4
} ri_tim_t;

void ri_set_tim_ic(ri_tim_t tim, int ic1, int ic2, int ic3, int ic4);
void ri_close_analog_switch(int n, int *portbits);
void ri_open_analog_switch(int n, int *portbits);
void ri_disable_hysteresis(int portbits);
void ri_enable_hysteresis(int portbits);
