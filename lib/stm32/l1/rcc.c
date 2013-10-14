/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Federico Ruiz-Ugalde <memeruiz at gmail dot com>
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
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

#include <stm32/l1/rcc.h>

void rcc_setup_rtc_lcd_clock(rcc_osc_t src, int div)
{
	int i;
	u32 rtcpre;
	u32 rtcsel;
	u32 reg32;

	if (src == RCC_HSE) {
		for (i = 0; i < 4 && div > (1 << (i + 1)); i++)
			;
		if (i >= 4)
			return;
		rtcpre = i << 29;

		reg32 = RCC_CR;
		reg32 &= ~(RCC_CR_RTCPRE1 | RCC_CR_RTCPRE0);
		RCC_CR = (reg32 | rtcpre);
	}

	switch (src) {
	case RCC_NOCLK:
		rtcsel = RCC_CSR_RTCSEL_NOCLK;
		break;
	case RCC_LSE:
		rtcsel = RCC_CSR_RTCSEL_LSECLK;
		break;
	case RCC_LSI:
		rtcsel = RCC_CSR_RTCSEL_LSICLK;
		break;
	case RCC_HSE:
		rtcsel = RCC_CSR_RTCSEL_HSECLK;
		break;
	default:
		return;
	}

	reg32 = RCC_CSR;
	reg32 &= ~(RCC_CSR_RTCSEL1 | RCC_CSR_RTCSEL0);
	RCC_CSR = (reg32 | rtcsel);
}

void rcc_enable_css(void)
{
	RCC_CR |= RCC_CR_CSSON;
}

void rcc_disable_css(void)
{
	RCC_CR &= ~RCC_CR_CSSON;
}

int rcc_get_csson(void)
{
	return RCC_CR & RCC_CR_CSSON;
}

void rcc_enable_osc(rcc_osc_t osc)
{
	switch (osc) {
	case RCC_PLL:
		RCC_CR |= RCC_CR_PLLON;
		while (!(RCC_CR & RCC_CR_PLLRDY))
			;
		break;
	case RCC_HSE:
		RCC_CR |= RCC_CR_HSEON;
		while (!(RCC_CR & RCC_CR_HSERDY))
			;
		break;
	case RCC_MSI:
		RCC_CR |= RCC_CR_MSION;
		while (!(RCC_CR & RCC_CR_MSIRDY))
			;
		break;
	case RCC_HSI:
		RCC_CR |= RCC_CR_HSION;
		while (!(RCC_CR & RCC_CR_HSIRDY))
			;
		break;
	case RCC_LSE:
		RCC_CSR |= RCC_CSR_LSEON;
		while (!(RCC_CSR & RCC_CSR_LSERDY))
			;
		break;
	case RCC_LSI:
		RCC_CSR |= RCC_CSR_LSION;
		while (!(RCC_CSR & RCC_CSR_LSIRDY))
			;
		break;
	default:
		break;
	}
}

void rcc_disable_osc(rcc_osc_t osc)
{
	switch (osc) {
	case RCC_PLL:
		RCC_CR &= ~RCC_CR_PLLON;
		while ((RCC_CR & RCC_CR_PLLRDY))
			;
		break;
	case RCC_HSE:
		RCC_CR &= ~RCC_CR_HSEON;
		while ((RCC_CR & RCC_CR_HSERDY))
			;
		break;
	case RCC_MSI:
		RCC_CR &= ~RCC_CR_MSION;
		while ((RCC_CR & RCC_CR_MSIRDY))
			;
		break;
	case RCC_HSI:
		RCC_CR &= ~RCC_CR_HSION;
		while ((RCC_CR & RCC_CR_HSIRDY))
			;
		break;
	case RCC_LSE:
		RCC_CSR &= ~RCC_CSR_LSEON;
		while ((RCC_CSR & RCC_CSR_LSERDY))
			;
		break;
	case RCC_LSI:
		RCC_CSR &= ~RCC_CSR_LSION;
		while ((RCC_CSR & RCC_CSR_LSIRDY))
			;
		break;
	default:
		break;
	}
}

void rcc_enable_osc_bypass(rcc_osc_t osc)
{
	switch (osc) {
	case RCC_HSE:
		RCC_CR |= RCC_CR_HSEBYP;
		break;
	case RCC_LSE:
		RCC_CSR |= RCC_CSR_LSEBYP;
		break;
	case RCC_PLL:
	case RCC_MSI:
	case RCC_HSI:
	case RCC_LSI:
		/* Do nothing, only HSE/LSE allowed here. */
		break;
	default:
		break;
	}
}

void rcc_disable_osc_bypass(rcc_osc_t osc)
{
	switch (osc) {
	case RCC_HSE:
		RCC_CR &= ~RCC_CR_HSEBYP;
		break;
	case RCC_LSE:
		RCC_CSR &= ~RCC_CSR_LSEBYP;
		break;
	case RCC_PLL:
	case RCC_MSI:
	case RCC_HSI:
	case RCC_LSI:
		/* Do nothing, only HSE/LSE allowed here. */
		break;
	default:
		break;
	}
}

void rcc_set_msi_range(rcc_msi_range_t range)
{
	u32 reg32;

	reg32 = RCC_ICSCR;
	reg32 &= ~(RCC_ICSCR_MSIRANGE2 | RCC_ICSCR_MSIRANGE1 |
		   RCC_ICSCR_MSIRANGE0);
	RCC_ICSCR = (reg32 | range);
}

rcc_msi_range_t rcc_get_msi_range(void)
{
	return RCC_ICSCR & (RCC_ICSCR_MSIRANGE2 | RCC_ICSCR_MSIRANGE1 |
			    RCC_ICSCR_MSIRANGE0);
}

void rcc_setup_mco(rcc_osc_t src, int div)
{
	u32 mcosel;
	int i;
	u32 mcopre;
	u32 reg32;

	switch (src) {
	case RCC_NOCLK:
		mcosel = RCC_CFGR_MCOSEL_NOCLK;
		break;
	case RCC_SYSCLK:
		mcosel = RCC_CFGR_MCOSEL_SYSCLK;
		break;
	case RCC_HSI:
		mcosel = RCC_CFGR_MCOSEL_HSI;
		break;
	case RCC_MSI:
		mcosel = RCC_CFGR_MCOSEL_MSI;
		break;
	case RCC_HSE:
		mcosel = RCC_CFGR_MCOSEL_HSE;
		break;
	case RCC_PLL:
		mcosel = RCC_CFGR_MCOSEL_PLL;
		break;
	case RCC_LSI:
		mcosel = RCC_CFGR_MCOSEL_LSI;
		break;
	case RCC_LSE:
		mcosel = RCC_CFGR_MCOSEL_LSE;
		break;
	default:
		return;
	}

	for (i = 0; i < 5 && div < (1 << i); i++)
		;
	if (i >= 5)
		return;
	mcopre = i << 28;

	reg32 = RCC_CFGR;
	reg32 &= ~(RCC_CFGR_MCOPRE2 | RCC_CFGR_MCOPRE1 | RCC_CFGR_MCOPRE0 |
		   RCC_CFGR_MCOSEL2 | RCC_CFGR_MCOSEL1 | RCC_CFGR_MCOSEL0);
	RCC_CFGR = (reg32 | mcopre | mcosel);
}

void rcc_setup_pll(rcc_osc_t src, int mul, int div)
{
	int i;
	u32 pllmul;
	u32 plldiv;
	u32 reg32;

	if (src != RCC_HSI && src != RCC_HSE)
		return;

	for (i = 0; i < 9 && mul > ((i & 1) ? 4 : 3) * (1 << (i >> 1)); i++)
		;
	if (i >= 9)
		return;
	pllmul = i << 18;

	for (i = 1; i < 4 && div > i + 1; i++)
		;
	if (i >= 4)
		return;
	plldiv = i << 22;

	reg32 = RCC_CFGR;
	reg32 &= ~(RCC_CFGR_PLLDIV1 | RCC_CFGR_PLLDIV0 |
		   RCC_CFGR_PLLMUL3 | RCC_CFGR_PLLMUL2 | RCC_CFGR_PLLMUL1 |
		   RCC_CFGR_PLLMUL0 |
		   RCC_CFGR_PLLSRC);
	if (src == RCC_HSE)
		reg32 |= RCC_CFGR_PLLSRC;
	RCC_CFGR = (reg32 | plldiv | pllmul);
}

void rcc_set_prescaler(int ahb, int apb1, int apb2)
{
	int i;
	u32 hpre;
	u32 ppre1;
	u32 ppre2;
	u32 reg32;

	for (i = 7; i < 16 && ahb > (1 << (i - 7)); i++)
		;
	if (i >= 16)
		return;
	if (i == 7)
		i = 0;
	hpre = i << 4;

	for (i = 3; i < 8 && apb1 > (1 << (i - 3)); i++)
		;
	if (i >= 8)
		return;
	if (i == 3)
		i = 0;
	ppre1 = i << 8;

	for (i = 3; i < 8 && apb2 > (1 << (i - 3)); i++)
		;
	if (i >= 8)
		return;
	if (i == 3)
		i = 0;
	ppre2 = i << 11;

	reg32 = RCC_CFGR;
	reg32 &= ~(RCC_CFGR_PPRE22 | RCC_CFGR_PPRE21 | RCC_CFGR_PPRE20 |
		   RCC_CFGR_PPRE12 | RCC_CFGR_PPRE11 | RCC_CFGR_PPRE10 |
		   RCC_CFGR_HPRE3 | RCC_CFGR_HPRE2 | RCC_CFGR_HPRE1 |
		   RCC_CFGR_HPRE0);
	RCC_CFGR = (reg32 | ppre2 | ppre1 | hpre);
}

void rcc_set_sysclk_source(rcc_osc_t src)
{
	u32 clk;
	u32 reg32;

	switch (src) {
	case RCC_MSI:
		clk = RCC_CFGR_SW_SYSCLKSEL_MSI;
		break;
	case RCC_HSI:
		clk = RCC_CFGR_SW_SYSCLKSEL_HSI;
		break;
	case RCC_HSE:
		clk = RCC_CFGR_SW_SYSCLKSEL_HSE;
		break;
	case RCC_PLL:
		clk = RCC_CFGR_SW_SYSCLKSEL_PLL;
		break;
	default:
		return;
	}

	reg32 = RCC_CFGR;
	reg32 &= ~(RCC_CFGR_SW1 | RCC_CFGR_SW0);
	RCC_CFGR = (reg32 | clk);
}

rcc_osc_t rcc_get_sysclk_source(void)
{
	switch (RCC_CFGR & (RCC_CFGR_SWS1 | RCC_CFGR_SWS0)) {
	case RCC_CFGR_SWS_SYSCLKSEL_MSI:
		return RCC_MSI;
	case RCC_CFGR_SWS_SYSCLKSEL_HSI:
		return RCC_HSI;
	case RCC_CFGR_SWS_SYSCLKSEL_HSE:
		return RCC_HSE;
	case RCC_CFGR_SWS_SYSCLKSEL_PLL:
		return RCC_PLL;
	default:
		break;
	}
	return RCC_MSI;
}

void rcc_enable_interrupt(int interrupt)
{
	RCC_CIR |= ((interrupt & 0x7f) << 8);
}

void rcc_disable_interrupt(int interrupt)
{
	RCC_CIR &= ~((interrupt & 0x7f) << 8);
}

int rcc_get_interrupt_mask(int interrupt)
{
	u32 reg32;

	reg32 = RCC_CIR;
	reg32 >>= 8;
	return reg32 & interrupt;
}

int rcc_get_interrupt_status(int interrupt)
{
	return RCC_CIR & interrupt;
}

void rcc_clear_interrupt(int interrupt)
{
	RCC_CIR |= ((interrupt & 0xff) << 16);
}

void rcc_enable_reset(rcc_peripheral_t peripheral)
{
	switch (peripheral) {
	case RCC_FSMC:
		RCC_AHBRSTR |= RCC_AHBRSTR_FSMCRST;
		break;
	case RCC_AES:
		RCC_AHBRSTR |= RCC_AHBRSTR_AESRST;
		break;
	case RCC_DMA2:
		RCC_AHBRSTR |= RCC_AHBRSTR_DMA2RST;
		break;
	case RCC_DMA1:
		RCC_AHBRSTR |= RCC_AHBRSTR_DMA1RST;
		break;
	case RCC_FLITF:
		RCC_AHBRSTR |= RCC_AHBRSTR_FLITFRST;
		break;
	case RCC_CRC:
		RCC_AHBRSTR |= RCC_AHBRSTR_CRCRST;
		break;
	case RCC_GPIOG:
		RCC_AHBRSTR |= RCC_AHBRSTR_GPIOGRST;
		break;
	case RCC_GPIOF:
		RCC_AHBRSTR |= RCC_AHBRSTR_GPIOFRST;
		break;
	case RCC_GPIOH:
		RCC_AHBRSTR |= RCC_AHBRSTR_GPIOHRST;
		break;
	case RCC_GPIOE:
		RCC_AHBRSTR |= RCC_AHBRSTR_GPIOERST;
		break;
	case RCC_GPIOD:
		RCC_AHBRSTR |= RCC_AHBRSTR_GPIODRST;
		break;
	case RCC_GPIOC:
		RCC_AHBRSTR |= RCC_AHBRSTR_GPIOCRST;
		break;
	case RCC_GPIOB:
		RCC_AHBRSTR |= RCC_AHBRSTR_GPIOBRST;
		break;
	case RCC_GPIOA:
		RCC_AHBRSTR |= RCC_AHBRSTR_GPIOARST;
		break;

	case RCC_USART1:
		RCC_APB2RSTR |= RCC_APB2RSTR_USART1RST;
		break;
	case RCC_SPI1:
		RCC_APB2RSTR |= RCC_APB2RSTR_SPI1RST;
		break;
	case RCC_SDIO:
		RCC_APB2RSTR |= RCC_APB2RSTR_SDIORST;
		break;
	case RCC_ADC:
		RCC_APB2RSTR |= RCC_APB2RSTR_ADC1RST;
		break;
	case RCC_TIM11:
		RCC_APB2RSTR |= RCC_APB2RSTR_TIM11RST;
		break;
	case RCC_TIM10:
		RCC_APB2RSTR |= RCC_APB2RSTR_TIM10RST;
		break;
	case RCC_TIM9:
		RCC_APB2RSTR |= RCC_APB2RSTR_TIM9RST;
		break;
	case RCC_SYSCFG:
		RCC_APB2RSTR |= RCC_APB2RSTR_SYSCFGRST;
		break;

	case RCC_COMP:
		RCC_APB1RSTR |= RCC_APB1RSTR_COMPRST;
		break;
	case RCC_DAC:
		RCC_APB1RSTR |= RCC_APB1RSTR_DACRST;
		break;
	case RCC_PWR:
		RCC_APB1RSTR |= RCC_APB1RSTR_PWRRST;
		break;
	case RCC_USB:
		RCC_APB1RSTR |= RCC_APB1RSTR_USBRST;
		break;
	case RCC_I2C2:
		RCC_APB1RSTR |= RCC_APB1RSTR_I2C2RST;
		break;
	case RCC_I2C1:
		RCC_APB1RSTR |= RCC_APB1RSTR_I2C1RST;
		break;
	case RCC_UART5:
		RCC_APB1RSTR |= RCC_APB1RSTR_UART5RST;
		break;
	case RCC_UART4:
		RCC_APB1RSTR |= RCC_APB1RSTR_UART4RST;
		break;
	case RCC_USART3:
		RCC_APB1RSTR |= RCC_APB1RSTR_USART3RST;
		break;
	case RCC_USART2:
		RCC_APB1RSTR |= RCC_APB1RSTR_USART2RST;
		break;
	case RCC_SPI3:
		RCC_APB1RSTR |= RCC_APB1RSTR_SPI3RST;
		break;
	case RCC_SPI2:
		RCC_APB1RSTR |= RCC_APB1RSTR_SPI2RST;
		break;
	case RCC_WWDG:
		RCC_APB1RSTR |= RCC_APB1RSTR_WWDGRST;
		break;
	case RCC_LCD:
		RCC_APB1RSTR |= RCC_APB1RSTR_LCDRST;
		break;
	case RCC_TIM7:
		RCC_APB1RSTR |= RCC_APB1RSTR_TIM7RST;
		break;
	case RCC_TIM6:
		RCC_APB1RSTR |= RCC_APB1RSTR_TIM6RST;
		break;
	case RCC_TIM5:
		RCC_APB1RSTR |= RCC_APB1RSTR_TIM5RST;
		break;
	case RCC_TIM4:
		RCC_APB1RSTR |= RCC_APB1RSTR_TIM4RST;
		break;
	case RCC_TIM3:
		RCC_APB1RSTR |= RCC_APB1RSTR_TIM3RST;
		break;
	case RCC_TIM2:
		RCC_APB1RSTR |= RCC_APB1RSTR_TIM2RST;
		break;

	case RCC_RTC:
		RCC_CSR |= RCC_CSR_RTCRST;
		break;

	default:
		break;
	}
}

void rcc_disable_reset(rcc_peripheral_t peripheral)
{
	switch (peripheral) {
	case RCC_FSMC:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_FSMCRST;
		break;
	case RCC_AES:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_AESRST;
		break;
	case RCC_DMA2:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_DMA2RST;
		break;
	case RCC_DMA1:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_DMA1RST;
		break;
	case RCC_FLITF:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_FLITFRST;
		break;
	case RCC_CRC:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_CRCRST;
		break;
	case RCC_GPIOG:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_GPIOGRST;
		break;
	case RCC_GPIOF:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_GPIOFRST;
		break;
	case RCC_GPIOH:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_GPIOHRST;
		break;
	case RCC_GPIOE:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_GPIOERST;
		break;
	case RCC_GPIOD:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_GPIODRST;
		break;
	case RCC_GPIOC:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_GPIOCRST;
		break;
	case RCC_GPIOB:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_GPIOBRST;
		break;
	case RCC_GPIOA:
		RCC_AHBRSTR &= ~RCC_AHBRSTR_GPIOARST;
		break;

	case RCC_USART1:
		RCC_APB2RSTR &= ~RCC_APB2RSTR_USART1RST;
		break;
	case RCC_SPI1:
		RCC_APB2RSTR &= ~RCC_APB2RSTR_SPI1RST;
		break;
	case RCC_SDIO:
		RCC_APB2RSTR &= ~RCC_APB2RSTR_SDIORST;
		break;
	case RCC_ADC:
		RCC_APB2RSTR &= ~RCC_APB2RSTR_ADC1RST;
		break;
	case RCC_TIM11:
		RCC_APB2RSTR &= ~RCC_APB2RSTR_TIM11RST;
		break;
	case RCC_TIM10:
		RCC_APB2RSTR &= ~RCC_APB2RSTR_TIM10RST;
		break;
	case RCC_TIM9:
		RCC_APB2RSTR &= ~RCC_APB2RSTR_TIM9RST;
		break;
	case RCC_SYSCFG:
		RCC_APB2RSTR &= ~RCC_APB2RSTR_SYSCFGRST;
		break;

	case RCC_COMP:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_COMPRST;
		break;
	case RCC_DAC:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_DACRST;
		break;
	case RCC_PWR:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_PWRRST;
		break;
	case RCC_USB:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_USBRST;
		break;
	case RCC_I2C2:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_I2C2RST;
		break;
	case RCC_I2C1:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_I2C1RST;
		break;
	case RCC_UART5:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_UART5RST;
		break;
	case RCC_UART4:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_UART4RST;
		break;
	case RCC_USART3:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_USART3RST;
		break;
	case RCC_USART2:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_USART2RST;
		break;
	case RCC_SPI3:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_SPI3RST;
		break;
	case RCC_SPI2:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_SPI2RST;
		break;
	case RCC_WWDG:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_WWDGRST;
		break;
	case RCC_LCD:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_LCDRST;
		break;
	case RCC_TIM7:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_TIM7RST;
		break;
	case RCC_TIM6:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;
		break;
	case RCC_TIM5:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_TIM5RST;
		break;
	case RCC_TIM4:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_TIM4RST;
		break;
	case RCC_TIM3:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;
		break;
	case RCC_TIM2:
		RCC_APB1RSTR &= ~RCC_APB1RSTR_TIM2RST;
		break;

	case RCC_RTC:
		RCC_CSR &= ~RCC_CSR_RTCRST;
		break;

	default:
		break;
	}
}

void rcc_enable_clock(rcc_peripheral_t peripheral)
{
	switch (peripheral) {
	case RCC_FSMC:
		RCC_AHBENR |= RCC_AHBENR_FSMCEN;
		break;
	case RCC_AES:
		RCC_AHBENR |= RCC_AHBENR_AESEN;
		break;
	case RCC_DMA2:
		RCC_AHBENR |= RCC_AHBENR_DMA2EN;
		break;
	case RCC_DMA1:
		RCC_AHBENR |= RCC_AHBENR_DMA1EN;
		break;
	case RCC_FLITF:
		RCC_AHBENR |= RCC_AHBENR_FLITFEN;
		break;
	case RCC_CRC:
		RCC_AHBENR |= RCC_AHBENR_CRCEN;
		break;
	case RCC_GPIOG:
		RCC_AHBENR |= RCC_AHBENR_GPIOGEN;
		break;
	case RCC_GPIOF:
		RCC_AHBENR |= RCC_AHBENR_GPIOFEN;
		break;
	case RCC_GPIOH:
		RCC_AHBENR |= RCC_AHBENR_GPIOHEN;
		break;
	case RCC_GPIOE:
		RCC_AHBENR |= RCC_AHBENR_GPIOEEN;
		break;
	case RCC_GPIOD:
		RCC_AHBENR |= RCC_AHBENR_GPIODEN;
		break;
	case RCC_GPIOC:
		RCC_AHBENR |= RCC_AHBENR_GPIOCEN;
		break;
	case RCC_GPIOB:
		RCC_AHBENR |= RCC_AHBENR_GPIOBEN;
		break;
	case RCC_GPIOA:
		RCC_AHBENR |= RCC_AHBENR_GPIOAEN;
		break;

	case RCC_USART1:
		RCC_APB2ENR |= RCC_APB2ENR_USART1EN;
		break;
	case RCC_SPI1:
		RCC_APB2ENR |= RCC_APB2ENR_SPI1EN;
		break;
	case RCC_SDIO:
		RCC_APB2ENR |= RCC_APB2ENR_SDIOEN;
		break;
	case RCC_ADC:
		RCC_APB2ENR |= RCC_APB2ENR_ADC1EN;
		break;
	case RCC_TIM11:
		RCC_APB2ENR |= RCC_APB2ENR_TIM11EN;
		break;
	case RCC_TIM10:
		RCC_APB2ENR |= RCC_APB2ENR_TIM10EN;
		break;
	case RCC_TIM9:
		RCC_APB2ENR |= RCC_APB2ENR_TIM9EN;
		break;
	case RCC_SYSCFG:
		RCC_APB2ENR |= RCC_APB2ENR_SYSCFGEN;
		break;

	case RCC_COMP:
		RCC_APB1ENR |= RCC_APB1ENR_COMPEN;
		break;
	case RCC_DAC:
		RCC_APB1ENR |= RCC_APB1ENR_DACEN;
		break;
	case RCC_PWR:
		RCC_APB1ENR |= RCC_APB1ENR_PWREN;
		break;
	case RCC_USB:
		RCC_APB1ENR |= RCC_APB1ENR_USBEN;
		break;
	case RCC_I2C2:
		RCC_APB1ENR |= RCC_APB1ENR_I2C2EN;
		break;
	case RCC_I2C1:
		RCC_APB1ENR |= RCC_APB1ENR_I2C1EN;
		break;
	case RCC_UART5:
		RCC_APB1ENR |= RCC_APB1ENR_UART5EN;
		break;
	case RCC_UART4:
		RCC_APB1ENR |= RCC_APB1ENR_UART4EN;
		break;
	case RCC_USART3:
		RCC_APB1ENR |= RCC_APB1ENR_USART3EN;
		break;
	case RCC_USART2:
		RCC_APB1ENR |= RCC_APB1ENR_USART2EN;
		break;
	case RCC_SPI3:
		RCC_APB1ENR |= RCC_APB1ENR_SPI3EN;
		break;
	case RCC_SPI2:
		RCC_APB1ENR |= RCC_APB1ENR_SPI2EN;
		break;
	case RCC_WWDG:
		RCC_APB1ENR |= RCC_APB1ENR_WWDGEN;
		break;
	case RCC_LCD:
		RCC_APB1ENR |= RCC_APB1ENR_LCDEN;
		break;
	case RCC_TIM7:
		RCC_APB1ENR |= RCC_APB1ENR_TIM7EN;
		break;
	case RCC_TIM6:
		RCC_APB1ENR |= RCC_APB1ENR_TIM6EN;
		break;
	case RCC_TIM5:
		RCC_APB1ENR |= RCC_APB1ENR_TIM5EN;
		break;
	case RCC_TIM4:
		RCC_APB1ENR |= RCC_APB1ENR_TIM4EN;
		break;
	case RCC_TIM3:
		RCC_APB1ENR |= RCC_APB1ENR_TIM3EN;
		break;
	case RCC_TIM2:
		RCC_APB1ENR |= RCC_APB1ENR_TIM2EN;
		break;

	case RCC_RTC:
		RCC_CSR |= RCC_CSR_RTCEN;
		break;

	default:
		break;
	}
}

void rcc_disable_clock(rcc_peripheral_t peripheral)
{
	switch (peripheral) {
	case RCC_FSMC:
		RCC_AHBENR &= ~RCC_AHBENR_FSMCEN;
		break;
	case RCC_AES:
		RCC_AHBENR &= ~RCC_AHBENR_AESEN;
		break;
	case RCC_DMA2:
		RCC_AHBENR &= ~RCC_AHBENR_DMA2EN;
		break;
	case RCC_DMA1:
		RCC_AHBENR &= ~RCC_AHBENR_DMA1EN;
		break;
	case RCC_FLITF:
		RCC_AHBENR &= ~RCC_AHBENR_FLITFEN;
		break;
	case RCC_CRC:
		RCC_AHBENR &= ~RCC_AHBENR_CRCEN;
		break;
	case RCC_GPIOG:
		RCC_AHBENR &= ~RCC_AHBENR_GPIOGEN;
		break;
	case RCC_GPIOF:
		RCC_AHBENR &= ~RCC_AHBENR_GPIOFEN;
		break;
	case RCC_GPIOH:
		RCC_AHBENR &= ~RCC_AHBENR_GPIOHEN;
		break;
	case RCC_GPIOE:
		RCC_AHBENR &= ~RCC_AHBENR_GPIOEEN;
		break;
	case RCC_GPIOD:
		RCC_AHBENR &= ~RCC_AHBENR_GPIODEN;
		break;
	case RCC_GPIOC:
		RCC_AHBENR &= ~RCC_AHBENR_GPIOCEN;
		break;
	case RCC_GPIOB:
		RCC_AHBENR &= ~RCC_AHBENR_GPIOBEN;
		break;
	case RCC_GPIOA:
		RCC_AHBENR &= ~RCC_AHBENR_GPIOAEN;
		break;

	case RCC_USART1:
		RCC_APB2ENR &= ~RCC_APB2ENR_USART1EN;
		break;
	case RCC_SPI1:
		RCC_APB2ENR &= ~RCC_APB2ENR_SPI1EN;
		break;
	case RCC_SDIO:
		RCC_APB2ENR &= ~RCC_APB2ENR_SDIOEN;
		break;
	case RCC_ADC:
		RCC_APB2ENR &= ~RCC_APB2ENR_ADC1EN;
		break;
	case RCC_TIM11:
		RCC_APB2ENR &= ~RCC_APB2ENR_TIM11EN;
		break;
	case RCC_TIM10:
		RCC_APB2ENR &= ~RCC_APB2ENR_TIM10EN;
		break;
	case RCC_TIM9:
		RCC_APB2ENR &= ~RCC_APB2ENR_TIM9EN;
		break;
	case RCC_SYSCFG:
		RCC_APB2ENR &= ~RCC_APB2ENR_SYSCFGEN;
		break;

	case RCC_COMP:
		RCC_APB1ENR &= ~RCC_APB1ENR_COMPEN;
		break;
	case RCC_DAC:
		RCC_APB1ENR &= ~RCC_APB1ENR_DACEN;
		break;
	case RCC_PWR:
		RCC_APB1ENR &= ~RCC_APB1ENR_PWREN;
		break;
	case RCC_USB:
		RCC_APB1ENR &= ~RCC_APB1ENR_USBEN;
		break;
	case RCC_I2C2:
		RCC_APB1ENR &= ~RCC_APB1ENR_I2C2EN;
		break;
	case RCC_I2C1:
		RCC_APB1ENR &= ~RCC_APB1ENR_I2C1EN;
		break;
	case RCC_UART5:
		RCC_APB1ENR &= ~RCC_APB1ENR_UART5EN;
		break;
	case RCC_UART4:
		RCC_APB1ENR &= ~RCC_APB1ENR_UART4EN;
		break;
	case RCC_USART3:
		RCC_APB1ENR &= ~RCC_APB1ENR_USART3EN;
		break;
	case RCC_USART2:
		RCC_APB1ENR &= ~RCC_APB1ENR_USART2EN;
		break;
	case RCC_SPI3:
		RCC_APB1ENR &= ~RCC_APB1ENR_SPI3EN;
		break;
	case RCC_SPI2:
		RCC_APB1ENR &= ~RCC_APB1ENR_SPI2EN;
		break;
	case RCC_WWDG:
		RCC_APB1ENR &= ~RCC_APB1ENR_WWDGEN;
		break;
	case RCC_LCD:
		RCC_APB1ENR &= ~RCC_APB1ENR_LCDEN;
		break;
	case RCC_TIM7:
		RCC_APB1ENR &= ~RCC_APB1ENR_TIM7EN;
		break;
	case RCC_TIM6:
		RCC_APB1ENR &= ~RCC_APB1ENR_TIM6EN;
		break;
	case RCC_TIM5:
		RCC_APB1ENR &= ~RCC_APB1ENR_TIM5EN;
		break;
	case RCC_TIM4:
		RCC_APB1ENR &= ~RCC_APB1ENR_TIM4EN;
		break;
	case RCC_TIM3:
		RCC_APB1ENR &= ~RCC_APB1ENR_TIM3EN;
		break;
	case RCC_TIM2:
		RCC_APB1ENR &= ~RCC_APB1ENR_TIM2EN;
		break;

	case RCC_RTC:
		RCC_CSR &= ~RCC_CSR_RTCEN;
		break;

	default:
		break;
	}
}

void rcc_enable_clock_low_power(rcc_peripheral_t peripheral)
{
	switch (peripheral) {
	case RCC_FSMC:
		RCC_AHBLPENR |= RCC_AHBLPENR_FSMCLPEN;
		break;
	case RCC_AES:
		RCC_AHBLPENR |= RCC_AHBLPENR_AESLPEN;
		break;
	case RCC_DMA2:
		RCC_AHBLPENR |= RCC_AHBLPENR_DMA2LPEN;
		break;
	case RCC_DMA1:
		RCC_AHBLPENR |= RCC_AHBLPENR_DMA1LPEN;
		break;
	case RCC_SRAM:
		RCC_AHBLPENR |= RCC_AHBLPENR_SRAMLPEN;
		break;
	case RCC_FLITF:
		RCC_AHBLPENR |= RCC_AHBLPENR_FLITFLPEN;
		break;
	case RCC_CRC:
		RCC_AHBLPENR |= RCC_AHBLPENR_CRCLPEN;
		break;
	case RCC_GPIOG:
		RCC_AHBLPENR |= RCC_AHBLPENR_GPIOGLPEN;
		break;
	case RCC_GPIOF:
		RCC_AHBLPENR |= RCC_AHBLPENR_GPIOFLPEN;
		break;
	case RCC_GPIOH:
		RCC_AHBLPENR |= RCC_AHBLPENR_GPIOHLPEN;
		break;
	case RCC_GPIOE:
		RCC_AHBLPENR |= RCC_AHBLPENR_GPIOELPEN;
		break;
	case RCC_GPIOD:
		RCC_AHBLPENR |= RCC_AHBLPENR_GPIODLPEN;
		break;
	case RCC_GPIOC:
		RCC_AHBLPENR |= RCC_AHBLPENR_GPIOCLPEN;
		break;
	case RCC_GPIOB:
		RCC_AHBLPENR |= RCC_AHBLPENR_GPIOBLPEN;
		break;
	case RCC_GPIOA:
		RCC_AHBLPENR |= RCC_AHBLPENR_GPIOALPEN;
		break;

	case RCC_USART1:
		RCC_APB2LPENR |= RCC_APB2LPENR_USART1LPEN;
		break;
	case RCC_SPI1:
		RCC_APB2LPENR |= RCC_APB2LPENR_SPI1LPEN;
		break;
	case RCC_SDIO:
		RCC_APB2LPENR |= RCC_APB2LPENR_SDIOLPEN;
		break;
	case RCC_ADC:
		RCC_APB2LPENR |= RCC_APB2LPENR_ADC1LPEN;
		break;
	case RCC_TIM11:
		RCC_APB2LPENR |= RCC_APB2LPENR_TIM11LPEN;
		break;
	case RCC_TIM10:
		RCC_APB2LPENR |= RCC_APB2LPENR_TIM10LPEN;
		break;
	case RCC_TIM9:
		RCC_APB2LPENR |= RCC_APB2LPENR_TIM9LPEN;
		break;
	case RCC_SYSCFG:
		RCC_APB2LPENR |= RCC_APB2LPENR_SYSCFGLPEN;
		break;

	case RCC_COMP:
		RCC_APB1LPENR |= RCC_APB1LPENR_COMPLPEN;
		break;
	case RCC_DAC:
		RCC_APB1LPENR |= RCC_APB1LPENR_DACLPEN;
		break;
	case RCC_PWR:
		RCC_APB1LPENR |= RCC_APB1LPENR_PWRLPEN;
		break;
	case RCC_USB:
		RCC_APB1LPENR |= RCC_APB1LPENR_USBLPEN;
		break;
	case RCC_I2C2:
		RCC_APB1LPENR |= RCC_APB1LPENR_I2C2LPEN;
		break;
	case RCC_I2C1:
		RCC_APB1LPENR |= RCC_APB1LPENR_I2C1LPEN;
		break;
	case RCC_UART5:
		RCC_APB1LPENR |= RCC_APB1LPENR_UART5LPEN;
		break;
	case RCC_UART4:
		RCC_APB1LPENR |= RCC_APB1LPENR_UART4LPEN;
		break;
	case RCC_USART3:
		RCC_APB1LPENR |= RCC_APB1LPENR_USART3LPEN;
		break;
	case RCC_USART2:
		RCC_APB1LPENR |= RCC_APB1LPENR_USART2LPEN;
		break;
	case RCC_SPI3:
		RCC_APB1LPENR |= RCC_APB1LPENR_SPI3LPEN;
		break;
	case RCC_SPI2:
		RCC_APB1LPENR |= RCC_APB1LPENR_SPI2LPEN;
		break;
	case RCC_WWDG:
		RCC_APB1LPENR |= RCC_APB1LPENR_WWDGLPEN;
		break;
	case RCC_LCD:
		RCC_APB1LPENR |= RCC_APB1LPENR_LCDLPEN;
		break;
	case RCC_TIM7:
		RCC_APB1LPENR |= RCC_APB1LPENR_TIM7LPEN;
		break;
	case RCC_TIM6:
		RCC_APB1LPENR |= RCC_APB1LPENR_TIM6LPEN;
		break;
	case RCC_TIM5:
		RCC_APB1LPENR |= RCC_APB1LPENR_TIM5LPEN;
		break;
	case RCC_TIM4:
		RCC_APB1LPENR |= RCC_APB1LPENR_TIM4LPEN;
		break;
	case RCC_TIM3:
		RCC_APB1LPENR |= RCC_APB1LPENR_TIM3LPEN;
		break;
	case RCC_TIM2:
		RCC_APB1LPENR |= RCC_APB1LPENR_TIM2LPEN;
		break;

	default:
		break;
	}
}

void rcc_disable_clock_low_power(rcc_peripheral_t peripheral)
{
	switch (peripheral) {
	case RCC_FSMC:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_FSMCLPEN;
		break;
	case RCC_AES:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_AESLPEN;
		break;
	case RCC_DMA2:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_DMA2LPEN;
		break;
	case RCC_DMA1:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_DMA1LPEN;
		break;
	case RCC_SRAM:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_SRAMLPEN;
		break;
	case RCC_FLITF:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_FLITFLPEN;
		break;
	case RCC_CRC:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_CRCLPEN;
		break;
	case RCC_GPIOG:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_GPIOGLPEN;
		break;
	case RCC_GPIOF:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_GPIOFLPEN;
		break;
	case RCC_GPIOH:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_GPIOHLPEN;
		break;
	case RCC_GPIOE:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_GPIOELPEN;
		break;
	case RCC_GPIOD:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_GPIODLPEN;
		break;
	case RCC_GPIOC:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_GPIOCLPEN;
		break;
	case RCC_GPIOB:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_GPIOBLPEN;
		break;
	case RCC_GPIOA:
		RCC_AHBLPENR &= ~RCC_AHBLPENR_GPIOALPEN;
		break;

	case RCC_USART1:
		RCC_APB2LPENR &= ~RCC_APB2LPENR_USART1LPEN;
		break;
	case RCC_SPI1:
		RCC_APB2LPENR &= ~RCC_APB2LPENR_SPI1LPEN;
		break;
	case RCC_SDIO:
		RCC_APB2LPENR &= ~RCC_APB2LPENR_SDIOLPEN;
		break;
	case RCC_ADC:
		RCC_APB2LPENR &= ~RCC_APB2LPENR_ADC1LPEN;
		break;
	case RCC_TIM11:
		RCC_APB2LPENR &= ~RCC_APB2LPENR_TIM11LPEN;
		break;
	case RCC_TIM10:
		RCC_APB2LPENR &= ~RCC_APB2LPENR_TIM10LPEN;
		break;
	case RCC_TIM9:
		RCC_APB2LPENR &= ~RCC_APB2LPENR_TIM9LPEN;
		break;
	case RCC_SYSCFG:
		RCC_APB2LPENR &= ~RCC_APB2LPENR_SYSCFGLPEN;
		break;

	case RCC_COMP:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_COMPLPEN;
		break;
	case RCC_DAC:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_DACLPEN;
		break;
	case RCC_PWR:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_PWRLPEN;
		break;
	case RCC_USB:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_USBLPEN;
		break;
	case RCC_I2C2:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_I2C2LPEN;
		break;
	case RCC_I2C1:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_I2C1LPEN;
		break;
	case RCC_UART5:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_UART5LPEN;
		break;
	case RCC_UART4:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_UART4LPEN;
		break;
	case RCC_USART3:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_USART3LPEN;
		break;
	case RCC_USART2:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_USART2LPEN;
		break;
	case RCC_SPI3:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_SPI3LPEN;
		break;
	case RCC_SPI2:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_SPI2LPEN;
		break;
	case RCC_WWDG:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_WWDGLPEN;
		break;
	case RCC_LCD:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_LCDLPEN;
		break;
	case RCC_TIM7:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_TIM7LPEN;
		break;
	case RCC_TIM6:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_TIM6LPEN;
		break;
	case RCC_TIM5:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_TIM5LPEN;
		break;
	case RCC_TIM4:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_TIM4LPEN;
		break;
	case RCC_TIM3:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_TIM3LPEN;
		break;
	case RCC_TIM2:
		RCC_APB1LPENR &= ~RCC_APB1LPENR_TIM2LPEN;
		break;

	default:
		break;
	}
}

int rcc_get_reset_flag(int flag)
{
	return RCC_CSR & flag;
}

void rcc_clear_reset_flag(void)
{
	RCC_CSR |= RCC_CSR_RMVF;
}

void rcc_enable_lsecss(void)
{
	RCC_CSR |= RCC_CSR_LSECSSON;
}

int rcc_get_lsecss_status(void)
{
	return RCC_CSR & (RCC_CSR_LSECSSD | RCC_CSR_LSECSSON);
}
