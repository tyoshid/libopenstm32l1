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

#include <stm32/l1/nvic.h>

bool nvic_irq_enabled(int irqn)
{
	return NVIC_ISER(irqn / 32) & (1 << (irqn % 32));
}

void nvic_enable_irq(int irqn)
{
	NVIC_ISER(irqn / 32) = (1 << (irqn % 32));
}

void nvic_disable_irq(int irqn)
{
	NVIC_ICER(irqn / 32) = (1 << (irqn % 32));
}

bool nvic_irq_pending(int irqn)
{
	return NVIC_ISPR(irqn / 32) & (1 << (irqn % 32));
}

void nvic_set_pending_irq(int irqn)
{
	NVIC_ISPR(irqn / 32) = (1 << (irqn % 32));
}

void nvic_clear_pending_irq(int irqn)
{
	NVIC_ICPR(irqn / 32) = (1 << (irqn % 32));
}

bool nvic_irq_active(int irqn)
{
	return NVIC_IABR(irqn / 32) & (1 << (irqn % 32));
}

void nvic_set_priority(int irqn, int priority)
{
	NVIC_IPR_BYTE(irqn) = priority;
}

void nvic_generate_software_interrupt(int irqn)
{
	NVIC_STIR = irqn;
}
