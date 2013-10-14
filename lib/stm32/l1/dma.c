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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stm32/l1/dma.h>

static u32 base_addr(dma_channel_t dma)
{
	switch (dma) {
	case DMA1_CHANNEL1:
	case DMA1_CHANNEL2:
	case DMA1_CHANNEL3:
	case DMA1_CHANNEL4:
	case DMA1_CHANNEL5:
	case DMA1_CHANNEL6:
	case DMA1_CHANNEL7:
		return DMA1_BASE;
	case DMA2_CHANNEL1:
	case DMA2_CHANNEL2:
	case DMA2_CHANNEL3:
	case DMA2_CHANNEL4:
	case DMA2_CHANNEL5:
		return DMA2_BASE;
	default:
		break;
	}
	return 0;
}

static int channel_num(dma_channel_t dma)
{
	switch (dma) {
	case DMA1_CHANNEL1:
		return 0;
	case DMA1_CHANNEL2:
		return 1;
	case DMA1_CHANNEL3:
		return 2;
	case DMA1_CHANNEL4:
		return 3;
	case DMA1_CHANNEL5:
		return 4;
	case DMA1_CHANNEL6:
		return 5;
	case DMA1_CHANNEL7:
		return 6;
	case DMA2_CHANNEL1:
		return 0;
	case DMA2_CHANNEL2:
		return 1;
	case DMA2_CHANNEL3:
		return 2;
	case DMA2_CHANNEL4:
		return 3;
	case DMA2_CHANNEL5:
		return 4;
	default:
		break;
	}
	return 0;
}

void dma_setup_channel(dma_channel_t dma, u32 ma, u32 pa, int ndt, int mode)
{
	u32 base;
	int channel;

	base = base_addr(dma);
	channel = channel_num(dma);
	DMA_CMAR(base, channel) = ma;
	DMA_CPAR(base, channel) = pa;
	DMA_CNDTR(base, channel) = (ndt & 0xffff);
	DMA_CCR(base, channel) = mode;
}

void dma_enable(dma_channel_t dma)
{
	DMA_CCR(base_addr(dma), channel_num(dma)) |= DMA_CCR_EN;
}

void dma_disable(dma_channel_t dma)
{
	DMA_CCR(base_addr(dma), channel_num(dma)) &= ~DMA_CCR_EN;
}

void dma_enable_interrupt(dma_channel_t dma, int ch_int)
{
	/*
	 * DMA_ERROR    = DMA_CCR_TEIE
	 * DMA_HALF     = DMA_CCR_HTIE
	 * DMA_COMPLETE = DMA_TCIE
	 */
	DMA_CCR(base_addr(dma), channel_num(dma)) |= ch_int;
}

void dma_disable_interrupt(dma_channel_t dma, int ch_int)
{
	DMA_CCR(base_addr(dma), channel_num(dma)) &= ~ch_int;
}

int dma_get_interrupt_mask(dma_channel_t dma, int ch_int)
{
	return DMA_CCR(base_addr(dma), channel_num(dma)) & ch_int;
}

int dma_get_interrupt_status(dma_channel_t dma, int ch_int)
{
	return (DMA_ISR(base_addr(dma)) >> (4 * channel_num(dma))) & ch_int;
}

void dma_clear_interrupt(dma_channel_t dma, int ch_int)
{
	u32 base;

	base = base_addr(dma);
	DMA_IFCR(base) = ch_int << (4 * channel_num(dma));
	DMA_IFCR(base) = 0;
}
