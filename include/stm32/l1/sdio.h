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
 * 28 Secure digital input/output interface (SDIO)
 */

/* --- SDIO registers ------------------------------------------------------ */
/*
 * Offset	Register
 * 0x00		SDIO_POWER	SDIO power control register
 * 0x04		SDIO_CLKCR	SDIO clock control register
 * 0x08		SDIO_ARG	SDIO argument register
 * 0x0c		SDIO_CMD	SDIO command register
 * 0x10		SDIO_RESPCMD	SDIO command response register
 * 0x14		SDIO_RESP1	SDIO response 1 register
 * 0x18		SDIO_RESP2	SDIO response 2 register
 * 0x1c		SDIO_RESP3	SDIO response 3 register
 * 0x20		SDIO_RESP4	SDIO response 4 register
 * 0x24		SDIO_DTIMER	SDIO data timer register
 * 0x28		SDIO_DLEN	SDIO data length register
 * 0x2c		SDIO_DCTRL	SDIO data control register
 * 0x30		SDIO_DCOUNT	SDIO data counter register
 * 0x34		SDIO_STA	SDIO status register
 * 0x38		SDIO_ICR	SDIO interrupt clear register
 * 0x3c		SDIO_MASK	SDIO mask register
 * 0x48		SDIO_FIFOCNT	SDIO FIFO counter register
 * 0x80		SDIO_FIFO	SDIO data FIFO register
 */

#define SDIO_POWER			MMIO32(SDIO_BASE + 0x00)
#define SDIO_CLKCR			MMIO32(SDIO_BASE + 0x04)
#define SDIO_ARG			MMIO32(SDIO_BASE + 0x08)
#define SDIO_CMD			MMIO32(SDIO_BASE + 0x0c)
#define SDIO_RESPCMD			MMIO32(SDIO_BASE + 0x10)
#define SDIO_RESP1			MMIO32(SDIO_BASE + 0x14)
#define SDIO_RESP2			MMIO32(SDIO_BASE + 0x18)
#define SDIO_RESP3			MMIO32(SDIO_BASE + 0x1c)
#define SDIO_RESP4			MMIO32(SDIO_BASE + 0x20)
#define SDIO_DTIMER			MMIO32(SDIO_BASE + 0x24)
#define SDIO_DLEN			MMIO32(SDIO_BASE + 0x28)
#define SDIO_DCTRL			MMIO32(SDIO_BASE + 0x2c)
#define SDIO_DCOUNT			MMIO32(SDIO_BASE + 0x30)
#define SDIO_STA			MMIO32(SDIO_BASE + 0x34)
#define SDIO_ICR			MMIO32(SDIO_BASE + 0x38)
#define SDIO_MASK			MMIO32(SDIO_BASE + 0x3c)
#define SDIO_FIFOCNT			MMIO32(SDIO_BASE + 0x48)
#define SDIO_FIFO			MMIO32(SDIO_BASE + 0x80)

/* --- SDIO_POWER values --------------------------------------------------- */

#define SDIO_POWER_PWRCTRL1		(1 << 1)
#define SDIO_POWER_PWRCTRL0		(1 << 0)

/* Power supply control bits */
#define SDIO_POWER_PWRCTRL_PWROFF	0
#define SDIO_POWER_PWRCTRL_PWRON	3

/* --- SDIO_CLKCR values --------------------------------------------------- */

#define SDIO_CLKCR_HWFC_EN		(1 << 14)
#define SDIO_CLKCR_NEGEDGE		(1 << 13)
#define SDIO_CLKCR_WIDBUS1		(1 << 12)
#define SDIO_CLKCR_WIDBUS0		(1 << 11)
#define SDIO_CLKCR_BYPASS		(1 << 10)
#define SDIO_CLKCR_PWRSAV		(1 << 9)
#define SDIO_CLKCR_CLKEN		(1 << 8)
#define SDIO_CLKCR_CLKDIV7		(1 << 7)
#define SDIO_CLKCR_CLKDIV6		(1 << 6)
#define SDIO_CLKCR_CLKDIV5		(1 << 5)
#define SDIO_CLKCR_CLKDIV4		(1 << 4)
#define SDIO_CLKCR_CLKDIV3		(1 << 3)
#define SDIO_CLKCR_CLKDIV2		(1 << 2)
#define SDIO_CLKCR_CLKDIV1		(1 << 1)
#define SDIO_CLKCR_CLKDIV0		(1 << 0)

/* Wide bus mode enable bit */
#define SDIO_CLKCR_WIDBUS_1		(0 << 11)
#define SDIO_CLKCR_WIDBUS_4		(1 << 11)
#define SDIO_CLKCR_WIDBUS_8		(2 << 11)

/* Clock divide factor */
/* SDIO_CK = SDIOCLK / (CLKDIV + 2) */

/* --- SDIO_ARG values ---------------------------------------------------- */

/* SDIO_ARG[31:0]: CMDARG: Command argument */

/* --- SDIO_CMD values ---------------------------------------------------- */

#define SDIO_CMD_ATACMD			(1 << 14)
#define SDIO_CMD_NIEN			(1 << 13)
#define SDIO_CMD_ENCMDCOMPL		(1 << 12)
#define SDIO_CMD_SDIOSUSPEND		(1 << 11)
#define SDIO_CMD_CPSMEN			(1 << 10)
#define SDIO_CMD_WAITPEND		(1 << 9)
#define SDIO_CMD_WAITINT		(1 << 8)
#define SDIO_CMD_WAITRESP1		(1 << 7)
#define SDIO_CMD_WAITRESP0		(1 << 6)
#define SDIO_CMD_CMDINDEX5		(1 << 5)
#define SDIO_CMD_CMDINDEX4		(1 << 4)
#define SDIO_CMD_CMDINDEX3		(1 << 3)
#define SDIO_CMD_CMDINDEX2		(1 << 2)
#define SDIO_CMD_CMDINDEX1		(1 << 1)
#define SDIO_CMD_CMDINDEX0		(1 << 0)

/* Wait for response bits */
#define SDIO_CMD_WAITRESP_NO		(0 << 6)
#define SDIO_CMD_WAITRESP_SHORT		(1 << 6)
#define SDIO_CMD_WAITRESP_LONG		(3 << 6)

/* --- SDIO_RESPCMD values ------------------------------------------------ */

#define SDIO_RESPCMD_RESPCMD5		(1 << 5)
#define SDIO_RESPCMD_RESPCMD4		(1 << 4)
#define SDIO_RESPCMD_RESPCMD3		(1 << 3)
#define SDIO_RESPCMD_RESPCMD2		(1 << 2)
#define SDIO_RESPCMD_RESPCMD1		(1 << 1)
#define SDIO_RESPCMD_RESPCMD0		(1 << 0)

/* --- SDIO_RESP1 values -------------------------------------------------- */

/* SDIO_RESP1[31:0]: Card Status[31:0](Short response) or Card Status[127:96] */

/* --- SDIO_RESP2 values -------------------------------------------------- */

/* SDIO_RESP2[31:0]: Card Status[95:64] */

/* --- SDIO_RESP3 values -------------------------------------------------- */

/* SDIO_RESP3[31:0]: Card Status[63:32] */

/* --- SDIO_RESP4 values -------------------------------------------------- */

/* SDIO_RESP4[31:1]: Card Status[31:1] */
/* SDIO_RESP4[0]: always 0 */

/* --- SDIO_DTIMER values ------------------------------------------------- */

/* SDIO_DTIMER[31:0]: DATATIME: Data timeout period */

/* --- SDIO_DLEN values --------------------------------------------------- */

/* SDIO_DLEN[24:0]: DATALENGTH: Data length value */

/* --- SDIO_DCTRL values -------------------------------------------------- */

#define SDIO_DCTRL_SDIOEN		(1 << 11)
#define SDIO_DCTRL_RWMOD		(1 << 10)
#define SDIO_DCTRL_RWSTOP		(1 << 9)
#define SDIO_DCTRL_RWSTART		(1 << 8)
#define SDIO_DCTRL_DBLOCKSIZE3		(1 << 7)
#define SDIO_DCTRL_DBLOCKSIZE2		(1 << 6)
#define SDIO_DCTRL_DBLOCKSIZE1		(1 << 5)
#define SDIO_DCTRL_DBLOCKSIZE0		(1 << 4)
#define SDIO_DCTRL_DMAEN		(1 << 3)
#define SDIO_DCTRL_DTMODE		(1 << 2)
#define SDIO_DCTRL_DTDIR		(1 << 1)
#define SDIO_DCTRL_DTEN			(1 << 0)

/* Data block size */
#define SDIO_DCTRL_DBLOCKSIZE_1		(0 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_2		(1 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_4		(2 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_8		(3 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_16	(4 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_32	(5 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_64	(6 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_128	(7 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_256	(8 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_512	(9 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_1024	(10 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_2048	(11 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_4096	(12 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_8192	(13 << 4)
#define SDIO_DCTRL_DBLOCKSIZE_16384	(14 << 4)

/* --- SDIO_DCOUNT values ------------------------------------------------- */

/* SDIO_DCOUNT[24:0]: DATACOUNT: Data count value */

/* --- SDIO_STA values ---------------------------------------------------- */

#define SDIO_STA_CEATAEND		(1 << 23)
#define SDIO_STA_SDIOIT			(1 << 22)
#define SDIO_STA_RXDAVL			(1 << 21)
#define SDIO_STA_TXDAVL			(1 << 20)
#define SDIO_STA_RXFIFOE		(1 << 19)
#define SDIO_STA_TXFIFOE		(1 << 18)
#define SDIO_STA_RXFIFOF		(1 << 17)
#define SDIO_STA_TXFIFOF		(1 << 16)
#define SDIO_STA_RXFIFOHF		(1 << 15)
#define SDIO_STA_TXFIFOHE		(1 << 14)
#define SDIO_STA_RXACT			(1 << 13)
#define SDIO_STA_TXACT			(1 << 12)
#define SDIO_STA_CMDACT			(1 << 11)
#define SDIO_STA_DBCKEND		(1 << 10)
#define SDIO_STA_STBITERR		(1 << 9)
#define SDIO_STA_DATAEND		(1 << 8)
#define SDIO_STA_CMDSENT		(1 << 7)
#define SDIO_STA_CMDREND		(1 << 6)
#define SDIO_STA_RXOVERR		(1 << 5)
#define SDIO_STA_TXUNDERR		(1 << 4)
#define SDIO_STA_DTIMEOUT		(1 << 3)
#define SDIO_STA_CTIMEOUT		(1 << 2)
#define SDIO_STA_DCRCFAIL		(1 << 1)
#define SDIO_STA_CCRCFAIL		(1 << 0)

/* --- SDIO_ICR values ---------------------------------------------------- */

#define SDIO_ICR_CEATAENDC		(1 << 23)
#define SDIO_ICR_SDIOITC		(1 << 22)
#define SDIO_ICR_DBCKENDC		(1 << 10)
#define SDIO_ICR_STBITERRC		(1 << 9)
#define SDIO_ICR_DATAENDC		(1 << 8)
#define SDIO_ICR_CMDSENTC		(1 << 7)
#define SDIO_ICR_CMDRENDC		(1 << 6)
#define SDIO_ICR_RXOVERRC		(1 << 5)
#define SDIO_ICR_TXUNDERRC		(1 << 4)
#define SDIO_ICR_DTIMEOUTC		(1 << 3)
#define SDIO_ICR_CTIMEOUTC		(1 << 2)
#define SDIO_ICR_DCRCFAILC		(1 << 1)
#define SDIO_ICR_CCRCFAILC		(1 << 0)

/* --- SDIO_MASK values --------------------------------------------------- */

#define SDIO_MASK_CEATAENDIE		(1 << 23)
#define SDIO_MASK_SDIOITIE		(1 << 22)
#define SDIO_MASK_RXDAVLIE		(1 << 21)
#define SDIO_MASK_TXDAVLIE		(1 << 20)
#define SDIO_MASK_RXFIFOEIE		(1 << 19)
#define SDIO_MASK_TXFIFOEIE		(1 << 18)
#define SDIO_MASK_RXFIFOFIE		(1 << 17)
#define SDIO_MASK_TXFIFOFIE		(1 << 16)
#define SDIO_MASK_RXFIFOHFIE		(1 << 15)
#define SDIO_MASK_TXFIFOHEIE		(1 << 14)
#define SDIO_MASK_RXACTIE		(1 << 13)
#define SDIO_MASK_TXACTIE		(1 << 12)
#define SDIO_MASK_CMDACTIE		(1 << 11)
#define SDIO_MASK_DBCKENDIE		(1 << 10)
#define SDIO_MASK_STBITERRIE		(1 << 9)
#define SDIO_MASK_DATAENDIE		(1 << 8)
#define SDIO_MASK_CMDSENTIE		(1 << 7)
#define SDIO_MASK_CMDRENDIE		(1 << 6)
#define SDIO_MASK_RXOVERRIE		(1 << 5)
#define SDIO_MASK_TXUNDERRIE		(1 << 4)
#define SDIO_MASK_DTIMEOUTIE		(1 << 3)
#define SDIO_MASK_CTIMEOUTIE		(1 << 2)
#define SDIO_MASK_DCRCFAILIE		(1 << 1)
#define SDIO_MASK_CCRCFAILIE		(1 << 0)

/* --- SDIO_FIFOCNT values ------------------------------------------------- */

/*
 * SDIO_FIFOCNT[23:0]: FIFOCOUNT: Remaining number of words to be written to
 * or read from the FIFO
 */

/* --- SDIO_FIFO values ---------------------------------------------------- */

/* SDIO_FIFO[31:0]: FIFOData: Receive and transmit FIFO data */
