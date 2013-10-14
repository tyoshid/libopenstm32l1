/*
 * This file is part of the libopencm3 project.
 *
 * Copyright (C) 2010 Piotr Esden-Tempski <piotr@esden.net>
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

/* Note: Entries marked (**) only exist in high and medium+ density devices. */
/* Note: Entries marked (***) only exist in high density devices. */
/* Note: Entries marked (****) only exist in STM32L16x devices. */

/* Symbols exported by the linker script(s): */
extern unsigned _data_loadaddr, _data, _edata, _ebss, _stack;

extern void main(void);
void reset_handler(void) __attribute__ ((naked));
void blocking_handler(void);
void null_handler(void);

extern void nmi_handler(void)	__attribute__ ((weak, alias ("null_handler")));
extern void hard_fault_handler(void) __attribute__ ((weak, alias ("blocking_handler")));
extern void mem_manage_handler(void) __attribute__ ((weak, alias ("blocking_handler")));
extern void bus_fault_handler(void) __attribute__ ((weak, alias ("blocking_handler")));
extern void usage_fault_handler(void) __attribute__ ((weak, alias ("blocking_handler")));
extern void sv_call_handler(void) __attribute__ ((weak, alias ("null_handler")));
extern void debug_monitor_handler(void) __attribute__ ((weak, alias ("null_handler")));
extern void pend_sv_handler(void) __attribute__ ((weak, alias ("null_handler")));
extern void sys_tick_handler(void) __attribute__ ((weak, alias ("null_handler")));
extern void wwdg_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void pvd_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void tamper_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void rtc_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void flash_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void rcc_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void exti0_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void exti1_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void exti2_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void exti3_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void exti4_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void dma1_channel1_isr(void) __attribute__ ((weak, alias ("null_handler")));
extern void dma1_channel2_isr(void) __attribute__ ((weak, alias ("null_handler")));
extern void dma1_channel3_isr(void) __attribute__ ((weak, alias ("null_handler")));
extern void dma1_channel4_isr(void) __attribute__ ((weak, alias ("null_handler")));
extern void dma1_channel5_isr(void) __attribute__ ((weak, alias ("null_handler")));
extern void dma1_channel6_isr(void) __attribute__ ((weak, alias ("null_handler")));
extern void dma1_channel7_isr(void) __attribute__ ((weak, alias ("null_handler")));
extern void adc_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void usb_hp_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void usb_lp_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void dac_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void comp_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void exti9_5_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void lcd_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void tim9_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void tim10_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void tim11_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void tim2_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void tim3_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void tim4_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void i2c1_ev_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void i2c1_er_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void i2c2_ev_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void i2c2_er_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void spi1_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void spi2_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void usart1_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void usart2_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void usart3_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void exti15_10_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void rtc_alarm_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void usb_fs_wkup_isr(void) __attribute__ ((weak, alias ("null_handler")));
extern void tim6_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void tim7_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void sdio_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void tim5_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void spi3_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void uart4_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void uart5_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void dma2_ch1_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void dma2_ch2_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void dma2_ch3_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void dma2_ch4_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void dma2_ch5_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void aes_isr(void)	__attribute__ ((weak, alias ("null_handler")));
extern void comp_acq_isr(void)	__attribute__ ((weak, alias ("null_handler")));

void (*const vector_table[]) (void) __attribute__ ((section(".vectors")));

void (*const vector_table[]) (void) = {
	(void *)&_stack,	/* Addr: 0x0000_0000 */
	reset_handler,		/* Addr: 0x0000_0004 */
	nmi_handler,		/* Addr: 0x0000_0008 */
	hard_fault_handler,	/* Addr: 0x0000_000C */
	mem_manage_handler,	/* Addr: 0x0000_0010 */
	bus_fault_handler,	/* Addr: 0x0000_0014 */
	usage_fault_handler,	/* Addr: 0x0000_0018 */
	0, 0, 0, 0,		/* Reserved Addr: 0x0000_001C - 0x0000_002B */
	sv_call_handler,	/* Addr: 0x0000_002C */
	debug_monitor_handler,	/* Addr: 0x0000_0030*/
	0,			/* Reserved Addr: 0x0000_00034 */
	pend_sv_handler,	/* Addr: 0x0000_0038 */
	sys_tick_handler,	/* Addr: 0x0000_003C */
	wwdg_isr,		/* Addr: 0x0000_0040 */
	pvd_isr,		/* Addr: 0x0000_0044 */
	tamper_isr,		/* Addr: 0x0000_0048 */
	rtc_isr,		/* Addr: 0x0000_004C */
	flash_isr,		/* Addr: 0x0000_0050 */
	rcc_isr,		/* Addr: 0x0000_0054 */
	exti0_isr,		/* Addr: 0x0000_0058 */
	exti1_isr,		/* Addr: 0x0000_005C */
	exti2_isr,		/* Addr: 0x0000_0060 */
	exti3_isr,		/* Addr: 0x0000_0064 */
	exti4_isr,		/* Addr: 0x0000_0068 */
	dma1_channel1_isr,	/* Addr: 0x0000_006C */
	dma1_channel2_isr,	/* Addr: 0x0000_0070 */
	dma1_channel3_isr,	/* Addr: 0x0000_0074 */
	dma1_channel4_isr,	/* Addr: 0x0000_0078 */
	dma1_channel5_isr,	/* Addr: 0x0000_007C */
	dma1_channel6_isr,	/* Addr: 0x0000_0080 */
	dma1_channel7_isr,	/* Addr: 0x0000_0084 */
	adc_isr,		/* Addr: 0x0000_0088 */
	usb_hp_isr,		/* Addr: 0x0000_008C */
	usb_lp_isr,		/* Addr: 0x0000_0090 */
	dac_isr,		/* Addr: 0x0000_0094 */
	comp_isr,		/* Addr: 0x0000_0098 */
	exti9_5_isr,		/* Addr: 0x0000_009C */
	lcd_isr,		/* Addr: 0x0000_00A0 */
	tim9_isr,		/* Addr: 0x0000_00A4 */
	tim10_isr,		/* Addr: 0x0000_00A8 */
	tim11_isr,		/* Addr: 0x0000_00AC */
	tim2_isr,		/* Addr: 0x0000_00B0 */
	tim3_isr,		/* Addr: 0x0000_00B4 */
	tim4_isr,		/* Addr: 0x0000_00B8 */
	i2c1_ev_isr,		/* Addr: 0x0000_00BC */
	i2c1_er_isr,		/* Addr: 0x0000_00C0 */
	i2c2_ev_isr,		/* Addr: 0x0000_00C4 */
	i2c2_er_isr,		/* Addr: 0x0000_00C8 */
	spi1_isr,		/* Addr: 0x0000_00CC */
	spi2_isr,		/* Addr: 0x0000_00D0 */
	usart1_isr,		/* Addr: 0x0000_00D4 */
	usart2_isr,		/* Addr: 0x0000_00D8 */
	usart3_isr,		/* Addr: 0x0000_00DC */
	exti15_10_isr,		/* Addr: 0x0000_00E0 */
	rtc_alarm_isr,		/* Addr: 0x0000_00E4 */
	usb_fs_wkup_isr,	/* Addr: 0x0000_00E8 */
	tim6_isr,		/* Addr: 0x0000_00EC */
	tim7_isr,		/* Addr: 0x0000_00F0 */
	sdio_isr,		/* Addr: 0x0000_00F4 */		/* (***) */
	tim5_isr,		/* Addr: 0x0000_00F8 */		/* (**) */
	spi3_isr,		/* Addr: 0x0000_00FC */		/* (**) */
	uart4_isr,		/* Addr: 0x0000_0100 */		/* (***) */
	uart5_isr,		/* Addr: 0x0000_0104 */		/* (***) */
	dma2_ch1_isr,		/* Addr: 0x0000_0108 */		/* (**) */
	dma2_ch2_isr,		/* Addr: 0x0000_010C */		/* (**) */
	dma2_ch3_isr,		/* Addr: 0x0000_0110 */		/* (**) */
	dma2_ch4_isr,		/* Addr: 0x0000_0114 */		/* (**) */
	dma2_ch5_isr,		/* Addr: 0x0000_0118 */		/* (**) */
	aes_isr,		/* Addr: 0x0000_011C */		/* (****) */
	comp_acq_isr,		/* Addr: 0x0000_0120 */		/* (**) */
};

void reset_handler(void)
{
	unsigned *src, *dest;

	__asm__ ("msr msp, %0" : : "r" (&_stack));

	src = &_data_loadaddr;
	dest = &_data;
	while (dest < &_edata)
		*dest++ = *src++;

	while (dest < &_ebss)
		*dest++ = 0;

	/* Call the application's entry point. */
	main();
}

void blocking_handler(void)
{
	while (1)
		;
}

void null_handler(void)
{
	/* Do nothing. */
}
