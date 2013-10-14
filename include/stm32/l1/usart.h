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
 * 26 Universal synchronous asynchronous receiver transmitter (USART)
 */

/* --- USART registers ----------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		USART_SR		Status register
 * 0x04		USART_DR		Data register
 * 0x08		USART_BRR		Baud rate register
 * 0x0c		USART_CR1		Control register 1
 * 0x10		USART_CR2		Control register 2
 * 0x14		USART_CR3		Control register 3
 * 0x18		USART_GTPR		Guard time and prescaler register
 */

/* USART_SR */
#define USART_SR(usart_base)		MMIO32((usart_base) + 0x00)
#define USART1_SR			USART_SR(USART1_BASE)
#define USART2_SR			USART_SR(USART2_BASE)
#define USART3_SR			USART_SR(USART3_BASE)
#define UART4_SR			USART_SR(UART4_BASE)
#define UART5_SR			USART_SR(UART5_BASE)

/* USART_DR */
#define USART_DR(usart_base)		MMIO32((usart_base) + 0x04)
#define USART1_DR			USART_DR(USART1_BASE)
#define USART2_DR			USART_DR(USART2_BASE)
#define USART3_DR			USART_DR(USART3_BASE)
#define UART4_DR			USART_DR(UART4_BASE)
#define UART5_DR			USART_DR(UART5_BASE)

/* USART_BRR */
#define USART_BRR(usart_base)		MMIO32((usart_base) + 0x08)
#define USART1_BRR			USART_BRR(USART1_BASE)
#define USART2_BRR			USART_BRR(USART2_BASE)
#define USART3_BRR			USART_BRR(USART3_BASE)
#define UART4_BRR			USART_BRR(UART4_BASE)
#define UART5_BRR			USART_BRR(UART5_BASE)

/* USART_CR1 */
#define USART_CR1(usart_base)		MMIO32((usart_base) + 0x0c)
#define USART1_CR1			USART_CR1(USART1_BASE)
#define USART2_CR1			USART_CR1(USART2_BASE)
#define USART3_CR1			USART_CR1(USART3_BASE)
#define UART4_CR1			USART_CR1(UART4_BASE)
#define UART5_CR1			USART_CR1(UART5_BASE)

/* USART_CR2 */
#define USART_CR2(usart_base)		MMIO32((usart_base) + 0x10)
#define USART1_CR2			USART_CR2(USART1_BASE)
#define USART2_CR2			USART_CR2(USART2_BASE)
#define USART3_CR2			USART_CR2(USART3_BASE)
#define UART4_CR2			USART_CR2(UART4_BASE)
#define UART5_CR2			USART_CR2(UART5_BASE)

/* USART_CR3 */
#define USART_CR3(usart_base)		MMIO32((usart_base) + 0x14)
#define USART1_CR3			USART_CR3(USART1_BASE)
#define USART2_CR3			USART_CR3(USART2_BASE)
#define USART3_CR3			USART_CR3(USART3_BASE)
#define UART4_CR3			USART_CR3(UART4_BASE)
#define UART5_CR3			USART_CR3(UART5_BASE)

/* USART_GTPR */
#define USART_GTPR(usart_base)		MMIO32((usart_base) + 0x18)
#define USART1_GTPR			USART_GTPR(USART1_BASE)
#define USART2_GTPR			USART_GTPR(USART2_BASE)
#define USART3_GTPR			USART_GTPR(USART3_BASE)
#define UART4_GTPR			USART_GTPR(UART4_BASE)
#define UART5_GTPR			USART_GTPR(UART5_BASE)

/* --- USART_SR values ----------------------------------------------------- */

#define USART_SR_CTS			(1 << 9)
#define USART_SR_LBD			(1 << 8)
#define USART_SR_TXE			(1 << 7)
#define USART_SR_TC			(1 << 6)
#define USART_SR_RXNE			(1 << 5)
#define USART_SR_IDLE			(1 << 4)
#define USART_SR_ORE			(1 << 3)
#define USART_SR_NF			(1 << 2)
#define USART_SR_FE			(1 << 1)
#define USART_SR_PE			(1 << 0)

/* --- USART_DR values ----------------------------------------------------- */

/* USART_DR[8:0]: DR[8:0]: Data value */

/* --- USART_BRR values ---------------------------------------------------- */

/* USART_BRR[15:4]: DIV_Mantissa[11:0]: mantissa of USARTDIV */
/* USART_BRR[3:0]: DIV_Fraction[3:0]: fraction of USARTDIV */

/* --- USART_CR1 values ---------------------------------------------------- */

#define USART_CR1_OVER8			(1 << 15)
#define USART_CR1_UE			(1 << 13)
#define USART_CR1_M			(1 << 12)
#define USART_CR1_WAKE			(1 << 11)
#define USART_CR1_PCE			(1 << 10)
#define USART_CR1_PS			(1 << 9)
#define USART_CR1_PEIE			(1 << 8)
#define USART_CR1_TXEIE			(1 << 7)
#define USART_CR1_TCIE			(1 << 6)
#define USART_CR1_RXNEIE		(1 << 5)
#define USART_CR1_IDLEIE		(1 << 4)
#define USART_CR1_TE			(1 << 3)
#define USART_CR1_RE			(1 << 2)
#define USART_CR1_RWU			(1 << 1)
#define USART_CR1_SBK			(1 << 0)

/* --- USART_CR2 values ---------------------------------------------------- */

#define USART_CR2_LINEN			(1 << 14)
#define USART_CR2_STOP1			(1 << 13)
#define USART_CR2_STOP0			(1 << 12)
#define USART_CR2_CLKEN			(1 << 11)
#define USART_CR2_CPOL			(1 << 10)
#define USART_CR2_CPHA			(1 << 9)
#define USART_CR2_LBCL			(1 << 8)
#define USART_CR2_LBDIE			(1 << 6)
#define USART_CR2_LBDL			(1 << 5)
#define USART_CR2_ADD3			(1 << 3)
#define USART_CR2_ADD2			(1 << 2)
#define USART_CR2_ADD1			(1 << 1)
#define USART_CR2_ADD0			(1 << 0)

/* STOP bits */
#define USART_CR2_STOPBITS_1		(0 << 12)     /* 1 stop bit */
#define USART_CR2_STOPBITS_0_5		(1 << 12)     /* 0.5 stop bits */
#define USART_CR2_STOPBITS_2		(2 << 12)     /* 2 stop bits */
#define USART_CR2_STOPBITS_1_5		(3 << 12)     /* 1.5 stop bits */

/* --- USART_CR3 values ---------------------------------------------------- */

#define USART_CR3_ONEBIT		(1 << 11)
#define USART_CR3_CTSIE			(1 << 10)
#define USART_CR3_CTSE			(1 << 9)
#define USART_CR3_RTSE			(1 << 8)
#define USART_CR3_DMAT			(1 << 7)
#define USART_CR3_DMAR			(1 << 6)
#define USART_CR3_SCEN			(1 << 5)
#define USART_CR3_NACK			(1 << 4)
#define USART_CR3_HDSEL			(1 << 3)
#define USART_CR3_IRLP			(1 << 2)
#define USART_CR3_IREN			(1 << 1)
#define USART_CR3_EIE			(1 << 0)

/* --- USART_GTPR values --------------------------------------------------- */

/* USART_GTPR[15:8]: GT[7:0]: Guard time value */
/* USART_GTPR[7:0]: PSC[7:0]: Prescaler value */

/* --- Function prototypes ------------------------------------------------- */

/* Device number */
typedef enum {
	USART1,
	USART2,
	USART3,
	UART4,
	UART5
} usart_t;

/* Stop bits */
typedef enum {
	USART_STOP_1 = (0 << 12),
	USART_STOP_0_5 = (1 << 12),
	USART_STOP_2 = (2 << 12),
	USART_STOP_1_5 = (3 << 12)
} usart_stop_t;

/* Parity */
typedef enum {
	USART_PARITY_NONE,
	USART_ODD,
	USART_EVEN
} usart_parity_t;

/* Flow control */
typedef enum {
	USART_FLOW_NONE = 0,
	USART_FLOW_RTS = (1 << 8),
	USART_FLOW_CTS = (1 << 9),
	USART_RTS_CTS = (3 << 8)
} usart_flowcontrol_t;

/* Mode */
typedef enum {
	USART_TX = (1 << 3),
	USART_RX = (1 << 2),
	USART_TX_RX = (3 << 2)
} usart_mode_t;

/* DMA */
typedef enum {
	USART_DMA_TX = (1 << 7),
	USART_DMA_RX = (1 << 6),
	USART_DMA_TX_RX = (3 << 6)
} usart_dma_t;

/* Interrupt */
enum {
	USART_CTS = (1 << 9),
	USART_LBD = (1 << 8),
	USART_TXE = (1 << 7),
	USART_TC = (1 << 6),
	USART_RXNE = (1 << 5),
	USART_IDLE = (1 << 4),
	USART_ORE = (1 << 3),
	USART_NF = (1 << 2),
	USART_FE = (1 << 1),
	USART_PE = (1 << 0),
	USART_ERROR = (1 << 10)
};

void usart_set_baudrate(usart_t usart, int clock, int baud);
void usart_set_databits(usart_t usart, int bits);
void usart_set_stopbits(usart_t usart, usart_stop_t stopbits);
void usart_set_parity(usart_t usart, usart_parity_t parity);
void usart_set_flow_control(usart_t usart, usart_flowcontrol_t flowcontrol);
void usart_set_mode(usart_t usart, usart_mode_t mode);
void usart_init(usart_t usart, int clock, int baud, int bits,
		usart_stop_t stopbits, usart_parity_t parity,
		usart_flowcontrol_t flowcontrol, usart_mode_t mode);
void usart_enable(usart_t usart);
void usart_disable(usart_t usart);
void usart_send(usart_t usart, u16 data);
u16 usart_recv(usart_t usart);
void usart_wait_send_ready(usart_t usart);
void usart_wait_recv_ready(usart_t usart);
void usart_send_blocking(usart_t usart, u16 data);
u16 usart_recv_blocking(usart_t usart);
void usart_enable_dma(usart_t usart, usart_dma_t dma);
void usart_disable_dma(usart_t usart, usart_dma_t dma);
void usart_enable_interrupt(usart_t usart, int interrupt);
void usart_disable_interrupt(usart_t usart, int interrupt);
int usart_get_interrupt_status(usart_t usart, int interrupt);
int usart_get_interrupt_mask(usart_t usart, int interrupt);
void usart_clear_interrupt(usart_t usart, int interrupt);
