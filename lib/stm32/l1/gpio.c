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

/*
 * Basic GPIO handling API.
 *
 * Examples:
 *  gpio_config_output(GPIO_PUSHPULL, GPIO_2MHZ, GPIO_NOPUPD, GPIO_PA12);
 *  gpio_set(GPIO_PB4);
 *  gpio_clear(GPIO_PG(2, 9));
 *  gpio_get(GPIO_PC1);
 *  gpio_toggle(GPIO_PA(7, 8));
 *  reg16 = gpio_port_read(GPIOD);
 *  gpio_port_write(GPIOF, 0xc8fe);
 *
 */

#include <stm32/l1/gpio.h>

static u32 base_addr(int port)
{
	switch (GPIO_PORT(port)) {
	case GPIOA:
		return GPIO_PORT_A_BASE;
	case GPIOB:
		return GPIO_PORT_B_BASE;
	case GPIOC:
		return GPIO_PORT_C_BASE;
	case GPIOD:
		return GPIO_PORT_D_BASE;
	case GPIOE:
		return GPIO_PORT_E_BASE;
	case GPIOF:
		return GPIO_PORT_F_BASE;
	case GPIOG:
		return GPIO_PORT_G_BASE;
	case GPIOH:
		return GPIO_PORT_H_BASE;
	default:
		break;
	}
	return 0;
}

void gpio_config_input(gpio_pupd_t pupd, int portbits)
{
	u32 base;
	u32 moder;
	u32 pupdr;
	int i;

	/*
	 * We want to set the config only for the pins mentioned in gpios,
	 * but keeping the others, so read out the actual config first.
	 */
	base = base_addr(portbits);
	pupdr = GPIO_PUPDR(base);
	moder = GPIO_MODER(base);

	/* Iterate over all bits, use i as the bitnumber. */
	for (i = 0; i < 16; i++) {
		/* Only set the config if the bit is set in gpios. */
		if (!((1 << i) & portbits))
			continue;

		pupdr &= ~(3 << (i * 2));
		pupdr |= (pupd << (i * 2));
		moder &= ~(3 << (i * 2));
		moder |= (GPIO_MODER_INPUT << (i * 2));
	}

	GPIO_PUPDR(base) = pupdr;
	GPIO_MODER(base) = moder;
}

void gpio_config_output(gpio_otype_t otype, gpio_ospeed_t ospeed,
			gpio_pupd_t pupd, int portbits)
{
	u32 base;
	u32 moder;
	u32 otyper;
	u32 ospeedr;
	u32 pupdr;
	int i;

	/*
	 * We want to set the config only for the pins mentioned in gpios,
	 * but keeping the others, so read out the actual config first.
	 */
	base = base_addr(portbits);
	otyper = GPIO_OTYPER(base);
	ospeedr = GPIO_OSPEEDR(base);
	pupdr = GPIO_PUPDR(base);
	moder = GPIO_MODER(base);

	/* Iterate over all bits, use i as the bitnumber. */
	for (i = 0; i < 16; i++) {
		/* Only set the config if the bit is set in gpios. */
		if (!((1 << i) & portbits))
			continue;

		otyper &= ~(1 << i);
		otyper |= (otype << i);
		ospeedr &= ~(3 << (i * 2));
		ospeedr |= (ospeed << (i * 2));
		pupdr &= ~(3 << (i * 2));
		pupdr |= (pupd << (i * 2));
		moder &= ~(3 << (i * 2));
		moder |= (GPIO_MODER_OUTPUT << (i * 2));
	}

	GPIO_OTYPER(base) = otyper;
	GPIO_OSPEEDR(base) = ospeedr;
	GPIO_PUPDR(base) = pupdr;
	GPIO_MODER(base) = moder;
}

void gpio_config_altfn(gpio_altfn_t altfn, gpio_otype_t otype,
		       gpio_ospeed_t ospeed, gpio_pupd_t pupd, int portbits)
{
	u32 base;
	u32 afrl;
	u32 afrh;
	u32 moder;
	u32 otyper;
	u32 ospeedr;
	u32 pupdr;
	int i;

	/*
	 * We want to set the config only for the pins mentioned in gpios,
	 * but keeping the others, so read out the actual config first.
	 */
	base = base_addr(portbits);
	afrl = GPIO_AFRL(base);
	afrh = GPIO_AFRH(base);
	otyper = GPIO_OTYPER(base);
	ospeedr = GPIO_OSPEEDR(base);
	pupdr = GPIO_PUPDR(base);
	moder = GPIO_MODER(base);

	/* Iterate over all bits, use i as the bitnumber. */
	for (i = 0; i < 16; i++) {
		/* Only set the config if the bit is set in gpios. */
		if (!((1 << i) & portbits))
			continue;

		if (i < 8) {
			afrl &= ~(0xf << (i * 4));
			afrl |= (altfn << (i * 4));
		} else {
			afrh &= ~(0xf << ((i - 8) * 4));
			afrh |= (altfn << ((i - 8) * 4));
		}
		otyper &= ~(1 << i);
		otyper |= (otype << i);
		ospeedr &= ~(3 << (i * 2));
		ospeedr |= (ospeed << (i * 2));
		pupdr &= ~(3 << (i * 2));
		pupdr |= (pupd << (i * 2));
		moder &= ~(3 << (i * 2));
		moder |= (GPIO_MODER_ALTFN << (i * 2));
	}

	GPIO_AFRL(base) = afrl;
	GPIO_AFRH(base) = afrh;
	GPIO_OTYPER(base) = otyper;
	GPIO_OSPEEDR(base) = ospeedr;
	GPIO_PUPDR(base) = pupdr;
	GPIO_MODER(base) = moder;
}

void gpio_config_analog(int portbits)
{
	u32 base;
	u32 pupdr;
	u32 moder;
	int i;

	/*
	 * We want to set the config only for the pins mentioned in gpios,
	 * but keeping the others, so read out the actual config first.
	 */
	base = base_addr(portbits);
	pupdr = GPIO_PUPDR(base);
	moder = GPIO_MODER(base);

	/* Iterate over all bits, use i as the bitnumber. */
	for (i = 0; i < 16; i++) {
		/* Only set the config if the bit is set in gpios. */
		if (!((1 << i) & portbits))
			continue;

		pupdr &= ~(3 << (i * 2));
		moder &= ~(3 << (i * 2));
		moder |= (GPIO_MODER_ANALOG << (i * 2));
	}

	GPIO_PUPDR(base) = pupdr;
	GPIO_MODER(base) = moder;
}

void gpio_set_mode(gpio_mode_t mode, int portbits)
{
	u32 base;
	u32 moder;
	int i;

	/*
	 * We want to set the config only for the pins mentioned in gpios,
	 * but keeping the others, so read out the actual config first.
	 */
	base = base_addr(portbits);
	moder = GPIO_MODER(base);

	/* Iterate over all bits, use i as the bitnumber. */
	for (i = 0; i < 16; i++) {
		/* Only set the config if the bit is set in gpios. */
		if (!((1 << i) & portbits))
			continue;

		moder &= ~(3 << (i * 2));
		moder |= (mode << (i * 2));
	}

	GPIO_MODER(base) = moder;
}

void gpio_set(int portbits)
{
	GPIO_BSRR(base_addr(portbits)) = (GPIO_BITS(portbits));
}

void gpio_clear(int portbits)
{
	GPIO_BSRR(base_addr(portbits)) = (GPIO_BITS(portbits) << 16);
}

u16 gpio_get(int portbits)
{
	return GPIO_IDR(base_addr(portbits)) & portbits;
}

void gpio_toggle(int portbits)
{
	GPIO_ODR(base_addr(portbits)) ^= (GPIO_BITS(portbits));
}

u16 gpio_port_read(int port)
{
	return (u16)GPIO_IDR(base_addr(port));
}

void gpio_port_write(int port, u16 data)
{
	GPIO_ODR(base_addr(port)) = data;
}

int gpio_port_config_lock(int portbits)
{
	u32 base;
	u32 bits;

	base = base_addr(portbits);
	bits = GPIO_BITS(portbits);
	/* Special "Lock Key Writing Sequence", see datasheet. */
	GPIO_LCKR(base) = GPIO_LCKK | bits;	/* Set LCKK. */
	GPIO_LCKR(base) = bits;			/* Clear LCKK. */
	GPIO_LCKR(base) = GPIO_LCKK | bits;	/* Set LCKK. */
	GPIO_LCKR(base);			/* Read LCKK. */

	return GPIO_LCKR(base) & GPIO_LCKK;
}
