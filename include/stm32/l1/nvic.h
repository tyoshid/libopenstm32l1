/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
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
 * 4.3 Nested vectored interrupt controller (NVIC)
 *
 *
 * RM0038: STM32L100xx, STM32L151xx, STM32L152xx and STM32L162xx
 *         advanced ARM-based 32-bit MCUs
 * (19-Apr-2013 Rev 8)
 *
 * 9.1 Nested vectored interrupt controller (NVIC)
 */

/* Note: Numbers marked (**) only exist in high and medium+ density devices. */
/* Note: Numbers marked (***) only exist in high density devices. */
/* Note: Numbers marked (****) only exist in STM32L16x devices. */

/* --- NVIC Registers ------------------------------------------------------ */
/*
 * Offset	Register
 * 0x000	NVIC_ISER0	Interrupt set-enable register
 * 0x004	NVIC_ISER1
 * 0x008	NVIC_ISER2
 * 0x080	NVIC_ICER0	Interrupt clear-enable register
 * 0x084	NVIC_ICER1
 * 0x088	NVIC_ICER2
 * 0x100	NVIC_ISPR0	Interrupt set-pending register
 * 0x104	NVIC_ISPR1
 * 0x108	NVIC_ISPR2
 * 0x180	NVIC_ICPR0	Interrupt clear-pending register
 * 0x184	NVIC_ICPR1
 * 0x188	NVIC_ICPR2
 * 0x200	NVIC_IABR0	Interrupt active bit register
 * 0x204	NVIC_IABR1
 * 0x208	NVIC_IABR2
 * 0x300	NVIC_IPR0	Interrupt priority register
 *   |
 * 0x350	NVIC_IPR20
 * 0xE00	NVIC_STIR	Software trigger interrupt register
 */

/* NVIC_ISERx */
#define NVIC_ISER(x)			MMIO32(NVIC_BASE + 0x000 + ((x) * 4))
#define NVIC_ISER0			NVIC_ISER(0)
#define NVIC_ISER1			NVIC_ISER(1)
#define NVIC_ISER2			NVIC_ISER(2)

/* NVIC_ICERx */
#define NVIC_ICER(x)			MMIO32(NVIC_BASE + 0x080 + ((x) * 4))
#define NVIC_ICER0			NVIC_ICER(0)
#define NVIC_ICER1			NVIC_ICER(1)
#define NVIC_ICER2			NVIC_ICER(2)

/* NVIC_ISPRx */
#define NVIC_ISPR(x)			MMIO32(NVIC_BASE + 0x100 + ((x) * 4))
#define NVIC_ISPR0			NVIC_ISPR(0)
#define NVIC_ISPR1			NVIC_ISPR(1)
#define NVIC_ISPR2			NVIC_ISPR(2)

/* NVIC_ICPRx */
#define NVIC_ICPR(x)			MMIO32(NVIC_BASE + 0x180 + ((x) * 4))
#define NVIC_ICPR0			NVIC_ICPR(0)
#define NVIC_ICPR1			NVIC_ICPR(1)
#define NVIC_ICPR2			NVIC_ICPR(2)

/* NVIC_IABRx */
#define NVIC_IABR(x)			MMIO32(NVIC_BASE + 0x200 + ((x) * 4))
#define NVIC_IABR0			NVIC_IABR(0)
#define NVIC_IABR1			NVIC_IABR(1)
#define NVIC_IABR2			NVIC_IABR(2)

/* NVIC_IPRx */
#define NVIC_IPR(x)			MMIO32(NVIC_BASE + 0x300 + ((x) * 4))
#define NVIC_IPR_BYTE(n)		MMIO8(NVIC_BASE + 0x300 + (n))
#define NVIC_IPR0			NVIC_IPR(0)
#define NVIC_IPR1			NVIC_IPR(1)
#define NVIC_IPR2			NVIC_IPR(2)
#define NVIC_IPR3			NVIC_IPR(3)
#define NVIC_IPR4			NVIC_IPR(4)
#define NVIC_IPR5			NVIC_IPR(5)
#define NVIC_IPR6			NVIC_IPR(6)
#define NVIC_IPR7			NVIC_IPR(7)
#define NVIC_IPR8			NVIC_IPR(8)
#define NVIC_IPR9			NVIC_IPR(9)
#define NVIC_IPR10			NVIC_IPR(10)
#define NVIC_IPR11			NVIC_IPR(11)
#define NVIC_IPR12			NVIC_IPR(12)
#define NVIC_IPR13			NVIC_IPR(13)
#define NVIC_IPR14			NVIC_IPR(14)
#define NVIC_IPR15			NVIC_IPR(15)
#define NVIC_IPR16			NVIC_IPR(16)
#define NVIC_IPR17			NVIC_IPR(17)
#define NVIC_IPR18			NVIC_IPR(18)
#define NVIC_IPR19			NVIC_IPR(19)
#define NVIC_IPR20			NVIC_IPR(20)

/* NVIC_STIR */
#define NVIC_STIR			MMIO32(NVIC_BASE + 0xe00)

/* --- NVIC_ISERx values --------------------------------------------------- */

/* NVIC_ISERx[31:0]: SETENA[31:0]: Interrupt set-enable bits. */

/* --- NVIC_ICERx values --------------------------------------------------- */

/* NVIC_ICERx[31:0]: CLRENA[31:0]: Interrupt clear-enable bits. */

/* --- NVIC_ISPRx values --------------------------------------------------- */

/* NVIC_ISPRx[31:0]: SETPEND[31:0]: Interrupt set-pending bits. */

/* --- NVIC_ICPRx values --------------------------------------------------- */

/* NVIC_ICPRx[31:0]: CLRPEND[31:0]: Interrupt clear-pending bits. */

/* --- NVIC_IABRx values --------------------------------------------------- */

/* NVIC_IABRx[31:0]: ACTIVE[31:0]: Interrupt active flags. */

/* --- NVIC_IPRx values ---------------------------------------------------- */

/* NVIC_IPRx[31:24]: IP[4x + 3]: Priority, byte offset 3 */
/* NVIC_IPRx[23:16]: IP[4x + 2]: Priority, byte offset 2 */
/* NVIC_IPRx[15:8]:  IP[4x + 1]: Priority, byte offset 1 */
/* NVIC_IPRx[7:0]:   IP[4x]:     Priority, byte offset 0 */

/* --- NVIC_STIR values ---------------------------------------------------- */

/* NVIC_STIR[8:0]: INTID[8:0]: Software generated interrupt ID */

/* --- IRQ channel numbers-------------------------------------------------- */

/* Cortex M3 System Interrupts */

#define NVIC_NMI_IRQ			-14
#define NVIC_HARD_FAULT_IRQ		-13
#define NVIC_MEM_MANAGE_IRQ		-12
#define NVIC_BUS_FAULT_IRQ		-11
#define NVIC_USAGE_FAULT_IRQ		-10
#define NVIC_SV_CALL_IRQ		-5
#define DEBUG_MONITOR_IRQ		-4
#define NVIC_PENDSV_IRQ			-2
#define NVIC_SYSTICK_IRQ		-1

/* User Interrupts */
#define NVIC_WWDG_IRQ			0
#define NVIC_PVD_IRQ			1
#define NVIC_TAMPER_IRQ			2
#define NVIC_RTC_IRQ			3
#define NVIC_FLASH_IRQ			4
#define NVIC_RCC_IRQ			5
#define NVIC_EXTI0_IRQ			6
#define NVIC_EXTI1_IRQ			7
#define NVIC_EXTI2_IRQ			8
#define NVIC_EXTI3_IRQ			9
#define NVIC_EXTI4_IRQ			10
#define NVIC_DMA1_CHANNEL1_IRQ		11
#define NVIC_DMA1_CHANNEL2_IRQ		12
#define NVIC_DMA1_CHANNEL3_IRQ		13
#define NVIC_DMA1_CHANNEL4_IRQ		14
#define NVIC_DMA1_CHANNEL5_IRQ		15
#define NVIC_DMA1_CHANNEL6_IRQ		16
#define NVIC_DMA1_CHANNEL7_IRQ		17
#define NVIC_ADC_IRQ			18
#define NVIC_USB_HP_IRQ			19
#define NVIC_USB_LP_IRQ			20
#define NVIC_DAC_IRQ			21
#define NVIC_COMP_IRQ			22
#define NVIC_EXTI9_5_IRQ		23
#define NVIC_LCD_IRQ			24
#define NVIC_TIM9_IRQ			25
#define NVIC_TIM10_IRQ			26
#define NVIC_TIM11_IRQ			27
#define NVIC_TIM2_IRQ			28
#define NVIC_TIM3_IRQ			29
#define NVIC_TIM4_IRQ			30
#define NVIC_I2C1_EV_IRQ		31
#define NVIC_I2C1_ER_IRQ		32
#define NVIC_I2C2_EV_IRQ		33
#define NVIC_I2C2_ER_IRQ		34
#define NVIC_SPI1_IRQ			35
#define NVIC_SPI2_IRQ			36
#define NVIC_USART1_IRQ			37
#define NVIC_USART2_IRQ			38
#define NVIC_USART3_IRQ			39
#define NVIC_EXTI15_10_IRQ		40
#define NVIC_RTC_ALARM_IRQ		41
#define NVIC_USB_FS_WKUP_IRQ		42
#define NVIC_TIM6_IRQ			43
#define NVIC_TIM7_IRQ			44
#define NVIC_SDIO_IRQ			45	/* (***) */
#define NVIC_TIM5_IRQ			46	/* (**) */
#define NVIC_SPI3_IRQ			47	/* (**) */
#define NVIC_UART4_IRQ			48	/* (***) */
#define NVIC_UART5_IRQ			49	/* (***) */
#define NVIC_DMA2_CHANNEL1_IRQ		50	/* (**) */
#define NVIC_DMA2_CHANNEL2_IRQ		51	/* (**) */
#define NVIC_DMA2_CHANNEL3_IRQ		52	/* (**) */
#define NVIC_DMA2_CHANNEL4_IRQ		53	/* (**) */
#define NVIC_DMA2_CHANNEL5_IRQ		54	/* (**) */
#define NVIC_AES_IRQ			55	/* (****) */
#define NVIC_COMP_ACQ_IRQ		56	/* (**) */

void nmi_handler(void)		__attribute__((weak));
void hard_fault_handler(void)	__attribute__((weak));
void mem_manage_handler(void)	__attribute__((weak));
void bus_fault_handler(void)	__attribute__((weak));
void usage_fault_handler(void)	__attribute__((weak));
void sv_call_handler(void)	__attribute__((weak));
void debug_monitor_handler(void) __attribute__((weak));
void pend_sv_handler(void)	__attribute__((weak));
void sys_tick_handler(void)	__attribute__((weak));

void wwdg_isr(void)		__attribute__((weak));
void pvd_isr(void)		__attribute__((weak));
void tamper_isr(void)		__attribute__((weak));
void rtc_isr(void)		__attribute__((weak));
void flash_isr(void)		__attribute__((weak));
void rcc_isr(void)		__attribute__((weak));
void exti0_isr(void)		__attribute__((weak));
void exti1_isr(void)		__attribute__((weak));
void exti2_isr(void)		__attribute__((weak));
void exti3_isr(void)		__attribute__((weak));
void exti4_isr(void)		__attribute__((weak));
void dma1_channel1_isr(void)	__attribute__((weak));
void dma1_channel2_isr(void)	__attribute__((weak));
void dma1_channel3_isr(void)	__attribute__((weak));
void dma1_channel4_isr(void)	__attribute__((weak));
void dma1_channel5_isr(void)	__attribute__((weak));
void dma1_channel6_isr(void)	__attribute__((weak));
void dma1_channel7_isr(void)	__attribute__((weak));
void adc_isr(void)		__attribute__((weak));
void usb_hp_isr(void)		__attribute__((weak));
void usb_lp_isr(void)		__attribute__((weak));
void dac_isr(void)		__attribute__((weak));
void comp_isr(void)		__attribute__((weak));
void exti9_5_isr(void)		__attribute__((weak));
void lcd_isr(void)		__attribute__((weak));
void tim9_isr(void)		__attribute__((weak));
void tim10_isr(void)		__attribute__((weak));
void tim11_isr(void)		__attribute__((weak));
void tim2_isr(void)		__attribute__((weak));
void tim3_isr(void)		__attribute__((weak));
void tim4_isr(void)		__attribute__((weak));
void i2c1_ev_isr(void)		__attribute__((weak));
void i2c1_er_isr(void)		__attribute__((weak));
void i2c2_ev_isr(void)		__attribute__((weak));
void i2c2_er_isr(void)		__attribute__((weak));
void spi1_isr(void)		__attribute__((weak));
void spi2_isr(void)		__attribute__((weak));
void usart1_isr(void)		__attribute__((weak));
void usart2_isr(void)		__attribute__((weak));
void usart3_isr(void)		__attribute__((weak));
void exti15_10_isr(void)	__attribute__((weak));
void rtc_alarm_isr(void)	__attribute__((weak));
void usb_fs_wkup_isr(void)	__attribute__((weak));
void tim6_isr(void)		__attribute__((weak));
void tim7_isr(void)		__attribute__((weak));
void sdio_isr(void)		__attribute__((weak));
void tim5_isr(void)		__attribute__((weak));
void spi3_isr(void)		__attribute__((weak));
void uart4_isr(void)		__attribute__((weak));
void uart5_isr(void)		__attribute__((weak));
void dma2_ch1_isr(void)		__attribute__((weak));
void dma2_ch2_isr(void)		__attribute__((weak));
void dma2_ch3_isr(void)		__attribute__((weak));
void dma2_ch4_isr(void)		__attribute__((weak));
void dma2_ch5_isr(void)		__attribute__((weak));
void aes_isr(void)		__attribute__((weak));
void comp_acq_isr(void)		__attribute__((weak));

/* --- Function Prototypes ------------------------------------------------- */

bool nvic_irq_enabled(int irqn);
void nvic_enable_irq(int irqn);
void nvic_disable_irq(int irqn);
bool nvic_irq_pending(int irqn);
void nvic_set_pending_irq(int irqn);
void nvic_clear_pending_irq(int irqn);
bool nvic_irq_active(int irqn);
void nvic_set_priority(int irqn, int priority);
void nvic_generate_software_interrupt(int irqn);
