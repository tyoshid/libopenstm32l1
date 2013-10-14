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

#include <rcc.h>
#include <pwr.h>
#include <flash.h>
#include <gpio.h>
#include <lcd.h>

/* Set STM32 to 16 MHz. */
static void clock_setup(void)
{
	/* Enable PWR clock. */
	rcc_enable_clock(RCC_PWR);

	/* Set VCORE to 1.8V */
	pwr_set_vos(PWR_1_8_V);

	/* Set Flash memory latency (0WS). */
	flash_enable_64bit_access(0);

	/* Enable internal high-speed oscillator (16MHz). */
	rcc_enable_osc(RCC_HSI);

	/* AHB, APB1 and APB2 prescaler value is default. */
	// rcc_set_prescaler(1, 1, 1);

	/* Select SYSCLK source. */
	rcc_set_sysclk_source(RCC_HSI);

	/*
	 * Enable write access to RTC, RTC backup register and RCC CSR
	 * (LSEON, LSEBYP, RTCSEL and RTCEN)
	 */
	pwr_disable_backup_write_protection();

	/* Enable LSE. */
	rcc_enable_osc(RCC_LSE);

	/* LCDCLK = 32.768kHz */
	rcc_setup_rtc_lcd_clock(RCC_LSE, 0);
}

static void gpio_setup(void)
{
	/* Enable GPIOB clock. */
	rcc_enable_clock(RCC_GPIOB);

	/* Set GPIO6 and GPIO7 (in GPIO port B) to 'output push-pull'. */
	gpio_config_output(GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			   GPIO_PB(6, 7));

	/* LED off */
	gpio_clear(GPIO_PB(6, 7));
}

static void lcd_setup(void)
{
	/* Enable GPIOA, GPIOB and GPIOC clock. */
	rcc_enable_clock(RCC_GPIOA);
	// rcc_enable_clock(RCC_GPIOB);
	rcc_enable_clock(RCC_GPIOC);

	/* Set GPIO1-3,8-10,15 (in GPIO port A) to 'alternate function'. */
	gpio_config_altfn(GPIO_LCD, GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			  GPIO_PA(LCD_SEG0, LCD_SEG1, LCD_SEG2, LCD_COM0,
				  LCD_COM1, LCD_COM2, LCD_SEG17));

	/* Set GPIO3-5,8-15 (in GPIO port B) to 'alternate function'. */
	gpio_config_altfn(GPIO_LCD, GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			  GPIO_PB(LCD_SEG7, LCD_SEG8, LCD_SEG9, LCD_SEG16,
				  LCD_COM3, LCD_SEG10, LCD_SEG11, LCD_SEG12,
				  LCD_SEG13, LCD_SEG14, LCD_SEG15));

	/* Set GPIO0-3,6-11 (in GPIO port C) to 'alternate function'. */
	gpio_config_altfn(GPIO_LCD, GPIO_PUSHPULL, GPIO_400KHZ, GPIO_NOPUPD,
			  GPIO_PC(LCD_SEG18, LCD_SEG19, LCD_SEG20, LCD_SEG21,
				  LCD_SEG24, LCD_SEG25, LCD_SEG26, LCD_SEG27,
				  LCD_COM4_SEG28_SEG40, LCD_COM5_SEG29_SEG41));

	/* Enable LCD clock. */
	rcc_enable_clock(RCC_LCD);

	/* Select internal step-up coverter(default). */
	// lcd_set_voltage_source(false);

	/* SEG[31:28] are multiplexed with SEG[43:40] */
	lcd_set_seg_pin_multiplex(true);

	/* 1/4 duty, 1/3 bias */
	lcd_set_duty_bias(LCD_DUTY_1_4, LCD_BIAS_1_3);

	/*
	 * fck_div = fLCDCLK / (8 * 16) = 256Hz
	 * fframe = fck_div * duty = 64Hz
	 */
	lcd_set_ck_div(8, 16);

	/* Wait for LCD_FCR register update. */
	lcd_wait_for_sync();

	/* Set contrast. */
	lcd_set_contrast(2);

	/* Wait for LCD_FCR register update. */
	lcd_wait_for_sync();

	/* Start display. */
	lcd_enable();

	/* Wait for step-up converter ready. */
	lcd_wait_for_ready();
}

int main(void)
{
	int i, j;
	int com, seg;

	clock_setup();
	gpio_setup();
	lcd_setup();

	while (1) {
		/* 1 segment */
		/* LED(PB6) on/off */
		gpio_toggle(GPIO_PB6);

		/* Clear all. */
		for (com = 0; com < 4; com++)
			LCD_RAM[com * 2] = 0;

		for (com = 0; com < 4; com++) {
			for (seg = 0; seg < 30; seg++) {
				if (seg < 3 || (seg > 6 && seg < 22) ||
				    seg > 23) {
					/* Set pixel. */
					LCD_RAM[com * 2] = (1 << seg);

					/* Update display. */
					lcd_update();

					/* Wait a bit. */
					for (i = 0; i < 2000000; i++)
						__asm__ ("nop");
				}
			}

			/* Clear pixel. */
			LCD_RAM[com * 2] = 0;

			/* Update display. */
			lcd_update();
		}

		/* All segments */
		/* LED(PB6) on/off */
		gpio_toggle(GPIO_PB6);

		for (com = 0; com < 4; com++) {
			for (seg = 0; seg < 30; seg++) {
				if (seg < 3 || (seg > 6 && seg < 22) ||
				    seg > 23) {
					/* Set pixel. */
					LCD_RAM[com * 2] |= (1 << seg);

					/* Update display. */
					lcd_update();

					/* Wait a bit. */
					for (i = 0; i < 800000; i++)
						__asm__ ("nop");
				}
			}
		}

		/* Blink */
		/* LED(PB6) on/off */
		gpio_toggle(GPIO_PB6);

		/* Set blink frequency (fck_div / 128 = 2Hz). */
		lcd_set_blink_frequency(128);

		/* Wait for LCD_FCR register update. */
		lcd_wait_for_sync();

		for (j = 0; j < 3; j++) {
			/* Blink mode */
			lcd_blink_mode_t mode[3] = {LCD_SEG0_COM0,
						    LCD_SEG0_ALL_COMS,
						    LCD_ALL_SEGS_ALL_COMS};

			/* LED(PB7) on/off */
			gpio_toggle(GPIO_PB7);

			/* Set blink mode. */
			lcd_set_blink_mode(mode[j]);

			/* Wait for LCD_FCR register update. */
			lcd_wait_for_sync();

			/* Wait a bit. */
			for (i = 0; i < 8000000; i++)
				__asm__ ("nop");
		}

		/* Disable blink. */
		lcd_set_blink_mode(LCD_BLINK_DISABLE);

		/* Wait for LCD_FCR register update. */
		lcd_wait_for_sync();

		/* Contrast */
		/* LED(PB6) on/off */
		gpio_toggle(GPIO_PB6);

		for (j = 0; j < 8; j++) {
			/* LED(PB7) on/off */
			gpio_toggle(GPIO_PB7);

			/* Set contrast. */
			lcd_set_contrast(j);

			/* Wait for LCD_FCR register update. */
			lcd_wait_for_sync();

			/* Wait a bit. */
			for (i = 0; i < 8000000; i++)
				__asm__ ("nop");
		}

		/* Set contrast (VLCD3). */
		lcd_set_contrast(3);

		/* Wait for LCD_FCR register update. */
		lcd_wait_for_sync();

		/* Dead time duration */
		/* LED(PB6) on/off */
		gpio_toggle(GPIO_PB6);

		for (j = 0; j < 7; j++) {
			/* LED(PB7) on/off */
			gpio_toggle(GPIO_PB7);

			/* Set dead time. */
			lcd_set_dead_time(j);

			/* Wait for LCD_FCR register update. */
			lcd_wait_for_sync();

			/* Wait a bit. */
			for (i = 0; i < 8000000; i++)
				__asm__ ("nop");
		}

		/* No dead time */
		lcd_set_dead_time(0);

		/* Wait for LCD_FCR register update. */
		lcd_wait_for_sync();

		/* Set contrast. */
		lcd_set_contrast(2);

		/* Wait for LCD_FCR register update. */
		lcd_wait_for_sync();
	}
	return 0;
}
