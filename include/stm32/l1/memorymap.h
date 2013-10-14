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

#ifndef LIBOPENCM3_MEMORYMAP_H
#define LIBOPENCM3_MEMORYMAP_H

/*
 * ARM Ltd.(www.arm.com)
 *
 * Cortex-M3 Technical Reference Manual
 * Revision r2p0
 *
 * 3.4 System address map
 */
/*
 * STMicroelectronics(www.st.com)
 *
 * PM0056: STM32F10xxx/20xxx/21xxx/L1xxxx Cortex-M3 programming manual
 * (27-May-2013 Rev 5)
 *
 * 4 Core peripherals
 *
 *
 * RM0038: STM32L100xx, STM32L151xx, STM32L152xx and STM32L162xx
 *         advanced ARM-based 32-bit MCUs
 * (19-Apr-2013 Rev 8)
 *
 * 2 Memory and bus architecture
 *
 *
 * DS6876: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         16KB SRAM, 4KB EEPROM, LCD, USB, ADC, DAC
 * (07-Feb-2013 Rev 8)
 *
 * 5 Memory mapping
 *
 *
 * DS8890: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 256KB Flash,
 *         32KB SRAM, 8KB EEPROM, LCD, USB, ADC, DAC
 * (01-Feb-2013 Rev 3)
 *
 * 5 Memory mapping
 *
 *
 * DS8576: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F
 * (01-Feb-2013 Rev 6)
 *
 * 5 Memory mapping
 *
 *
 * DS8669: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F, AES
 * (06-Feb-2013 Rev 4)
 *
 * 5 Memory mapping
 *
 *
 * DS9496: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         10KB SRAM, 2KB EEPROM, LCD, USB, ADC, DAC
 * (21-Feb-2013 Rev 1)
 *
 * 5 Memory mapping
 */

/*
 * Note: Addresses marked (**) only exist in high and medium+ density
 * devices.
 */
/* Note: Addresses marked (***) only exist in high density devices. */
/* Note: Addresses marked (****) only exist in STM32L16x devices. */

/* --- STM32L1xx memory map ------------------------------------------------ */

#define FLASH_MEMORY_BASE		0x08000000
#define SYSTEM_MEMORY_BASE		0x1ff00000
#define SRAM_BASE			0x20000000
#define PERIPH_BASE			0x40000000

#define FSMC_EXT_MEMORY_BASE		0x60000000	/* (***) */
#define FSMC_BASE			0xa0000000	/* (***) */

#define PPBI_BASE			0xe0000000
#define PPBE_BASE                       0xe0040000

/* --- Flash memory -------------------------------------------------------- */

#define DATA_EEPROM_BASE		(FLASH_MEMORY_BASE + 0x80000)

/* --- System memory ------------------------------------------------------- */

#define OPTION_BYTE_BASE		(SYSTEM_MEMORY_BASE + 0x80000)

/* Device Electronic Signature */
/* medium density device */
#define DESIG_FLASH_SIZE_BASE           (SYSTEM_MEMORY_BASE + 0x8004c)
#define DESIG_UNIQUE_ID_BASE            (SYSTEM_MEMORY_BASE + 0x80050)
/* medium+ and high density device */
#define DESIG_H_FLASH_SIZE_BASE		(SYSTEM_MEMORY_BASE + 0x800cc)
#define DESIG_H_UNIQUE_ID_BASE		(SYSTEM_MEMORY_BASE + 0x800d0)

/* --- Peripherals --------------------------------------------------------- */

/* Memory map for all busses */
#define PERIPH_BASE_APB1		(PERIPH_BASE + 0x00000)
#define PERIPH_BASE_APB2		(PERIPH_BASE + 0x10000)
#define PERIPH_BASE_AHB			(PERIPH_BASE + 0x20000)

/* Register boundary addresses */

/* APB1 */
#define TIM2_BASE			(PERIPH_BASE_APB1 + 0x0000)
#define TIM3_BASE			(PERIPH_BASE_APB1 + 0x0400)
#define TIM4_BASE			(PERIPH_BASE_APB1 + 0x0800)
#define TIM5_BASE			(PERIPH_BASE_APB1 + 0x0c00) /* (**) */
#define TIM6_BASE			(PERIPH_BASE_APB1 + 0x1000)
#define TIM7_BASE			(PERIPH_BASE_APB1 + 0x1400)

#define LCD_BASE                        (PERIPH_BASE_APB1 + 0x2400)
#define RTC_BASE			(PERIPH_BASE_APB1 + 0x2800)
#define WWDG_BASE			(PERIPH_BASE_APB1 + 0x2c00)
#define IWDG_BASE			(PERIPH_BASE_APB1 + 0x3000)

#define SPI2_BASE			(PERIPH_BASE_APB1 + 0x3800)
#define SPI3_BASE			(PERIPH_BASE_APB1 + 0x3c00) /* (**) */

#define USART2_BASE			(PERIPH_BASE_APB1 + 0x4400)
#define USART3_BASE			(PERIPH_BASE_APB1 + 0x4800)
#define UART4_BASE			(PERIPH_BASE_APB1 + 0x4c00) /* (***) */
#define UART5_BASE			(PERIPH_BASE_APB1 + 0x5000) /* (***) */
#define I2C1_BASE			(PERIPH_BASE_APB1 + 0x5400)
#define I2C2_BASE			(PERIPH_BASE_APB1 + 0x5800)
#define USB_DEV_FS_BASE			(PERIPH_BASE_APB1 + 0x5c00)
#define USB_DEV_FS_SRAM_BASE		(PERIPH_BASE_APB1 + 0x6000)

#define PWR_BASE			(PERIPH_BASE_APB1 + 0x7000)
#define DAC_BASE			(PERIPH_BASE_APB1 + 0x7400)
#define COMP_BASE			(PERIPH_BASE_APB1 + 0x7c00)
#define RI_BASE				(PERIPH_BASE_APB1 + 0x7c00)
#define OPAMP_BASE			(PERIPH_BASE_APB1 + 0x7c5c) /* (**) */

/* APB2 */
#define SYSCFG_BASE			(PERIPH_BASE_APB2 + 0x0000)
#define EXTI_BASE			(PERIPH_BASE_APB2 + 0x0400)
#define TIM9_BASE			(PERIPH_BASE_APB2 + 0x0800)
#define TIM10_BASE			(PERIPH_BASE_APB2 + 0x0c00)
#define TIM11_BASE			(PERIPH_BASE_APB2 + 0x1000)

#define ADC_BASE			(PERIPH_BASE_APB2 + 0x2400)

#define SDIO_BASE			(PERIPH_BASE_APB2 + 0x2c00) /* (***) */
#define SPI1_BASE			(PERIPH_BASE_APB2 + 0x3000)

#define USART1_BASE			(PERIPH_BASE_APB2 + 0x3800)

/* AHB */
#define GPIO_PORT_A_BASE		(PERIPH_BASE_AHB + 0x0000)
#define GPIO_PORT_B_BASE		(PERIPH_BASE_AHB + 0x0400)
#define GPIO_PORT_C_BASE		(PERIPH_BASE_AHB + 0x0800)
#define GPIO_PORT_D_BASE		(PERIPH_BASE_AHB + 0x0c00)
#define GPIO_PORT_E_BASE		(PERIPH_BASE_AHB + 0x1000)
#define GPIO_PORT_H_BASE		(PERIPH_BASE_AHB + 0x1400)
#define GPIO_PORT_F_BASE		(PERIPH_BASE_AHB + 0x1800) /* (**) */
#define GPIO_PORT_G_BASE		(PERIPH_BASE_AHB + 0x1c00) /* (**) */

#define CRC_BASE			(PERIPH_BASE_AHB + 0x3000)

#define RCC_BASE			(PERIPH_BASE_AHB + 0x3800)
#define FLASH_MEM_INTERFACE_BASE	(PERIPH_BASE_AHB + 0x3c00)

#define DMA1_BASE			(PERIPH_BASE_AHB + 0x6000)
#define DMA2_BASE			(PERIPH_BASE_AHB + 0x6400) /* (**) */

/* AES */
#define AES_BASE			(PERIPH_BASE +  0x10060000) /* (****) */

/* --- ARM Cortex-M3 specific definitions ---------------------------------- */

/* Private peripheral bus - Internal */
#define ITM_BASE			(PPBI_BASE + 0x0000)
#define DWT_BASE			(PPBI_BASE + 0x1000)
#define FPB_BASE			(PPBI_BASE + 0x2000)
#define SCS_BASE			(PPBI_BASE + 0xe000)

/* Private peripheral bus - External */
#define TPIU_BASE			(PPBE_BASE + 0x00000)
#define ETM_BASE			(PPBE_BASE + 0x01000)
/* DBG */
#define DBGMCU_BASE			(PPBE_BASE + 0x02000)
#define ROM_TABLE_BASE			(PPBE_BASE + 0xbf000)

/* SCS: System Control Space */

#define STK_BASE			(SCS_BASE + 0x010)
#define NVIC_BASE			(SCS_BASE + 0x100)
#define SCB_BASE			(SCS_BASE + 0xd00)
#define MPU_BASE                        (SCS_BASE + 0xd90)

#endif
