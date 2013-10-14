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
 * 12 Digital-to-analog converter (DAC)
 *
 *
 * DS6876: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         16KB SRAM, 4KB EEPROM, LCD, USB, ADC, DAC
 * (07-Feb-2013 Rev 8)
 *
 * Table 57. DAC characteristics
 *
 *
 * DS8890: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 256KB Flash,
 *         32KB SRAM, 8KB EEPROM, LCD, USB, ADC, DAC
 * (01-Feb-2013 Rev 3)
 *
 * Table 58. DAC characteristics
 *
 *
 * DS8576: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F
 * (01-Feb-2013 Rev 6)
 *
 * Table 67. DAC characteristics
 *
 *
 * DS8669: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F, AES
 * (06-Feb-2013 Rev 4)
 *
 * Table 66. DAC characteristics
 *
 *
 * DS9496: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         10KB SRAM, 2KB EEPROM, LCD, USB, ADC, DAC
 * (21-Feb-2013 Rev 1)
 *
 * Table 55. DAC characteristics
 */

/* --- DAC registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		DAC_CR		DAC control register
 * 0x04		DAC_SWTRIGR	DAC software trigger register
 * 0x08		DAC_DHR12R1	DAC channel1 12-bit right-aligned data holding
 *					register
 * 0x0c		DAC_DHR12L1	DAC channel1 12-bit left aligned data holding
 *					register
 * 0x10		DAC_DHR8R1	DAC channel1 8-bit right aligned data holding
 *					register
 * 0x14		DAC_DHR12R2	DAC channel2 12-bit right aligned data holding
 *					register
 * 0x18		DAC_DHR12L2	DAC channel2 12-bit left aligned data holding
 *					register
 * 0x1c		DAC_DHR8R2	DAC channel2 8-bit right-aligned data holding
 *					register
 * 0x20		DAC_DHR12RD	Dual DAC 12-bit right-aligned data holding
 *					register
 * 0x24		DAC_DHR12LD	DUAL DAC 12-bit left aligned data holding
 *					register
 * 0x28		DAC_DHR8RD	DUAL DAC 8-bit right aligned data holding
 *					register
 * 0x2c		DAC_DOR1	DAC channel1 data output register
 * 0x30		DAC_DOR2	DAC channel2 data output register
 * 0x34		DAC_SR		DAC status register
 */

#define DAC_CR				MMIO32(DAC_BASE + 0x00)
#define DAC_SWTRIGR			MMIO32(DAC_BASE + 0x04)
#define DAC_DHR12R1			MMIO32(DAC_BASE + 0x08)
#define DAC_DHR12L1			MMIO32(DAC_BASE + 0x0c)
#define DAC_DHR8R1			MMIO32(DAC_BASE + 0x10)
#define DAC_DHR12R2			MMIO32(DAC_BASE + 0x14)
#define DAC_DHR12L2			MMIO32(DAC_BASE + 0x18)
#define DAC_DHR8R2			MMIO32(DAC_BASE + 0x1c)
#define DAC_DHR12RD			MMIO32(DAC_BASE + 0x20)
#define DAC_DHR12LD			MMIO32(DAC_BASE + 0x24)
#define DAC_DHR8RD			MMIO32(DAC_BASE + 0x28)
#define DAC_DOR1			MMIO32(DAC_BASE + 0x2c)
#define DAC_DOR2			MMIO32(DAC_BASE + 0x30)
#define DAC_SR				MMIO32(DAC_BASE + 0x34)

/* --- DAC_CR values ------------------------------------------------------- */

#define DAC_CR_DMAUDRIE2		(1 << 29)
#define DAC_CR_DMAEN2			(1 << 28)
#define DAC_CR_MAMP23			(1 << 27)
#define DAC_CR_MAMP22			(1 << 26)
#define DAC_CR_MAMP21			(1 << 25)
#define DAC_CR_MAMP20			(1 << 24)
#define DAC_CR_WAVE21			(1 << 23)
#define DAC_CR_WAVE20			(1 << 22)
#define DAC_CR_TSEL22			(1 << 21)
#define DAC_CR_TSEL21			(1 << 20)
#define DAC_CR_TSEL20			(1 << 19)
#define DAC_CR_TEN2			(1 << 18)
#define DAC_CR_BOFF2			(1 << 17)
#define DAC_CR_EN2			(1 << 16)
#define DAC_CR_DMAUDRIE1		(1 << 13)
#define DAC_CR_DMAEN1			(1 << 12)
#define DAC_CR_MAMP13			(1 << 11)
#define DAC_CR_MAMP12			(1 << 10)
#define DAC_CR_MAMP11			(1 << 9)
#define DAC_CR_MAMP10			(1 << 8)
#define DAC_CR_WAVE11			(1 << 7)
#define DAC_CR_WAVE10			(1 << 6)
#define DAC_CR_TSEL12			(1 << 5)
#define DAC_CR_TSEL11			(1 << 4)
#define DAC_CR_TSEL10			(1 << 3)
#define DAC_CR_TEN1			(1 << 2)
#define DAC_CR_BOFF1			(1 << 1)
#define DAC_CR_EN1			(1 << 0)

/* DAC channel2 mask/amplitude selector */
#define DAC_CR_MAMP2_1			(0 << 24)
#define DAC_CR_MAMP2_2			(1 << 24)
#define DAC_CR_MAMP2_3			(2 << 24)
#define DAC_CR_MAMP2_4			(3 << 24)
#define DAC_CR_MAMP2_5			(4 << 24)
#define DAC_CR_MAMP2_6			(5 << 24)
#define DAC_CR_MAMP2_7			(6 << 24)
#define DAC_CR_MAMP2_8			(7 << 24)
#define DAC_CR_MAMP2_9			(8 << 24)
#define DAC_CR_MAMP2_10			(9 << 24)
#define DAC_CR_MAMP2_11			(10 << 24)
#define DAC_CR_MAMP2_12			(11 << 24)

/* DAC channel2 noise/triangle wave generation enable */
#define DAC_CR_WAVE2_DISABLE		(0 << 22)
#define DAC_CR_WAVE2_NOISE		(1 << 22)
#define DAC_CR_WAVE2_TRI		(2 << 22)

/* DAC channel2 trigger selection */
#define DAC_CR_TSEL2_TIM6		(0 << 19)
#define DAC_CR_TSEL2_TIM7		(2 << 19)
#define DAC_CR_TSEL2_TIM9		(3 << 19)
#define DAC_CR_TSEL2_TIM2		(4 << 19)
#define DAC_CR_TSEL2_TIM4		(5 << 19)
#define DAC_CR_TSEL2_EXTI9		(6 << 19)
#define DAC_CR_TSEL2_SW			(7 << 19)

/* DAC channel1 mask/amplitude selector */
#define DAC_CR_MAMP1_1			(0 << 8)
#define DAC_CR_MAMP1_2			(1 << 8)
#define DAC_CR_MAMP1_3			(2 << 8)
#define DAC_CR_MAMP1_4			(3 << 8)
#define DAC_CR_MAMP1_5			(4 << 8)
#define DAC_CR_MAMP1_6			(5 << 8)
#define DAC_CR_MAMP1_7			(6 << 8)
#define DAC_CR_MAMP1_8			(7 << 8)
#define DAC_CR_MAMP1_9			(8 << 8)
#define DAC_CR_MAMP1_10			(9 << 8)
#define DAC_CR_MAMP1_11			(10 << 8)
#define DAC_CR_MAMP1_12			(11 << 8)

/* DAC channel1 noise/triangle wave generation enable */
#define DAC_CR_WAVE1_DISABLE		(0 << 6)
#define DAC_CR_WAVE1_NOISE		(1 << 6)
#define DAC_CR_WAVE1_TRI		(2 << 6)

/* DAC channel1 trigger selection */
#define DAC_CR_TSEL1_TIM6		(0 << 3)
#define DAC_CR_TSEL1_TIM7		(2 << 3)
#define DAC_CR_TSEL1_TIM9		(3 << 3)
#define DAC_CR_TSEL1_TIM2		(4 << 3)
#define DAC_CR_TSEL1_TIM4		(5 << 3)
#define DAC_CR_TSEL1_EXTI9		(6 << 3)
#define DAC_CR_TSEL1_SW			(7 << 3)

/* --- DAC_SWTRIGR values -------------------------------------------------- */

#define DAC_SWTRIGR_SWTRIG2		(1 << 1)
#define DAC_SWTRIGR_SWTRIG1		(1 << 0)

/* --- DAC_DHR12R1 values -------------------------------------------------- */

/* DAC_DHR12R1[11:0]: DACC1DHR[11:0]: DAC channel1 12-bit right-aligned data */

/* --- DAC_DHR12L1 values -------------------------------------------------- */

/* DAC_DHR12L1[15:4]: DACC1DHR[11:0]: DAC channel1 12-bit left-aligned data */

/* --- DAC_DHR8R1 values --------------------------------------------------- */

/* DAC_DHR8R1[7:0]: DACC1DHR[7:0]: DAC channel1 8-bit right-aligned data */

/* --- DAC_DHR12R2 values -------------------------------------------------- */

/* DAC_DHR12R2[11:0]: DACC2DHR[11:0]: DAC channel2 12-bit right-aligned data */

/* --- DAC_DHR12L2 values -------------------------------------------------- */

/* DAC_DHR12L2[15:4]: DACC2DHR[11:0]: DAC channel2 12-bit left-aligned data */

/* --- DAC_DHR8R2 values --------------------------------------------------- */

/* DAC_DHR8R2[7:0]: DACC2DHR[7:0]: DAC channel2 8-bit right-aligned data */

/* --- DAC_DHR12RD values -------------------------------------------------- */

/* DAC_DHR12RD[27:16]: DACC2DHR[11:0]: DAC channel2 12-bit right-aligned data */
/* DAC_DHR12RD[11:0]: DACC1DHR[11:0]: DAC channel1 12-bit right-aligned data */

/* --- DAC_DHR12LD values -------------------------------------------------- */

/* DAC_DHR12LD[31:20]: DACC2DHR[11:0]: DAC channel2 12-bit left-aligned data */
/* DAC_DHR12LD[15:4]: DACC1DHR[11:0]: DAC channel1 12-bit left-aligned data */

/* --- DAC_DHR8RD values --------------------------------------------------- */

/* DAC_DHR8RD[15:8]: DACC2DHR[7:0]: DAC channel2 8-bit right-aligned data */
/* DAC_DHR8RD[7:0]: DACC1DHR[7:0]: DAC channel1 8-bit right-aligned data */

/* --- DAC_DOR1 values ----------------------------------------------------- */

/* DACC1DOR[11:0]: DAC channel1 data output */

/* --- DAC_DOR2 values ----------------------------------------------------- */

/* DACC2DOR[11:0]: DAC channel2 data output */

/* --- DAC_SR values ------------------------------------------------------- */

#define DAC_SR_DMAUDR2			(1 << 29)
#define DAC_SR_DMAUDR1			(1 << 13)

/* --- Wakeup time --------------------------------------------------------- */

#define DAC_T_WAKEUP			15	/* 15 usec */

/* --- Function prototypes ------------------------------------------------- */

/* Data format */
typedef enum {
	DAC_8R,
	DAC_12L,
	DAC_12R
} dac_format_t;

/* Channel */
typedef enum {
	DAC_CH1,
	DAC_CH2,
	DAC_DUAL
} dac_channel_t;

/* Interrupt */
enum {
	DAC_CH1_DMA_UNDERRUN = (1 << 13),
	DAC_CH2_DMA_UNDERRUN = (1 << 29)
};

/* Trigger */
enum {
	DAC_TRIGGER_NONE = 0,

	DAC_CH1_TIM6_TRGO = (1 << 2),
	DAC_CH1_TIM7_TRGO = (5 << 2),
	DAC_CH1_TIM9_TRGO = (7 << 2),
	DAC_CH1_TIM2_TRGO = (9 << 2),
	DAC_CH1_TIM4_TRGO = (11 << 2),
	DAC_CH1_EXTI9 = (13 << 2),
	DAC_CH1_SOFTWARE = (15 << 2),

	DAC_CH2_TIM6_TRGO = (1 << 18),
	DAC_CH2_TIM7_TRGO = (5 << 18),
	DAC_CH2_TIM9_TRGO = (7 << 18),
	DAC_CH2_TIM2_TRGO = (9 << 18),
	DAC_CH2_TIM4_TRGO = (11 << 18),
	DAC_CH2_EXTI9 = (13 << 18),
	DAC_CH2_SOFTWARE = (15 << 18)
};

/* Noise generation */
/* Unmask bits[(b - 1):0] of LFSR */
#define DAC_CH1_NOISE(b)		(((b) - 1) << 8 | 1 << 6)
#define DAC_CH2_NOISE(b)		(((b) - 1) << 24 | 1 << 22)

/* Triangle wave generation */
/* Triangle amplitude equal to (2**b - 1) */
#define DAC_CH1_TRIANGLE(b)		(((b) - 1) << 8 | 2 << 6)
#define DAC_CH2_TRIANGLE(b)		(((b) - 1) << 24 | 2 << 22)

/* Others */
enum {
	DAC_CH1_DMA = (1 << 12),
	DAC_CH2_DMA = (1 << 28),

	DAC_CH1_BUFFER_OFF = (1 << 1),
	DAC_CH2_BUFFER_OFF = (1 << 17),

	DAC_CH1_ENABLE = (1 << 0),
	DAC_CH2_ENABLE = (1 << 16)
};

void dac_set_mode(int mode);
void dac_enable(dac_channel_t channel);
void dac_disable(dac_channel_t channel);
void dac_set_data(dac_format_t format, dac_channel_t channel, u32 data);
void dac_set_software_trigger(dac_channel_t channel);
void dac_enable_interrupt(int interrupt);
void dac_disable_interrupt(int interrupt);
int dac_get_interrupt_mask(int interrupt);
int dac_get_interrupt_status(int interrupt);
void dac_clear_interrupt(int interrupt);
