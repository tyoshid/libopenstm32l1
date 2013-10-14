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

#include <stm32/l1/ri.h>

struct port_cr {
	int reg;
	u32 bit;
};

static struct port_cr pa_cr[16] = {
	{1, (1 << 0)},		/* CH0/GR1_1 */
	{1, (1 << 1)},		/* CH1/GR1_2 */
	{1, (1 << 2)},		/* CH2/GR1_3 */
	{1, (1 << 3)},		/* CH3/GR1_4 */
	{1, (1 << 4)},		/* CH4 */
	{1, (1 << 5)},		/* CH5 */
	{1, (1 << 6)},		/* CH6/GR2_1 */
	{1, (1 << 7)},		/* CH7/GR2_2 */
	{2, (1 << 9)},		/* GR4_1 */
	{2, (1 << 10)},		/* GR4_2 */
	{2, (1 << 11)},		/* GR4_3 */
	{2, (1 << 15)},		/* GR4_4 */
	{0, 0},
	{2, (1 << 6)},		/* GR5_1 */
	{2, (1 << 7)},		/* GR5_2 */
	{2, (1 << 8)}		/* GR5_3 */
};

static struct port_cr pb_cr[16] = {
	{1, (1 << 8)},		/* CH8/GR3_1 */
	{1, (1 << 9)},		/* CH9/GR3_2 */
	{2, (1 << 16)},		/* CH9b/GR3_3 */
	{0, 0},
	{2, (1 << 4)},		/* GR6_1 */
	{2, (1 << 5)},		/* GR6_2 */
	{2, (1 << 27)},		/* GR6_3 */
	{2, (1 << 28)},		/* GR6_4 */
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{1, (1 << 18)},		/* CH18/GR7_1 */
	{1, (1 << 19)},		/* CH19/GR7_2 */
	{1, (1 << 20)},		/* CH20/GR7_3 */
	{1, (1 << 21)}		/* CH21/GR7_4 */
};

static struct port_cr pc_cr[16] = {
	{1, (1 << 10)},		/* CH10/GR8_1 */
	{1, (1 << 11)},		/* CH11/GR8_2 */
	{1, (1 << 12)},		/* CH12/GR8_3 */
	{1, (1 << 13)},		/* CH13/GR8_4 */
	{1, (1 << 14)},		/* CH14/GR9_1 */
	{1, (1 << 15)},		/* CH15/GR9_2 */
	{2, (1 << 0)},		/* GR10_1 */
	{2, (1 << 1)},		/* GR10_2 */
	{2, (1 << 2)},		/* GR10_3 */
	{2, (1 << 3)},		/* GR10_4 */
	{2, (1 << 29)},		/* GR5_4 */
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0}
};

static struct port_cr pe_cr[16] = {
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{1, (1 << 22)},		/* CH22 */
	{1, (1 << 23)},		/* CH23 */
	{1, (1 << 24)},		/* CH24 */
	{1, (1 << 25)},		/* CH25 */
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0}
};

static struct port_cr pf_cr[16] = {
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{1, (1 << 27)},		/* CH27/GR11_1 */
	{1, (1 << 28)},		/* CH28/GR8_2 */
	{1, (1 << 29)},		/* CH29/GR8_3 */
	{1, (1 << 30)},		/* CH30/GR8_4 */
	{1, (1 << 16)},		/* CH31/GR9_1 */
	{2, (1 << 17)},		/* CH1b/GR3_4 */
	{2, (1 << 18)},		/* CH2b/GR3_5 */
	{2, (1 << 19)},		/* CH3b/GR9_3 */
	{2, (1 << 20)},		/* CH6b/GR9_4 */
	{2, (1 << 21)}		/* CH7b/GR2_3 */
};

static struct port_cr pg_cr[16] = {
	{2, (1 << 22)},		/* CH8b/GR2_4 */
	{2, (1 << 23)},		/* CH9b/GR2_5 */
	{2, (1 << 24)},		/* CH10b/GR7_5 */
	{2, (1 << 25)},		/* CH11b/GR7_6 */
	{2, (1 << 26)},		/* CH12b/GR7_7 */
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0}
};

static struct port_cr other_cr[16] = {
	{1, (1 << 26)},		/* VCOMP */
	{1, (1 << 31)},		/* ADC */
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0},
	{0, 0}
};

void ri_set_tim_ic(ri_tim_t tim, int ic1, int ic2, int ic3, int ic4)
{
	u32 reg32;
	int i;

	static int ic1_port[16] = {RI_PA0, RI_PA4, RI_PA8, RI_PA12,
				   RI_PC0, RI_PC4, RI_PC8, RI_PC12,
				   RI_PD0, RI_PD4, RI_PD8, RI_PD12,
				   RI_PE0, RI_PE4, RI_PE8, RI_PE12};
	static int ic2_port[16] = {RI_PA1, RI_PA5, RI_PA9, RI_PA13,
				   RI_PC1, RI_PC5, RI_PC9, RI_PC13,
				   RI_PD1, RI_PD5, RI_PD9, RI_PD13,
				   RI_PE1, RI_PE5, RI_PE9, RI_PE13};
	static int ic3_port[16] = {RI_PA2, RI_PA6, RI_PA10, RI_PA14,
				   RI_PC2, RI_PC6, RI_PC10, RI_PC14,
				   RI_PD2, RI_PD6, RI_PD10, RI_PD14,
				   RI_PE2, RI_PE6, RI_PE10, RI_PE14};
	static int ic4_port[16] = {RI_PA3, RI_PA7, RI_PA11, RI_PA15,
				   RI_PC3, RI_PC7, RI_PC11, RI_PC15,
				   RI_PD3, RI_PD7, RI_PD11, RI_PD15,
				   RI_PE3, RI_PE7, RI_PE11, RI_PE15};

	reg32 = RI_ICR;
	reg32 &= ~(3 << 16);
	reg32 |= (tim << 16);
	if (tim == RI_TIM_NONE) {
		RI_ICR = reg32;
		return;
	}

	reg32 &= ~RI_ICR_IC1;
	if (ic1) {
		for (i = 0; i < 16; i++) {
			if (ic1 == ic1_port[i]) {
				reg32 &= ~(15 << 0);
				reg32 |= ((i << 0) | RI_ICR_IC1);
				break;
			}
		}
	}

	reg32 &= ~RI_ICR_IC2;
	if (ic2) {
		for (i = 0; i < 16; i++) {
			if (ic2 == ic2_port[i]) {
				reg32 &= ~(15 << 4);
				reg32 |= ((i << 4) | RI_ICR_IC2);
				break;
			}
		}
	}

	reg32 &= ~RI_ICR_IC3;
	if (ic3) {
		for (i = 0; i < 16; i++) {
			if (ic3 == ic3_port[i]) {
				reg32 &= ~(15 << 8);
				reg32 |= ((i << 8) | RI_ICR_IC3);
				break;
			}
		}
	}

	reg32 &= ~RI_ICR_IC4;
	if (ic4) {
		for (i = 0; i < 16; i++) {
			if (ic4 == ic4_port[i]) {
				reg32 &= ~(15 << 12);
				reg32 |= ((i << 12) | RI_ICR_IC4);
				break;
			}
		}
	}
	RI_ICR = reg32;
}

void ri_close_analog_switch(int n, int *portbits)
{
	int i;
	struct port_cr *p;
	int j;
	u32 ascr1 = 0;
	u32 ascr2 = 0;

	for (i = 0; i < n; i++) {
		switch (*portbits & ~0xffff) {
		case RI_PA:
			p = pa_cr;
			break;
		case RI_PB:
			p = pb_cr;
			break;
		case RI_PC:
			p = pc_cr;
			break;
		case RI_PE:
			p = pe_cr;
			break;
		case RI_PF:
			p = pf_cr;
			break;
		case RI_PG:
			p = pg_cr;
			break;
		case RI_OTHER:
			p = other_cr;
			break;
		default:
			p = 0;
			break;
		}
		if (p) {
			for (j = 0; j < 16; j++) {
				if (*portbits & (1 << j)) {
					if (p->reg == 1)
						ascr1 |= p->bit;
					else if (p->reg == 2)
						ascr2 |= p->bit;
				}
				p++;
			}
		}
		portbits++;
	}

	if (ascr1)
		RI_ASCR1 |= ascr1;
	if (ascr2)
		RI_ASCR2 |= ascr2;
}

void ri_open_analog_switch(int n, int *portbits)
{
	int i;
	struct port_cr *p;
	int j;
	u32 ascr1 = 0;
	u32 ascr2 = 0;

	for (i = 0; i < n; i++) {
		switch (*portbits & ~0xffff) {
		case RI_PA:
			p = pa_cr;
			break;
		case RI_PB:
			p = pb_cr;
			break;
		case RI_PC:
			p = pc_cr;
			break;
		case RI_PE:
			p = pe_cr;
			break;
		case RI_PF:
			p = pf_cr;
			break;
		case RI_PG:
			p = pg_cr;
			break;
		case RI_OTHER:
			p = other_cr;
			break;
		default:
			p = 0;
			break;
		}
		if (p) {
			for (j = 0; j < 16; j++) {
				if (*portbits & (1 << j)) {
					if (p->reg == 1)
						ascr1 |= p->bit;
					else if (p->reg == 2)
						ascr2 |= p->bit;
				}
				p++;
			}
		}
		portbits++;
	}

	if (ascr1)
		RI_ASCR1 &= ~ascr1;
	if (ascr2)
		RI_ASCR2 &= ~ascr2;
}

void ri_disable_hysteresis(int portbits)
{
	switch (portbits & ~0xffff) {
	case RI_PA:
		RI_HYSCR1 |= (portbits & 0xffff);
		break;
	case RI_PB:
		RI_HYSCR1 |= ((portbits & 0xffff) << 16);
		break;
	case RI_PC:
		RI_HYSCR2 |= (portbits & 0xffff);
		break;
	case RI_PD:
		RI_HYSCR2 |= ((portbits & 0xffff) << 16);
		break;
	case RI_PE:
		RI_HYSCR3 |= (portbits & 0xffff);
		break;
	case RI_PF:
		RI_HYSCR3 |= ((portbits & 0xffff) << 16);
		break;
	case RI_PG:
		RI_HYSCR4 |= (portbits & 0xffff);
		break;
	default:
		break;
	}
}

void ri_enable_hysteresis(int portbits)
{
	switch (portbits & ~0xffff) {
	case RI_PA:
		RI_HYSCR1 &= ~(portbits & 0xffff);
		break;
	case RI_PB:
		RI_HYSCR1 &= ~((portbits & 0xffff) << 16);
		break;
	case RI_PC:
		RI_HYSCR2 &= ~(portbits & 0xffff);
		break;
	case RI_PD:
		RI_HYSCR2 &= ~((portbits & 0xffff) << 16);
		break;
	case RI_PE:
		RI_HYSCR3 &= ~(portbits & 0xffff);
		break;
	case RI_PF:
		RI_HYSCR3 &= ~((portbits & 0xffff) << 16);
		break;
	case RI_PG:
		RI_HYSCR4 &= ~(portbits & 0xffff);
		break;
	default:
		break;
	}
}
