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

#include <stm32/l1/pwr.h>

void pwr_set_vos(pwr_vos_t vos)
{
	u32 reg32;

	while (PWR_CSR & PWR_CSR_VOSF)
		;
	reg32 = PWR_CR;
	if ((reg32 & (PWR_CR_VOS1 | PWR_CR_VOS0)) == vos)
		return;
	reg32 &= ~(PWR_CR_VOS1 | PWR_CR_VOS0);
	reg32 |= vos;
	PWR_CR = reg32;
	while (PWR_CSR & PWR_CSR_VOSF)
		;
}

int pwr_get_vos(void)
{
	return PWR_CR & (PWR_CR_VOS1 | PWR_CR_VOS0);
}

/* Run mode */
void pwr_set_run_mode(void)
{
	u32 reg32;

	reg32 = PWR_CR;
	reg32 &= ~PWR_CR_LPRUN;
	PWR_CR = reg32;
	reg32 &= ~(PWR_CR_PDDS | PWR_CR_LPSDSR);
	PWR_CR = reg32;
}

/* Low power run mode */
void pwr_set_low_power_run_mode(void)
{
	u32 reg32;

	reg32 = PWR_CR;
	reg32 &= ~PWR_CR_PDDS;
	reg32 |= PWR_CR_LPSDSR;
	PWR_CR = reg32;
	reg32 |= PWR_CR_LPRUN;
	PWR_CR = reg32;
}

/* Sleep mode */
void pwr_set_sleep_mode(void)
{
	u32 reg32;

	reg32 = PWR_CR;
	reg32 &= ~PWR_CR_LPRUN;
	PWR_CR = reg32;
	reg32 &= ~(PWR_CR_PDDS | PWR_CR_LPSDSR);
	PWR_CR = reg32;
}

/* Low power sleep mode */
void pwr_set_low_power_sleep_mode(void)
{
	u32 reg32;

	reg32 = PWR_CR;
	reg32 &= ~(PWR_CR_LPRUN | PWR_CR_PDDS);
	PWR_CR = reg32;
	reg32 |= PWR_CR_LPSDSR;
	PWR_CR = reg32;
}

/* Stop mode */
void pwr_set_stop_mode(void)
{
	u32 reg32;

	reg32 = PWR_CR;
	reg32 &= ~(PWR_CR_LPRUN | PWR_CR_PDDS);
	PWR_CR = reg32;
	reg32 |= (PWR_CR_CWUF | PWR_CR_LPSDSR);
	PWR_CR = reg32;
}

/* Standby mode */
void pwr_set_standby_mode(void)
{
	u32 reg32;

	reg32 = PWR_CR;
	reg32 &= ~PWR_CR_LPRUN;
	PWR_CR = reg32;
	reg32 &= ~PWR_CR_LPSDSR;
	reg32 |= (PWR_CR_CSBF | PWR_CR_CWUF | PWR_CR_PDDS);
	PWR_CR = reg32;
}

void pwr_enable_ultralow_power_mode(bool fast_wakeup)
{
	u32 reg32;

	reg32 = PWR_CR;
	if (fast_wakeup) {
		reg32 |= (PWR_CR_ULP | PWR_CR_FWU);
	} else {
		reg32 &= ~PWR_CR_FWU;
		reg32 |= PWR_CR_ULP;
	}
	PWR_CR = reg32;
}

void pwr_disable_ultralow_power_mode(void)
{
	PWR_CR &= ~PWR_CR_ULP;
}

void pwr_disable_backup_write_protection(void)
{
	PWR_CR |= PWR_CR_DBP;
}

void pwr_enable_backup_write_protection(void)
{
	PWR_CR &= ~PWR_CR_DBP;
}

void pwr_set_pvd_level(pwr_pvd_t level)
{
	u32 reg32;

	reg32 = PWR_CR;
	reg32 &= ~(PWR_CR_PLS2 | PWR_CR_PLS1 | PWR_CR_PLS0);
	reg32 |= level;
	PWR_CR = reg32;
}

void pwr_enable_pvd(void)
{
	PWR_CR |= PWR_CR_PVDE;
}

void pwr_disable_pvd(void)
{
	PWR_CR &= ~PWR_CR_PVDE;
}

void pwr_enable_wkup_pin(int ewup)
{
	PWR_CSR |= ewup;
}

void pwr_disable_wkup_pin(int ewup)
{
	PWR_CSR &= ~ewup;
}

int pwr_get_flag(int flag)
{
	return PWR_CSR & flag;
}

void pwr_clear_standby_flag(void)
{
	PWR_CR |= PWR_CR_CSBF;
}

void pwr_clear_wakeup_flag(void)
{
	PWR_CR |= PWR_CR_CWUF;
}

void pwr_wait_for_regulator_main_mode(void)
{
	while (PWR_CSR & PWR_CSR_REGLPF)
		;
}

void pwr_wait_for_vrefint_ready(void)
{
	while (!(PWR_CSR & PWR_CSR_VREFINTRDYF))
		;
}
