/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Uwe Hermann <uwe@hermann-uwe.de>
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
 * 19 Real-time clock (RTC)
 */

/*
 * Note: Registers and bits marked (**) only exist in high and medium+ density
 * devices.
 */

/* --- RTC registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		RTC_TR		RTC time register
 * 0x04		RTC_DR		RTC date register
 * 0x08		RTC_CR		RTC control register
 * 0x0c		RTC_ISR		RTC initialization and status register
 * 0x10		RTC_PRER	RTC prescaler register
 * 0x14		RTC_WUTR	RTC wakeup timer register
 * 0x18		RTC_CALIBR	RTC calibration register
 * 0x1c		RTC_ALRMAR	RTC alarm A register
 * 0x20		RTC_ALRMBR	RTC alarm B register
 * 0x24		RTC_WPR		RTC write protection register
 * 0x28		RTC_SSR		RTC sub second register
 * 0x2c		RTC_SHIFTR	RTC shift control register
 * 0x30		RTC_TSTR	RTC time stamp time register
 * 0x34		RTC_TSDR	RTC time stamp date register
 * 0x38		RTC_TSSSR	RTC timestamp sub second register
 * 0x3c		RTC_CALR	RTC calibration register
 * 0x40		RTC_TAFCR	RTC tamper and alternate function configuration
 *					register
 * 0x44		RTC_ALRMASSR	RTC alarm A sub second register
 * 0x48		RTC_ALRMBSSR	RTC alarm B sub second register
 * 0x50		RTC_BKP0R	RTC backup registers
 *   |
 * 0xcc		RTC_BKP31R
 */

#define RTC_TR				MMIO32(RTC_BASE + 0x00)
#define RTC_DR				MMIO32(RTC_BASE + 0x04)
#define RTC_CR				MMIO32(RTC_BASE + 0x08)
#define RTC_ISR				MMIO32(RTC_BASE + 0x0c)
#define RTC_PRER			MMIO32(RTC_BASE + 0x10)
#define RTC_WUTR			MMIO32(RTC_BASE + 0x14)
#define RTC_CALIBR			MMIO32(RTC_BASE + 0x18)
#define RTC_ALRMAR			MMIO32(RTC_BASE + 0x1c)
#define RTC_ALRMBR			MMIO32(RTC_BASE + 0x20)
#define RTC_WPR				MMIO32(RTC_BASE + 0x24)
#define RTC_SSR				MMIO32(RTC_BASE + 0x28)	/* (**) */
#define RTC_SHIFTR			MMIO32(RTC_BASE + 0x2c)	/* (**) */
#define RTC_TSTR			MMIO32(RTC_BASE + 0x30)
#define RTC_TSDR			MMIO32(RTC_BASE + 0x34)
#define RTC_TSSSR			MMIO32(RTC_BASE + 0x38)	/* (**) */
#define RTC_CALR			MMIO32(RTC_BASE + 0x3c)	/* (**) */
#define RTC_TAFCR			MMIO32(RTC_BASE + 0x40)
#define RTC_ALRMASSR			MMIO32(RTC_BASE + 0x44)	/* (**) */
#define RTC_ALRMBSSR			MMIO32(RTC_BASE + 0x48)	/* (**) */
#define RTC_BKP0R			MMIO32(RTC_BASE + 0x50)
#define RTC_BKP1R			MMIO32(RTC_BASE + 0x54)
#define RTC_BKP2R			MMIO32(RTC_BASE + 0x58)
#define RTC_BKP3R			MMIO32(RTC_BASE + 0x5c)
#define RTC_BKP4R			MMIO32(RTC_BASE + 0x60)
#define RTC_BKP5R			MMIO32(RTC_BASE + 0x64)
#define RTC_BKP6R			MMIO32(RTC_BASE + 0x68)
#define RTC_BKP7R			MMIO32(RTC_BASE + 0x6c)
#define RTC_BKP8R			MMIO32(RTC_BASE + 0x70)
#define RTC_BKP9R			MMIO32(RTC_BASE + 0x74)
#define RTC_BKP10R			MMIO32(RTC_BASE + 0x78)
#define RTC_BKP11R			MMIO32(RTC_BASE + 0x7c)
#define RTC_BKP12R			MMIO32(RTC_BASE + 0x80)
#define RTC_BKP13R			MMIO32(RTC_BASE + 0x84)
#define RTC_BKP14R			MMIO32(RTC_BASE + 0x88)
#define RTC_BKP15R			MMIO32(RTC_BASE + 0x8c)
#define RTC_BKP16R			MMIO32(RTC_BASE + 0x90)
#define RTC_BKP17R			MMIO32(RTC_BASE + 0x94)
#define RTC_BKP18R			MMIO32(RTC_BASE + 0x98)
#define RTC_BKP19R			MMIO32(RTC_BASE + 0x9c)
#define RTC_BKP20R			MMIO32(RTC_BASE + 0xa0)	/* (**) */
#define RTC_BKP21R			MMIO32(RTC_BASE + 0xa4)	/* (**) */
#define RTC_BKP22R			MMIO32(RTC_BASE + 0xa8)	/* (**) */
#define RTC_BKP23R			MMIO32(RTC_BASE + 0xac)	/* (**) */
#define RTC_BKP24R			MMIO32(RTC_BASE + 0xb0)	/* (**) */
#define RTC_BKP25R			MMIO32(RTC_BASE + 0xb4)	/* (**) */
#define RTC_BKP26R			MMIO32(RTC_BASE + 0xb8)	/* (**) */
#define RTC_BKP27R			MMIO32(RTC_BASE + 0xbc)	/* (**) */
#define RTC_BKP28R			MMIO32(RTC_BASE + 0xc0)	/* (**) */
#define RTC_BKP29R			MMIO32(RTC_BASE + 0xc4)	/* (**) */
#define RTC_BKP30R			MMIO32(RTC_BASE + 0xc8)	/* (**) */
#define RTC_BKP31R			MMIO32(RTC_BASE + 0xcc) /* (**) */

/* --- RTC_TR values ------------------------------------------------------- */

#define RTC_TR_PM			(1 << 22)
#define RTC_TR_HT1			(1 << 21)
#define RTC_TR_HT0			(1 << 20)
#define RTC_TR_HU3			(1 << 19)
#define RTC_TR_HU2			(1 << 18)
#define RTC_TR_HU1			(1 << 17)
#define RTC_TR_HU0			(1 << 16)
#define RTC_TR_MNT2			(1 << 14)
#define RTC_TR_MNT1			(1 << 13)
#define RTC_TR_MNT0			(1 << 12)
#define RTC_TR_MNU3			(1 << 11)
#define RTC_TR_MNU2			(1 << 10)
#define RTC_TR_MNU1			(1 << 9)
#define RTC_TR_MNU0			(1 << 8)
#define RTC_TR_ST2			(1 << 6)
#define RTC_TR_ST1			(1 << 5)
#define RTC_TR_ST0			(1 << 4)
#define RTC_TR_SU3			(1 << 3)
#define RTC_TR_SU2			(1 << 2)
#define RTC_TR_SU1			(1 << 1)
#define RTC_TR_SU0			(1 << 0)

/* --- RTC_DR values ------------------------------------------------------- */

#define RTC_DR_YT3			(1 << 23)
#define RTC_DR_YT2			(1 << 22)
#define RTC_DR_YT1			(1 << 21)
#define RTC_DR_YT0			(1 << 20)
#define RTC_DR_YU3			(1 << 19)
#define RTC_DR_YU2			(1 << 18)
#define RTC_DR_YU1			(1 << 17)
#define RTC_DR_YU0			(1 << 16)
#define RTC_DR_WDU2			(1 << 15)
#define RTC_DR_WDU1			(1 << 14)
#define RTC_DR_WDU0			(1 << 13)
#define RTC_DR_MT			(1 << 12)
#define RTC_DR_MU3			(1 << 11)
#define RTC_DR_MU2			(1 << 10)
#define RTC_DR_MU1			(1 << 9)
#define RTC_DR_MU0			(1 << 8)
#define RTC_DR_DT1			(1 << 5)
#define RTC_DR_DT0			(1 << 4)
#define RTC_DR_DU3			(1 << 3)
#define RTC_DR_DU2			(1 << 2)
#define RTC_DR_DU1			(1 << 1)
#define RTC_DR_DU0			(1 << 0)

/* --- RTC_CR values ------------------------------------------------------- */

#define RTC_CR_COE			(1 << 23)
#define RTC_CR_OSEL1			(1 << 22)
#define RTC_CR_OSEL0			(1 << 21)
#define RTC_CR_POL			(1 << 20)
#define RTC_CR_COSEL			(1 << 19)	/* (**) */
#define RTC_CR_BKP			(1 << 18)
#define RTC_CR_SUB1H			(1 << 17)
#define RTC_CR_ADD1H			(1 << 16)
#define RTC_CR_TSIE			(1 << 15)
#define RTC_CR_WUTIE			(1 << 14)
#define RTC_CR_ALRBIE			(1 << 13)
#define RTC_CR_ALRAIE			(1 << 12)
#define RTC_CR_TSE			(1 << 11)
#define RTC_CR_WUTE			(1 << 10)
#define RTC_CR_ALRBE			(1 << 9)
#define RTC_CR_ALRAE			(1 << 8)
#define RTC_CR_DCE			(1 << 7)
#define RTC_CR_FMT			(1 << 6)
#define RTC_CR_BYPSHAD			(1 << 5)	/* (**) */
#define RTC_CR_REFCKON			(1 << 4)
#define RTC_CR_TSEDGE			(1 << 3)
#define RTC_CR_WUCKSEL2			(1 << 2)
#define RTC_CR_WUCKSEL1			(1 << 1)
#define RTC_CR_WUCKSEL0			(1 << 0)

/* Output */
#define RTC_CR_OSEL_DISABLE		(0 << 21)
#define RTC_CR_OSEL_ALARM_A		(1 << 21)
#define RTC_CR_OSEL_ALARM_B		(2 << 21)
#define RTC_CR_OSEL_WAKEUP		(3 << 21)

/* Wakeup clock */
#define RTC_CR_WUCKSEL_RTC_16		0
#define RTC_CR_WUCKSEL_RTC_8		1
#define RTC_CR_WUCKSEL_RTC_4		2
#define RTC_CR_WUCKSEL_RTC_2		3
#define RTC_CR_WUCKSEL_CK_SPRE		4
#define RTC_CR_WUCKSEL_CK_SPRE_0X10000	6

/* --- RTC_ISR values ------------------------------------------------------ */

#define RTC_ISR_RECALPF			(1 << 16)
#define RTC_ISR_TAMP3F			(1 << 15)
#define RTC_ISR_TAMP2F			(1 << 14)
#define RTC_ISR_TAMP1F			(1 << 13)
#define RTC_ISR_TSOVF			(1 << 12)
#define RTC_ISR_TSF			(1 << 11)
#define RTC_ISR_WUTF			(1 << 10)
#define RTC_ISR_ALRBF			(1 << 9)
#define RTC_ISR_ALRAF			(1 << 8)
#define RTC_ISR_INIT			(1 << 7)
#define RTC_ISR_INITF			(1 << 6)
#define RTC_ISR_RSF			(1 << 5)
#define RTC_ISR_INITS			(1 << 4)
#define RTC_ISR_SHPF			(1 << 3)
#define RTC_ISR_WUTWF			(1 << 2)
#define RTC_ISR_ALRBWF			(1 << 1)
#define RTC_ISR_ALRAWF			(1 << 0)

/* --- RTC_PRER values ----------------------------------------------------- */

#define RTC_PRER_PREDIV_A6		(1 << 22)
#define RTC_PRER_PREDIV_A5		(1 << 21)
#define RTC_PRER_PREDIV_A4		(1 << 20)
#define RTC_PRER_PREDIV_A3		(1 << 19)
#define RTC_PRER_PREDIV_A2		(1 << 18)
#define RTC_PRER_PREDIV_A1		(1 << 17)
#define RTC_PRER_PREDIV_A0		(1 << 16)
#define RTC_PRER_PREDIV_S14		(1 << 14)	/* (**) */
#define RTC_PRER_PREDIV_S13		(1 << 13)	/* (**) */
#define RTC_PRER_PREDIV_S12		(1 << 12)
#define RTC_PRER_PREDIV_S11		(1 << 11)
#define RTC_PRER_PREDIV_S10		(1 << 10)
#define RTC_PRER_PREDIV_S9		(1 << 9)
#define RTC_PRER_PREDIV_S8		(1 << 8)
#define RTC_PRER_PREDIV_S7		(1 << 7)
#define RTC_PRER_PREDIV_S6		(1 << 6)
#define RTC_PRER_PREDIV_S5		(1 << 5)
#define RTC_PRER_PREDIV_S4		(1 << 4)
#define RTC_PRER_PREDIV_S3		(1 << 3)
#define RTC_PRER_PREDIV_S2		(1 << 2)
#define RTC_PRER_PREDIV_S1		(1 << 1)
#define RTC_PRER_PREDIV_S0		(1 << 0)

/* --- RTC_WUTR values ----------------------------------------------------- */

/* RTC_WUTR[15:0]: WUT[15:0]: Wakeup auto-reload value bits */

/* --- RTC_CALIBR values --------------------------------------------------- */

#define RTC_CALIBR_DCS			(1 << 7)
#define RTC_CALIBR_DC4			(1 << 4)
#define RTC_CALIBR_DC3			(1 << 3)
#define RTC_CALIBR_DC2			(1 << 2)
#define RTC_CALIBR_DC1			(1 << 1)
#define RTC_CALIBR_DC0			(1 << 0)

/* --- RTC_ALRMAR values --------------------------------------------------- */

#define RTC_ALRMAR_MSK4			(1 << 31)
#define RTC_ALRMAR_WDSEL		(1 << 30)
#define RTC_ALRMAR_DT1			(1 << 29)
#define RTC_ALRMAR_DT0			(1 << 28)
#define RTC_ALRMAR_DU3			(1 << 27)
#define RTC_ALRMAR_DU2			(1 << 26)
#define RTC_ALRMAR_DU1			(1 << 25)
#define RTC_ALRMAR_DU0			(1 << 24)
#define RTC_ALRMAR_MSK3			(1 << 23)
#define RTC_ALRMAR_PM			(1 << 22)
#define RTC_ALRMAR_HT1			(1 << 21)
#define RTC_ALRMAR_HT0			(1 << 20)
#define RTC_ALRMAR_HU3			(1 << 19)
#define RTC_ALRMAR_HU2			(1 << 18)
#define RTC_ALRMAR_HU1			(1 << 17)
#define RTC_ALRMAR_HU0			(1 << 16)
#define RTC_ALRMAR_MSK2			(1 << 15)
#define RTC_ALRMAR_MNT2			(1 << 14)
#define RTC_ALRMAR_MNT1			(1 << 13)
#define RTC_ALRMAR_MNT0			(1 << 12)
#define RTC_ALRMAR_MNU3			(1 << 11)
#define RTC_ALRMAR_MNU2			(1 << 10)
#define RTC_ALRMAR_MNU1			(1 << 9)
#define RTC_ALRMAR_MNU0			(1 << 8)
#define RTC_ALRMAR_MSK1			(1 << 7)
#define RTC_ALRMAR_ST2			(1 << 6)
#define RTC_ALRMAR_ST1			(1 << 5)
#define RTC_ALRMAR_ST0			(1 << 4)
#define RTC_ALRMAR_SU3			(1 << 3)
#define RTC_ALRMAR_SU2			(1 << 2)
#define RTC_ALRMAR_SU1			(1 << 1)
#define RTC_ALRMAR_SU0			(1 << 0)

/* --- RTC_ALRMBR values --------------------------------------------------- */

#define RTC_ALRMBR_MSK4			(1 << 31)
#define RTC_ALRMBR_WDSEL		(1 << 30)
#define RTC_ALRMBR_DT1			(1 << 29)
#define RTC_ALRMBR_DT0			(1 << 28)
#define RTC_ALRMBR_DU3			(1 << 27)
#define RTC_ALRMBR_DU2			(1 << 26)
#define RTC_ALRMBR_DU1			(1 << 25)
#define RTC_ALRMBR_DU0			(1 << 24)
#define RTC_ALRMBR_MSK3			(1 << 23)
#define RTC_ALRMBR_PM			(1 << 22)
#define RTC_ALRMBR_HT1			(1 << 21)
#define RTC_ALRMBR_HT0			(1 << 20)
#define RTC_ALRMBR_HU3			(1 << 19)
#define RTC_ALRMBR_HU2			(1 << 18)
#define RTC_ALRMBR_HU1			(1 << 17)
#define RTC_ALRMBR_HU0			(1 << 16)
#define RTC_ALRMBR_MSK2			(1 << 15)
#define RTC_ALRMBR_MNT2			(1 << 14)
#define RTC_ALRMBR_MNT1			(1 << 13)
#define RTC_ALRMBR_MNT0			(1 << 12)
#define RTC_ALRMBR_MNU3			(1 << 11)
#define RTC_ALRMBR_MNU2			(1 << 10)
#define RTC_ALRMBR_MNU1			(1 << 9)
#define RTC_ALRMBR_MNU0			(1 << 8)
#define RTC_ALRMBR_MSK1			(1 << 7)
#define RTC_ALRMBR_ST2			(1 << 6)
#define RTC_ALRMBR_ST1			(1 << 5)
#define RTC_ALRMBR_ST0			(1 << 4)
#define RTC_ALRMBR_SU3			(1 << 3)
#define RTC_ALRMBR_SU2			(1 << 2)
#define RTC_ALRMBR_SU1			(1 << 1)
#define RTC_ALRMBR_SU0			(1 << 0)

/* --- RTC_WPR values ------------------------------------------------------ */

/* RTC_WPR[7:0]: KEY[7:0]: Write protection key */

/* --- RTC_SSR values (**) ------------------------------------------------- */

/* RTC_SSR[15:0]: SS[15:0]: Sub second value */

/* --- RTC_SHIFTR values (**) ---------------------------------------------- */

#define RTC_SHIFTR_ADD1S		(1 << 31)

/* RTC_SHIFTR[14:0]: SUBFS[14:0]: Subtract a fraction of a second */

/* --- RTC_TSTR values ----------------------------------------------------- */

#define RTC_TSTR_PM			(1 << 22)
#define RTC_TSTR_HT1			(1 << 21)
#define RTC_TSTR_HT0			(1 << 20)
#define RTC_TSTR_HU3			(1 << 19)
#define RTC_TSTR_HU2			(1 << 18)
#define RTC_TSTR_HU1			(1 << 17)
#define RTC_TSTR_HU0			(1 << 16)
#define RTC_TSTR_MNT2			(1 << 14)
#define RTC_TSTR_MNT1			(1 << 13)
#define RTC_TSTR_MNT0			(1 << 12)
#define RTC_TSTR_MNU3			(1 << 11)
#define RTC_TSTR_MNU2			(1 << 10)
#define RTC_TSTR_MNU1			(1 << 9)
#define RTC_TSTR_MNU0			(1 << 8)
#define RTC_TSTR_ST2			(1 << 6)
#define RTC_TSTR_ST1			(1 << 5)
#define RTC_TSTR_ST0			(1 << 4)
#define RTC_TSTR_SU3			(1 << 3)
#define RTC_TSTR_SU2			(1 << 2)
#define RTC_TSTR_SU1			(1 << 1)
#define RTC_TSTR_SU0			(1 << 0)

/* --- RTC_TSDR values ----------------------------------------------------- */

#define RTC_TSDR_WDU2			(1 << 15)
#define RTC_TSDR_WDU1			(1 << 14)
#define RTC_TSDR_WDU0			(1 << 13)
#define RTC_TSDR_MT			(1 << 12)
#define RTC_TSDR_MU3			(1 << 11)
#define RTC_TSDR_MU2			(1 << 10)
#define RTC_TSDR_MU1			(1 << 9)
#define RTC_TSDR_MU0			(1 << 8)
#define RTC_TSDR_DT1			(1 << 5)
#define RTC_TSDR_DT0			(1 << 4)
#define RTC_TSDR_DU3			(1 << 3)
#define RTC_TSDR_DU2			(1 << 2)
#define RTC_TSDR_DU1			(1 << 1)
#define RTC_TSDR_DU0			(1 << 0)

/* --- RTC_TSSSR values (**) ----------------------------------------------- */

/* RTC_TSSSR[15:0]: SS[15:0]: Sub second value */

/* --- RTC_CALR values (**) ------------------------------------------------ */

#define RTC_CALR_CALP			(1 << 15)
#define RTC_CALR_CALW8			(1 << 14)
#define RTC_CALR_CALW16			(1 << 13)
#define RTC_CALR_CALM8			(1 << 8)
#define RTC_CALR_CALM7			(1 << 7)
#define RTC_CALR_CALM6			(1 << 6)
#define RTC_CALR_CALM5			(1 << 5)
#define RTC_CALR_CALM4			(1 << 4)
#define RTC_CALR_CALM3			(1 << 3)
#define RTC_CALR_CALM2			(1 << 2)
#define RTC_CALR_CALM1			(1 << 1)
#define RTC_CALR_CALM0			(1 << 0)

/* --- RTC_TAFCR values ---------------------------------------------------- */

#define RTC_TAFCR_ALARMOUTTYPE		(1 << 18)
#define RTC_TAFCR_TAMPPUDIS		(1 << 15)	/* (**) */
#define RTC_TAFCR_TAMPPRCH1		(1 << 14)	/* (**) */
#define RTC_TAFCR_TAMPPRCH0		(1 << 13)	/* (**) */
#define RTC_TAFCR_TAMPFLT1		(1 << 12)	/* (**) */
#define RTC_TAFCR_TAMPFLT0		(1 << 11)	/* (**) */
#define RTC_TAFCR_TAMPFREQ2		(1 << 10)	/* (**) */
#define RTC_TAFCR_TAMPFREQ1		(1 << 9)	/* (**) */
#define RTC_TAFCR_TAMPFREQ0		(1 << 8)	/* (**) */
#define RTC_TAFCR_TAMPTS		(1 << 7)	/* (**) */
#define RTC_TAFCR_TAMP3TRG		(1 << 6)
#define RTC_TAFCR_TAMP3E		(1 << 5)
#define RTC_TAFCR_TAMP2TRG		(1 << 4)	/* (**) */
#define RTC_TAFCR_TAMP2E		(1 << 3)	/* (**) */
#define RTC_TAFCR_TAMPIE		(1 << 2)
#define RTC_TAFCR_TAMP1TRG		(1 << 1)
#define RTC_TAFCR_TAMP1E		(1 << 0)

/* Tamper precharge duration (**) */
#define RTC_TAFCR_TAMPPRCH_1		(0 << 13)
#define RTC_TAFCR_TAMPPRCH_2		(1 << 13)
#define RTC_TAFCR_TAMPPRCH_4		(2 << 13)
#define RTC_TAFCR_TAMPPRCH_8		(3 << 13)

/* Tamper filter count (**) */
#define RTC_TAFCR_TAMPFLT_0		(0 << 11)
#define RTC_TAFCR_TAMPFLT_2		(1 << 11)
#define RTC_TAFCR_TAMPFLT_4		(2 << 11)
#define RTC_TAFCR_TAMPFLT_8		(3 << 11)

/* Tamper sampling frequency (**) */
#define RTC_TAFCR_TAMPFREQ_RTCCLK_32768	(0 << 8)
#define RTC_TAFCR_TAMPFREQ_RTCCLK_16384	(1 << 8)
#define RTC_TAFCR_TAMPFREQ_RTCCLK_8192	(2 << 8)
#define RTC_TAFCR_TAMPFREQ_RTCCLK_4096	(3 << 8)
#define RTC_TAFCR_TAMPFREQ_RTCCLK_2048	(4 << 8)
#define RTC_TAFCR_TAMPFREQ_RTCCLK_1024	(5 << 8)
#define RTC_TAFCR_TAMPFREQ_RTCCLK_512	(6 << 8)
#define RTC_TAFCR_TAMPFREQ_RTCCLK_256	(7 << 8)

/* --- RTC_ALRMASSR values (**) -------------------------------------------- */

#define RTC_ALRMASSR_MASKSS3		(1 << 27)
#define RTC_ALRMASSR_MASKSS2		(1 << 26)
#define RTC_ALRMASSR_MASKSS1		(1 << 25)
#define RTC_ALRMASSR_MASKSS0		(1 << 24)
#define RTC_ALRMASSR_SS14		(1 << 14)
#define RTC_ALRMASSR_SS13		(1 << 13)
#define RTC_ALRMASSR_SS12		(1 << 12)
#define RTC_ALRMASSR_SS11		(1 << 11)
#define RTC_ALRMASSR_SS10		(1 << 10)
#define RTC_ALRMASSR_SS9		(1 << 9)
#define RTC_ALRMASSR_SS8		(1 << 8)
#define RTC_ALRMASSR_SS7		(1 << 7)
#define RTC_ALRMASSR_SS6		(1 << 6)
#define RTC_ALRMASSR_SS5		(1 << 5)
#define RTC_ALRMASSR_SS4		(1 << 4)
#define RTC_ALRMASSR_SS3		(1 << 3)
#define RTC_ALRMASSR_SS2		(1 << 2)
#define RTC_ALRMASSR_SS1		(1 << 1)
#define RTC_ALRMASSR_SS0		(1 << 0)

/* Mask the most-significant bits starting at this bit */
#define RTC_ALRMASSR_MASKSS_NO		(0 << 24)
#define RTC_ALRMASSR_MASKSS_0		(1 << 24)
#define RTC_ALRMASSR_MASKSS_1_0		(2 << 24)
#define RTC_ALRMASSR_MASKSS_2_0		(3 << 24)
#define RTC_ALRMASSR_MASKSS_3_0		(4 << 24)
#define RTC_ALRMASSR_MASKSS_4_0		(5 << 24)
#define RTC_ALRMASSR_MASKSS_5_0		(6 << 24)
#define RTC_ALRMASSR_MASKSS_6_0		(7 << 24)
#define RTC_ALRMASSR_MASKSS_7_0		(8 << 24)
#define RTC_ALRMASSR_MASKSS_8_0		(9 << 24)
#define RTC_ALRMASSR_MASKSS_9_0		(10 << 24)
#define RTC_ALRMASSR_MASKSS_10_0	(11 << 24)
#define RTC_ALRMASSR_MASKSS_11_0	(12 << 24)
#define RTC_ALRMASSR_MASKSS_12_0	(13 << 24)
#define RTC_ALRMASSR_MASKSS_13_0	(14 << 24)
#define RTC_ALRMASSR_MASKSS_14_0	(15 << 24)

/* --- RTC_ALRMBSSR values (**) -------------------------------------------- */

#define RTC_ALRMBSSR_MASKSS3		(1 << 27)
#define RTC_ALRMBSSR_MASKSS2		(1 << 26)
#define RTC_ALRMBSSR_MASKSS1		(1 << 25)
#define RTC_ALRMBSSR_MASKSS0		(1 << 24)
#define RTC_ALRMBSSR_SS14		(1 << 14)
#define RTC_ALRMBSSR_SS13		(1 << 13)
#define RTC_ALRMBSSR_SS12		(1 << 12)
#define RTC_ALRMBSSR_SS11		(1 << 11)
#define RTC_ALRMBSSR_SS10		(1 << 10)
#define RTC_ALRMBSSR_SS9		(1 << 9)
#define RTC_ALRMBSSR_SS8		(1 << 8)
#define RTC_ALRMBSSR_SS7		(1 << 7)
#define RTC_ALRMBSSR_SS6		(1 << 6)
#define RTC_ALRMBSSR_SS5		(1 << 5)
#define RTC_ALRMBSSR_SS4		(1 << 4)
#define RTC_ALRMBSSR_SS3		(1 << 3)
#define RTC_ALRMBSSR_SS2		(1 << 2)
#define RTC_ALRMBSSR_SS1		(1 << 1)
#define RTC_ALRMBSSR_SS0		(1 << 0)

/* Mask the most-significant bits starting at this bit */
#define RTC_ALRMBSSR_MASKSS_NO		(0 << 24)
#define RTC_ALRMBSSR_MASKSS_0		(1 << 24)
#define RTC_ALRMBSSR_MASKSS_1_0		(2 << 24)
#define RTC_ALRMBSSR_MASKSS_2_0		(3 << 24)
#define RTC_ALRMBSSR_MASKSS_3_0		(4 << 24)
#define RTC_ALRMBSSR_MASKSS_4_0		(5 << 24)
#define RTC_ALRMBSSR_MASKSS_5_0		(6 << 24)
#define RTC_ALRMBSSR_MASKSS_6_0		(7 << 24)
#define RTC_ALRMBSSR_MASKSS_7_0		(8 << 24)
#define RTC_ALRMBSSR_MASKSS_8_0		(9 << 24)
#define RTC_ALRMBSSR_MASKSS_9_0		(10 << 24)
#define RTC_ALRMBSSR_MASKSS_10_0	(11 << 24)
#define RTC_ALRMBSSR_MASKSS_11_0	(12 << 24)
#define RTC_ALRMBSSR_MASKSS_12_0	(13 << 24)
#define RTC_ALRMBSSR_MASKSS_13_0	(14 << 24)
#define RTC_ALRMBSSR_MASKSS_14_0	(15 << 24)

/* --- RTC_BKPxR values (x = 0..19) ---------------------------------------- */
/*                      (x = 0..31) high and medium+ density devices         */
/*                      (x = 0..4)  value line devices                       */
/* RTC_BKPxR[31:0]: BKP[31:0]: Backup data */

/* --- Function prototypes ------------------------------------------------- */

/* Wakeup clock */
typedef enum {
	RTC_16 = 0,
	RTC_8 = 1,
	RTC_4 = 2,
	RTC_2 = 3,
	RTC_CK_SPRE = 4,
	RTC_CK_SPRE_0X10000 = 6
} rtc_wakeup_clock_t;

/* Alarm output(AFO_ALARM) */
typedef enum {
	RTC_OUTPUT_DISABLE = (0 << 21),
	RTC_OUTPUT_ALARMA = (1 << 21),
	RTC_OUTPUT_ALARMB = (2 << 21),
	RTC_OUTPUT_WAKEUP = (3 << 21)
} rtc_alarm_output_t;

/* Calibration output(AFO_CALIB) (**) */
typedef enum {
	RTC_CALIBRATION_UNKNOWN,
	RTC_512HZ,
	RTC_1HZ
} rtc_calibration_output_t;

/* Interrupt */
enum {
	RTC_ALARM_A = (1 << 8),
	RTC_ALARM_B = (1 << 9),
	RTC_WAKEUP_TIMER = (1 << 10),
	RTC_TIMESTAMP = (1 << 11),
	RTC_TIMESTAMP_OVERFLOW = (1 << 12),
	RTC_TAMPER1 = (1 << 13),
	RTC_TAMPER2 = (1 << 14),
	RTC_TAMPER3 = (1 << 15)
};

void rtc_unlock(void);
void rtc_lock(void);
void rtc_init(int prediv_a, int prediv_s, u32 date, u32 time, bool fmt, u32 dc);
void rtc_enable_daylight_saving_time(void);
void rtc_disable_daylight_saving_time(void);
void rtc_enable_alarm_a(u32 time, u32 ss);
void rtc_enable_alarm_b(u32 time, u32 ss);
void rtc_enable_wakeup_timer(rtc_wakeup_clock_t wucksel, int autoreload);
void rtc_get_calendar(u32 *date, u32 *time, u32 *ss);
void rtc_get_calendar_read_twice(u32 *date, u32 *time, u32 *ss);
void rtc_syncronize(u32 shift);
void rtc_enable_reference_clock_detection(void);
void rtc_disable_reference_clock_detection(void);
void rtc_set_smooth_digital_calibration(u32 cal);
void rtc_get_timestamp(u32 *date, u32 *time, u32 *ss);
void rtc_enable_interrupt(int interrupt);
void rtc_disable_interrupt(int interrupt);
int rtc_get_interrupt_mask(int interrupt);
int rtc_get_interrupt_status(int interrupt);
void rtc_clear_interrupt(int interrupt);
void rtc_set_afo_alarm(rtc_alarm_output_t osel, bool low_level, bool pushpull);
void rtc_enable_afo_calib(rtc_calibration_output_t cosel);
void rtc_disable_afo_calib(void);
void rtc_enable_timestamp(bool falling);
void rtc_disable_timestamp(void);
void rtc_set_tamper(u32 tamper);
