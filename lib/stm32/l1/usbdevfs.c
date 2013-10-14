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

#include <stm32/l1/usbdevfs.h>

void usbdevfs_enable_function(int function)
{
	USB_CNTR |= function;
}

void usbdevfs_disable_function(int function)
{
	USB_CNTR &= ~function;
}

int usbdevfs_get_function(int function)
{
	return USB_CNTR & function;
}

int usbdevfs_get_interrupt_status(int interrupt)
{
	return USB_ISTR & interrupt;
}

void usbdevfs_clear_interrupt(int interrupt)
{
	USB_ISTR &= ~interrupt;
}

u16 usbdevfs_get_frame_number(void)
{
	return USB_FNR;
}

void usbdevfs_set_device_address(u8 addr)
{
	USB_DADDR = USB_DADDR_EF | addr;
}

u8 usbdevfs_get_device_address(void)
{
	return USB_DADDR;
}

void usbdevfs_set_buffer_table_address(u16 offset)
{
	USB_BTABLE = offset;
}

u16 usbdevfs_get_buffer_table_address(void)
{
	return USB_BTABLE;
}

int usbdevfs_assign_packet_memory_tx(int ep_id, int offset, int size)
{
	USB_ADDR_TX(ep_id) = offset;
	return offset + ((size + 1) & ~1);
}

int usbdevfs_assign_packet_memory_tx1(int ep_id, int offset, int size)
{
	USB_ADDR_RX(ep_id) = offset;
	return offset + ((size + 1) & ~1);
}

int usbdevfs_assign_packet_memory_rx(int ep_id, int offset, int size)
{
	USB_ADDR_RX(ep_id) = offset;
	if (size <= 62) {
		USB_COUNT_RX(ep_id) = ((size + 1) / 2) << 10;
		offset += ((size + 1) & ~1);
	} else {
		USB_COUNT_RX(ep_id) = USB_COUNT_RX_BL_SIZE |
			((size + 31) / 32 - 1) << 10;
		offset += ((size + 31) & ~31);
	}

	return offset;
}

int usbdevfs_assign_packet_memory_rx0(int ep_id, int offset, int size)
{
	USB_ADDR_TX(ep_id) = offset;
	if (size <= 62) {
		USB_COUNT_TX(ep_id) = ((size + 1) / 2) << 10;
		offset += ((size + 1) & ~1);
	} else {
		USB_COUNT_TX(ep_id) = USB_COUNT_RX_BL_SIZE |
			((size + 31) / 32 - 1) << 10;
		offset += ((size + 31) & ~31);
	}

	return offset;
}

void usbdevfs_setup_endpoint(int ep_id, usbdevfs_endpoint_t type, u8 address)
{
	USB_EPR(ep_id) = (type << 8) | (address & 0xf);
}

int usbdevfs_get_ep_status(int ep_id, int status)
{
	return USB_EPR(ep_id) & status;
}

void usbdevfs_set_ep_bit(int ep_id, int setbit, int resetbit)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	reg16 ^= (USB_EPR_TOGGLE & setbit);
	reg16 &= ~((USB_EPR_TOGGLE & ~setbit & ~resetbit) |
		   (USB_EPR_C_W0 & resetbit) |
		   (resetbit & ~USB_EPR_TOGGLE & ~USB_EPR_C_W0));
	USB_EPR(ep_id) = (reg16 | (USB_EPR_C_W0 & ~resetbit) |
			  (setbit & ~USB_EPR_TOGGLE & ~USB_EPR_C_W0));
}

int usbdevfs_write(int ep_id, u16 *buf, int len)
{
	u32 *d;
	int i;

	d = (u32 *)(USB_DEV_FS_SRAM_BASE + USB_ADDR_TX(ep_id) * 2);

	for (i = 0; i < len / 2; i++)
		*d++ = *buf++;

	if (len % 2)
		*d = *(u8 *)buf;

	USB_COUNT_TX(ep_id) = len;

	return len;
}

int usbdevfs_write1(int ep_id, u16 *buf, int len)
{
	u32 *d;
	int i;

	d = (u32 *)(USB_DEV_FS_SRAM_BASE + USB_ADDR_RX(ep_id) * 2);

	for (i = 0; i < len / 2; i++)
		*d++ = *buf++;

	if (len % 2)
		*d = *(u8 *)buf;

	USB_COUNT_RX(ep_id) = len;

	return len;
}

int usbdevfs_read(int ep_id, u16 *buf, int buflen)
{
	u32 *s;
	int len;
	int n;
	int i;

	s = (u32 *)(USB_DEV_FS_SRAM_BASE + USB_ADDR_RX(ep_id) * 2);
	len = USB_COUNT_RX(ep_id) & 0x3ff;

	if (buflen < len)
		n = buflen;
	else
		n = len;
	for (i = 0; i < n / 2; i++)
		*buf++ = *s++;

	if (n % 2)
		*(u8 *)buf = *s;

	return len;
}

int usbdevfs_read0(int ep_id, u16 *buf, int buflen)
{
	u32 *s;
	int len;
	int n;
	int i;

	s = (u32 *)(USB_DEV_FS_SRAM_BASE + USB_ADDR_TX(ep_id) * 2);
	len = USB_COUNT_TX(ep_id) & 0x3ff;

	if (buflen < len)
		n = buflen;
	else
		n = len;
	for (i = 0; i < n / 2; i++)
		*buf++ = *s++;

	if (n % 2)
		*(u8 *)buf = *s;

	return len;
}


/* Control endpoint */
void usbdevfs_set_control_state(int ep_id, usbdevfs_control_state_t state)
{
	switch (state) {
	case USBDEVFS_STALL:
		usbdevfs_set_ep_bit(ep_id, USB_EPR_STAT_RX0 | USB_EPR_STAT_TX0,
				    USB_EPR_STAT_RX1 | USB_EPR_STAT_TX1 |
				    USB_EPR_EP_KIND |
				    USB_EPR_CTR_RX | USB_EPR_CTR_TX);
		break;
	case USBDEVFS_DATA_OUT:
		/* RX VALID, TX STALL */
		usbdevfs_set_ep_bit(ep_id, USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0 |
				    USB_EPR_STAT_TX0,
				    USB_EPR_EP_KIND | USB_EPR_STAT_TX1 |
				    USB_EPR_CTR_RX);
		break;
	case USBDEVFS_LAST_DATA_OUT:
		/* RX VALID, TX NAK */
		usbdevfs_set_ep_bit(ep_id, USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0 |
				    USB_EPR_STAT_TX1,
				    USB_EPR_EP_KIND | USB_EPR_STAT_TX0 |
				    USB_EPR_CTR_RX);
		break;
	case USBDEVFS_STATUS_OUT:
		/* RX VALID, STATUS_OUT, TX STALL */
		usbdevfs_set_ep_bit(ep_id,
				    USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0 |
				    USB_EPR_EP_KIND | USB_EPR_STAT_TX0,
				    USB_EPR_STAT_TX1 | USB_EPR_CTR_TX);
		break;
	case USBDEVFS_DATA_IN:
		/* RX STALL, TX VALID */
		usbdevfs_set_ep_bit(ep_id, USB_EPR_STAT_RX0 |
				    USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0,
				    USB_EPR_STAT_RX1 | USB_EPR_EP_KIND |
				    USB_EPR_CTR_RX | USB_EPR_CTR_TX);
		break;
	case USBDEVFS_LAST_DATA_IN:
		/* RX NAK, TX VALID */
		usbdevfs_set_ep_bit(ep_id, USB_EPR_STAT_RX1 |
				    USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0,
				    USB_EPR_STAT_RX0 |
				    USB_EPR_CTR_RX | USB_EPR_CTR_TX);
		break;
	case USBDEVFS_STATUS_IN:
		/* RX STALL, TX VALID */
		usbdevfs_set_ep_bit(ep_id, USB_EPR_STAT_RX0 |
				    USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0,
				    USB_EPR_STAT_RX1 | USB_EPR_CTR_RX);
		break;
	}
}

/* Bulk, interrupt, isochronous endpoint */
void usbdevfs_enable_endpoint_tx(int ep_id)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	if ((reg16 & (USB_EPR_EP_TYPE1 | USB_EPR_EP_TYPE0)) ==
	    USB_EPR_EP_TYPE_ISO) {
		/* TX VALID */
		if ((reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) ==
		    USB_EPR_STAT_TX_DISABLED)
			USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_TX1 |
					  USB_EPR_STAT_TX0);
	} else {
		/* TX NAK */
		switch (reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) {
		case USB_EPR_STAT_TX_DISABLED:
			USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_TX1);
			break;
		case USB_EPR_STAT_TX_STALL:
			USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_TX1 |
					  USB_EPR_STAT_TX0);
			break;
		default:
			break;
		}
	}
}

void usbdevfs_start_endpoint_tx(int ep_id)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	if ((reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) ==
	    USB_EPR_STAT_TX_NAK) {
		reg16 &= ~(USB_EPR_DTOG_TX | USB_EPR_DTOG_RX |
			   USB_EPR_STAT_TX1);
		/* TX VALID */
		USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_TX0);
	}
}

void usbdevfs_enable_endpoint_rx(int ep_id)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	/* RX VALID */
	switch (reg16 & (USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0)) {
	case USB_EPR_STAT_RX_DISABLED:
		USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_RX1 |
					  USB_EPR_STAT_RX0);
		break;
	case USB_EPR_STAT_RX_STALL:
		USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_RX1);
		break;
	case USB_EPR_STAT_RX_NAK:
		reg16 &= ~(USB_EPR_CTR_RX | USB_EPR_DTOG_RX | USB_EPR_DTOG_TX |
			   USB_EPR_STAT_RX1);
		USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_RX0);
		break;
	default:
		break;
	}
}

void usbdevfs_disable_endpoint_tx(int ep_id)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	if ((reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) ==
	    USB_EPR_STAT_TX_DISABLED)
		return;
	reg16 &= ~USB_EPR_CTR_TX; /* Clear interrupt */
	USB_EPR(ep_id) = reg16;
	if ((reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) ==
	    USB_EPR_STAT_TX_STALL ||
	    (reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) ==
	    USB_EPR_STAT_TX_NAK)
		return;
	reg16 = USB_EPR(ep_id);
	if ((reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) !=
	    USB_EPR_STAT_TX_DISABLED)
		USB_EPR(ep_id) = reg16;
}

void usbdevfs_disable_endpoint_rx(int ep_id)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	if ((reg16 & (USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0)) ==
	    USB_EPR_STAT_RX_DISABLED)
		return;
	reg16 &= ~USB_EPR_CTR_RX; /* Clear interrupt */
	USB_EPR(ep_id) = reg16;
	if ((reg16 & (USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0)) ==
	    USB_EPR_STAT_RX_STALL ||
	    (reg16 & (USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0)) ==
	    USB_EPR_STAT_RX_NAK)
		return;
	reg16 = USB_EPR(ep_id);
	if ((reg16 & (USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0)) !=
	    USB_EPR_STAT_RX_DISABLED)
		USB_EPR(ep_id) = reg16;
}

void usbdevfs_halt_endpoint_tx(int ep_id)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	reg16 &= ~USB_EPR_CTR_TX; /* Clear interrupt */
	if ((reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) ==
	    USB_EPR_STAT_TX_NAK) {
		USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0);
		return;
	}
	if ((reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) ==
	    USB_EPR_STAT_TX_VALID) {
		reg16 &= ~USB_EPR_STAT_TX0;
		USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_TX1);
		reg16 = USB_EPR(ep_id);
		if ((reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) ==
		    USB_EPR_STAT_TX_DISABLED)
			USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_TX0);
		else if ((reg16 & (USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0)) ==
			 USB_EPR_STAT_TX_NAK)
			USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_TX1 |
					  USB_EPR_STAT_TX0);
	}
}

void usbdevfs_halt_endpoint_rx(int ep_id)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	reg16 &= ~USB_EPR_CTR_RX; /* Clear interrupt */
	if ((reg16 & (USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0)) ==
	    USB_EPR_STAT_RX_NAK) {
		USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0);
		return;
	}
	if ((reg16 & (USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0)) ==
	    USB_EPR_STAT_RX_VALID) {
		reg16 &= ~USB_EPR_STAT_RX0;
		USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_RX1);
		reg16 = USB_EPR(ep_id);
		if ((reg16 & (USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0)) ==
		    USB_EPR_STAT_RX_DISABLED)
			USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_RX0);
		else if ((reg16 & (USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0)) ==
			 USB_EPR_STAT_RX_NAK)
			USB_EPR(ep_id) = (reg16 | USB_EPR_STAT_RX1 |
					  USB_EPR_STAT_RX0);
	}
}

void usbdevfs_clear_endpoint_interrupt(int ep_id)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	USB_EPR(ep_id) = (reg16 & ~(USB_EPR_CTR_RX | USB_EPR_CTR_TX |
				    USB_EPR_DTOG_RX | USB_EPR_DTOG_TX |
				    USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0 |
				    USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0));
}

void usbdevfs_reset_endpoint_data_toggle(int ep_id)
{
	u16 reg16;

	reg16 = USB_EPR(ep_id);
	USB_EPR(ep_id) = (reg16 & ~(USB_EPR_STAT_RX1 | USB_EPR_STAT_RX0 |
				    USB_EPR_STAT_TX1 | USB_EPR_STAT_TX0));
}
