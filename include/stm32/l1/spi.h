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
 * 27 Serial peripheral interface (SPI)
 */

/* In low and medium density devices, the I2S protocol is not available. */

/*
 * Note: Register marked (**) only exist in high and medium+ density devices.
 */

/* --- SPI registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		SPI_CR1		SPI control register 1
 * 0x04		SPI_CR2		SPI control register 2
 * 0x08		SPI_SR		SPI status register
 * 0x0c		SPI_DR		SPI data register
 * 0x10		SPI_CRCPR	SPI CRC polynomial register
 * 0x14		SPI_RXCRCR	SPI RX CRC register
 * 0x18		SPI_TXCRCR	SPI TX CRC register
 * 0x1c		SPI_I2SCFGR	SPI I2S configuration register
 * 0x20		SPI_I2SPR	SPI I2S prescaler register
 */

/* SPIx_CR1 */
#define SPI_CR1(base)			MMIO32((base) + 0x00)
#define SPI1_CR1			SPI_CR1(SPI1_BASE)
#define SPI2_CR1			SPI_CR1(SPI2_BASE)
#define SPI3_CR1			SPI_CR1(SPI3_BASE)	/* (**) */

/* SPIx_CR2 */
#define SPI_CR2(base)			MMIO32((base) + 0x04)
#define SPI1_CR2			SPI_CR2(SPI1_BASE)
#define SPI2_CR2			SPI_CR2(SPI2_BASE)
#define SPI3_CR2			SPI_CR2(SPI3_BASE)	/* (**) */

/* SPIx_SR */
#define SPI_SR(base)			MMIO32((base) + 0x08)
#define SPI1_SR				SPI_SR(SPI1_BASE)
#define SPI2_SR				SPI_SR(SPI2_BASE)
#define SPI3_SR				SPI_SR(SPI3_BASE)	/* (**) */

/* SPIx_DR */
#define SPI_DR(base)			MMIO32((base) + 0x0c)
#define SPI1_DR				SPI_DR(SPI1_BASE)
#define SPI2_DR				SPI_DR(SPI2_BASE)
#define SPI3_DR				SPI_DR(SPI3_BASE)	/* (**) */

/* SPIx_CRCPR */
#define SPI_CRCPR(base)			MMIO32((base) + 0x10)
#define SPI1_CRCPR			SPI_CRCPR(SPI1_BASE)
#define SPI2_CRCPR			SPI_CRCPR(SPI2_BASE)
#define SPI3_CRCPR			SPI_CRCPR(SPI3_BASE)	/* (**) */

/* SPIx_RXCRCR */
#define SPI_RXCRCR(base)		MMIO32((base) + 0x14)
#define SPI1_RXCRCR			SPI_RXCRCR(SPI1_BASE)
#define SPI2_RXCRCR			SPI_RXCRCR(SPI2_BASE)
#define SPI3_RXCRCR			SPI_RXCRCR(SPI3_BASE)	/* (**) */

/* SPIx_TXCRCR */
#define SPI_TXCRCR(base)		MMIO32((base) + 0x18)
#define SPI1_TXCRCR			SPI_TXCRCR(SPI1_BASE)
#define SPI2_TXCRCR			SPI_TXCRCR(SPI2_BASE)
#define SPI3_TXCRCR			SPI_TXCRCR(SPI3_BASE)	/* (**) */

/* SPIx_I2SCFGR */
#define SPI_I2SCFGR(base)		MMIO32((base) + 0x1c)
#define SPI1_I2SCFGR			SPI_I2SCFGR(SPI1_BASE)
#define SPI2_I2SCFGR			SPI_I2SCFGR(SPI2_BASE)
#define SPI3_I2SCFGR			SPI_I2SCFGR(SPI3_BASE)	/* (**) */

/* SPIx_I2SPR */
#define SPI_I2SPR(base)			MMIO32((base) + 0x20)
#define SPI1_I2SPR			SPI_I2SPR(SPI1_BASE)
#define SPI2_I2SPR			SPI_I2SPR(SPI2_BASE)
#define SPI3_I2SPR			SPI_I2SPR(SPI3_BASE)	/* (**) */

/* --- SPI_CR1 values ------------------------------------------------------ */

#define SPI_CR1_BIDIMODE		(1 << 15)
#define SPI_CR1_BIDIOE			(1 << 14)
#define SPI_CR1_CRCEN			(1 << 13)
#define SPI_CR1_CRCNEXT			(1 << 12)
#define SPI_CR1_DFF			(1 << 11)
#define SPI_CR1_RXONLY			(1 << 10)
#define SPI_CR1_SSM			(1 << 9)
#define SPI_CR1_SSI			(1 << 8)
#define SPI_CR1_LSBFIRST		(1 << 7)
#define SPI_CR1_SPE			(1 << 6)
#define SPI_CR1_BR2			(1 << 5)
#define SPI_CR1_BR1			(1 << 4)
#define SPI_CR1_BR0			(1 << 3)
#define SPI_CR1_MSTR			(1 << 2)
#define SPI_CR1_CPOL			(1 << 1)
#define SPI_CR1_CPHA			(1 << 0)

/* BR[2:0]: Baud rate control */
#define SPI_CR1_BR_FPCLK_DIV_2		(0 << 3)
#define SPI_CR1_BR_FPCLK_DIV_4		(1 << 3)
#define SPI_CR1_BR_FPCLK_DIV_8		(2 << 3)
#define SPI_CR1_BR_FPCLK_DIV_16		(3 << 3)
#define SPI_CR1_BR_FPCLK_DIV_32		(4 << 3)
#define SPI_CR1_BR_FPCLK_DIV_64		(5 << 3)
#define SPI_CR1_BR_FPCLK_DIV_128	(6 << 3)
#define SPI_CR1_BR_FPCLK_DIV_256	(7 << 3)

/* --- SPI_CR2 values ------------------------------------------------------ */

#define SPI_CR2_TXEIE			(1 << 7)
#define SPI_CR2_RXNEIE			(1 << 6)
#define SPI_CR2_ERRIE			(1 << 5)
#define SPI_CR2_FRF			(1 << 4)
#define SPI_CR2_SSOE			(1 << 2)
#define SPI_CR2_TXDMAEN			(1 << 1)
#define SPI_CR2_RXDMAEN			(1 << 0)

/* --- SPI_SR values ------------------------------------------------------- */

#define SPI_SR_FRE			(1 << 8)
#define SPI_SR_BSY			(1 << 7)
#define SPI_SR_OVR			(1 << 6)
#define SPI_SR_MODF			(1 << 5)
#define SPI_SR_CRCERR			(1 << 4)
#define SPI_SR_UDR			(1 << 3)
#define SPI_SR_CHSIDE			(1 << 2)
#define SPI_SR_TXE			(1 << 1)
#define SPI_SR_RXNE			(1 << 0)

#define SPI_SR_ERROR			(SPI_SR_FRE | SPI_SR_OVR | \
					 SPI_SR_MODF | SPI_SR_CRCERR)

#define SPI_SR_I2S_ERROR		(SPI_SR_FRE | SPI_SR_OVR | \
					 SPI_SR_UDR)

/* --- SPI_DR values ------------------------------------------------------- */

/* SPI_DR[15:0]: Data Register. */

/* --- SPI_CRCPR values ---------------------------------------------------- */

/* SPI_CRCPR[15:0]: CRC Polynomial Register. */

/* --- SPI_RXCRCR values --------------------------------------------------- */

/* SPI_RXCRCR[15:0]: RX CRC Register. */

/* --- SPI_TXCRCR values --------------------------------------------------- */

/* SPI_TXCRCR[15:0]: TX CRC Register. */

/* --- SPI_I2SCFGR values -------------------------------------------------- */

#define SPI_I2SCFGR_I2SMOD			(1 << 11)
#define SPI_I2SCFGR_I2SE			(1 << 10)
#define SPI_I2SCFGR_I2SCFG1			(1 << 9)
#define SPI_I2SCFGR_I2SCFG0			(1 << 8)
#define SPI_I2SCFGR_PCMSYNC			(1 << 7)
#define SPI_I2SCFGR_I2SSTD1			(1 << 5)
#define SPI_I2SCFGR_I2SSTD0			(1 << 4)
#define SPI_I2SCFGR_CKPOL			(1 << 3)
#define SPI_I2SCFGR_DATLEN1			(1 << 2)
#define SPI_I2SCFGR_DATLEN0			(1 << 1)
#define SPI_I2SCFGR_CHLEN			(1 << 0)

/* I2S configuration mode */
#define SPI_I2SCFGR_I2SCFG_SLAVE_TRANSMIT	(0 << 8)
#define SPI_I2SCFGR_I2SCFG_SLAVE_RECEIVE	(1 << 8)
#define SPI_I2SCFGR_I2SCFG_MASTER_TRANSMIT	(2 << 8)
#define SPI_I2SCFGR_I2SCFG_MASTER_RECEIVE	(3 << 8)

/* I2S standard selection */
#define SPI_I2SCFGR_I2SSTD_I2S_PHILLIPS		(0 << 4)
#define SPI_I2SCFGR_I2SSTD_MSB_JUSTIFIED	(1 << 4)
#define SPI_I2SCFGR_I2SSTD_LSB_JUSTIFIED	(2 << 4)
#define SPI_I2SCFGR_I2SSTD_PCM			(3 << 4)

/* Data length to be transferred */
#define SPI_I2SCFGR_DATLEN_16BIT		(0 << 1)
#define SPI_I2SCFGR_DATLEN_24BIT		(1 << 1)
#define SPI_I2SCFGR_DATLEN_32BIT		(2 << 1)

/* --- SPI_I2SPR values ---------------------------------------------------- */

#define SPI_I2SPR_MCKOE				(1 << 9)
#define SPI_I2SPR_ODD				(1 << 8)

/* I2SDIV[7:0]: I2S linear prescaler */
/* 0 and 1 are forbidden values */

/* --- Function prototypes ------------------------------------------------- */

/* SPI device number */
typedef enum {
	SPI1,
	SPI2,
	SPI3
} spi_t;

/* Interrupt/Status */
enum {
	SPI_TXE = (1 << 1),
	SPI_RXNE = (1 << 0),

	SPI_FRAME_ERROR = (1 << 8),
	SPI_BUSY = (1 << 7),
	SPI_OVERRUN = (1 << 6),
	SPI_MODE_FAULT = (1 << 5),
	SPI_CRC_ERROR = (1 << 4),

	SPI_ERROR = (23 << 4)
};

/* DMA */
typedef enum {
	SPI_DMA_TX,
	SPI_DMA_RX,
	SPI_DMA_TX_RX
} spi_dma_t;

/* Mode */
enum {
	SPI_CLOCK_POLARITY = (1 << 1),
	SPI_CLOCK_PHASE = (1 << 0),
	SPI_16BIT = (1 << 11),
	SPI_LSB_FIRST = (1 << 7),

	SPI_NSS_SOFTWARE = (1 << 9),
	SPI_NSS_HIGH = (1 << 8),
	SPI_NSS_OUTPUT = (1 << 16),

	SPI_CRC_ENABLE = (1 << 13),
	SPI_CRC_NEXT = (1 << 12),

	SPI_BIDIRECTIONAL = (1 << 15),
	SPI_BIDI_OUTPUT = (1 << 14),
	SPI_RXONLY = (1 << 10),
	SPI_TI_MODE = (1 << 17),

	SPI_MASTER = (1 << 2),
	SPI_ENABLE = (1 << 6)
};

void spi_set_mode(spi_t spi, int prescaler, int mode);
void spi_enable(spi_t spi);
void spi_disable(spi_t spi);
void spi_set_nss(spi_t spi, bool value);
void spi_set_bidi_output(spi_t spi);
void spi_set_bidi_input(spi_t spi);
void spi_enable_interrupt(spi_t spi, int interrupt);
void spi_disable_interrupt(spi_t spi, int interrupt);
int spi_get_interrupt_mask(spi_t spi, int interrupt);
int spi_get_interrupt_status(spi_t spi, int interrupt);
void spi_clear_interrupt(spi_t spi, int interrupt);
void spi_enable_dma(spi_t spi, spi_dma_t dma);
void spi_disable_dma(spi_t spi, spi_dma_t dma);
void spi_set_crc_polynomial(spi_t spi, u16 polynomial);
void spi_enable_crc_phase(spi_t spi);
void spi_disable_crc_phase(spi_t spi);
void spi_send(spi_t spi, u16 data);
u16 spi_recv(spi_t spi);

/* Error */
#define SPI_TRANSFER_ERROR			0x10000

int spi_transfer(spi_t spi, u16 data);
