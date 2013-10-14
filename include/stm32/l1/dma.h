 /*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Thomas Otto <tommi@viadmin.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
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
 * 10 Direct memory access controller (DMA)
 */

/*
 * Note: Registers and numbers marked (**) only exist in high and medium+
 * density devices.
 */
/* Note: Numbers marked (***) only exist in high density devices. */
/* Note: Numbers marked (****) only exist in STM32L16x devices. */

/* --- DMA registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		DMA_ISR		DMA interrupt status register
 * 0x04		DMA_IFCR	DMA interrupt flag clear register
 * 0x08		DMA_CCR1	DMA channel 1 configuration register
 * 0x0c		DMA_CNDTR1	DMA channel 1 number of data register
 * 0x10		DMA_CPAR1	DMA channel 1 peripheral address register
 * 0x14		DMA_CMAR1	DMA channel 1 memory address register
 * 0x1c		DMA_CCR2	DMA channel 2 configuration register
 * 0x20		DMA_CNDTR2	DMA channel 2 number of data register
 * 0x24		DMA_CPAR2	DMA channel 2 peripheral address register
 * 0x28		DMA_CMAR2	DMA channel 2 memory address register
 * 0x30		DMA_CCR3	DMA channel 3 configuration register
 * 0x34		DMA_CNDTR3	DMA channel 3 number of data register
 * 0x38		DMA_CPAR3	DMA channel 3 peripheral address register
 * 0x3c		DMA_CMAR3	DMA channel 3 memory address register
 * 0x44		DMA_CCR4	DMA channel 4 configuration register
 * 0x48		DMA_CNDTR4	DMA channel 4 number of data register
 * 0x4c		DMA_CPAR4	DMA channel 4 peripheral address register
 * 0x50		DMA_CMAR4	DMA channel 4 memory address register
 * 0x58		DMA_CCR5	DMA channel 5 configuration register
 * 0x5c		DMA_CNDTR5	DMA channel 5 number of data register
 * 0x60		DMA_CPAR5	DMA channel 5 peripheral address register
 * 0x64		DMA_CMAR5	DMA channel 5 memory address register
 * 0x6c		DMA_CCR6	DMA channel 6 configuration register
 * 0x70		DMA_CNDTR6	DMA channel 6 number of data register
 * 0x74		DMA_CPAR6	DMA channel 6 peripheral address register
 * 0x78		DMA_CMAR6	DMA channel 6 memory address register
 * 0x80		DMA_CCR7	DMA channel 7 configuration register
 * 0x84		DMA_CNDTR7	DMA channel 7 number of data register
 * 0x88		DMA_CPAR7	DMA channel 7 peripheral address register
 * 0x8c		DMA_CMAR7	DMA channel 7 memory address register
 */

/* DMAx_ISR */
#define DMA_ISR(base)			MMIO32((base) + 0x00)
#define DMA1_ISR			DMA_ISR(DMA1_BASE)
#define DMA2_ISR			DMA_ISR(DMA2_BASE)

/* DMAx_IFCR */
#define DMA_IFCR(base)			MMIO32((base) + 0x04)
#define DMA1_IFCR			DMA_IFCR(DMA1_BASE)
#define DMA2_IFCR			DMA_IFCR(DMA2_BASE)

/* DMAx_CCRy */
#define DMA_CCR(base, ch)		MMIO32((base) + 0x08 + (0x14 * (ch)))

#define DMA1_CCR(ch)			DMA_CCR(DMA1_BASE, ch)
#define DMA1_CCR1			DMA_CCR(DMA1_BASE, 0)
#define DMA1_CCR2			DMA_CCR(DMA1_BASE, 1)
#define DMA1_CCR3			DMA_CCR(DMA1_BASE, 2)
#define DMA1_CCR4			DMA_CCR(DMA1_BASE, 3)
#define DMA1_CCR5			DMA_CCR(DMA1_BASE, 4)
#define DMA1_CCR6			DMA_CCR(DMA1_BASE, 5)
#define DMA1_CCR7			DMA_CCR(DMA1_BASE, 6)

#define DMA2_CCR(ch)			DMA_CCR(DMA2_BASE, ch)	/* (**) */
#define DMA2_CCR1			DMA_CCR(DMA2_BASE, 0)	/* (**) */
#define DMA2_CCR2			DMA_CCR(DMA2_BASE, 1)	/* (**) */
#define DMA2_CCR3			DMA_CCR(DMA2_BASE, 2)	/* (**) */
#define DMA2_CCR4			DMA_CCR(DMA2_BASE, 3)	/* (**) */
#define DMA2_CCR5			DMA_CCR(DMA2_BASE, 4)	/* (**) */

/* DMAx_CNDTRy */
#define DMA_CNDTR(base, ch)		MMIO32((base) + 0x0c + (0x14 * (ch)))

#define DMA1_CNDTR(ch)			DMA_CNDTR(DMA1_BASE, ch)
#define DMA1_CNDTR1			DMA_CNDTR(DMA1_BASE, 0)
#define DMA1_CNDTR2			DMA_CNDTR(DMA1_BASE, 1)
#define DMA1_CNDTR3			DMA_CNDTR(DMA1_BASE, 2)
#define DMA1_CNDTR4			DMA_CNDTR(DMA1_BASE, 3)
#define DMA1_CNDTR5			DMA_CNDTR(DMA1_BASE, 4)
#define DMA1_CNDTR6			DMA_CNDTR(DMA1_BASE, 5)
#define DMA1_CNDTR7			DMA_CNDTR(DMA1_BASE, 6)

#define DMA2_CNDTR(ch)			DMA_CNDTR(DMA2_BASE, ch) /* (**) */
#define DMA2_CNDTR1			DMA_CNDTR(DMA2_BASE, 0)	/* (**) */
#define DMA2_CNDTR2			DMA_CNDTR(DMA2_BASE, 1)	/* (**) */
#define DMA2_CNDTR3			DMA_CNDTR(DMA2_BASE, 2)	/* (**) */
#define DMA2_CNDTR4			DMA_CNDTR(DMA2_BASE, 3)	/* (**) */
#define DMA2_CNDTR5			DMA_CNDTR(DMA2_BASE, 4)	/* (**) */

/* DMAx_CPARy */
#define DMA_CPAR(base, ch)		MMIO32((base) + 0x10 + (0x14 * (ch)))

#define DMA1_CPAR(ch)			DMA_CPAR(DMA1_BASE, ch)
#define DMA1_CPAR1			DMA_CPAR(DMA1_BASE, 0)
#define DMA1_CPAR2			DMA_CPAR(DMA1_BASE, 1)
#define DMA1_CPAR3			DMA_CPAR(DMA1_BASE, 2)
#define DMA1_CPAR4			DMA_CPAR(DMA1_BASE, 3)
#define DMA1_CPAR5			DMA_CPAR(DMA1_BASE, 4)
#define DMA1_CPAR6			DMA_CPAR(DMA1_BASE, 5)
#define DMA1_CPAR7			DMA_CPAR(DMA1_BASE, 6)

#define DMA2_CPAR(ch)			DMA_CPAR(DMA2_BASE, ch)	/* (**) */
#define DMA2_CPAR1			DMA_CPAR(DMA2_BASE, 0)	/* (**) */
#define DMA2_CPAR2			DMA_CPAR(DMA2_BASE, 1)	/* (**) */
#define DMA2_CPAR3			DMA_CPAR(DMA2_BASE, 2)	/* (**) */
#define DMA2_CPAR4			DMA_CPAR(DMA2_BASE, 3)	/* (**) */
#define DMA2_CPAR5			DMA_CPAR(DMA2_BASE, 4)	/* (**) */

/* DMAx_CMARy */

#define DMA_CMAR(base, ch)		MMIO32((base) + 0x14 + (0x14 * (ch)))

#define DMA1_CMAR(ch)			DMA_CMAR(DMA1_BASE, ch)
#define DMA1_CMAR1			DMA_CMAR(DMA1_BASE, 0)
#define DMA1_CMAR2			DMA_CMAR(DMA1_BASE, 1)
#define DMA1_CMAR3			DMA_CMAR(DMA1_BASE, 2)
#define DMA1_CMAR4			DMA_CMAR(DMA1_BASE, 3)
#define DMA1_CMAR5			DMA_CMAR(DMA1_BASE, 4)
#define DMA1_CMAR6			DMA_CMAR(DMA1_BASE, 5)
#define DMA1_CMAR7			DMA_CMAR(DMA1_BASE, 6)

#define DMA2_CMAR(ch)			DMA_CMAR(DMA2_BASE, ch)	/* (**) */
#define DMA2_CMAR1			DMA_CMAR(DMA2_BASE, 0)	/* (**) */
#define DMA2_CMAR2			DMA_CMAR(DMA2_BASE, 1)	/* (**) */
#define DMA2_CMAR3			DMA_CMAR(DMA2_BASE, 2)	/* (**) */
#define DMA2_CMAR4			DMA_CMAR(DMA2_BASE, 3)	/* (**) */
#define DMA2_CMAR5			DMA_CMAR(DMA2_BASE, 4)	/* (**) */

/* --- DMA_ISR values ------------------------------------------------------ */

#define DMA_ISR_TEIF7			(1 << 27)
#define DMA_ISR_HTIF7			(1 << 26)
#define DMA_ISR_TCIF7			(1 << 25)
#define DMA_ISR_GIF7			(1 << 24)
#define DMA_ISR_TEIF6			(1 << 23)
#define DMA_ISR_HTIF6			(1 << 22)
#define DMA_ISR_TCIF6			(1 << 21)
#define DMA_ISR_GIF6			(1 << 20)
#define DMA_ISR_TEIF5			(1 << 19)
#define DMA_ISR_HTIF5			(1 << 18)
#define DMA_ISR_TCIF5			(1 << 17)
#define DMA_ISR_GIF5			(1 << 16)
#define DMA_ISR_TEIF4			(1 << 15)
#define DMA_ISR_HTIF4			(1 << 14)
#define DMA_ISR_TCIF4			(1 << 13)
#define DMA_ISR_GIF4			(1 << 12)
#define DMA_ISR_TEIF3			(1 << 11)
#define DMA_ISR_HTIF3			(1 << 10)
#define DMA_ISR_TCIF3			(1 << 9)
#define DMA_ISR_GIF3			(1 << 8)
#define DMA_ISR_TEIF2			(1 << 7)
#define DMA_ISR_HTIF2			(1 << 6)
#define DMA_ISR_TCIF2			(1 << 5)
#define DMA_ISR_GIF2			(1 << 4)
#define DMA_ISR_TEIF1			(1 << 3)
#define DMA_ISR_HTIF1			(1 << 2)
#define DMA_ISR_TCIF1			(1 << 1)
#define DMA_ISR_GIF1			(1 << 0)

/* TEIF: Transfer error interrupt flag */
#define DMA_ISR_TEIF_BIT		(1 << 3)
#define DMA_ISR_TEIF(ch)		(DMA_ISR_TEIF_BIT << (4 * (ch)))

/* HTIF: Half transfer interrupt flag */
#define DMA_ISR_HTIF_BIT		(1 << 2)
#define DMA_ISR_HTIF(ch)		(DMA_ISR_HTIF_BIT << (4 * (ch)))

/* TCIF: Transfer complete interrupt flag */
#define DMA_ISR_TCIF_BIT		(1 << 1)
#define DMA_ISR_TCIF(ch)		(DMA_ISR_TCIF_BIT << (4 * (ch)))

/* GIF: Global interrupt flag */
#define DMA_ISR_GIF_BIT			(1 << 0)
#define DMA_ISR_GIF(ch)			(DMA_ISR_GIF_BIT << (4 * (ch)))

/* --- DMA_IFCR values ----------------------------------------------------- */

#define DMA_IFCR_CTEIF7			(1 << 27)
#define DMA_IFCR_CHTIF7			(1 << 26)
#define DMA_IFCR_CTCIF7			(1 << 25)
#define DMA_IFCR_CGIF7			(1 << 24)
#define DMA_IFCR_CTEIF6			(1 << 23)
#define DMA_IFCR_CHTIF6			(1 << 22)
#define DMA_IFCR_CTCIF6			(1 << 21)
#define DMA_IFCR_CGIF6			(1 << 20)
#define DMA_IFCR_CTEIF5			(1 << 19)
#define DMA_IFCR_CHTIF5			(1 << 18)
#define DMA_IFCR_CTCIF5			(1 << 17)
#define DMA_IFCR_CGIF5			(1 << 16)
#define DMA_IFCR_CTEIF4			(1 << 15)
#define DMA_IFCR_CHTIF4			(1 << 14)
#define DMA_IFCR_CTCIF4			(1 << 13)
#define DMA_IFCR_CGIF4			(1 << 12)
#define DMA_IFCR_CTEIF3			(1 << 11)
#define DMA_IFCR_CHTIF3			(1 << 10)
#define DMA_IFCR_CTCIF3			(1 << 9)
#define DMA_IFCR_CGIF3			(1 << 8)
#define DMA_IFCR_CTEIF2			(1 << 7)
#define DMA_IFCR_CHTIF2			(1 << 6)
#define DMA_IFCR_CTCIF2			(1 << 5)
#define DMA_IFCR_CGIF2			(1 << 4)
#define DMA_IFCR_CTEIF1			(1 << 3)
#define DMA_IFCR_CHTIF1			(1 << 2)
#define DMA_IFCR_CTCIF1			(1 << 1)
#define DMA_IFCR_CGIF1			(1 << 0)

/* CTEIF: Transfer error clear */
#define DMA_IFCR_CTEIF_BIT		(1 << 3)
#define DMA_IFCR_CTEIF(ch)		(DMA_IFCR_CTEIF_BIT << (4 * (ch)))

/* CHTIF: Half transfer clear */
#define DMA_IFCR_CHTIF_BIT		(1 << 2)
#define DMA_IFCR_CHTIF(ch)		(DMA_IFCR_CHTIF_BIT << (4 * (ch)))

/* CTCIF: Transfer complete clear */
#define DMA_IFCR_CTCIF_BIT		(1 << 1)
#define DMA_IFCR_CTCIF(ch)		(DMA_IFCR_CTCIF_BIT << (4 * (ch)))

/* CGIF: Global interrupt clear */
#define DMA_IFCR_CGIF_BIT		(1 << 0)
#define DMA_IFCR_CGIF(ch)		(DMA_IFCR_CGIF_BIT << (4 * (ch)))

/* --- DMA_CCRx values ----------------------------------------------------- */

#define DMA_CCR_MEM2MEM			(1 << 14)
#define DMA_CCR_PL1			(1 << 13)
#define DMA_CCR_PL0			(1 << 12)
#define DMA_CCR_MSIZE1			(1 << 11)
#define DMA_CCR_MSIZE0			(1 << 10)
#define DMA_CCR_PSIZE1			(1 << 9)
#define DMA_CCR_PSIZE0			(1 << 8)
#define DMA_CCR_MINC			(1 << 7)
#define DMA_CCR_PINC			(1 << 6)
#define DMA_CCR_CIRC			(1 << 5)
#define DMA_CCR_DIR			(1 << 4)
#define DMA_CCR_TEIE			(1 << 3)
#define DMA_CCR_HTIE			(1 << 2)
#define DMA_CCR_TCIE			(1 << 1)
#define DMA_CCR_EN			(1 << 0)

/* Channel priority level */
#define DMA_CCR_PL_LOW			(0 << 12)
#define DMA_CCR_PL_MEDIUM		(1 << 12)
#define DMA_CCR_PL_HIGH			(2 << 12)
#define DMA_CCR_PL_VERY_HIGH		(3 << 12)

/* Memory size */
#define DMA_CCR_MSIZE_8BIT		(0 << 10)
#define DMA_CCR_MSIZE_16BIT		(1 << 10)
#define DMA_CCR_MSIZE_32BIT		(2 << 10)

/* Peripheral size */
#define DMA_CCR_PSIZE_8BIT		(0 << 8)
#define DMA_CCR_PSIZE_16BIT		(1 << 8)
#define DMA_CCR_PSIZE_32BIT		(2 << 8)

/* --- DMA_CNDTRx values --------------------------------------------------- */

/* DMA_CNDTR[15:0]: NDT[15:0]: Number of data to transfer */

/* --- DMA_CPARx values ---------------------------------------------------- */

/* PA[31:0]: Peripheral address */

/* --- DMA_CMARx values ---------------------------------------------------- */

/* MA[31:0]: Memory address */

/* --- DMA Channel definition ---------------------------------------------- */

enum dma_channel {
	DMA1_CHANNEL1,
	DMA1_CHANNEL2,
	DMA1_CHANNEL3,
	DMA1_CHANNEL4,
	DMA1_CHANNEL5,
	DMA1_CHANNEL6,
	DMA1_CHANNEL7,
	DMA2_CHANNEL1,	/* (**) */
	DMA2_CHANNEL2,	/* (**) */
	DMA2_CHANNEL3,	/* (**) */
	DMA2_CHANNEL4,	/* (**) */
	DMA2_CHANNEL5	/* (**) */
};

/* --- DMA request mapping ------------------------------------------------- */

#define DMA_ADC				DMA1_CHANNEL1
#define DMA_TIM2_CH3			DMA1_CHANNEL1
#define DMA_TIM4_CH1			DMA1_CHANNEL1
#define DMA_USART3_TX			DMA1_CHANNEL2
#define DMA_TIM2_UP			DMA1_CHANNEL2
#define DMA_TIM3_CH3			DMA1_CHANNEL2
#define DMA_SPI1_RX			DMA1_CHANNEL2
#define DMA_TIM6_UP			DMA1_CHANNEL2
#define DMA_DAC_CHANNEL1		DMA1_CHANNEL2
#define DMA_USART3_RX			DMA1_CHANNEL3
#define DMA_TIM3_CH4			DMA1_CHANNEL3
#define DMA_TIM3_UP			DMA1_CHANNEL3
#define DMA_SPI1_TX			DMA1_CHANNEL3
#define DMA_TIM7_UP			DMA1_CHANNEL3
#define DMA_DAC_CHANNEL2		DMA1_CHANNEL3
#define DMA_USART1_TX			DMA1_CHANNEL4
#define DMA_TIM4_CH2			DMA1_CHANNEL4
#define DMA_SPI2_RX			DMA1_CHANNEL4
#define DMA_I2C2_TX			DMA1_CHANNEL4
#define DMA_USART1_RX			DMA1_CHANNEL5
#define DMA_SPI2_TX			DMA1_CHANNEL5
#define DMA_TIM2_CH1			DMA1_CHANNEL5
#define DMA_TIM4_CH3			DMA1_CHANNEL5
#define DMA_I2C2_RX			DMA1_CHANNEL5
#define DMA_USART2_RX			DMA1_CHANNEL6
#define DMA_TIM3_CH1			DMA1_CHANNEL6
#define DMA_TIM3_TRIG			DMA1_CHANNEL6
#define DMA_I2C1_TX			DMA1_CHANNEL6
#define DMA_USART2_TX			DMA1_CHANNEL7
#define DMA_TIM2_CH2			DMA1_CHANNEL7
#define DMA_TIM2_CH4			DMA1_CHANNEL7
#define DMA_TIM4_UP			DMA1_CHANNEL7
#define DMA_I2C1_RX			DMA1_CHANNEL7

#define DMA_SPI3_RX			DMA2_CHANNEL1	/* (**) */
#define DMA_USART5_TX			DMA2_CHANNEL1	/* (***) */
#define DMA_TIM5_CH4			DMA2_CHANNEL1	/* (**) */
#define DMA_TIM5_TRIG			DMA2_CHANNEL1	/* (**) */
#define DMA_TIM5_COM			DMA2_CHANNEL1	/* (**) */
#define DMA_SPI3_TX			DMA2_CHANNEL2	/* (**) */
#define DMA_USART5_RX			DMA2_CHANNEL2	/* (***) */
#define DMA_TIM5_CH3			DMA2_CHANNEL2	/* (**) */
#define DMA_TIM5_UP			DMA2_CHANNEL2	/* (**) */
#define DMA_USART4_RX			DMA2_CHANNEL3	/* (***) */
#define DMA_AES_OUT			DMA2_CHANNEL3	/* (****) */
#define DMA_TIM5_CH2			DMA2_CHANNEL4	/* (**) */
#define DMA_SD_MMC			DMA2_CHANNEL4	/* (***) */
#define DMA_USART4_TX			DMA2_CHANNEL5	/* (***) */
#define DMA_TIM5_CH1			DMA2_CHANNEL5	/* (**) */
#define DMA_AES_IN			DMA2_CHANNEL5	/* (****) */

/* --- Alias --------------------------------------------------------------- */

#define DMA_RCC_ADC			RCC_DMA1
#define DMA_RCC_TIM2_CH3		RCC_DMA1
#define DMA_RCC_TIM4_CH1		RCC_DMA1
#define DMA_RCC_USART3_TX		RCC_DMA1
#define DMA_RCC_TIM2_UP			RCC_DMA1
#define DMA_RCC_TIM3_CH3		RCC_DMA1
#define DMA_RCC_SPI1_RX			RCC_DMA1
#define DMA_RCC_TIM6_UP			RCC_DMA1
#define DMA_RCC_DAC_CHANNEL1		RCC_DMA1
#define DMA_RCC_USART3_RX		RCC_DMA1
#define DMA_RCC_TIM3_CH4		RCC_DMA1
#define DMA_RCC_TIM3_UP			RCC_DMA1
#define DMA_RCC_SPI1_TX			RCC_DMA1
#define DMA_RCC_TIM7_UP			RCC_DMA1
#define DMA_RCC_DAC_CHANNEL2		RCC_DMA1
#define DMA_RCC_USART1_TX		RCC_DMA1
#define DMA_RCC_TIM4_CH2		RCC_DMA1
#define DMA_RCC_SPI2_RX			RCC_DMA1
#define DMA_RCC_I2C2_TX			RCC_DMA1
#define DMA_RCC_USART1_RX		RCC_DMA1
#define DMA_RCC_SPI2_TX			RCC_DMA1
#define DMA_RCC_TIM2_CH1		RCC_DMA1
#define DMA_RCC_TIM4_CH3		RCC_DMA1
#define DMA_RCC_I2C2_RX			RCC_DMA1
#define DMA_RCC_USART2_RX		RCC_DMA1
#define DMA_RCC_TIM3_CH1		RCC_DMA1
#define DMA_RCC_TIM3_TRIG		RCC_DMA1
#define DMA_RCC_I2C1_TX			RCC_DMA1
#define DMA_RCC_USART2_TX		RCC_DMA1
#define DMA_RCC_TIM2_CH2		RCC_DMA1
#define DMA_RCC_TIM2_CH4		RCC_DMA1
#define DMA_RCC_TIM4_UP			RCC_DMA1
#define DMA_RCC_I2C1_RX			RCC_DMA1

#define DMA_RCC_TIM2			RCC_DMA1
#define DMA_RCC_TIM3			RCC_DMA1
#define DMA_RCC_TIM4			RCC_DMA1
#define DMA_RCC_TIM6			RCC_DMA1
#define DMA_RCC_TIM7			RCC_DMA1
#define DMA_RCC_DAC			RCC_DMA1
#define DMA_RCC_USART1			RCC_DMA1
#define DMA_RCC_USART2			RCC_DMA1
#define DMA_RCC_USART3			RCC_DMA1
#define DMA_RCC_SPI1			RCC_DMA1
#define DMA_RCC_SPI2			RCC_DMA1
#define DMA_RCC_I2C1			RCC_DMA1
#define DMA_RCC_I2C2			RCC_DMA1

#define DMA_RCC_SPI3_RX			RCC_DMA2	/* (**) */
#define DMA_RCC_USART5_TX		RCC_DMA2	/* (***) */
#define DMA_RCC_TIM5_CH4		RCC_DMA2	/* (**) */
#define DMA_RCC_TIM5_TRIG		RCC_DMA2	/* (**) */
#define DMA_RCC_TIM5_COM		RCC_DMA2	/* (**) */
#define DMA_RCC_SPI3_TX			RCC_DMA2	/* (**) */
#define DMA_RCC_USART5_RX		RCC_DMA2	/* (***) */
#define DMA_RCC_TIM5_CH3		RCC_DMA2	/* (**) */
#define DMA_RCC_TIM5_UP			RCC_DMA2	/* (**) */
#define DMA_RCC_USART4_RX		RCC_DMA2	/* (***) */
#define DMA_RCC_AES_OUT			RCC_DMA2	/* (****) */
#define DMA_RCC_TIM5_CH2		RCC_DMA2	/* (**) */
#define DMA_RCC_SD_MMC			RCC_DMA2	/* (***) */
#define DMA_RCC_USART4_TX		RCC_DMA2	/* (***) */
#define DMA_RCC_TIM5_CH1		RCC_DMA2	/* (**) */
#define DMA_RCC_AES_IN			RCC_DMA2	/* (****) */

#define DMA_RCC_TIM5			RCC_DMA2	/* (**) */
#define DMA_RCC_USART4			RCC_DMA2	/* (***) */
#define DMA_RCC_USART5			RCC_DMA2	/* (***) */
#define DMA_RCC_SPI3			RCC_DMA2	/* (**) */
#define DMA_RCC_AES			RCC_DMA2	/* (****) */

#define DMA_ADC_IRQ			NVIC_DMA1_CHANNEL1_IRQ
#define DMA_TIM2_CH3_IRQ		NVIC_DMA1_CHANNEL1_IRQ
#define DMA_TIM4_CH1_IRQ		NVIC_DMA1_CHANNEL1_IRQ
#define DMA_USART3_TX_IRQ		NVIC_DMA1_CHANNEL2_IRQ
#define DMA_TIM2_UP_IRQ			NVIC_DMA1_CHANNEL2_IRQ
#define DMA_TIM3_CH3_IRQ		NVIC_DMA1_CHANNEL2_IRQ
#define DMA_SPI1_RX_IRQ			NVIC_DMA1_CHANNEL2_IRQ
#define DMA_TIM6_UP_IRQ			NVIC_DMA1_CHANNEL2_IRQ
#define DMA_DAC_CHANNEL1_IRQ		NVIC_DMA1_CHANNEL2_IRQ
#define DMA_USART3_RX_IRQ		NVIC_DMA1_CHANNEL3_IRQ
#define DMA_TIM3_CH4_IRQ		NVIC_DMA1_CHANNEL3_IRQ
#define DMA_TIM3_UP_IRQ			NVIC_DMA1_CHANNEL3_IRQ
#define DMA_SPI1_TX_IRQ			NVIC_DMA1_CHANNEL3_IRQ
#define DMA_TIM7_UP_IRQ			NVIC_DMA1_CHANNEL3_IRQ
#define DMA_DAC_CHANNEL2_IRQ		NVIC_DMA1_CHANNEL3_IRQ
#define DMA_USART1_TX_IRQ		NVIC_DMA1_CHANNEL4_IRQ
#define DMA_TIM4_CH2_IRQ		NVIC_DMA1_CHANNEL4_IRQ
#define DMA_SPI2_RX_IRQ			NVIC_DMA1_CHANNEL4_IRQ
#define DMA_I2C2_TX_IRQ			NVIC_DMA1_CHANNEL4_IRQ
#define DMA_USART1_RX_IRQ		NVIC_DMA1_CHANNEL5_IRQ
#define DMA_SPI2_TX_IRQ			NVIC_DMA1_CHANNEL5_IRQ
#define DMA_TIM2_CH1_IRQ		NVIC_DMA1_CHANNEL5_IRQ
#define DMA_TIM4_CH3_IRQ		NVIC_DMA1_CHANNEL5_IRQ
#define DMA_I2C2_RX_IRQ			NVIC_DMA1_CHANNEL5_IRQ
#define DMA_USART2_RX_IRQ		NVIC_DMA1_CHANNEL6_IRQ
#define DMA_TIM3_CH1_IRQ		NVIC_DMA1_CHANNEL6_IRQ
#define DMA_TIM3_TRIG_IRQ		NVIC_DMA1_CHANNEL6_IRQ
#define DMA_I2C1_TX_IRQ			NVIC_DMA1_CHANNEL6_IRQ
#define DMA_USART2_TX_IRQ		NVIC_DMA1_CHANNEL7_IRQ
#define DMA_TIM2_CH2_IRQ		NVIC_DMA1_CHANNEL7_IRQ
#define DMA_TIM2_CH4_IRQ		NVIC_DMA1_CHANNEL7_IRQ
#define DMA_TIM4_UP_IRQ			NVIC_DMA1_CHANNEL7_IRQ
#define DMA_I2C1_RX_IRQ			NVIC_DMA1_CHANNEL7_IRQ

#define DMA_SPI3_RX_IRQ			NVIC_DMA2_CHANNEL1_IRQ	/* (**) */
#define DMA_USART5_TX_IRQ		NVIC_DMA2_CHANNEL1_IRQ	/* (***) */
#define DMA_TIM5_CH4_IRQ		NVIC_DMA2_CHANNEL1_IRQ	/* (**) */
#define DMA_TIM5_TRIG_IRQ		NVIC_DMA2_CHANNEL1_IRQ	/* (**) */
#define DMA_TIM5_COM_IRQ		NVIC_DMA2_CHANNEL1_IRQ	/* (**) */
#define DMA_SPI3_TX_IRQ			NVIC_DMA2_CHANNEL2_IRQ	/* (**) */
#define DMA_USART5_RX_IRQ		NVIC_DMA2_CHANNEL2_IRQ	/* (***) */
#define DMA_TIM5_CH3_IRQ		NVIC_DMA2_CHANNEL2_IRQ	/* (**) */
#define DMA_TIM5_UP_IRQ			NVIC_DMA2_CHANNEL2_IRQ	/* (**) */
#define DMA_USART4_RX_IRQ		NVIC_DMA2_CHANNEL3_IRQ	/* (***) */
#define DMA_AES_OUT_IRQ			NVIC_DMA2_CHANNEL3_IRQ	/* (****) */
#define DMA_TIM5_CH2_IRQ		NVIC_DMA2_CHANNEL4_IRQ	/* (**) */
#define DMA_SD_MMC_IRQ			NVIC_DMA2_CHANNEL4_IRQ	/* (***) */
#define DMA_USART4_TX_IRQ		NVIC_DMA2_CHANNEL5_IRQ	/* (***) */
#define DMA_TIM5_CH1_IRQ		NVIC_DMA2_CHANNEL5_IRQ	/* (**) */
#define DMA_AES_IN_IRQ			NVIC_DMA2_CHANNEL5_IRQ	/* (****) */

#define dma_adc_isr			dma1_channel1_isr
#define dma_tim2_ch3_isr		dma1_channel1_isr
#define dma_tim4_ch1_isr		dma1_channel1_isr
#define dma_usart3_tx_isr		dma1_channel2_isr
#define dma_tim2_up_isr			dma1_channel2_isr
#define dma_tim3_ch3_isr		dma1_channel2_isr
#define dma_spi1_rx_isr			dma1_channel2_isr
#define dma_tim6_up_isr			dma1_channel2_isr
#define dma_dac_channel1_isr		dma1_channel2_isr
#define dma_usart3_rx_isr		dma1_channel3_isr
#define dma_tim3_ch4_isr		dma1_channel3_isr
#define dma_tim3_up_isr			dma1_channel3_isr
#define dma_spi1_tx_isr			dma1_channel3_isr
#define dma_tim7_up_isr			dma1_channel3_isr
#define dma_dac_channel2_isr		dma1_channel3_isr
#define dma_usart1_tx_isr		dma1_channel4_isr
#define dma_tim4_ch2_isr		dma1_channel4_isr
#define dma_spi2_rx_isr			dma1_channel4_isr
#define dma_i2c2_tx_isr			dma1_channel4_isr
#define dma_usart1_rx_isr		dma1_channel5_isr
#define dma_spi2_tx_isr			dma1_channel5_isr
#define dma_tim2_ch1_isr		dma1_channel5_isr
#define dma_tim4_ch3_isr		dma1_channel5_isr
#define dma_i2c2_rx_isr			dma1_channel5_isr
#define dma_usart2_rx_isr		dma1_channel6_isr
#define dma_tim3_ch1_isr		dma1_channel6_isr
#define dma_tim3_trig_isr		dma1_channel6_isr
#define dma_i2c1_tx_isr			dma1_channel6_isr
#define dma_usart2_tx_isr		dma1_channel7_isr
#define dma_tim2_ch2_isr		dma1_channel7_isr
#define dma_tim2_ch4_isr		dma1_channel7_isr
#define dma_tim4_up_isr			dma1_channel7_isr
#define dma_i2c1_rx_isr			dma1_channel7_isr

#define dma_spi3_rx_isr			dma2_channel1_isr	/* (**) */
#define dma_usart5_tx_isr		dma2_channel1_isr	/* (***) */
#define dma_tim5_ch4_isr		dma2_channel1_isr	/* (**) */
#define dma_tim5_trig_isr		dma2_channel1_isr	/* (**) */
#define dma_tim5_com_isr		dma2_channel1_isr	/* (**) */
#define dma_spi3_tx_isr			dma2_channel2_isr	/* (**) */
#define dma_usart5_rx_isr		dma2_channel2_isr	/* (***) */
#define dma_tim5_ch3_isr		dma2_channel2_isr	/* (**) */
#define dma_tim5_up_isr			dma2_channel2_isr	/* (**) */
#define dma_usart4_rx_isr		dma2_channel3_isr	/* (***) */
#define dma_aes_out_isr			dma2_channel3_isr	/* (****) */
#define dma_tim5_ch2_isr		dma2_channel4_isr	/* (**) */
#define dma_sd_mmc_isr			dma2_channel4_isr	/* (***) */
#define dma_usart4_tx_isr		dma2_channel5_isr	/* (***) */
#define dma_tim5_ch1_isr		dma2_channel5_isr	/* (**) */
#define dma_aes_in_isr			dma2_channel5_isr	/* (****) */

/* --- Function prototypes ------------------------------------------------- */

/* Channel */
typedef enum dma_channel dma_channel_t;

/* Interrupt */
enum {
	DMA_ERROR	= (1 << 3),
	DMA_HALF	= (1 << 2),
	DMA_COMPLETE	= (1 << 1),
	DMA_GLOBAL	= (1 << 0)
};

/* Mode */
enum {
	DMA_ENABLE = (1 << 0),
	DMA_M_TO_P = (1 << 4),
	DMA_P_TO_M = 0,
	DMA_CIRCULAR = (1 << 5),
	DMA_P_INC = (1 << 6),
	DMA_M_INC = (1 << 7),
	DMA_P_8BIT = (0 << 8),
	DMA_P_16BIT = (1 << 8),
	DMA_P_32BIT = (2 << 8),
	DMA_M_8BIT = (0 << 10),
	DMA_M_16BIT = (1 << 10),
	DMA_M_32BIT = (2 << 10),
	DMA_LOW = (0 << 12),
	DMA_MEDIUM = (1 << 12),
	DMA_HIGH = (2 << 12),
	DMA_VERYHIGH = (3 << 12),
	DMA_M_TO_M = (1 << 14)
};

void dma_setup_channel(dma_channel_t dma, u32 ma, u32 pa, int ndt, int mode);
void dma_enable(dma_channel_t dma);
void dma_disable(dma_channel_t dma);
void dma_enable_interrupt(dma_channel_t dma, int ch_int);
void dma_disable_interrupt(dma_channel_t dma, int ch_int);
int dma_get_interrupt_mask(dma_channel_t dma, int ch_int);
int dma_get_interrupt_status(dma_channel_t dma, int ch_int);
void dma_clear_interrupt(dma_channel_t dma, int ch_int);
