/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2009 Piotr Esden-Tempski <piotr@esden.net>
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
 * 23 Universal serial bus full-speed device interface (USB)
 *
 *
 * DS6876: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         16KB SRAM, 4KB EEPROM, LCD, USB, ADC, DAC
 * (07-Feb-2013 Rev 8)
 *
 * Table 50. USB startup time
 *
 *
 * DS8890: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 256KB Flash,
 *         32KB SRAM, 8KB EEPROM, LCD, USB, ADC, DAC
 * (01-Feb-2013 Rev 3)
 *
 * Table 51. USB startup time
 *
 *
 * DS8576: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F
 * (01-Feb-2013 Rev 6)
 *
 * Table 60. USB startup time
 *
 *
 * DS8669: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 384KB Flash,
 *         48KB SRAM, 12KB EEPROM, LCD, USB, ADC, DAC, memory I/F, AES
 * (06-Feb-2013 Rev 4)
 *
 * Table 59. USB startup time
 *
 *
 * DS9496: Ultra-low-power 32-bit MCU ARM-based Cortex-M3, 128KB Flash,
 *         10KB SRAM, 2KB EEPROM, LCD, USB, ADC, DAC
 * (21-Feb-2013 Rev 1)
 *
 * Table 48. USB startup time
 */

/* --- USB registers ------------------------------------------------------- */
/*
 * Offset	Register
 * 0x00		USB_EP0R	USB endpont 0 register
 * 0x04		USB_EP1R	USB endpont 1 register
 * 0x08		USB_EP2R	USB endpont 2 register
 * 0x0c		USB_EP3R	USB endpont 3 register
 * 0x10		USB_EP4R	USB endpont 4 register
 * 0x14		USB_EP5R	USB endpont 5 register
 * 0x18		USB_EP6R	USB endpont 6 register
 * 0x1c		USB_EP7R	USB endpont 7 register
 * 0x40		USB_CNTR	USB control register
 * 0x44		USB_ISTR	USB interrupt status register
 * 0x48		USB_FNR		USB frame number register
 * 0x4c		USB_DADDR	USB device address
 * 0x50		USB_BTABLE	Buffer table address
 */

/* USB_EPnR */
#define USB_EPR(n)			MMIO32(USB_DEV_FS_BASE + (n) * 4)
#define USB_EP0R			USB_EPR(0)
#define USB_EP1R			USB_EPR(1)
#define USB_EP2R			USB_EPR(2)
#define USB_EP3R			USB_EPR(3)
#define USB_EP4R			USB_EPR(4)
#define USB_EP5R			USB_EPR(5)
#define USB_EP6R			USB_EPR(6)
#define USB_EP7R			USB_EPR(7)

/* Common registers */
#define USB_CNTR			MMIO32(USB_DEV_FS_BASE + 0x40)
#define USB_ISTR			MMIO32(USB_DEV_FS_BASE + 0x44)
#define USB_FNR				MMIO32(USB_DEV_FS_BASE + 0x48)
#define USB_DADDR			MMIO32(USB_DEV_FS_BASE + 0x4C)
#define USB_BTABLE			MMIO32(USB_DEV_FS_BASE + 0x50)

/* Transmission buffer address n */
#define USB_ADDR_TX(n)			MMIO32(USB_DEV_FS_SRAM_BASE + \
					       (USB_BTABLE + (n) * 8 + 0) * 2)
#define USB_ADDR0_TX			USB_ADDR_TX(0)
#define USB_ADDR1_TX			USB_ADDR_TX(1)
#define USB_ADDR2_TX			USB_ADDR_TX(2)
#define USB_ADDR3_TX			USB_ADDR_TX(3)
#define USB_ADDR4_TX			USB_ADDR_TX(4)
#define USB_ADDR5_TX			USB_ADDR_TX(5)
#define USB_ADDR6_TX			USB_ADDR_TX(6)
#define USB_ADDR7_TX			USB_ADDR_TX(7)

/* Transmission byte count n */
#define USB_COUNT_TX(n)			MMIO32(USB_DEV_FS_SRAM_BASE + \
					       (USB_BTABLE + (n) * 8 + 2) * 2)
#define USB_COUNT0_TX			USB_COUNT_TX(0)
#define USB_COUNT1_TX			USB_COUNT_TX(1)
#define USB_COUNT2_TX			USB_COUNT_TX(2)
#define USB_COUNT3_TX			USB_COUNT_TX(3)
#define USB_COUNT4_TX			USB_COUNT_TX(4)
#define USB_COUNT5_TX			USB_COUNT_TX(5)
#define USB_COUNT6_TX			USB_COUNT_TX(6)
#define USB_COUNT7_TX			USB_COUNT_TX(7)

/* Reception buffer address n */
#define USB_ADDR_RX(n)			MMIO32(USB_DEV_FS_SRAM_BASE + \
					       (USB_BTABLE + (n) * 8 + 4) * 2)
#define USB_ADDR0_RX			USB_ADDR_RX(0)
#define USB_ADDR1_RX			USB_ADDR_RX(1)
#define USB_ADDR2_RX			USB_ADDR_RX(2)
#define USB_ADDR3_RX			USB_ADDR_RX(3)
#define USB_ADDR4_RX			USB_ADDR_RX(4)
#define USB_ADDR5_RX			USB_ADDR_RX(5)
#define USB_ADDR6_RX			USB_ADDR_RX(6)
#define USB_ADDR7_RX			USB_ADDR_RX(7)

/* Reception byte count n */
#define USB_COUNT_RX(n)			MMIO32(USB_DEV_FS_SRAM_BASE + \
					       (USB_BTABLE + (n) * 8 + 6) * 2)
#define USB_COUNT0_RX			USB_COUNT_RX(0)
#define USB_COUNT1_RX			USB_COUNT_RX(1)
#define USB_COUNT2_RX			USB_COUNT_RX(2)
#define USB_COUNT3_RX			USB_COUNT_RX(3)
#define USB_COUNT4_RX			USB_COUNT_RX(4)
#define USB_COUNT5_RX			USB_COUNT_RX(5)
#define USB_COUNT6_RX			USB_COUNT_RX(6)
#define USB_COUNT7_RX			USB_COUNT_RX(7)

/* --- Common registers ---------------------------------------------------- */

/* --- USB_CNTR values ----------------------------------------------------- */

#define USB_CNTR_CTRM			(1 << 15)
#define USB_CNTR_PMAOVRM		(1 << 14)
#define USB_CNTR_ERRM			(1 << 13)
#define USB_CNTR_WKUPM			(1 << 12)
#define USB_CNTR_SUSPM			(1 << 11)
#define USB_CNTR_RESETM			(1 << 10)
#define USB_CNTR_SOFM			(1 << 9)
#define USB_CNTR_ESOFM			(1 << 8)
#define USB_CNTR_RESUME			(1 << 4)
#define USB_CNTR_FSUSP			(1 << 3)
#define USB_CNTR_LP_MODE		(1 << 2)
#define USB_CNTR_PDWN			(1 << 1)
#define USB_CNTR_FRES			(1 << 0)

/* --- USB_ISTR values ----------------------------------------------------- */

#define USB_ISTR_CTR			(1 << 15)
#define USB_ISTR_PMAOVR			(1 << 14)
#define USB_ISTR_ERR			(1 << 13)
#define USB_ISTR_WKUP			(1 << 12)
#define USB_ISTR_SUSP			(1 << 11)
#define USB_ISTR_RESET			(1 << 10)
#define USB_ISTR_SOF			(1 << 9)
#define USB_ISTR_ESOF			(1 << 8)
#define USB_ISTR_DIR			(1 << 4)
#define USB_ISTR_EP_ID3			(1 << 3)
#define USB_ISTR_EP_ID2			(1 << 2)
#define USB_ISTR_EP_ID1			(1 << 1)
#define USB_ISTR_EP_ID0			(1 << 0)

#define USB_ISTR_EP_ID_MASK		(15 << 0)

/* --- USB_FNR values ------------------------------------------------------ */

#define USB_FNR_RXDP			(1 << 15)
#define USB_FNR_RXDM			(1 << 14)
#define USB_FNR_LCK			(1 << 13)
#define USB_FNR_LSOF1			(1 << 12)
#define USB_FNR_LSOF0			(1 << 11)
#define USB_FNR_FN10			(1 << 10)
#define USB_FNR_FN9			(1 << 9)
#define USB_FNR_FN8			(1 << 8)
#define USB_FNR_FN7			(1 << 7)
#define USB_FNR_FN6			(1 << 6)
#define USB_FNR_FN5			(1 << 5)
#define USB_FNR_FN4			(1 << 4)
#define USB_FNR_FN3			(1 << 3)
#define USB_FNR_FN2			(1 << 2)
#define USB_FNR_FN1			(1 << 1)
#define USB_FNR_FN0			(1 << 0)

/* --- USB_DADDR values ---------------------------------------------------- */

#define USB_DADDR_EF			(1 << 7)
#define USB_DADDR_ADD6			(1 << 6)
#define USB_DADDR_ADD5			(1 << 5)
#define USB_DADDR_ADD4			(1 << 4)
#define USB_DADDR_ADD3			(1 << 3)
#define USB_DADDR_ADD2			(1 << 2)
#define USB_DADDR_ADD1			(1 << 1)
#define USB_DADDR_ADD0			(1 << 0)

/* --- USB_BTABLE values --------------------------------------------------- */

/* USB_BTABLE[15:3]: BTABLE[15:3]: Buffer table */

/* --- Endpoint-specific registers ----------------------------------------- */

/* --- USB_EPnR values ----------------------------------------------------- */

#define USB_EPR_CTR_RX			(1 << 15)
#define USB_EPR_DTOG_RX			(1 << 14)
#define USB_EPR_STAT_RX1		(1 << 13)
#define USB_EPR_STAT_RX0		(1 << 12)
#define USB_EPR_SETUP			(1 << 11)
#define USB_EPR_EP_TYPE1		(1 << 10)
#define USB_EPR_EP_TYPE0		(1 << 9)
#define USB_EPR_EP_KIND			(1 << 8)
#define USB_EPR_CTR_TX			(1 << 7)
#define USB_EPR_DTOG_TX			(1 << 6)
#define USB_EPR_STAT_TX1		(1 << 5)
#define USB_EPR_STAT_TX0		(1 << 4)
#define USB_EPR_EA3			(1 << 3)
#define USB_EPR_EA2			(1 << 2)
#define USB_EPR_EA1			(1 << 1)
#define USB_EPR_EA0			(1 << 0)

/* Reception status */
#define USB_EPR_STAT_RX_DISABLED	(0 << 12)
#define USB_EPR_STAT_RX_STALL		(1 << 12)
#define USB_EPR_STAT_RX_NAK		(2 << 12)
#define USB_EPR_STAT_RX_VALID		(3 << 12)

/* Endpoint type */
#define USB_EPR_EP_TYPE_BULK		(0 << 9)
#define USB_EPR_EP_TYPE_CONTROL		(1 << 9)
#define USB_EPR_EP_TYPE_ISO		(2 << 9)
#define USB_EPR_EP_TYPE_INTERRUPT	(3 << 9)
#define USB_EPR_EP_TYPE_MASK		(3 << 9)

/* Endpoint Address */
#define USB_EPR_EA_MASK			(15 << 0)

/* Transmission status */
#define USB_EPR_STAT_TX_DISABLED	(0 << 4)
#define USB_EPR_STAT_TX_STALL		(1 << 4)
#define USB_EPR_STAT_TX_NAK		(2 << 4)
#define USB_EPR_STAT_TX_VALID		(3 << 4)

#define USB_EPR_TOGGLE			(USB_EPR_DTOG_RX | USB_EPR_STAT_RX1 | \
					 USB_EPR_STAT_RX0 | USB_EPR_DTOG_TX | \
					 USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)

#define USB_EPR_C_W0			(USB_EPR_CTR_RX | USB_EPR_CTR_TX)

/* --- USB_ADDRn_TX values ------------------------------------------------- */

/* ADDRn_TX[15:1]: Transmission buffer address */
/* ADDRn_TX[0]: 0 */

/* --- USB_COUNTn_TX values ------------------------------------------------ */

/* COUNTn_TX[9:0]: transmission byte count */

/* --- USB_ADDRn_RX values ------------------------------------------------- */

/* ADDRn_RX[15:1]: Reception buffer address */
/* ADDRn_RX[0]: 0 */

/* --- USB_COUNTn_RX values ------------------------------------------------ */

#define USB_COUNT_RX_BL_SIZE		(1 << 15)
/* USB_COUNTn_RX[14:10]: NUM_BLOCK[4:0]: Number of blocks */
/* USB_COUNTn_RX[9:0]: COUNTn_RX[9:0]: Reception byte count */

/* --- USB transceiver startup time ---------------------------------------- */

#define USBDEVFS_T_STARTUP		1	/* 1 usec */

/* --- Function prototypes ------------------------------------------------- */

#define USBDEVFS_EP_MAX			8

/* Buffer descriptor table */
#define USBDEVFS_BUFFER_TABLE_SIZE	(2 * 4 * USBDEVFS_EP_MAX)

/* Alias */
#define usbdevfs_enable_interrupt usbdevfs_enable_function
#define usbdevfs_disable_interrupt usbdevfs_disable_function
#define usbdevfs_get_interrupt_mask usbdevfs_get_function
#define usbdevfs_assign_packet_memory_tx0 usbdevfs_assign_packet_memory_tx
#define usbdevfs_assign_packet_memory_rx1 usbdevfs_assign_packet_memory_rx
#define usbdevfs_write0 usbdevfs_write
#define usbdevfs_read1 usbdevfs_read

/* Function */
enum {
	USBDEVFS_FORCE_RESET = (1 << 0),
	USBDEVFS_POWER_DOWN = (1 << 1),
	USBDEVFS_LOW_POWER_MODE = (1 << 2),
	USBDEVFS_FORCE_SUSPEND = (1 << 3),
	USBDEVFS_RESUME = (1 << 4)
};

/* Interrupt */
enum {
	USBDEVFS_EXPECTED_SOF = (1 << 8),
	USBDEVFS_SOF = (1 << 9),
	USBDEVFS_RESET = (1 << 10),
	USBDEVFS_SUSPEND = (1 << 11),
	USBDEVFS_WAKEUP = (1 << 12),
	USBDEVFS_ERROR = (1 << 13),
	USBDEVFS_OVERRUN = (1 << 14),
	USBDEVFS_CORRECT_TRANSFER = (1 << 15),
	USBDEVFS_ALL_INTERRUPT = 0xff00
};

/* Status */
enum {
	USBDEVFS_DIR = (1 << 4),
	USBDEVFS_EP_ID = (15 << 0)
};

/* Endpoint status */
enum {
	USBDEVFS_TX_DATA1 = (1 << 6),
	USBDEVFS_TX = (1 << 7),
	USBDEVFS_SETUP = (1 << 11),
	USBDEVFS_RX_DATA1 = (1 << 14),
	USBDEVFS_RX = (1 << 15)
};

/* Control transfer state */
typedef enum {
	USBDEVFS_STALL,
	USBDEVFS_DATA_IN,
	USBDEVFS_DATA_OUT,
	USBDEVFS_LAST_DATA_IN,
	USBDEVFS_LAST_DATA_OUT,
	USBDEVFS_STATUS_IN,
	USBDEVFS_STATUS_OUT
} usbdevfs_control_state_t;

/* Endpoint type */
typedef enum {
	USBDEVFS_BULK = 0,
	USBDEVFS_BULK_DBL_BUF = 1,
	USBDEVFS_CONTROL = 2,
	USBDEVFS_ISOCHRONOUS = 4,
	USBDEVFS_INTERRUPT = 6
} usbdevfs_endpoint_t;

void usbdevfs_enable_function(int function);
void usbdevfs_disable_function(int function);
int usbdevfs_get_function(int function);
int usbdevfs_get_interrupt_status(int interrupt);
void usbdevfs_clear_interrupt(int interrupt);
u16 usbdevfs_get_frame_number(void);
void usbdevfs_set_device_address(u8 addr);
u8 usbdevfs_get_device_address(void);
void usbdevfs_set_buffer_table_address(u16 offset);
u16 usbdevfs_get_buffer_table_address(void);
int usbdevfs_assign_packet_memory_tx(int ep_id, int offset, int size);
int usbdevfs_assign_packet_memory_tx1(int ep_id, int offset, int size);
int usbdevfs_assign_packet_memory_rx(int ep_id, int offset, int size);
int usbdevfs_assign_packet_memory_rx0(int ep_id, int offset, int size);
void usbdevfs_setup_endpoint(int ep_id, usbdevfs_endpoint_t type, u8 address);
int usbdevfs_get_ep_status(int ep_id, int status);
void usbdevfs_set_ep_bit(int ep_id, int setbit, int resetbit);
int usbdevfs_write(int ep_id, u16 *buf, int len);
int usbdevfs_write1(int ep_id, u16 *buf, int len);
int usbdevfs_read(int ep_id, u16 *buf, int buflen);
int usbdevfs_read0(int ep_id, u16 *buf, int buflen);

void usbdevfs_set_control_state(int ep_id, usbdevfs_control_state_t state);

void usbdevfs_enable_endpoint_tx(int ep_id);
void usbdevfs_start_endpoint_tx(int ep_id);
void usbdevfs_enable_endpoint_rx(int ep_id);
void usbdevfs_disable_endpoint_tx(int ep_id);
void usbdevfs_disable_endpoint_rx(int ep_id);
void usbdevfs_halt_endpoint_tx(int ep_id);
void usbdevfs_halt_endpoint_rx(int ep_id);
void usbdevfs_clear_endpoint_interrupt(int ep_id);
void usbdevfs_reset_endpoint_data_toggle(int ep_id);

