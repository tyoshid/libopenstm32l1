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

#include <stm32/l1/spi.h>

static u32 base_addr(spi_t spi)
{
	switch (spi) {
	case SPI1:
		return SPI1_BASE;
	case SPI2:
		return SPI2_BASE;
	case SPI3:
		return SPI3_BASE;
	default:
		break;
	}
	return 0;
}

void spi_set_mode(spi_t spi, int prescaler, int mode)
{
	int i;
	u32 base;
	u32 cr1;
	u32 cr2;

	for (i = 0; i < 8; i++)
		if (prescaler == (1 << (i + 1)))
			break;
	if (i >= 8)
		return;
	cr1 = (i << 3);
	cr1 |= (mode & 0xffc7);

	cr2 = 0;
	if (mode & SPI_NSS_OUTPUT)
		cr2 |= SPI_CR2_SSOE;
	if (mode & SPI_TI_MODE)
		cr2 |= SPI_CR2_FRF;

	base = base_addr(spi);
	SPI_CR2(base) = cr2;
	SPI_CR1(base) = cr1;
}

void spi_enable(spi_t spi)
{
	SPI_CR1(base_addr(spi)) |= SPI_CR1_SPE;
}

void spi_disable(spi_t spi)
{
	SPI_CR1(base_addr(spi)) &= ~SPI_CR1_SPE;
}

void spi_set_nss(spi_t spi, bool value)
{
	if (value)
		SPI_CR1(base_addr(spi)) |= SPI_CR1_SSI;
	else
		SPI_CR1(base_addr(spi)) &= ~SPI_CR1_SSI;
}

void spi_set_bidi_output(spi_t spi)
{
	SPI_CR1(base_addr(spi)) |= SPI_CR1_BIDIOE;
}

void spi_set_bidi_input(spi_t spi)
{
	SPI_CR1(base_addr(spi)) &= ~SPI_CR1_BIDIOE;
}

void spi_enable_interrupt(spi_t spi, int interrupt)
{
	if (interrupt & SPI_TXE)
		SPI_CR2(base_addr(spi)) |= SPI_CR2_TXEIE;
	if (interrupt & SPI_RXNE)
		SPI_CR2(base_addr(spi)) |= SPI_CR2_RXNEIE;
	if (interrupt & SPI_ERROR)
		SPI_CR2(base_addr(spi)) |= SPI_CR2_ERRIE;
}

void spi_disable_interrupt(spi_t spi, int interrupt)
{
	if (interrupt & SPI_TXE)
		SPI_CR2(base_addr(spi)) &= ~SPI_CR2_TXEIE;
	if (interrupt & SPI_RXNE)
		SPI_CR2(base_addr(spi)) &= ~SPI_CR2_RXNEIE;
	if (interrupt & SPI_ERROR)
		SPI_CR2(base_addr(spi)) &= ~SPI_CR2_ERRIE;
}

int spi_get_interrupt_mask(spi_t spi, int interrupt)
{
	u32 reg32;
	u32 r = 0;

	reg32 = SPI_CR2(base_addr(spi));
	if (reg32 & SPI_CR2_TXEIE)
		r |= SPI_TXE;
	if (reg32 & SPI_CR2_RXNEIE)
		r |= SPI_RXNE;
	if (reg32 & SPI_CR2_ERRIE)
		r |= SPI_ERROR;

	return r & interrupt;
}

int spi_get_interrupt_status(spi_t spi, int interrupt)
{
	return SPI_SR(base_addr(spi)) & interrupt;
}

void spi_clear_interrupt(spi_t spi, int interrupt)
{
	/* CRC error */
	SPI_SR(base_addr(spi)) &= ~interrupt;
}

void spi_enable_dma(spi_t spi, spi_dma_t dma)
{
	switch (dma) {
	case SPI_DMA_TX:
		SPI_CR2(base_addr(spi)) |= SPI_CR2_TXDMAEN;
		break;
	case SPI_DMA_RX:
		SPI_CR2(base_addr(spi)) |= SPI_CR2_RXDMAEN;
		break;
	case SPI_DMA_TX_RX:
		SPI_CR2(base_addr(spi)) |= (SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
		break;
	default:
		break;
	}
}

void spi_disable_dma(spi_t spi, spi_dma_t dma)
{
	switch (dma) {
	case SPI_DMA_TX:
		SPI_CR2(base_addr(spi)) &= ~SPI_CR2_TXDMAEN;
		break;
	case SPI_DMA_RX:
		SPI_CR2(base_addr(spi)) &= ~SPI_CR2_RXDMAEN;
		break;
	case SPI_DMA_TX_RX:
		SPI_CR2(base_addr(spi)) &= ~(SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
		break;
	default:
		break;
	}
}

void spi_set_crc_polynomial(spi_t spi, u16 polynomial)
{
	SPI_CRCPR(base_addr(spi)) = polynomial;
}

void spi_enable_crc_phase(spi_t spi)
{
	SPI_CR1(base_addr(spi)) |= SPI_CR1_CRCNEXT;
}

void spi_disable_crc_phase(spi_t spi)
{
	SPI_CR1(base_addr(spi)) &= ~SPI_CR1_CRCNEXT;
}

void spi_send(spi_t spi, u16 data)
{
	SPI_DR(base_addr(spi)) = data;
}

u16 spi_recv(spi_t spi)
{

	return SPI_DR(base_addr(spi));
}


/* Polling mode function */

int spi_transfer(spi_t spi, u16 data)
{
	u32 r;

	while (!((r = SPI_SR(base_addr(spi))) & (SPI_SR_TXE | SPI_SR_ERROR)))
		;
	if (r & SPI_SR_ERROR)
		return -SPI_TRANSFER_ERROR | r;

	SPI_DR(base_addr(spi)) = data;
	while (!((r = SPI_SR(base_addr(spi))) & (SPI_SR_RXNE | SPI_SR_ERROR)))
		;
	if (r & SPI_SR_ERROR)
		return -SPI_TRANSFER_ERROR | r;

	return SPI_DR(base_addr(spi));
}
