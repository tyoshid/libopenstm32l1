/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
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
 * 6 General-purpose I/Os (GPIO)
 *
 *
 * DS6876: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         16KB SRAM, 4KB EEPROM, LCD, USB, ADC, DAC
 * (07-Feb-2013 Rev 8)
 *
 * Table 10. Alternate function input/output
 *
 *
 * DS8890: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 256KB Flash,
 *         32KB SRAM, 8KB EEPROM, LCD, USB, ADC, DAC
 * (01-Feb-2013 Rev 3)
 *
 * Table 10. Alternate function input/output
 *
 *
 * DS8576: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F
 * (01-Feb-2013 Rev 6)
 *
 * Table 10. Alternate function input/output
 *
 *
 * DS8669: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F, AES
 * (06-Feb-2013 Rev 4)
 *
 * Table 9. Alternate function input/output
 *
 *
 * DS9496: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         10KB SRAM, 2KB EEPROM, LCD, USB, ADC, DAC
 * (21-Feb-2013 Rev 1)
 *
 * Table 8. Alternate function input/output
 */

/*
 * Note: Registers and bits marked (**) only exist in high and medium+ density
 * devices.
 */
/* Note: Registers and bits marked (***) only exist in high density devices. */

/* --- GPIO registers ------------------------------------------------------ */
/*
 * Offset	Register
 * 0x00		GPIOx_MODER		GPIO port mode register
 * 0x04		GPIOx_OTYPER		GPIO port output type register
 * 0x08		GPIOx_OSPEEDR		GPIO port output speed register
 * 0x0c		GPIOx_PUPDR		GPIO port pull-up/pull-down register
 * 0x10		GPIOx_IDR		GPIO port input data register
 * 0x14		GPIOx_ODR		GPIO port output data register
 * 0x18		GPIOx_BSRR		GPIO port bit set/reset register
 * 0x1c		GPIOx_LCKR		GPIO port configuration lock register
 * 0x20		GPIOx_AFRL		GPIO alternate function low register
 * 0x24		GPIOx_AFRH		GPIO alternate function high register
 */

/* GPIOx_MODER */
#define GPIO_MODER(base)		MMIO32((base) + 0x00)
#define GPIOA_MODER			GPIO_MODER(GPIO_PORT_A_BASE)
#define GPIOB_MODER			GPIO_MODER(GPIO_PORT_B_BASE)
#define GPIOC_MODER			GPIO_MODER(GPIO_PORT_C_BASE)
#define GPIOD_MODER			GPIO_MODER(GPIO_PORT_D_BASE)
#define GPIOE_MODER			GPIO_MODER(GPIO_PORT_E_BASE)
#define GPIOF_MODER			GPIO_MODER(GPIO_PORT_F_BASE) /* (**) */
#define GPIOG_MODER			GPIO_MODER(GPIO_PORT_G_BASE) /* (**) */
#define GPIOH_MODER			GPIO_MODER(GPIO_PORT_H_BASE)

/* GPIOx_OTYPER */
#define GPIO_OTYPER(base)		MMIO32((base) + 0x04)
#define GPIOA_OTYPER			GPIO_OTYPER(GPIO_PORT_A_BASE)
#define GPIOB_OTYPER			GPIO_OTYPER(GPIO_PORT_B_BASE)
#define GPIOC_OTYPER			GPIO_OTYPER(GPIO_PORT_C_BASE)
#define GPIOD_OTYPER			GPIO_OTYPER(GPIO_PORT_D_BASE)
#define GPIOE_OTYPER			GPIO_OTYPER(GPIO_PORT_E_BASE)
#define GPIOF_OTYPER			GPIO_OTYPER(GPIO_PORT_F_BASE) /* (**) */
#define GPIOG_OTYPER			GPIO_OTYPER(GPIO_PORT_G_BASE) /* (**) */
#define GPIOH_OTYPER			GPIO_OTYPER(GPIO_PORT_H_BASE)

/* GPIOx_OSPEEDR */
#define GPIO_OSPEEDR(base)		MMIO32((base) + 0x08)
#define GPIOA_OSPEEDR			GPIO_OSPEEDR(GPIO_PORT_A_BASE)
#define GPIOB_OSPEEDR			GPIO_OSPEEDR(GPIO_PORT_B_BASE)
#define GPIOC_OSPEEDR			GPIO_OSPEEDR(GPIO_PORT_C_BASE)
#define GPIOD_OSPEEDR			GPIO_OSPEEDR(GPIO_PORT_D_BASE)
#define GPIOE_OSPEEDR			GPIO_OSPEEDR(GPIO_PORT_E_BASE)
#define GPIOF_OSPEEDR			GPIO_OSPEEDR(GPIO_PORT_F_BASE) /*(**)*/
#define GPIOG_OSPEEDR			GPIO_OSPEEDR(GPIO_PORT_G_BASE) /*(**)*/
#define GPIOH_OSPEEDR			GPIO_OSPEEDR(GPIO_PORT_H_BASE)

/* GPIOx_PUPDR */
#define GPIO_PUPDR(base)		MMIO32((base) + 0x0c)
#define GPIOA_PUPDR			GPIO_PUPDR(GPIO_PORT_A_BASE)
#define GPIOB_PUPDR			GPIO_PUPDR(GPIO_PORT_B_BASE)
#define GPIOC_PUPDR			GPIO_PUPDR(GPIO_PORT_C_BASE)
#define GPIOD_PUPDR			GPIO_PUPDR(GPIO_PORT_D_BASE)
#define GPIOE_PUPDR			GPIO_PUPDR(GPIO_PORT_E_BASE)
#define GPIOF_PUPDR			GPIO_PUPDR(GPIO_PORT_F_BASE) /* (**) */
#define GPIOG_PUPDR			GPIO_PUPDR(GPIO_PORT_G_BASE) /* (**) */
#define GPIOH_PUPDR			GPIO_PUPDR(GPIO_PORT_H_BASE)

/* GPIOx_IDR */
#define GPIO_IDR(base)			MMIO32((base) + 0x10)
#define GPIOA_IDR			GPIO_IDR(GPIO_PORT_A_BASE)
#define GPIOB_IDR			GPIO_IDR(GPIO_PORT_B_BASE)
#define GPIOC_IDR			GPIO_IDR(GPIO_PORT_C_BASE)
#define GPIOD_IDR			GPIO_IDR(GPIO_PORT_D_BASE)
#define GPIOE_IDR			GPIO_IDR(GPIO_PORT_E_BASE)
#define GPIOF_IDR			GPIO_IDR(GPIO_PORT_F_BASE) /* (**) */
#define GPIOG_IDR			GPIO_IDR(GPIO_PORT_G_BASE) /* (**) */
#define GPIOH_IDR			GPIO_IDR(GPIO_PORT_H_BASE)

/* GPIOx_ODR */
#define GPIO_ODR(base)			MMIO32((base) + 0x14)
#define GPIOA_ODR			GPIO_ODR(GPIO_PORT_A_BASE)
#define GPIOB_ODR			GPIO_ODR(GPIO_PORT_B_BASE)
#define GPIOC_ODR			GPIO_ODR(GPIO_PORT_C_BASE)
#define GPIOD_ODR			GPIO_ODR(GPIO_PORT_D_BASE)
#define GPIOE_ODR			GPIO_ODR(GPIO_PORT_E_BASE)
#define GPIOF_ODR			GPIO_ODR(GPIO_PORT_F_BASE) /* (**) */
#define GPIOG_ODR			GPIO_ODR(GPIO_PORT_G_BASE) /* (**) */
#define GPIOH_ODR			GPIO_ODR(GPIO_PORT_H_BASE)

/* GPIOx_BSRR */
#define GPIO_BSRR(base)			MMIO32((base) + 0x18)
#define GPIOA_BSRR			GPIO_BSRR(GPIO_PORT_A_BASE)
#define GPIOB_BSRR			GPIO_BSRR(GPIO_PORT_B_BASE)
#define GPIOC_BSRR			GPIO_BSRR(GPIO_PORT_C_BASE)
#define GPIOD_BSRR			GPIO_BSRR(GPIO_PORT_D_BASE)
#define GPIOE_BSRR			GPIO_BSRR(GPIO_PORT_E_BASE)
#define GPIOF_BSRR			GPIO_BSRR(GPIO_PORT_F_BASE) /* (**) */
#define GPIOG_BSRR			GPIO_BSRR(GPIO_PORT_G_BASE) /* (**) */
#define GPIOH_BSRR			GPIO_BSRR(GPIO_PORT_H_BASE)

/* GPIOx_LCKR */
#define GPIO_LCKR(base)			MMIO32((base) + 0x1c)
#define GPIOA_LCKR			GPIO_LCKR(GPIO_PORT_A_BASE)
#define GPIOB_LCKR			GPIO_LCKR(GPIO_PORT_B_BASE)
#define GPIOC_LCKR			GPIO_LCKR(GPIO_PORT_C_BASE)
#define GPIOD_LCKR			GPIO_LCKR(GPIO_PORT_D_BASE)
#define GPIOE_LCKR			GPIO_LCKR(GPIO_PORT_E_BASE)
#define GPIOF_LCKR			GPIO_LCKR(GPIO_PORT_F_BASE) /* (**) */
#define GPIOG_LCKR			GPIO_LCKR(GPIO_PORT_G_BASE) /* (**) */
#define GPIOH_LCKR			GPIO_LCKR(GPIO_PORT_H_BASE)

/* GPIOx_AFRL */
#define GPIO_AFRL(base)			MMIO32((base) + 0x20)
#define GPIOA_AFRL			GPIO_AFRL(GPIO_PORT_A_BASE)
#define GPIOB_AFRL			GPIO_AFRL(GPIO_PORT_B_BASE)
#define GPIOC_AFRL			GPIO_AFRL(GPIO_PORT_C_BASE)
#define GPIOD_AFRL			GPIO_AFRL(GPIO_PORT_D_BASE)
#define GPIOE_AFRL			GPIO_AFRL(GPIO_PORT_E_BASE)
#define GPIOF_AFRL			GPIO_AFRL(GPIO_PORT_F_BASE) /* (**) */
#define GPIOG_AFRL			GPIO_AFRL(GPIO_PORT_G_BASE) /* (**) */
#define GPIOH_AFRL			GPIO_AFRL(GPIO_PORT_H_BASE)

/* GPIOx_AFRH */
#define GPIO_AFRH(base)			MMIO32((base) + 0x24)
#define GPIOA_AFRH			GPIO_AFRH(GPIO_PORT_A_BASE)
#define GPIOB_AFRH			GPIO_AFRH(GPIO_PORT_B_BASE)
#define GPIOC_AFRH			GPIO_AFRH(GPIO_PORT_C_BASE)
#define GPIOD_AFRH			GPIO_AFRH(GPIO_PORT_D_BASE)
#define GPIOE_AFRH			GPIO_AFRH(GPIO_PORT_E_BASE)
#define GPIOF_AFRH			GPIO_AFRH(GPIO_PORT_F_BASE) /* (**) */
#define GPIOG_AFRH			GPIO_AFRH(GPIO_PORT_G_BASE) /* (**) */
#define GPIOH_AFRH			GPIO_AFRH(GPIO_PORT_H_BASE)

/* --- GPIO_MODER values --------------------------------------------------- */

/* GPIO_MODER[2y+1:2y]: MODERy[1:0]: Port configuration bits (y = 0..15) */
#define GPIO_MODER_INPUT		0
#define GPIO_MODER_OUTPUT		1
#define GPIO_MODER_ALTFN		2
#define GPIO_MODER_ANALOG		3

/* --- GPIO_OTYPER values -------------------------------------------------- */

/* GPIO_OTYPER[15:0]: OTy: Port configuration bits (y = 0..15) */
#define GPIO_OTYPER_PUSHPULL		0
#define GPIO_OTYPER_OPENDRAIN		1

/* --- GPIO_OSPEEDR values ------------------------------------------------- */

/* GPIO_OSPEEDR[2y+1:2y]: OSPEEDRy[1:0]: Port configuration bits (y = 0..15) */
#define GPIO_OSPEEDR_400_KHZ		0
#define GPIO_OSPEEDR_2_MHZ		1
#define GPIO_OSPEEDR_10_MHZ		2
#define GPIO_OSPEEDR_40_MHZ		3

/* --- GPIO_PUPDR values --------------------------------------------------- */

/* GPIO_PUPDR[2y+1:2y]: PUPDRy[1:0]: Port configuration bits (y = 0..15) */
#define GPIO_PUPDR_NONE			0
#define GPIO_PUPDR_FLOAT		0
#define GPIO_PUPDR_PULLUP		1
#define GPIO_PUPDR_PULLDOWN		2

/* --- GPIO_IDR values ----------------------------------------------------- */

/* GPIO_IDR[15:0]: IDRy: Port input data (y = 0..15) */

/* --- GPIO_ODR values ----------------------------------------------------- */

/* GPIO_ODR[15:0]: ODRy: Port output data (y = 0..15) */

/* --- GPIO_BSRR values ---------------------------------------------------- */

/* GPIO_BSRR[31:16]: BRy: Port reset bit y (y = 0..15) */
/* GPIO_BSRR[15:0]: BSy: Port set bit y (y = 0..15) */

/* --- GPIO_LCKR values ---------------------------------------------------- */

#define GPIO_LCKK			(1 << 16)
/* GPIO_LCKR[15:0]: LCKy: Port lock bit y (y = 0..15) */

/* --- GPIO_AFRL/GPIO_AFRH values ------------------------------------------ */

/*
 * GPIO_AFRL[4y+3:4y]: AFRLy[3:0]: Alternate function selection (y = 0..7)
 * GPIO_AFRH[4(y-8)+3:4(y-8)]: AFRHy[3:0]: Alternate function selection
 *   (y = 8..15)
 */
#define GPIO_AFR_AF0			0
#define GPIO_AFR_AF1			1
#define GPIO_AFR_AF2			2
#define GPIO_AFR_AF3			3
#define GPIO_AFR_AF4			4
#define GPIO_AFR_AF5			5
#define GPIO_AFR_AF6			6
#define GPIO_AFR_AF7			7
#define GPIO_AFR_AF8			8
#define GPIO_AFR_AF9			9
#define GPIO_AFR_AF10			10
#define GPIO_AFR_AF11			11
#define GPIO_AFR_AF12			12
#define GPIO_AFR_AF13			13
#define GPIO_AFR_AF14			14
#define GPIO_AFR_AF15			15

/* --- Port definitions ---------------------------------------------------- */

/* GPIO port name definitions */
enum {
	GPIOA = (0 << 16),
	GPIOB = (1 << 16),
	GPIOC = (2 << 16),
	GPIOD = (3 << 16),
	GPIOE = (4 << 16),
	GPIOF = (5 << 16),	/* (**) */
	GPIOG = (6 << 16),	/* (**) */
	GPIOH = (7 << 16)
};

#define GPIO_PORT(x)			(x & ~0xffff)

/* GPIO bit number definitions */
enum {
	GPIO0 = (1 << 0),
	GPIO1 = (1 << 1),
	GPIO2 = (1 << 2),
	GPIO3 = (1 << 3),
	GPIO4 = (1 << 4),
	GPIO5 = (1 << 5),
	GPIO6 = (1 << 6),
	GPIO7 = (1 << 7),
	GPIO8 = (1 << 8),
	GPIO9 = (1 << 9),
	GPIO10 = (1 << 10),
	GPIO11 = (1 << 11),
	GPIO12 = (1 << 12),
	GPIO13 = (1 << 13),
	GPIO14 = (1 << 14),
	GPIO15 = (1 << 15),
	GPIO_ALL = 0xffff
};

#define GPIO_BITS(x)			(x & 0xffff)

/* GPIO port name and bit number definitions */
/* GPIOA */
#define GPIO_PA0			(GPIOA | GPIO0)
#define GPIO_PA1			(GPIOA | GPIO1)
#define GPIO_PA2			(GPIOA | GPIO2)
#define GPIO_PA3			(GPIOA | GPIO3)
#define GPIO_PA4			(GPIOA | GPIO4)
#define GPIO_PA5			(GPIOA | GPIO5)
#define GPIO_PA6			(GPIOA | GPIO6)
#define GPIO_PA7			(GPIOA | GPIO7)
#define GPIO_PA8			(GPIOA | GPIO8)
#define GPIO_PA9			(GPIOA | GPIO9)
#define GPIO_PA10			(GPIOA | GPIO10)
#define GPIO_PA11			(GPIOA | GPIO11)
#define GPIO_PA12			(GPIOA | GPIO12)
#define GPIO_PA13			(GPIOA | GPIO13)
#define GPIO_PA14			(GPIOA | GPIO14)
#define GPIO_PA15			(GPIOA | GPIO15)

#define GPIO_PA_0			(GPIOA | GPIO0)
#define GPIO_PA_1			(GPIOA | GPIO1)
#define GPIO_PA_2			(GPIOA | GPIO2)
#define GPIO_PA_3			(GPIOA | GPIO3)
#define GPIO_PA_4			(GPIOA | GPIO4)
#define GPIO_PA_5			(GPIOA | GPIO5)
#define GPIO_PA_6			(GPIOA | GPIO6)
#define GPIO_PA_7			(GPIOA | GPIO7)
#define GPIO_PA_8			(GPIOA | GPIO8)
#define GPIO_PA_9			(GPIOA | GPIO9)
#define GPIO_PA_10			(GPIOA | GPIO10)
#define GPIO_PA_11			(GPIOA | GPIO11)
#define GPIO_PA_12			(GPIOA | GPIO12)
#define GPIO_PA_13			(GPIOA | GPIO13)
#define GPIO_PA_14			(GPIOA | GPIO14)
#define GPIO_PA_15			(GPIOA | GPIO15)
#define GPIO_PA_ALL			(GPIOA | GPIO_ALL)
#define GPIO_PA_NONE			0

/* GPIOB */
#define GPIO_PB0			(GPIOB | GPIO0)
#define GPIO_PB1			(GPIOB | GPIO1)
#define GPIO_PB2			(GPIOB | GPIO2)
#define GPIO_PB3			(GPIOB | GPIO3)
#define GPIO_PB4			(GPIOB | GPIO4)
#define GPIO_PB5			(GPIOB | GPIO5)
#define GPIO_PB6			(GPIOB | GPIO6)
#define GPIO_PB7			(GPIOB | GPIO7)
#define GPIO_PB8			(GPIOB | GPIO8)
#define GPIO_PB9			(GPIOB | GPIO9)
#define GPIO_PB10			(GPIOB | GPIO10)
#define GPIO_PB11			(GPIOB | GPIO11)
#define GPIO_PB12			(GPIOB | GPIO12)
#define GPIO_PB13			(GPIOB | GPIO13)
#define GPIO_PB14			(GPIOB | GPIO14)
#define GPIO_PB15			(GPIOB | GPIO15)

#define GPIO_PB_0			(GPIOB | GPIO0)
#define GPIO_PB_1			(GPIOB | GPIO1)
#define GPIO_PB_2			(GPIOB | GPIO2)
#define GPIO_PB_3			(GPIOB | GPIO3)
#define GPIO_PB_4			(GPIOB | GPIO4)
#define GPIO_PB_5			(GPIOB | GPIO5)
#define GPIO_PB_6			(GPIOB | GPIO6)
#define GPIO_PB_7			(GPIOB | GPIO7)
#define GPIO_PB_8			(GPIOB | GPIO8)
#define GPIO_PB_9			(GPIOB | GPIO9)
#define GPIO_PB_10			(GPIOB | GPIO10)
#define GPIO_PB_11			(GPIOB | GPIO11)
#define GPIO_PB_12			(GPIOB | GPIO12)
#define GPIO_PB_13			(GPIOB | GPIO13)
#define GPIO_PB_14			(GPIOB | GPIO14)
#define GPIO_PB_15			(GPIOB | GPIO15)
#define GPIO_PB_ALL			(GPIOB | GPIO_ALL)
#define GPIO_PB_NONE			0

/* GPIOC */
#define GPIO_PC0			(GPIOC | GPIO0)
#define GPIO_PC1			(GPIOC | GPIO1)
#define GPIO_PC2			(GPIOC | GPIO2)
#define GPIO_PC3			(GPIOC | GPIO3)
#define GPIO_PC4			(GPIOC | GPIO4)
#define GPIO_PC5			(GPIOC | GPIO5)
#define GPIO_PC6			(GPIOC | GPIO6)
#define GPIO_PC7			(GPIOC | GPIO7)
#define GPIO_PC8			(GPIOC | GPIO8)
#define GPIO_PC9			(GPIOC | GPIO9)
#define GPIO_PC10			(GPIOC | GPIO10)
#define GPIO_PC11			(GPIOC | GPIO11)
#define GPIO_PC12			(GPIOC | GPIO12)
#define GPIO_PC13			(GPIOC | GPIO13)
#define GPIO_PC14			(GPIOC | GPIO14)
#define GPIO_PC15			(GPIOC | GPIO15)

#define GPIO_PC_0			(GPIOC | GPIO0)
#define GPIO_PC_1			(GPIOC | GPIO1)
#define GPIO_PC_2			(GPIOC | GPIO2)
#define GPIO_PC_3			(GPIOC | GPIO3)
#define GPIO_PC_4			(GPIOC | GPIO4)
#define GPIO_PC_5			(GPIOC | GPIO5)
#define GPIO_PC_6			(GPIOC | GPIO6)
#define GPIO_PC_7			(GPIOC | GPIO7)
#define GPIO_PC_8			(GPIOC | GPIO8)
#define GPIO_PC_9			(GPIOC | GPIO9)
#define GPIO_PC_10			(GPIOC | GPIO10)
#define GPIO_PC_11			(GPIOC | GPIO11)
#define GPIO_PC_12			(GPIOC | GPIO12)
#define GPIO_PC_13			(GPIOC | GPIO13)
#define GPIO_PC_14			(GPIOC | GPIO14)
#define GPIO_PC_15			(GPIOC | GPIO15)
#define GPIO_PC_ALL			(GPIOC | GPIO_ALL)
#define GPIO_PC_NONE			0

/* GPIOD */
#define GPIO_PD0			(GPIOD | GPIO0)
#define GPIO_PD1			(GPIOD | GPIO1)
#define GPIO_PD2			(GPIOD | GPIO2)
#define GPIO_PD3			(GPIOD | GPIO3)
#define GPIO_PD4			(GPIOD | GPIO4)
#define GPIO_PD5			(GPIOD | GPIO5)
#define GPIO_PD6			(GPIOD | GPIO6)
#define GPIO_PD7			(GPIOD | GPIO7)
#define GPIO_PD8			(GPIOD | GPIO8)
#define GPIO_PD9			(GPIOD | GPIO9)
#define GPIO_PD10			(GPIOD | GPIO10)
#define GPIO_PD11			(GPIOD | GPIO11)
#define GPIO_PD12			(GPIOD | GPIO12)
#define GPIO_PD13			(GPIOD | GPIO13)
#define GPIO_PD14			(GPIOD | GPIO14)
#define GPIO_PD15			(GPIOD | GPIO15)

#define GPIO_PD_0			(GPIOD | GPIO0)
#define GPIO_PD_1			(GPIOD | GPIO1)
#define GPIO_PD_2			(GPIOD | GPIO2)
#define GPIO_PD_3			(GPIOD | GPIO3)
#define GPIO_PD_4			(GPIOD | GPIO4)
#define GPIO_PD_5			(GPIOD | GPIO5)
#define GPIO_PD_6			(GPIOD | GPIO6)
#define GPIO_PD_7			(GPIOD | GPIO7)
#define GPIO_PD_8			(GPIOD | GPIO8)
#define GPIO_PD_9			(GPIOD | GPIO9)
#define GPIO_PD_10			(GPIOD | GPIO10)
#define GPIO_PD_11			(GPIOD | GPIO11)
#define GPIO_PD_12			(GPIOD | GPIO12)
#define GPIO_PD_13			(GPIOD | GPIO13)
#define GPIO_PD_14			(GPIOD | GPIO14)
#define GPIO_PD_15			(GPIOD | GPIO15)
#define GPIO_PD_ALL			(GPIOD | GPIO_ALL)
#define GPIO_PD_NONE			0

/* GPIOE */
#define GPIO_PE0			(GPIOE | GPIO0)
#define GPIO_PE1			(GPIOE | GPIO1)
#define GPIO_PE2			(GPIOE | GPIO2)
#define GPIO_PE3			(GPIOE | GPIO3)
#define GPIO_PE4			(GPIOE | GPIO4)
#define GPIO_PE5			(GPIOE | GPIO5)
#define GPIO_PE6			(GPIOE | GPIO6)
#define GPIO_PE7			(GPIOE | GPIO7)
#define GPIO_PE8			(GPIOE | GPIO8)
#define GPIO_PE9			(GPIOE | GPIO9)
#define GPIO_PE10			(GPIOE | GPIO10)
#define GPIO_PE11			(GPIOE | GPIO11)
#define GPIO_PE12			(GPIOE | GPIO12)
#define GPIO_PE13			(GPIOE | GPIO13)
#define GPIO_PE14			(GPIOE | GPIO14)
#define GPIO_PE15			(GPIOE | GPIO15)

#define GPIO_PE_0			(GPIOE | GPIO0)
#define GPIO_PE_1			(GPIOE | GPIO1)
#define GPIO_PE_2			(GPIOE | GPIO2)
#define GPIO_PE_3			(GPIOE | GPIO3)
#define GPIO_PE_4			(GPIOE | GPIO4)
#define GPIO_PE_5			(GPIOE | GPIO5)
#define GPIO_PE_6			(GPIOE | GPIO6)
#define GPIO_PE_7			(GPIOE | GPIO7)
#define GPIO_PE_8			(GPIOE | GPIO8)
#define GPIO_PE_9			(GPIOE | GPIO9)
#define GPIO_PE_10			(GPIOE | GPIO10)
#define GPIO_PE_11			(GPIOE | GPIO11)
#define GPIO_PE_12			(GPIOE | GPIO12)
#define GPIO_PE_13			(GPIOE | GPIO13)
#define GPIO_PE_14			(GPIOE | GPIO14)
#define GPIO_PE_15			(GPIOE | GPIO15)
#define GPIO_PE_ALL			(GPIOE | GPIO_ALL)
#define GPIO_PE_NONE			0

/* GPIOF (**) */
#define GPIO_PF0			(GPIOF | GPIO0)
#define GPIO_PF1			(GPIOF | GPIO1)
#define GPIO_PF2			(GPIOF | GPIO2)
#define GPIO_PF3			(GPIOF | GPIO3)
#define GPIO_PF4			(GPIOF | GPIO4)
#define GPIO_PF5			(GPIOF | GPIO5)
#define GPIO_PF6			(GPIOF | GPIO6)
#define GPIO_PF7			(GPIOF | GPIO7)
#define GPIO_PF8			(GPIOF | GPIO8)
#define GPIO_PF9			(GPIOF | GPIO9)
#define GPIO_PF10			(GPIOF | GPIO10)
#define GPIO_PF11			(GPIOF | GPIO11)
#define GPIO_PF12			(GPIOF | GPIO12)
#define GPIO_PF13			(GPIOF | GPIO13)
#define GPIO_PF14			(GPIOF | GPIO14)
#define GPIO_PF15			(GPIOF | GPIO15)

#define GPIO_PF_0			(GPIOF | GPIO0)
#define GPIO_PF_1			(GPIOF | GPIO1)
#define GPIO_PF_2			(GPIOF | GPIO2)
#define GPIO_PF_3			(GPIOF | GPIO3)
#define GPIO_PF_4			(GPIOF | GPIO4)
#define GPIO_PF_5			(GPIOF | GPIO5)
#define GPIO_PF_6			(GPIOF | GPIO6)
#define GPIO_PF_7			(GPIOF | GPIO7)
#define GPIO_PF_8			(GPIOF | GPIO8)
#define GPIO_PF_9			(GPIOF | GPIO9)
#define GPIO_PF_10			(GPIOF | GPIO10)
#define GPIO_PF_11			(GPIOF | GPIO11)
#define GPIO_PF_12			(GPIOF | GPIO12)
#define GPIO_PF_13			(GPIOF | GPIO13)
#define GPIO_PF_14			(GPIOF | GPIO14)
#define GPIO_PF_15			(GPIOF | GPIO15)
#define GPIO_PF_ALL			(GPIOF | GPIO_ALL)
#define GPIO_PF_NONE			0

/* GPIOG (**) */
#define GPIO_PG0			(GPIOG | GPIO0)
#define GPIO_PG1			(GPIOG | GPIO1)
#define GPIO_PG2			(GPIOG | GPIO2)
#define GPIO_PG3			(GPIOG | GPIO3)
#define GPIO_PG4			(GPIOG | GPIO4)
#define GPIO_PG5			(GPIOG | GPIO5)
#define GPIO_PG6			(GPIOG | GPIO6)
#define GPIO_PG7			(GPIOG | GPIO7)
#define GPIO_PG8			(GPIOG | GPIO8)
#define GPIO_PG9			(GPIOG | GPIO9)
#define GPIO_PG10			(GPIOG | GPIO10)
#define GPIO_PG11			(GPIOG | GPIO11)
#define GPIO_PG12			(GPIOG | GPIO12)
#define GPIO_PG13			(GPIOG | GPIO13)
#define GPIO_PG14			(GPIOG | GPIO14)
#define GPIO_PG15			(GPIOG | GPIO15)

#define GPIO_PG_0			(GPIOG | GPIO0)
#define GPIO_PG_1			(GPIOG | GPIO1)
#define GPIO_PG_2			(GPIOG | GPIO2)
#define GPIO_PG_3			(GPIOG | GPIO3)
#define GPIO_PG_4			(GPIOG | GPIO4)
#define GPIO_PG_5			(GPIOG | GPIO5)
#define GPIO_PG_6			(GPIOG | GPIO6)
#define GPIO_PG_7			(GPIOG | GPIO7)
#define GPIO_PG_8			(GPIOG | GPIO8)
#define GPIO_PG_9			(GPIOG | GPIO9)
#define GPIO_PG_10			(GPIOG | GPIO10)
#define GPIO_PG_11			(GPIOG | GPIO11)
#define GPIO_PG_12			(GPIOG | GPIO12)
#define GPIO_PG_13			(GPIOG | GPIO13)
#define GPIO_PG_14			(GPIOG | GPIO14)
#define GPIO_PG_15			(GPIOG | GPIO15)
#define GPIO_PG_ALL			(GPIOG | GPIO_ALL)
#define GPIO_PG_NONE			0

/* GPIOH */
#define GPIO_PH0			(GPIOH | GPIO0)
#define GPIO_PH1			(GPIOH | GPIO1)
#define GPIO_PH2			(GPIOH | GPIO2)
#define GPIO_PH3			(GPIOH | GPIO3)
#define GPIO_PH4			(GPIOH | GPIO4)
#define GPIO_PH5			(GPIOH | GPIO5)
#define GPIO_PH6			(GPIOH | GPIO6)
#define GPIO_PH7			(GPIOH | GPIO7)
#define GPIO_PH8			(GPIOH | GPIO8)
#define GPIO_PH9			(GPIOH | GPIO9)
#define GPIO_PH10			(GPIOH | GPIO10)
#define GPIO_PH11			(GPIOH | GPIO11)
#define GPIO_PH12			(GPIOH | GPIO12)
#define GPIO_PH13			(GPIOH | GPIO13)
#define GPIO_PH14			(GPIOH | GPIO14)
#define GPIO_PH15			(GPIOH | GPIO15)

#define GPIO_PH_0			(GPIOH | GPIO0)
#define GPIO_PH_1			(GPIOH | GPIO1)
#define GPIO_PH_2			(GPIOH | GPIO2)
#define GPIO_PH_3			(GPIOH | GPIO3)
#define GPIO_PH_4			(GPIOH | GPIO4)
#define GPIO_PH_5			(GPIOH | GPIO5)
#define GPIO_PH_6			(GPIOH | GPIO6)
#define GPIO_PH_7			(GPIOH | GPIO7)
#define GPIO_PH_8			(GPIOH | GPIO8)
#define GPIO_PH_9			(GPIOH | GPIO9)
#define GPIO_PH_10			(GPIOH | GPIO10)
#define GPIO_PH_11			(GPIOH | GPIO11)
#define GPIO_PH_12			(GPIOH | GPIO12)
#define GPIO_PH_13			(GPIOH | GPIO13)
#define GPIO_PH_14			(GPIOH | GPIO14)
#define GPIO_PH_15			(GPIOH | GPIO15)
#define GPIO_PH_ALL			(GPIOH | GPIO_ALL)
#define GPIO_PH_NONE			0

#define GPIO_CONCAT(port, num) GPIO_P##port##_##num

#define GPIO_OR(port, b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, \
		b12, b13, b14, b15, ...)				\
	(GPIO_CONCAT(port, b0) | GPIO_CONCAT(port, b1) | \
	 GPIO_CONCAT(port, b2) | GPIO_CONCAT(port, b3) | \
	 GPIO_CONCAT(port, b4) | GPIO_CONCAT(port, b5) | \
	 GPIO_CONCAT(port, b6) | GPIO_CONCAT(port, b7) | \
	 GPIO_CONCAT(port, b8) | GPIO_CONCAT(port, b9) | \
	 GPIO_CONCAT(port, b10) | GPIO_CONCAT(port, b11) | \
	 GPIO_CONCAT(port, b12) | GPIO_CONCAT(port, b13) | \
	 GPIO_CONCAT(port, b14) | GPIO_CONCAT(port, b15))

#define GPIO_PA(...) GPIO_OR(A, __VA_ARGS__, NONE, NONE, NONE, NONE, \
			     NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			     NONE, NONE, NONE, NONE)

#define GPIO_PB(...) GPIO_OR(B, __VA_ARGS__, NONE, NONE, NONE, NONE, \
			     NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			     NONE, NONE, NONE, NONE)

#define GPIO_PC(...) GPIO_OR(C, __VA_ARGS__, NONE, NONE, NONE, NONE, \
			     NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			     NONE, NONE, NONE, NONE)

#define GPIO_PD(...) GPIO_OR(D, __VA_ARGS__, NONE, NONE, NONE, NONE, \
			     NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			     NONE, NONE, NONE, NONE)

#define GPIO_PE(...) GPIO_OR(E, __VA_ARGS__, NONE, NONE, NONE, NONE, \
			     NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			     NONE, NONE, NONE, NONE)

#define GPIO_PF(...) GPIO_OR(F, __VA_ARGS__, NONE, NONE, NONE, NONE, \
			     NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			     NONE, NONE, NONE, NONE)

#define GPIO_PG(...) GPIO_OR(G, __VA_ARGS__, NONE, NONE, NONE, NONE, \
			     NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			     NONE, NONE, NONE, NONE)

#define GPIO_PH(...) GPIO_OR(H, __VA_ARGS__, NONE, NONE, NONE, NONE, \
			     NONE, NONE, NONE, NONE, NONE, NONE, NONE,	\
			     NONE, NONE, NONE, NONE)

/* --- Alternate function GPIOs -------------------------------------------- */
/* SYSTEM */
#define GPIO_PA_WKUP1			GPIO_PA0
#define GPIO_PA_MCO			GPIO_PA8
#define GPIO_PA_JTMS_SWDAT		GPIO_PA13
#define GPIO_PA_JTMS_SWCLK		GPIO_PA14
#define GPIO_PA_JTDI			GPIO_PA15

#define GPIO_PB_BOOT1			GPIO_PB2
#define GPIO_PB_JTDO			GPIO_PB3
#define GPIO_PB_JTRST			GPIO_PB4
#define GPIO_PB_50_60_HZ		GPIO_PB15

#define GPIO_PC_RTC_AF1_WKUP2		GPIO_PC13
#define GPIO_PC_OSC32_IN		GPIO_PC14
#define GPIO_PC_OSC32_OUT		GPIO_PC15

#define GPIO_PE_TRACECK			GPIO_PE2
#define GPIO_PE_TRACED0			GPIO_PE3
#define GPIO_PE_TRACED1			GPIO_PE4
#define GPIO_PE_TRACED2			GPIO_PE5
#define GPIO_PE_TRACED3_WKUP3		GPIO_PE6

#define GPIO_PH_OSC_IN			GPIO_PH0
#define GPIO_PH_OSC_OUT			GPIO_PH1

/* TIM2 */
#define GPIO_PA_TIM2_CH1_ETR		GPIO_PA0
#define GPIO_PA_TIM2_CH2		GPIO_PA1
#define GPIO_PA_TIM2_CH3		GPIO_PA2
#define GPIO_PA_TIM2_CH4		GPIO_PA3
#define GPIO_PA_TIM2_CH1_ETR_5		GPIO_PA5
#define GPIO_PA_TIM2_CH1_ETR_15		GPIO_PA15

#define GPIO_PB_TIM2_CH2		GPIO_PB3
#define GPIO_PB_TIM2_CH3		GPIO_PB10
#define GPIO_PB_TIM2_CH4		GPIO_PB11

#define GPIO_PE_TIM2_CH1_ETH		GPIO_PE9
#define GPIO_PE_TIM2_CH2		GPIO_PE10
#define GPIO_PE_TIM2_CH3		GPIO_PE11
#define GPIO_PE_TIM2_CH4		GPIO_PE12

/* TIM3/4/5(**) */
#define GPIO_PA_TIM5_CH1		GPIO_PA0	  /* (**) */
#define GPIO_PA_TIM5_CH2		GPIO_PA1	  /* (**) */
#define GPIO_PA_TIM5_CH3		GPIO_PA2	  /* (**) */
#define GPIO_PA_TIM5_CH4		GPIO_PA3	  /* (**) */
#define GPIO_PA_TIM3_CH1		GPIO_PA6
#define GPIO_PA_TIM3_CH2		GPIO_PA7

#define GPIO_PB_TIM3_CH3		GPIO_PB0
#define GPIO_PB_TIM3_CH4		GPIO_PB1
#define GPIO_PB_TIM3_CH1		GPIO_PB4
#define GPIO_PB_TIM3_CH2		GPIO_PB5
#define GPIO_PB_TIM4_CH1		GPIO_PB6
#define GPIO_PB_TIM4_CH2		GPIO_PB7
#define GPIO_PB_TIM4_CH3		GPIO_PB8
#define GPIO_PB_TIM4_CH4		GPIO_PB9

#define GPIO_PC_TIM3_CH1		GPIO_PC6
#define GPIO_PC_TIM3_CH2		GPIO_PC7
#define GPIO_PC_TIM3_CH3		GPIO_PC8
#define GPIO_PC_TIM3_CH4		GPIO_PC9

#define GPIO_PD_TIM3_ETR		GPIO_PD2
#define GPIO_PD_TIM4_CH1		GPIO_PD12
#define GPIO_PD_TIM4_CH2		GPIO_PD13
#define GPIO_PD_TIM4_CH3		GPIO_PD14
#define GPIO_PD_TIM4_CH4		GPIO_PD15

#define GPIO_PE_TIM4_ETR		GPIO_PE0
#define GPIO_PE_TIM3_ETR		GPIO_PE2
#define GPIO_PE_TIM3_CH1		GPIO_PE3
#define GPIO_PE_TIM3_CH2		GPIO_PE4

#define GPIO_PF_TIM5_CH1_ETR		GPIO_PF6	/* (**) */
#define GPIO_PF_TIM5_CH2		GPIO_PF7	/* (**) */
#define GPIO_PF_TIM5_CH3		GPIO_PF8	/* (**) */
#define GPIO_PF_TIM5_CH4		GPIO_PF9	/* (**) */

/* TIM9/10/11 */
#define GPIO_PA_TIM9_CH1		GPIO_PA2
#define GPIO_PA_TIM9_CH2		GPIO_PA3
#define GPIO_PA_TIM10_CH1		GPIO_PA6
#define GPIO_PA_TIM11_CH1		GPIO_PA7

#define GPIO_PB_TIM10_CH1		GPIO_PB8
#define GPIO_PB_TIM11_CH1		GPIO_PB9
#define GPIO_PB_TIM10_CH1_12		GPIO_PB12
#define GPIO_PB_TIM9_CH1		GPIO_PB13
#define GPIO_PB_TIM9_CH2		GPIO_PB14
#define GPIO_PB_TIM11_CH1_15		GPIO_PB15

#define GPIO_PD_TIM9_CH1		GPIO_PD0
#define GPIO_PD_TIM9_CH2		GPIO_PD7

#define GPIO_PE_TIM10_CH1		GPIO_PE0
#define GPIO_PE_TIM11_CH1		GPIO_PE1
#define GPIO_PE_TIM9_CH1		GPIO_PE5
#define GPIO_PE_TIM9_CH2		GPIO_PE6

/* I2C1/2 */
#define GPIO_PB_I2C1_SMBA		GPIO_PB5
#define GPIO_PB_I2C1_SCL		GPIO_PB6
#define GPIO_PB_I2C1_SDA		GPIO_PB7
#define GPIO_PB_I2C1_SCL_8		GPIO_PB8
#define GPIO_PB_I2C1_SDA_9		GPIO_PB9
#define GPIO_PB_I2C2_SCL		GPIO_PB10
#define GPIO_PB_I2C2_SDA		GPIO_PB11
#define GPIO_PB_I2C2_SMBA		GPIO_PB12

/* SPI1/2 */
#define GPIO_PA_SPI1_NSS		GPIO_PA4
#define GPIO_PA_SPI1_SCK		GPIO_PA5
#define GPIO_PA_SPI1_MISO		GPIO_PA6
#define GPIO_PA_SPI1_MOSI		GPIO_PA7
#define GPIO_PA_SPI1_MISO_11		GPIO_PA11
#define GPIO_PA_SPI1_MOSI_12		GPIO_PA12
#define GPIO_PA_SPI1_NSS_15		GPIO_PA15

#define GPIO_PB_SPI1_SCK		GPIO_PB3
#define GPIO_PB_SPI1_MISO		GPIO_PB4
#define GPIO_PB_SPI1_MOSI		GPIO_PB5
#define GPIO_PB_SPI2_NSS		GPIO_PB12
#define GPIO_PB_SPI2_SCK		GPIO_PB13
#define GPIO_PB_SPI2_MISO		GPIO_PB14
#define GPIO_PB_SPI2_MOSI		GPIO_PB15

#define GPIO_PD_SPI2_NSS		GPIO_PD0
#define GPIO_PD_SPI2_SCK		GPIO_PD1
#define GPIO_PD_SPI2_MISO		GPIO_PD3
#define GPIO_PD_SPI2_MOSI		GPIO_PD4

#define GPIO_PE_SPI1_NSS		GPIO_PE12
#define GPIO_PE_SPI1_SCK		GPIO_PE13
#define GPIO_PE_SPI1_MISO		GPIO_PE14
#define GPIO_PE_SPI1_MOSI		GPIO_PE15

/* SPI3 (**) */
#define GPIO_PA_SPI3_NSS_I2S3_WS	GPIO_PA4
#define GPIO_PA_SPI3_NSS_I2S3_WS_15	GPIO_PA15

#define GPIO_PB_SPI3_SCK_I2S3_CK	GPIO_PB3
#define GPIO_PB_SPI3_MISO		GPIO_PB4
#define GPIO_PB_SPI3_MOSI_I2S3_SD	GPIO_PB5

#define GPIO_PC_I2S3_MCK		GPIO_PC7
#define GPIO_PC_SPI3_SCK_I2S3_CK	GPIO_PC10
#define GPIO_PC_SPI3_MISO		GPIO_PC11
#define GPIO_PC_SPI3_MOSI_I2S3_SD	GPIO_PC12

/* USART1/2/3 */
#define GPIO_PA_USART2_CTS		GPIO_PA0
#define GPIO_PA_USART2_RTS		GPIO_PA1
#define GPIO_PA_USART2_TX		GPIO_PA2
#define GPIO_PA_USART2_RX		GPIO_PA3
#define GPIO_PA_USART2_CK		GPIO_PA4
#define GPIO_PA_USART1_CK		GPIO_PA8
#define GPIO_PA_USART1_TX		GPIO_PA9
#define GPIO_PA_USART1_RX		GPIO_PA10
#define GPIO_PA_USART1_CTS		GPIO_PA11
#define GPIO_PA_USART1_RTS		GPIO_PA12

#define GPIO_PB_USART1_TX		GPIO_PB6
#define GPIO_PB_USART1_RX		GPIO_PB7
#define GPIO_PB_USART3_TX		GPIO_PB10
#define GPIO_PB_USART3_RX		GPIO_PB11
#define GPIO_PB_USART3_CK		GPIO_PB12
#define GPIO_PB_USART3_CTS		GPIO_PB13
#define GPIO_PB_USART3_RTS		GPIO_PB14

#define GPIO_PC_USART3_TX		GPIO_PC10
#define GPIO_PC_USART3_RX		GPIO_PC11
#define GPIO_PC_USART3_CK		GPIO_PC12

#define GPIO_PD_USART2_CTS		GPIO_PD3
#define GPIO_PD_USART2_RTS		GPIO_PD4
#define GPIO_PD_USART2_TX		GPIO_PD5
#define GPIO_PD_USART2_RX		GPIO_PD6
#define GPIO_PD_USART2_CK		GPIO_PD7
#define GPIO_PD_USART3_TX		GPIO_PD8
#define GPIO_PD_USART3_RX		GPIO_PD9
#define GPIO_PD_USART3_CK		GPIO_PD10
#define GPIO_PD_USART3_CTS		GPIO_PD11
#define GPIO_PD_USART3_RTS		GPIO_PD12

/* UART4/5 (***) */
#define GPIO_PC_UART4_TX		GPIO_PC10
#define GPIO_PC_UART4_RX		GPIO_PC11
#define GPIO_PC_UART5_TX		GPIO_PC12

#define GPIO_PD_UART5_RX		GPIO_PD2

/* USB */
#define GPIO_PA_USB_DM			GPIO_PA11
#define GPIO_PA_USB_DP			GPIO_PA12

/* LCD */
#define GPIO_PA_LCD_SEG0		GPIO_PA1
#define GPIO_PA_LCD_SEG1		GPIO_PA2
#define GPIO_PA_LCD_SEG2		GPIO_PA3
#define GPIO_PA_LCD_SEG3		GPIO_PA6
#define GPIO_PA_LCD_SEG4		GPIO_PA7
#define GPIO_PA_LCD_COM0		GPIO_PA8
#define GPIO_PA_LCD_COM1		GPIO_PA9
#define GPIO_PA_LCD_COM2		GPIO_PA10
#define GPIO_PA_LCD_SEG17		GPIO_PA15

#define GPIO_PB_LCD_SEG5		GPIO_PB0
#define GPIO_PB_LCD_SEG6		GPIO_PB1
#define GPIO_PB_LCD_SEG7		GPIO_PB3
#define GPIO_PB_LCD_SEG8		GPIO_PB4
#define GPIO_PB_LCD_SEG9		GPIO_PB5
#define GPIO_PB_LCD_SEG16		GPIO_PB8
#define GPIO_PB_LCD_COM3		GPIO_PB9
#define GPIO_PB_LCD_SEG10		GPIO_PB10
#define GPIO_PB_LCD_SEG11		GPIO_PB11
#define GPIO_PB_LCD_SEG12		GPIO_PB12
#define GPIO_PB_LCD_SEG13		GPIO_PB13
#define GPIO_PB_LCD_SEG14		GPIO_PB14
#define GPIO_PB_LCD_SEG15		GPIO_PB15

#define GPIO_PC_LCD_SEG18		GPIO_PC0
#define GPIO_PC_LCD_SEG19		GPIO_PC1
#define GPIO_PC_LCD_SEG20		GPIO_PC2
#define GPIO_PC_LCD_SEG21		GPIO_PC3
#define GPIO_PC_LCD_SEG22		GPIO_PC4
#define GPIO_PC_LCD_SEG23		GPIO_PC5
#define GPIO_PC_LCD_SEG24		GPIO_PC6
#define GPIO_PC_LCD_SEG25		GPIO_PC7
#define GPIO_PC_LCD_SEG26		GPIO_PC8
#define GPIO_PC_LCD_SEG27		GPIO_PC9
#define GPIO_PC_LCD_COM4_SEG28_SEG40	GPIO_PC10
#define GPIO_PC_LCD_COM5_SEG29_SEG41	GPIO_PC11
#define GPIO_PC_LCD_COM6_SEG30_SEG42	GPIO_PC12

#define GPIO_PD_LCD_COM7_SEG31_SEG43	GPIO_PD2
#define GPIO_PD_LCD_SEG28		GPIO_PD8
#define GPIO_PD_LCD_SEG29		GPIO_PD9
#define GPIO_PD_LCD_SEG30		GPIO_PD10
#define GPIO_PD_LCD_SEG31		GPIO_PD11
#define GPIO_PD_LCD_SEG32		GPIO_PD12
#define GPIO_PD_LCD_SEG33		GPIO_PD13
#define GPIO_PD_LCD_SEG34		GPIO_PD14
#define GPIO_PD_LCD_SEG35		GPIO_PD15

#define GPIO_PE_LCD_SEG36		GPIO_PE0
#define GPIO_PE_LCD_SEG37		GPIO_PE1
#define GPIO_PE_LCD_SEG38		GPIO_PE2
#define GPIO_PE_LCD_SEG39		GPIO_PE3

/* FSMC/SDIO (***) */
#define GPIO_PB_FSMC_NL			GPIO_PB7
#define GPIO_PB_SDIO_D4			GPIO_PB8
#define GPIO_PB_SDIO_D5			GPIO_PB9

#define GPIO_PC_SDIO_D6			GPIO_PC6
#define GPIO_PC_SDIO_D7			GPIO_PC7
#define GPIO_PC_SDIO_D0			GPIO_PC8
#define GPIO_PC_SDIO_D1			GPIO_PC9
#define GPIO_PC_SDIO_D2			GPIO_PC10
#define GPIO_PC_SDIO_D3			GPIO_PC11
#define GPIO_PC_SDIO_CK			GPIO_PC12

#define GPIO_PD_FSMC_D2			GPIO_PD0
#define GPIO_PD_FSMC_D3			GPIO_PD1
#define GPIO_PD_SDIO_CMD		GPIO_PD2
#define GPIO_PD_FSMC_CLK		GPIO_PD3
#define GPIO_PD_FSMC_NOE		GPIO_PD4
#define GPIO_PD_FSMC_NWE		GPIO_PD5
#define GPIO_PD_FSMC_NWAIT		GPIO_PD6
#define GPIO_PD_FSMC_NE1		GPIO_PD7
#define GPIO_PD_FSMC_D13		GPIO_PD8
#define GPIO_PD_FSMC_D14		GPIO_PD9
#define GPIO_PD_FSMC_D15		GPIO_PD10
#define GPIO_PD_FSMC_A16		GPIO_PD11
#define GPIO_PD_FSMC_A17		GPIO_PD12
#define GPIO_PD_FSMC_A18		GPIO_PD13
#define GPIO_PD_FSMC_D0			GPIO_PD14
#define GPIO_PD_FSMC_D1			GPIO_PD15

#define GPIO_PE_FSMC_NBL0		GPIO_PE0
#define GPIO_PE_FSMC_NBL1		GPIO_PE1
#define GPIO_PE_FSMC_A23		GPIO_PE2
#define GPIO_PE_FSMC_A19		GPIO_PE3
#define GPIO_PE_FSMC_A20		GPIO_PE4
#define GPIO_PE_FSMC_A21		GPIO_PE5
#define GPIO_PE_FSMC_D4			GPIO_PE7
#define GPIO_PE_FSMC_D5			GPIO_PE8
#define GPIO_PE_FSMC_D6			GPIO_PE9
#define GPIO_PE_FSMC_D7			GPIO_PE10
#define GPIO_PE_FSMC_D8			GPIO_PE11
#define GPIO_PE_FSMC_D9			GPIO_PE12
#define GPIO_PE_FSMC_D10		GPIO_PE13
#define GPIO_PE_FSMC_D11		GPIO_PE14
#define GPIO_PE_FSMC_D12		GPIO_PE15

#define GPIO_PF_FSMC_A0			GPIO_PF0
#define GPIO_PF_FSMC_A1			GPIO_PF1
#define GPIO_PF_FSMC_A2			GPIO_PF2
#define GPIO_PF_FSMC_A3			GPIO_PF3
#define GPIO_PF_FSMC_A4			GPIO_PF4
#define GPIO_PF_FSMC_A5			GPIO_PF5
#define GPIO_PF_FSMC_A6			GPIO_PF12
#define GPIO_PF_FSMC_A7			GPIO_PF13
#define GPIO_PF_FSMC_A8			GPIO_PF14
#define GPIO_PF_FSMC_A9			GPIO_PF15

#define GPIO_PG_FSMC_A10		GPIO_PG0
#define GPIO_PG_FSMC_A11		GPIO_PG1
#define GPIO_PG_FSMC_A12		GPIO_PG2
#define GPIO_PG_FSMC_A13		GPIO_PG3
#define GPIO_PG_FSMC_A14		GPIO_PG4
#define GPIO_PG_FSMC_A15		GPIO_PG5
#define GPIO_PG_FSMC_NE2		GPIO_PG9
#define GPIO_PG_FSMC_NE3		GPIO_PG10
#define GPIO_PG_FSMC_NE4		GPIO_PG12
#define GPIO_PG_FSMC_A24		GPIO_PG13
#define GPIO_PG_FSMC_A25		GPIO_PG14

#define GPIO_PH_FSMC_A22		GPIO_PH2

/* --- Function prototypes ------------------------------------------------- */

/* Output type */
typedef enum {
	GPIO_PUSHPULL = 0,
	GPIO_OPENDRAIN = 1
} gpio_otype_t;

/* Output speed */
typedef enum {
	GPIO_400KHZ = 0,
	GPIO_2MHZ = 1,
	GPIO_10MHZ = 2,
	GPIO_40MHZ = 3,
	GPIO_VERYLOW_SPEED = 0,
	GPIO_LOW_SPEED = 1,
	GPIO_MEDIUM_SPEED = 2,
	GPIO_HIGH_SPEED = 3
} gpio_ospeed_t;

/* Pull-up/pull-down */
typedef enum {
	GPIO_NOPUPD = 0,
	GPIO_FLOAT = 0,
	GPIO_PULLUP = 1,
	GPIO_PULLDOWN = 2
} gpio_pupd_t;

/* Alternate function */
typedef enum {
	GPIO_SYSTEM = 0,
	GPIO_TIM2 = 1,
	GPIO_TIM3_4 = 2,
	GPIO_TIM3_5 = 2,			/* (**) */
	GPIO_TIM9_11 = 3,
	GPIO_I2C1_2 = 4,
	GPIO_SPI1_2 = 5,
	GPIO_SPI3 = 6,				/* (**) */
	GPIO_USART1_3 = 7,
	GPIO_UART4_5 = 7,			/* (***) */
	GPIO_LCD = 11,
	GPIO_FSMC_SDIO = 12,			/* (***) */
	GPIO_EVENTOUT = 15
} gpio_altfn_t;

/* Mode */
typedef enum {
	GPIO_INPUT,
	GPIO_OUTPUT,
	GPIO_ALTFN,
	GPIO_ANALOG
} gpio_mode_t;

void gpio_config_input(gpio_pupd_t pupd, int portbits);
void gpio_config_output(gpio_otype_t otype, gpio_ospeed_t ospeed,
			gpio_pupd_t pupd, int portbits);
void gpio_config_altfn(gpio_altfn_t altfn, gpio_otype_t otype,
		       gpio_ospeed_t ospeed, gpio_pupd_t pupd, int portbits);
void gpio_config_analog(int portbits);
void gpio_set_mode(gpio_mode_t mode, int portbits);
void gpio_set(int portbits);
void gpio_clear(int portbits);
u16 gpio_get(int portbits);
void gpio_toggle(int portbits);
u16 gpio_port_read(int port);
void gpio_port_write(int port, u16 data);
int gpio_port_config_lock(int portbits);
