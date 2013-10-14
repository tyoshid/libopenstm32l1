/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
 * Copyright (C) 2009 Federico Ruiz-Ugalde <memeruiz at gmail dot com>
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
 * 5 Reset and clock control (RCC)
 */

/* Note: Bits marked (**) only exist in high and medium+ density devices. */
/* Note: Bits marked (***) only exist in high density devices. */
/* Note: Bits marked (****) only exist in STM32L16x devices. */

/* --- RCC registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		RCC_CR		Clock control register
 * 0x04		RCC_ICSCR	Internal clock sources calibration register
 * 0x08		RCC_CFGR	Clock configuration register
 * 0x0c		RCC_CIR		Clock interrupt register
 * 0x10		RCC_AHBRSTR	AHB peripheral reset register
 * 0x14		RCC_APB2RSTR	APB2 peripheral reset register
 * 0x18		RCC_APB1RSTR	APB1 peripheral reset register
 * 0x1c		RCC_AHBENR	AHB peripheral clock enable register
 * 0x20		RCC_APB2ENR	APB2 peripheral clock enable register
 * 0x24		RCC_APB1ENR	APB1 peripheral clock enable register
 * 0x28		RCC_AHBLPENR	AHB peripheral clock enable in low power mode
 *					register
 * 0x2c		RCC_APB2LPENR	APB2 peripheral clock enable in	low power mode
 *					register
 * 0x30		RCC_APB1LPENR	APB1 peripheral clock enable in low power mode
 *					register
 * 0x34		RCC_CSR		Control/status register
 */

#define RCC_CR					MMIO32(RCC_BASE + 0x00)
#define RCC_ICSCR				MMIO32(RCC_BASE + 0x04)
#define RCC_CFGR				MMIO32(RCC_BASE + 0x08)
#define RCC_CIR					MMIO32(RCC_BASE + 0x0c)
#define RCC_AHBRSTR				MMIO32(RCC_BASE + 0x10)
#define RCC_APB2RSTR				MMIO32(RCC_BASE + 0x14)
#define RCC_APB1RSTR				MMIO32(RCC_BASE + 0x18)
#define RCC_AHBENR				MMIO32(RCC_BASE + 0x1c)
#define RCC_APB2ENR				MMIO32(RCC_BASE + 0x20)
#define RCC_APB1ENR				MMIO32(RCC_BASE + 0x24)
#define RCC_AHBLPENR				MMIO32(RCC_BASE + 0x28)
#define RCC_APB2LPENR				MMIO32(RCC_BASE + 0x2c)
#define RCC_APB1LPENR				MMIO32(RCC_BASE + 0x30)
#define RCC_CSR					MMIO32(RCC_BASE + 0x34)

/* --- RCC_CR values ------------------------------------------------------- */

#define RCC_CR_RTCPRE1				(1 << 30)
#define RCC_CR_RTCPRE0				(1 << 29)
#define RCC_CR_CSSON				(1 << 28)
#define RCC_CR_PLLRDY				(1 << 25)
#define RCC_CR_PLLON				(1 << 24)
#define RCC_CR_HSEBYP				(1 << 18)
#define RCC_CR_HSERDY				(1 << 17)
#define RCC_CR_HSEON				(1 << 16)
#define RCC_CR_MSIRDY				(1 << 9)
#define RCC_CR_MSION				(1 << 8)
#define RCC_CR_HSIRDY				(1 << 1)
#define RCC_CR_HSION				(1 << 0)

/* RTC/LCD prescaler */
#define RCC_CR_RTCPRE_HSE_CLK_DIV2		(0 << 29)
#define RCC_CR_RTCPRE_HSE_CLK_DIV4		(1 << 29)
#define RCC_CR_RTCPRE_HSE_CLK_DIV8		(2 << 29)
#define RCC_CR_RTCPRE_HSE_CLK_DIV16		(3 << 29)

/* --- RCC_ICSCR values ---------------------------------------------------- */

#define RCC_ICSCR_MSITRIM7			(1 << 31)
#define RCC_ICSCR_MSITRIM6			(1 << 30)
#define RCC_ICSCR_MSITRIM5			(1 << 29)
#define RCC_ICSCR_MSITRIM4			(1 << 28)
#define RCC_ICSCR_MSITRIM3			(1 << 27)
#define RCC_ICSCR_MSITRIM2			(1 << 26)
#define RCC_ICSCR_MSITRIM1			(1 << 25)
#define RCC_ICSCR_MSITRIM0			(1 << 24)
#define RCC_ICSCR_MSICAL7			(1 << 23)
#define RCC_ICSCR_MSICAL6			(1 << 22)
#define RCC_ICSCR_MSICAL5			(1 << 21)
#define RCC_ICSCR_MSICAL4			(1 << 20)
#define RCC_ICSCR_MSICAL3			(1 << 19)
#define RCC_ICSCR_MSICAL2			(1 << 18)
#define RCC_ICSCR_MSICAL1			(1 << 17)
#define RCC_ICSCR_MSICAL0			(1 << 16)
#define RCC_ICSCR_MSIRANGE2			(1 << 15)
#define RCC_ICSCR_MSIRANGE1			(1 << 14)
#define RCC_ICSCR_MSIRANGE0			(1 << 13)
#define RCC_ICSCR_HSITRIM4			(1 << 12)
#define RCC_ICSCR_HSITRIM3			(1 << 11)
#define RCC_ICSCR_HSITRIM2			(1 << 10)
#define RCC_ICSCR_HSITRIM1			(1 << 9)
#define RCC_ICSCR_HSITRIM0			(1 << 8)
#define RCC_ICSCR_HSICAL7			(1 << 7)
#define RCC_ICSCR_HSICAL6			(1 << 6)
#define RCC_ICSCR_HSICAL5			(1 << 5)
#define RCC_ICSCR_HSICAL4			(1 << 4)
#define RCC_ICSCR_HSICAL3			(1 << 3)
#define RCC_ICSCR_HSICAL2			(1 << 2)
#define RCC_ICSCR_HSICAL1			(1 << 1)
#define RCC_ICSCR_HSICAL0			(1 << 0)

/* MSI clock ranges */
#define RCC_ICSCR_MSIRANGE_RANGE0		(0 << 13) /* 65.536 kHz */
#define RCC_ICSCR_MSIRANGE_RANGE1		(1 << 13) /* 131.072 kHz */
#define RCC_ICSCR_MSIRANGE_RANGE2		(2 << 13) /* 262.144 kHz */
#define RCC_ICSCR_MSIRANGE_RANGE3		(3 << 13) /* 524.288 kHz */
#define RCC_ICSCR_MSIRANGE_RANGE4		(4 << 13) /* 1.048 MHz */
#define RCC_ICSCR_MSIRANGE_RANGE5		(5 << 13) /* 2.097 MHz */
#define RCC_ICSCR_MSIRANGE_RANGE6		(6 << 13) /* 4.194 MHz */

/* --- RCC_CFGR values ----------------------------------------------------- */

#define RCC_CFGR_MCOPRE2			(1 << 30)
#define RCC_CFGR_MCOPRE1			(1 << 29)
#define RCC_CFGR_MCOPRE0			(1 << 28)
#define RCC_CFGR_MCOSEL2			(1 << 26)
#define RCC_CFGR_MCOSEL1			(1 << 25)
#define RCC_CFGR_MCOSEL0			(1 << 24)
#define RCC_CFGR_PLLDIV1			(1 << 23)
#define RCC_CFGR_PLLDIV0			(1 << 22)
#define RCC_CFGR_PLLMUL3			(1 << 21)
#define RCC_CFGR_PLLMUL2			(1 << 20)
#define RCC_CFGR_PLLMUL1			(1 << 19)
#define RCC_CFGR_PLLMUL0			(1 << 18)
#define RCC_CFGR_PLLSRC				(1 << 16)
#define RCC_CFGR_PPRE22				(1 << 13)
#define RCC_CFGR_PPRE21				(1 << 12)
#define RCC_CFGR_PPRE20				(1 << 11)
#define RCC_CFGR_PPRE12				(1 << 10)
#define RCC_CFGR_PPRE11				(1 << 9)
#define RCC_CFGR_PPRE10				(1 << 8)
#define RCC_CFGR_HPRE3				(1 << 7)
#define RCC_CFGR_HPRE2				(1 << 6)
#define RCC_CFGR_HPRE1				(1 << 5)
#define RCC_CFGR_HPRE0				(1 << 4)
#define RCC_CFGR_SWS1				(1 << 3)
#define RCC_CFGR_SWS0				(1 << 2)
#define RCC_CFGR_SW1				(1 << 1)
#define RCC_CFGR_SW0				(1 << 0)

/* Microcontroller clock output prescaler */
#define RCC_CFGR_MCOPRE_NODIV			(0 << 28)
#define RCC_CFGR_MCOPRE_DIV2			(1 << 28)
#define RCC_CFGR_MCOPRE_DIV4			(2 << 28)
#define RCC_CFGR_MCOPRE_DIV8			(3 << 28)
#define RCC_CFGR_MCOPRE_DIV16			(4 << 28)

/* Microcontroller clock output selection */
#define RCC_CFGR_MCOSEL_NOCLK			(0 << 24)
#define RCC_CFGR_MCOSEL_SYSCLK			(1 << 24)
#define RCC_CFGR_MCOSEL_HSI			(2 << 24)
#define RCC_CFGR_MCOSEL_MSI			(3 << 24)
#define RCC_CFGR_MCOSEL_HSE			(4 << 24)
#define RCC_CFGR_MCOSEL_PLL			(5 << 24)
#define RCC_CFGR_MCOSEL_LSI			(6 << 24)
#define RCC_CFGR_MCOSEL_LSE			(7 << 24)

/* PLL output division */
#define RCC_CFGR_PLLDIV_PLL_VCO_DIV2		(1 << 22)
#define RCC_CFGR_PLLDIV_PLL_VCO_DIV3		(2 << 22)
#define RCC_CFGR_PLLDIV_PLL_VCO_DIV4		(3 << 22)

/* PLL multiplication factor */
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL3		(0 << 18)
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL4		(1 << 18)
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL6		(2 << 18)
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL8		(3 << 18)
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL12		(4 << 18)
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL16		(5 << 18)
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL24		(6 << 18)
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL32		(7 << 18)
#define RCC_CFGR_PLLMUL_PLL_CLK_MUL48		(8 << 18)

/* PLL entry clock source */
#define RCC_CFGR_PLLSRC_HSI_CLK			(0 << 16)
#define RCC_CFGR_PLLSRC_HSE_CLK			(1 << 16)

/* APB high-speed prescaler (APB2) */
#define RCC_CFGR_PPRE2_HCLK_NODIV		(0 << 11)
#define RCC_CFGR_PPRE2_HCLK_DIV2		(4 << 11)
#define RCC_CFGR_PPRE2_HCLK_DIV4		(5 << 11)
#define RCC_CFGR_PPRE2_HCLK_DIV8		(6 << 11)
#define RCC_CFGR_PPRE2_HCLK_DIV16		(7 << 11)

/* APB low-speed prescaler (APB1) */
#define RCC_CFGR_PPRE1_HCLK_NODIV		(0 << 8)
#define RCC_CFGR_PPRE1_HCLK_DIV2		(4 << 8)
#define RCC_CFGR_PPRE1_HCLK_DIV4		(5 << 8)
#define RCC_CFGR_PPRE1_HCLK_DIV8		(6 << 8)
#define RCC_CFGR_PPRE1_HCLK_DIV16		(7 << 8)

/* AHB prescaler */
#define RCC_CFGR_HPRE_SYSCLK_NODIV		(0 << 4)
#define RCC_CFGR_HPRE_SYSCLK_DIV2		(8 << 4)
#define RCC_CFGR_HPRE_SYSCLK_DIV4		(9 << 4)
#define RCC_CFGR_HPRE_SYSCLK_DIV8		(10 << 4)
#define RCC_CFGR_HPRE_SYSCLK_DIV16		(11 << 4)
#define RCC_CFGR_HPRE_SYSCLK_DIV64		(12 << 4)
#define RCC_CFGR_HPRE_SYSCLK_DIV128		(13 << 4)
#define RCC_CFGR_HPRE_SYSCLK_DIV256		(14 << 4)
#define RCC_CFGR_HPRE_SYSCLK_DIV512		(15 << 4)

/* System clock switch status */
#define RCC_CFGR_SWS_SYSCLKSEL_MSI		(0 << 2)
#define RCC_CFGR_SWS_SYSCLKSEL_HSI		(1 << 2)
#define RCC_CFGR_SWS_SYSCLKSEL_HSE		(2 << 2)
#define RCC_CFGR_SWS_SYSCLKSEL_PLL		(3 << 2)

/* System clock switch */
#define RCC_CFGR_SW_SYSCLKSEL_MSI		(0 << 0)
#define RCC_CFGR_SW_SYSCLKSEL_HSI		(1 << 0)
#define RCC_CFGR_SW_SYSCLKSEL_HSE		(2 << 0)
#define RCC_CFGR_SW_SYSCLKSEL_PLL		(3 << 0)

/* --- RCC_CIR values ------------------------------------------------------ */

#define RCC_CIR_CSSC				(1 << 23)
#define RCC_CIR_LSECSSC				(1 << 22)	/* (**) */
#define RCC_CIR_MSIRDYC				(1 << 21)
#define RCC_CIR_PLLRDYC				(1 << 20)
#define RCC_CIR_HSERDYC				(1 << 19)
#define RCC_CIR_HSIRDYC				(1 << 18)
#define RCC_CIR_LSERDYC				(1 << 17)
#define RCC_CIR_LSIRDYC				(1 << 16)
#define RCC_CIR_LSECSSIE			(1 << 14)	/* (**) */
#define RCC_CIR_MSIRDYIE			(1 << 13)
#define RCC_CIR_PLLRDYIE			(1 << 12)
#define RCC_CIR_HSERDYIE			(1 << 11)
#define RCC_CIR_HSIRDYIE			(1 << 10)
#define RCC_CIR_LSERDYIE			(1 << 9)
#define RCC_CIR_LSIRDYIE			(1 << 8)
#define RCC_CIR_CSSF				(1 << 7)
#define RCC_CIR_LSECSSF				(1 << 6)	/* (**) */
#define RCC_CIR_MSIRDYF				(1 << 5)
#define RCC_CIR_PLLRDYF				(1 << 4)
#define RCC_CIR_HSERDYF				(1 << 3)
#define RCC_CIR_HSIRDYF				(1 << 2)
#define RCC_CIR_LSERDYF				(1 << 1)
#define RCC_CIR_LSIRDYF				(1 << 0)

/* --- RCC_AHBRSTR values -------------------------------------------------- */

#define RCC_AHBRSTR_FSMCRST			(1 << 30)	/* (***) */
#define RCC_AHBRSTR_AESRST			(1 << 27)	/* (****) */
#define RCC_AHBRSTR_DMA2RST			(1 << 25)	/* (**) */
#define RCC_AHBRSTR_DMA1RST			(1 << 24)
#define RCC_AHBRSTR_FLITFRST			(1 << 15)
#define RCC_AHBRSTR_CRCRST			(1 << 12)
#define RCC_AHBRSTR_GPIOGRST			(1 << 7)	/* (**) */
#define RCC_AHBRSTR_GPIOFRST			(1 << 6)	/* (**) */
#define RCC_AHBRSTR_GPIOHRST			(1 << 5)
#define RCC_AHBRSTR_GPIOERST			(1 << 4)
#define RCC_AHBRSTR_GPIODRST			(1 << 3)
#define RCC_AHBRSTR_GPIOCRST			(1 << 2)
#define RCC_AHBRSTR_GPIOBRST			(1 << 1)
#define RCC_AHBRSTR_GPIOARST			(1 << 0)

/* --- RCC_APB2RSTR values ------------------------------------------------- */

#define RCC_APB2RSTR_USART1RST			(1 << 14)
#define RCC_APB2RSTR_SPI1RST			(1 << 12)
#define RCC_APB2RSTR_SDIORST			(1 << 11)	/* (***) */
#define RCC_APB2RSTR_ADC1RST			(1 << 9)
#define RCC_APB2RSTR_TIM11RST			(1 << 4)
#define RCC_APB2RSTR_TIM10RST			(1 << 3)
#define RCC_APB2RSTR_TIM9RST			(1 << 2)
#define RCC_APB2RSTR_SYSCFGRST			(1 << 0)

/* --- RCC_APB1RSTR values ------------------------------------------------- */

#define RCC_APB1RSTR_COMPRST			(1 << 31)
#define RCC_APB1RSTR_DACRST			(1 << 29)
#define RCC_APB1RSTR_PWRRST			(1 << 28)
#define RCC_APB1RSTR_USBRST			(1 << 23)
#define RCC_APB1RSTR_I2C2RST			(1 << 22)
#define RCC_APB1RSTR_I2C1RST			(1 << 21)
#define RCC_APB1RSTR_UART5RST			(1 << 20)	/* (***) */
#define RCC_APB1RSTR_UART4RST			(1 << 19)	/* (***) */
#define RCC_APB1RSTR_USART3RST			(1 << 18)
#define RCC_APB1RSTR_USART2RST			(1 << 17)
#define RCC_APB1RSTR_SPI3RST			(1 << 15)	/* (**) */
#define RCC_APB1RSTR_SPI2RST			(1 << 14)
#define RCC_APB1RSTR_WWDGRST			(1 << 11)
#define RCC_APB1RSTR_LCDRST			(1 << 9)
#define RCC_APB1RSTR_TIM7RST			(1 << 5)
#define RCC_APB1RSTR_TIM6RST			(1 << 4)
#define RCC_APB1RSTR_TIM5RST			(1 << 3)	/* (**) */
#define RCC_APB1RSTR_TIM4RST			(1 << 2)
#define RCC_APB1RSTR_TIM3RST			(1 << 1)
#define RCC_APB1RSTR_TIM2RST			(1 << 0)

/* --- RCC_AHBENR values --------------------------------------------------- */

#define RCC_AHBENR_FSMCEN			(1 << 30)	/* (***) */
#define RCC_AHBENR_AESEN			(1 << 27)	/* (****) */
#define RCC_AHBENR_DMA2EN			(1 << 25)	/* (**) */
#define RCC_AHBENR_DMA1EN			(1 << 24)
#define RCC_AHBENR_FLITFEN			(1 << 15)
#define RCC_AHBENR_CRCEN			(1 << 12)
#define RCC_AHBENR_GPIOGEN			(1 << 7)	/* (**) */
#define RCC_AHBENR_GPIOFEN			(1 << 6)	/* (**) */
#define RCC_AHBENR_GPIOHEN			(1 << 5)
#define RCC_AHBENR_GPIOEEN			(1 << 4)
#define RCC_AHBENR_GPIODEN			(1 << 3)
#define RCC_AHBENR_GPIOCEN			(1 << 2)
#define RCC_AHBENR_GPIOBEN			(1 << 1)
#define RCC_AHBENR_GPIOAEN			(1 << 0)

/* --- RCC_APB2ENR values -------------------------------------------------- */

#define RCC_APB2ENR_USART1EN			(1 << 14)
#define RCC_APB2ENR_SPI1EN			(1 << 12)
#define RCC_APB2ENR_SDIOEN			(1 << 11)	/* (***) */
#define RCC_APB2ENR_ADC1EN			(1 << 9)
#define RCC_APB2ENR_TIM11EN			(1 << 4)
#define RCC_APB2ENR_TIM10EN			(1 << 3)
#define RCC_APB2ENR_TIM9EN			(1 << 2)
#define RCC_APB2ENR_SYSCFGEN			(1 << 0)

/* --- RCC_APB1ENR values -------------------------------------------------- */

#define RCC_APB1ENR_COMPEN			(1 << 31)
#define RCC_APB1ENR_DACEN			(1 << 29)
#define RCC_APB1ENR_PWREN			(1 << 28)
#define RCC_APB1ENR_USBEN			(1 << 23)
#define RCC_APB1ENR_I2C2EN			(1 << 22)
#define RCC_APB1ENR_I2C1EN			(1 << 21)
#define RCC_APB1ENR_UART5EN			(1 << 20)	/* (***) */
#define RCC_APB1ENR_UART4EN			(1 << 19)	/* (***) */
#define RCC_APB1ENR_USART3EN			(1 << 18)
#define RCC_APB1ENR_USART2EN			(1 << 17)
#define RCC_APB1ENR_SPI3EN			(1 << 15)	/* (**) */
#define RCC_APB1ENR_SPI2EN			(1 << 14)
#define RCC_APB1ENR_WWDGEN			(1 << 11)
#define RCC_APB1ENR_LCDEN			(1 << 9)
#define RCC_APB1ENR_TIM7EN			(1 << 5)
#define RCC_APB1ENR_TIM6EN			(1 << 4)
#define RCC_APB1ENR_TIM5EN			(1 << 3)	/* (**) */
#define RCC_APB1ENR_TIM4EN			(1 << 2)
#define RCC_APB1ENR_TIM3EN			(1 << 1)
#define RCC_APB1ENR_TIM2EN			(1 << 0)

/* --- RCC_AHBLPENR values ------------------------------------------------- */

#define RCC_AHBLPENR_FSMCLPEN			(1 << 30)	/* (***) */
#define RCC_AHBLPENR_AESLPEN			(1 << 27)	/* (****) */
#define RCC_AHBLPENR_DMA2LPEN			(1 << 25)	/* (**) */
#define RCC_AHBLPENR_DMA1LPEN			(1 << 24)
#define RCC_AHBLPENR_SRAMLPEN			(1 << 16)
#define RCC_AHBLPENR_FLITFLPEN			(1 << 15)
#define RCC_AHBLPENR_CRCLPEN			(1 << 12)
#define RCC_AHBLPENR_GPIOGLPEN			(1 << 7)	/* (**) */
#define RCC_AHBLPENR_GPIOFLPEN			(1 << 6)	/* (**) */
#define RCC_AHBLPENR_GPIOHLPEN			(1 << 5)
#define RCC_AHBLPENR_GPIOELPEN			(1 << 4)
#define RCC_AHBLPENR_GPIODLPEN			(1 << 3)
#define RCC_AHBLPENR_GPIOCLPEN			(1 << 2)
#define RCC_AHBLPENR_GPIOBLPEN			(1 << 1)
#define RCC_AHBLPENR_GPIOALPEN			(1 << 0)

/* --- RCC_APB2LPENR values ------------------------------------------------ */

#define RCC_APB2LPENR_USART1LPEN		(1 << 14)
#define RCC_APB2LPENR_SPI1LPEN			(1 << 12)
#define RCC_APB2LPENR_SDIOLPEN			(1 << 11)	/* (***) */
#define RCC_APB2LPENR_ADC1LPEN			(1 << 9)
#define RCC_APB2LPENR_TIM11LPEN			(1 << 4)
#define RCC_APB2LPENR_TIM10LPEN			(1 << 3)
#define RCC_APB2LPENR_TIM9LPEN			(1 << 2)
#define RCC_APB2LPENR_SYSCFGLPEN		(1 << 0)

/* --- RCC_APB1LPENR values ------------------------------------------------ */

#define RCC_APB1LPENR_COMPLPEN			(1 << 31)
#define RCC_APB1LPENR_DACLPEN			(1 << 29)
#define RCC_APB1LPENR_PWRLPEN			(1 << 28)
#define RCC_APB1LPENR_USBLPEN			(1 << 23)
#define RCC_APB1LPENR_I2C2LPEN			(1 << 22)
#define RCC_APB1LPENR_I2C1LPEN			(1 << 21)
#define RCC_APB1LPENR_UART5LPEN			(1 << 20)	/* (***) */
#define RCC_APB1LPENR_UART4LPEN			(1 << 19)	/* (***) */
#define RCC_APB1LPENR_USART3LPEN		(1 << 18)
#define RCC_APB1LPENR_USART2LPEN		(1 << 17)
#define RCC_APB1LPENR_SPI3LPEN			(1 << 15)	/* (**) */
#define RCC_APB1LPENR_SPI2LPEN			(1 << 14)
#define RCC_APB1LPENR_WWDGLPEN			(1 << 11)
#define RCC_APB1LPENR_LCDLPEN			(1 << 9)
#define RCC_APB1LPENR_TIM7LPEN			(1 << 5)
#define RCC_APB1LPENR_TIM6LPEN			(1 << 4)
#define RCC_APB1LPENR_TIM5LPEN			(1 << 3)	/* (**) */
#define RCC_APB1LPENR_TIM4LPEN			(1 << 2)
#define RCC_APB1LPENR_TIM3LPEN			(1 << 1)
#define RCC_APB1LPENR_TIM2LPEN			(1 << 0)

/* --- RCC_CSR values ------------------------------------------------------ */

#define RCC_CSR_LPWRRSTF			(1 << 31)
#define RCC_CSR_WWDGRSTF			(1 << 30)
#define RCC_CSR_IWDGRSTF			(1 << 29)
#define RCC_CSR_SFTRSTF				(1 << 28)
#define RCC_CSR_PORRSTF				(1 << 27)
#define RCC_CSR_PINRSTF				(1 << 26)
#define RCC_CSR_OBLRSTF				(1 << 25)
#define RCC_CSR_RMVF				(1 << 24)
#define RCC_CSR_RTCRST				(1 << 23)
#define RCC_CSR_RTCEN				(1 << 22)
#define RCC_CSR_RTCSEL1				(1 << 17)
#define RCC_CSR_RTCSEL0				(1 << 16)
#define RCC_CSR_LSECSSD				(1 << 12)	/* (**) */
#define RCC_CSR_LSECSSON			(1 << 11)	/* (**) */
#define RCC_CSR_LSEBYP				(1 << 10)
#define RCC_CSR_LSERDY				(1 << 9)
#define RCC_CSR_LSEON				(1 << 8)
#define RCC_CSR_LSIRDY				(1 << 1)
#define RCC_CSR_LSION				(1 << 0)

/* RTC and LCD clock source selection */
#define RCC_CSR_RTCSEL_NOCLK			(0 << 16)
#define RCC_CSR_RTCSEL_LSECLK			(1 << 16)
#define RCC_CSR_RTCSEL_LSICLK			(2 << 16)
#define RCC_CSR_RTCSEL_HSECLK			(3 << 16)

/* --- Function prototypes ------------------------------------------------- */

/* Oscillator (clock source) */
typedef enum {
	RCC_HSE, RCC_MSI, RCC_HSI, RCC_LSE, RCC_LSI,
	RCC_PLL,
	RCC_SYSCLK, RCC_NOCLK
} rcc_osc_t;

/* Peripheral */
typedef enum {
	RCC_FSMC, RCC_AES, RCC_DMA2, RCC_DMA1, RCC_SRAM, RCC_FLITF, RCC_CRC,
	RCC_GPIOG, RCC_GPIOF, RCC_GPIOH, RCC_GPIOE, RCC_GPIOD, RCC_GPIOC,
	RCC_GPIOB,
	RCC_GPIOA,
	RCC_USART1, RCC_SPI1, RCC_SDIO, RCC_ADC, RCC_TIM11, RCC_TIM10,
	RCC_TIM9, RCC_SYSCFG,
	RCC_COMP, RCC_DAC, RCC_PWR, RCC_USB, RCC_I2C2, RCC_I2C1, RCC_UART5,
	RCC_UART4, RCC_USART3, RCC_USART2, RCC_SPI3, RCC_SPI2, RCC_WWDG,
	RCC_LCD, RCC_TIM7, RCC_TIM6, RCC_TIM5, RCC_TIM4, RCC_TIM3, RCC_TIM2,
	RCC_RTC
} rcc_peripheral_t;

/* MSI frequency range */
typedef enum {
	RCC_MSI_65KHZ = (0 << 13),
	RCC_MSI_131KHZ = (1 << 13),
	RCC_MSI_262KHZ = (2 << 13),
	RCC_MSI_524KHZ = (3 << 13),
	RCC_MSI_1MHZ = (4 << 13),
	RCC_MSI_2MHZ = (5 << 13),
	RCC_MSI_4MHZ = (6 << 13),
	RCC_MSI_RANGE0 = (0 << 13),
	RCC_MSI_RANGE1 = (1 << 13),
	RCC_MSI_RANGE2 = (2 << 13),
	RCC_MSI_RANGE3 = (3 << 13),
	RCC_MSI_RANGE4 = (4 << 13),
	RCC_MSI_RANGE5 = (5 << 13),
	RCC_MSI_RANGE6 = (6 << 13)
} rcc_msi_range_t;

/* Interrupt */
enum {
	RCC_LSIRDY = (1 << 0),
	RCC_LSERDY = (1 << 1),
	RCC_HSIRDY = (1 << 2),
	RCC_HSERDY = (1 << 3),
	RCC_PLLRDY = (1 << 4),
	RCC_MSIRDY = (1 << 5),
	RCC_LSECSS = (1 << 6),
	RCC_CSS = (1 << 7)
};

/* Reset flag */
enum {
	RCC_OPTION_BYTE_LOADER = (1 << 25),
	RCC_EXTERNAL = (1 << 26),
	RCC_POR_PDR = (1 << 27),
	RCC_SOFTWARE = (1 << 28),
	RCC_IWDG_RESET = (1 << 29),
	RCC_WWDG_RESET = (1 << 30),
	RCC_LOW_POWER = (1 << 31)
};

void rcc_setup_rtc_lcd_clock(rcc_osc_t src, int div);
void rcc_enable_css(void);
void rcc_disable_css(void);
int rcc_get_csson(void);
void rcc_enable_osc(rcc_osc_t osc);
void rcc_disable_osc(rcc_osc_t osc);
void rcc_enable_osc_bypass(rcc_osc_t osc);
void rcc_disable_osc_bypass(rcc_osc_t osc);
void rcc_set_msi_range(rcc_msi_range_t range);
rcc_msi_range_t rcc_get_msi_range(void);
void rcc_setup_mco(rcc_osc_t src, int div);
void rcc_setup_pll(rcc_osc_t src, int mul, int div);
void rcc_set_prescaler(int ahb, int apb1, int apb2);
void rcc_set_sysclk_source(rcc_osc_t src);
rcc_osc_t rcc_get_sysclk_source(void);
void rcc_enable_interrupt(int interrupt);
void rcc_disable_interrupt(int interrupt);
int rcc_get_interrupt_mask(int interrupt);
int rcc_get_interrupt_status(int interrupt);
void rcc_clear_interrupt(int interrupt);
void rcc_enable_reset(rcc_peripheral_t peripheral);
void rcc_disable_reset(rcc_peripheral_t peripheral);
void rcc_enable_clock(rcc_peripheral_t peripheral);
void rcc_disable_clock(rcc_peripheral_t peripheral);
void rcc_enable_clock_low_power(rcc_peripheral_t peripheral);
void rcc_disable_clock_low_power(rcc_peripheral_t peripheral);
int rcc_get_reset_flag(int flag);
void rcc_clear_reset_flag(void);
void rcc_enable_lsecss(void);
int rcc_get_lsecss_status(void);
