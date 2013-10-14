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

#include <stm32/l1/usart.h>

static u32 base_addr(usart_t usart)
{
	switch (usart) {
	case USART1:
		return USART1_BASE;
	case USART2:
		return USART2_BASE;
	case USART3:
		return USART3_BASE;
	case UART4:
		return UART4_BASE;
	case UART5:
		return UART5_BASE;
	default:
		break;
	}
	return 0;
}

void usart_set_baudrate(usart_t usart, int clock, int baud)
{
	/*
	 * Yes it is as simple as that. The reference manual is
	 * talking about fractional calculation but it seems to be only
	 * marketting babble to sound awesome. It is nothing else but a
	 * simple divider to generate the correct baudrate.
	 *
	 * Note: We round() the value rather than floor()ing it, for more
	 * accurate divisor selection.
	 */
	/* oversampling by 16 (OVER8 = 0) */
	USART_BRR(base_addr(usart)) =
		((2 * clock) + baud) / (2 * baud);
}

void usart_set_databits(usart_t usart, int bits)
{
	u32 base;

	base = base_addr(usart);
	if (bits == 8)
		USART_CR1(base) &= ~USART_CR1_M; /* 8 data bits */
	else if (bits == 9)
		USART_CR1(base) |= USART_CR1_M;  /* 9 data bits */
}

void usart_set_stopbits(usart_t usart, usart_stop_t stopbits)
{
	u32 base;
	u32 reg32;

	base = base_addr(usart);
	reg32 = USART_CR2(base);
	reg32 &= ~(USART_CR2_STOP1 | USART_CR2_STOP0);
	USART_CR2(base) = (reg32 | stopbits);
}

void usart_set_parity(usart_t usart, usart_parity_t parity)
{
	u32 base;
	u32 reg32;

	base = base_addr(usart);
	reg32 = USART_CR1(base);
	switch (parity) {
	case USART_ODD:
		reg32 |= (USART_CR1_PS | USART_CR1_PCE);
		break;
	case USART_EVEN:
		reg32 &= ~USART_CR1_PS;
		reg32 |= USART_CR1_PCE;
		break;
	default:
		reg32 &= ~USART_CR1_PCE;
		break;
	}
	USART_CR1(base) = reg32;
}

void usart_set_flow_control(usart_t usart, usart_flowcontrol_t flowcontrol)
{
	u32 base;
	u32 reg32;

	base = base_addr(usart);
	reg32 = USART_CR3(base);
	reg32 &= ~(USART_CR3_RTSE | USART_CR3_CTSE);
	USART_CR3(base) = (reg32 | flowcontrol);
}

void usart_set_mode(usart_t usart, usart_mode_t mode)
{
	u32 base;
	u32 reg32;

	base = base_addr(usart);
	reg32 = USART_CR1(base);
	reg32 &= ~(USART_CR1_RE | USART_CR1_TE);
	USART_CR1(base) = (reg32 | mode);
}

void usart_init(usart_t usart, int clock, int baud, int bits,
		usart_stop_t stopbits, usart_parity_t parity,
		usart_flowcontrol_t flowcontrol, usart_mode_t mode)
{
	u32 base;
	u32 reg32;

	base = base_addr(usart);

	/* Baudrate */
	USART_BRR(base) = ((2 * clock) + baud) / (2 * baud);

	/* Data bits */
	reg32 = USART_CR1(base);
	if (bits == 8)
		reg32 &= ~USART_CR1_M;
	else if (bits == 9)
		reg32 |= USART_CR1_M;

	/* Parity */
	switch (parity) {
	case USART_ODD:
		reg32 |= (USART_CR1_PS | USART_CR1_PCE);
		break;
	case USART_EVEN:
		reg32 &= ~USART_CR1_PS;
		reg32 |= USART_CR1_PCE;
		break;
	default:
		reg32 &= ~USART_CR1_PCE;
		break;
	}

	/* Mode */
	reg32 &= ~(USART_CR1_RE | USART_CR1_TE);
	USART_CR1(base) = (reg32 | mode);

	/* Stop bits */
	reg32 = USART_CR2(base);
	reg32 &= ~(USART_CR2_STOP1 | USART_CR2_STOP0);
	USART_CR2(base) = (reg32 | stopbits);

	/* Flow control */
	reg32 = USART_CR3(base);
	reg32 &= ~(USART_CR3_RTSE | USART_CR3_CTSE);
	USART_CR3(base) = (reg32 | flowcontrol);

	/* Enable USART */
	USART_CR1(base) |= USART_CR1_UE;
}

void usart_enable(usart_t usart)
{
	USART_CR1(base_addr(usart)) |= USART_CR1_UE;
}

void usart_disable(usart_t usart)
{
	USART_CR1(base_addr(usart)) &= ~USART_CR1_UE;
}

void usart_send(usart_t usart, u16 data)
{
	/* Send data. */
	USART_DR(base_addr(usart)) = (data & 0x1ff);
}

u16 usart_recv(usart_t usart)
{
	/* Receive data. */
	return USART_DR(base_addr(usart));
}

void usart_wait_send_ready(usart_t usart)
{
	/* Wait until the data has been transferred into the shift register. */
	while (!(USART_SR(base_addr(usart)) & USART_SR_TXE))
		;
}

void usart_wait_recv_ready(usart_t usart)
{
	/* Wait until the data is ready to be received. */
	while (!(USART_SR(base_addr(usart)) & USART_SR_RXNE))
		;
}

void usart_send_blocking(usart_t usart, u16 data)
{
	usart_wait_send_ready(usart);
	usart_send(usart, data);
}

u16 usart_recv_blocking(usart_t usart)
{
	usart_wait_recv_ready(usart);
	return usart_recv(usart);
}

void usart_enable_dma(usart_t usart, usart_dma_t dma)
{
	USART_CR3(base_addr(usart)) |= dma;
}

void usart_disable_dma(usart_t usart, usart_dma_t dma)
{
	USART_CR3(base_addr(usart)) &= ~dma;
}

void usart_enable_interrupt(usart_t usart, int interrupt)
{
	u32 cr1 = 0;
	u32 cr2 = 0;
	u32 cr3 = 0;

	if (interrupt & USART_CTS)
		cr3 |= USART_CR3_CTSIE;
	if (interrupt & USART_LBD)
		cr2 |= USART_CR2_LBDIE;
	cr1 = (interrupt & (USART_TXE | USART_TC | USART_RXNE | USART_IDLE));
	if (interrupt & USART_ERROR)
		cr3 |= USART_CR3_EIE;
	if (interrupt & USART_PE)
		cr1 |= USART_CR1_PEIE;

	if (cr1)
		USART_CR1(base_addr(usart)) |= cr1;
	if (cr2)
		USART_CR2(base_addr(usart)) |= cr2;
	if (cr3)
		USART_CR3(base_addr(usart)) |= cr3;
}

void usart_disable_interrupt(usart_t usart, int interrupt)
{
	u32 cr1 = 0;
	u32 cr2 = 0;
	u32 cr3 = 0;

	if (interrupt & USART_CTS)
		cr3 |= USART_CR3_CTSIE;
	if (interrupt & USART_LBD)
		cr2 |= USART_CR2_LBDIE;
	cr1 = (interrupt & (USART_TXE | USART_TC | USART_RXNE | USART_IDLE));
	if (interrupt & USART_ERROR)
		cr3 |= USART_CR3_EIE;
	if (interrupt & USART_PE)
		cr1 |= USART_CR1_PEIE;

	if (cr1)
		USART_CR1(base_addr(usart)) &= ~cr1;
	if (cr2)
		USART_CR2(base_addr(usart)) &= ~cr2;
	if (cr3)
		USART_CR3(base_addr(usart)) &= ~cr3;
}

int usart_get_interrupt_status(usart_t usart, int interrupt)
{
	return USART_SR(base_addr(usart)) & interrupt;
}

int usart_get_interrupt_mask(usart_t usart, int interrupt)
{
	u32 reg32;
	u32 r = 0;

	if (interrupt & (USART_TXE | USART_TC | USART_RXNE | USART_IDLE |
			 USART_PE)) {
		reg32 = USART_CR1(base_addr(usart));
		r = reg32 & interrupt & (USART_TXE | USART_TC | USART_RXNE |
					 USART_IDLE);
		if (reg32 & USART_CR1_PEIE)
			r |= (interrupt & USART_PE);
	}
	if (interrupt & USART_LBD) {
		if (USART_CR2(base_addr(usart)) & USART_CR2_LBDIE)
			r |= USART_LBD;
	}
	if (interrupt & (USART_CTS | USART_ERROR)) {
		reg32 = USART_CR3(base_addr(usart));
		if (reg32 & USART_CR3_CTSIE)
			r |= (interrupt & USART_CTS);
		if (reg32 & USART_CR3_EIE)
			r |= (interrupt & USART_ERROR);
	}

	return r;
}

void usart_clear_interrupt(usart_t usart, int interrupt)
{
	USART_SR(base_addr(usart)) &= ~interrupt;
}
