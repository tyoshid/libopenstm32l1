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

#include <stm32/l1/memorymap.h>
#include <libopencm3.h>

/*
 * STMicroelectronics(www.st.com)
 *
 * RM0038: STM32L100xx, STM32L151xx, STM32L152xx and STM32L162xx
 *         advanced ARM-based 32-bit MCUs
 * (19-Apr-2013 Rev 8)
 *
 * 25 Inter-integrated circuit (I2C) interface
 */

/* --- I2C registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		I2C_CR1		I2C Control register 1
 * 0x04		I2C_CR2		I2C Control register 2
 * 0x08		I2C_OAR1	I2C Own address register 1
 * 0x0c		I2C_OAR2	I2C Own address register 2
 * 0x10		I2C_DR		I2C Data register
 * 0x14		I2C_SR1		I2C Status register 1
 * 0x18		I2C_SR2		I2C Status register 2
 * 0x1c		I2C_CCR		I2C Clock control register
 * 0x20		I2C_TRISE	I2C TRISE register
 */

/* I2Cx_CR1 */
#define I2C_CR1(base)			MMIO32((base) + 0x00)
#define I2C1_CR1			I2C_CR1(I2C1_BASE)
#define I2C2_CR1			I2C_CR1(I2C2_BASE)

/* I2Cx_CR2 */
#define I2C_CR2(base)			MMIO32((base) + 0x04)
#define I2C1_CR2			I2C_CR2(I2C1_BASE)
#define I2C2_CR2			I2C_CR2(I2C2_BASE)

/* I2Cx_OAR1 */
#define I2C_OAR1(base)			MMIO32((base) + 0x08)
#define I2C1_OAR1			I2C_OAR1(I2C1_BASE)
#define I2C2_OAR1			I2C_OAR1(I2C2_BASE)

/* I2Cx_OAR2 */
#define I2C_OAR2(base)			MMIO32((base) + 0x0c)
#define I2C1_OAR2			I2C_OAR2(I2C1_BASE)
#define I2C2_OAR2			I2C_OAR2(I2C2_BASE)

/* I2Cx_DR */
#define I2C_DR(base)			MMIO32((base) + 0x10)
#define I2C1_DR				I2C_DR(I2C1_BASE)
#define I2C2_DR				I2C_DR(I2C2_BASE)

/* I2Cx_SR1 */
#define I2C_SR1(base)			MMIO32((base) + 0x14)
#define I2C1_SR1			I2C_SR1(I2C1_BASE)
#define I2C2_SR1			I2C_SR1(I2C2_BASE)

/* I2Cx_SR2 */
#define I2C_SR2(base)			MMIO32((base) + 0x18)
#define I2C1_SR2			I2C_SR2(I2C1_BASE)
#define I2C2_SR2			I2C_SR2(I2C2_BASE)

/* I2Cx_CCR */
#define I2C_CCR(base)			MMIO32((base) + 0x1c)
#define I2C1_CCR			I2C_CCR(I2C1_BASE)
#define I2C2_CCR			I2C_CCR(I2C2_BASE)

/* I2Cx_TRISE */
#define I2C_TRISE(base)			MMIO32((base) + 0x20)
#define I2C1_TRISE			I2C_TRISE(I2C1_BASE)
#define I2C2_TRISE			I2C_TRISE(I2C2_BASE)

/* --- I2C_CR1 values ------------------------------------------------------ */

#define I2C_CR1_SWRST			(1 << 15)
#define I2C_CR1_ALERT			(1 << 13)
#define I2C_CR1_PEC			(1 << 12)
#define I2C_CR1_POS			(1 << 11)
#define I2C_CR1_ACK			(1 << 10)
#define I2C_CR1_STOP			(1 << 9)
#define I2C_CR1_START			(1 << 8)
#define I2C_CR1_NOSTRETCH		(1 << 7)
#define I2C_CR1_ENGC			(1 << 6)
#define I2C_CR1_ENPEC			(1 << 5)
#define I2C_CR1_ENARP			(1 << 4)
#define I2C_CR1_SMBTYPE			(1 << 3)
#define I2C_CR1_SMBUS			(1 << 1)
#define I2C_CR1_PE			(1 << 0)

/* --- I2C_CR2 values ------------------------------------------------------ */

#define I2C_CR2_LAST			(1 << 12)
#define I2C_CR2_DMAEN			(1 << 11)
#define I2C_CR2_ITBUFEN			(1 << 10)
#define I2C_CR2_ITEVTEN			(1 << 9)
#define I2C_CR2_ITERREN			(1 << 8)
#define I2C_CR2_FREQ5			(1 << 5)
#define I2C_CR2_FREQ4			(1 << 4)
#define I2C_CR2_FREQ3			(1 << 3)
#define I2C_CR2_FREQ2			(1 << 2)
#define I2C_CR2_FREQ1			(1 << 1)
#define I2C_CR2_FREQ0			(1 << 0)

/* Peripheral clock frequency (valid values: 2-32 MHz) */
#define I2C_CR2_FREQ_2MHZ		2
#define I2C_CR2_FREQ_3MHZ		3
#define I2C_CR2_FREQ_4MHZ		4
#define I2C_CR2_FREQ_5MHZ		5
#define I2C_CR2_FREQ_6MHZ		6
#define I2C_CR2_FREQ_7MHZ		7
#define I2C_CR2_FREQ_8MHZ		8
#define I2C_CR2_FREQ_9MHZ		9
#define I2C_CR2_FREQ_10MHZ		10
#define I2C_CR2_FREQ_11MHZ		11
#define I2C_CR2_FREQ_12MHZ		12
#define I2C_CR2_FREQ_13MHZ		13
#define I2C_CR2_FREQ_14MHZ		14
#define I2C_CR2_FREQ_15MHZ		15
#define I2C_CR2_FREQ_16MHZ		16
#define I2C_CR2_FREQ_17MHZ		17
#define I2C_CR2_FREQ_18MHZ		18
#define I2C_CR2_FREQ_19MHZ		19
#define I2C_CR2_FREQ_20MHZ		20
#define I2C_CR2_FREQ_21MHZ		21
#define I2C_CR2_FREQ_22MHZ		22
#define I2C_CR2_FREQ_23MHZ		23
#define I2C_CR2_FREQ_24MHZ		24
#define I2C_CR2_FREQ_25MHZ		25
#define I2C_CR2_FREQ_26MHZ		26
#define I2C_CR2_FREQ_27MHZ		27
#define I2C_CR2_FREQ_28MHZ		28
#define I2C_CR2_FREQ_29MHZ		29
#define I2C_CR2_FREQ_30MHZ		30
#define I2C_CR2_FREQ_31MHZ		31
#define I2C_CR2_FREQ_32MHZ		32

/* --- I2C_OAR1 values ----------------------------------------------------- */

#define I2C_OAR1_ADDMODE		(1 << 15)
#define I2C_OAR1_BIT14			(1 << 14)
/* Note: Bit 14 should always be kept at 1 by software! */
/* ADD[9:8]: Interface address (10-bit addressing mode) */
/* ADD[7:1]: Interface address */
/* ADD[0]: Interface address (10-bit addressing mode) */

/* --- I2C_OAR2 values ----------------------------------------------------- */

/* ADD2[7:1]: Interface address */
#define I2C_OAR2_ENDUAL			(1 << 0)

/* --- I2C_DR values ------------------------------------------------------- */

/* DR[7:0] 8-bit data register */

/* --- I2C_SR1 values ------------------------------------------------------ */

#define I2C_SR1_SMBALERT		(1 << 15)
#define I2C_SR1_TIMEOUT			(1 << 14)
#define I2C_SR1_PECERR			(1 << 12)
#define I2C_SR1_OVR			(1 << 11)
#define I2C_SR1_AF			(1 << 10)
#define I2C_SR1_ARLO			(1 << 9)
#define I2C_SR1_BERR			(1 << 8)
#define I2C_SR1_TXE			(1 << 7)
#define I2C_SR1_RXNE			(1 << 6)
#define I2C_SR1_STOPF			(1 << 4)
#define I2C_SR1_ADD10			(1 << 3)
#define I2C_SR1_BTF			(1 << 2)
#define I2C_SR1_ADDR			(1 << 1)
#define I2C_SR1_SB			(1 << 0)

#define I2C_SR1_ERROR			(I2C_SR1_SMBALERT | I2C_SR1_TIMEOUT | \
					 I2C_SR1_PECERR | I2C_SR1_OVR | \
					 I2C_SR1_AF | I2C_SR1_ARLO | \
					 I2C_SR1_BERR)

/* --- I2C_SR2 values ------------------------------------------------------ */

/* I2C_SR2[15:8]: PEC[7:0]: Packet error checking register */
#define I2C_SR2_DUALF			(1 << 7)
#define I2C_SR2_SMBHOST			(1 << 6)
#define I2C_SR2_SMBDEFAULT		(1 << 5)
#define I2C_SR2_GENCALL			(1 << 4)
#define I2C_SR2_TRA			(1 << 2)
#define I2C_SR2_BUSY			(1 << 1)
#define I2C_SR2_MSL			(1 << 0)

/* --- I2C_CCR values ------------------------------------------------------ */

#define I2C_CCR_F_S			(1 << 15)
#define I2C_CCR_DUTY			(1 << 14)
/* CCR[11:0]: Clock control register in Fast/Standard mode (Master mode) */

/* --- I2C_TRISE values ---------------------------------------------------- */

/* TRISE[5:0]: Maximum rise time in Fast/Standard mode (Master mode) */

/* --- Funtion prototypes -------------------------------------------------- */

/* Device number */
typedef enum {
	I2C1,
	I2C2
} i2c_t;

/* Master mode */
typedef enum {
	I2C_STANDARD,
	I2C_FAST,
	I2C_FAST_DUTY
} i2c_mode_t;

/* Interrupt */
enum {
	I2C_TXE = (1 << 7),
	I2C_RXNE = (1 << 6),

	I2C_BUFFER = (3 << 6),

	I2C_START = (1 << 0),
	I2C_ADDR = (1 << 1),
	I2C_ADD10 = (1 << 3),
	I2C_BTF = (1 << 2),
	I2C_STOP = (1 << 4),

	I2C_EVENT = (31 << 0),

	I2C_BERR = (1 << 8),
	I2C_ARLO = (1 << 9),
	I2C_AF = (1 << 10),
	I2C_OVR = (1 << 11),
	I2C_PECERR = (1 << 12),
	I2C_TIMEOUT = (1 << 14),
	I2C_SMBALERT = (1 << 15),

	I2C_ERROR = (223 << 8)
};

/* Status */
enum {
	I2C_MASTER = (1 << 0),
	I2C_BUSY = (1 << 1),
	I2C_TRANSMIT = (1 << 2),
	I2C_GENCALL = (1 << 4),
	I2C_SMBDEFAULT = (1 << 5),
	I2C_SMBHOST = (1 << 6),
	I2C_OAR2 = (1 << 7)
};

/* Action */
enum {
	I2C_RESET = (1 << 14),
 	I2C_PEC = (1 << 12),
	I2C_POS = (1 << 11),
	I2C_ACK = (1 << 10)
/*
	I2C_SMBALERT,
	I2C_START,
	I2C_STOP,
*/
};

/* Bus mode */
enum {
	I2C_ENABLE = (1 << 0),
	I2C_SMBUS = (1 << 1),
	I2C_SMBUS_HOST = (1 << 3),
	I2C_ARP = (1 << 2),
	I2C_NOSTRETCH = (1 << 7)
/*
	I2C_PEC,
	I2C_GENCALL,
*/
};

/* Read/Write */
enum {
	I2C_WRITE,
	I2C_READ
};

void i2c_set_clock(i2c_t i2c, int apb1, i2c_mode_t mode, int scl, int t_r_max);
void i2c_set_bus_mode(i2c_t i2c, int mode);
void i2c_enable_action(i2c_t i2c, int action);
void i2c_disable_action(i2c_t i2c, int action);
int i2c_get_action_status(i2c_t i2c, int action);
void i2c_enable_interrupt(i2c_t i2c, int interrupt);
void i2c_disable_interrupt(i2c_t i2c, int interrupt);
int i2c_get_interrupt_mask(i2c_t i2c, int interrupt);
int i2c_get_interrupt_status(i2c_t i2c, int interrupt);
void i2c_clear_interrupt(i2c_t i2c, int interrupt);
int i2c_get_status(i2c_t i2c, int status);
void i2c_set_own_address(i2c_t i2c, u16 slave);
void i2c_set_own_address2(i2c_t i2c, u8 slave);
void i2c_put_data(i2c_t i2c, u8 data);
u8 i2c_get_data(i2c_t i2c);

/* Error */
enum {
	I2C_ERROR_STATUS = 1,
	I2C_ERROR_TIMEOUT
};

int i2c_software_reset(i2c_t i2c, int timeout);
int i2c_start(i2c_t i2c, int timeout);
int i2c_addr(i2c_t i2c, u8 sla);
int i2c_write(i2c_t i2c, u8 *buf, int nbyte);
int i2c_read(i2c_t i2c, u8 *buf, int nbyte);
int i2c_stop(i2c_t i2c, int timeout);
int i2c_wait_previous_action(i2c_t i2c, int timeout);
int i2c_transfer(i2c_t i2c, u8 sla, u8 *sbuf, int sbyte, u8 *rbuf, int rbyte,
		 int timeout);
