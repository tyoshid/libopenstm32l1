#include <rcc.h>
#include <pwr.h>
#include <flash.h>
#include <tim.h>
#include <gpio.h>
#include <lcd.h>
#include <dbgmcu.h>
#include <desig.h>

#include <syscall.h>
#include <stdio.h>

#define TIMX_CLK_APB1 16000000

/* LCD connections */
struct segmap {
	int com;
	int seg;
};

/* A, B, ..., Q */
struct segmap charseg[6][14] = {
	{
		{1, 28}, {0, 28}, {1, 1}, {1, 0}, {0, 0}, {1, 29}, {0, 29},
		{3, 29}, {3, 28}, {2, 28}, {0, 1}, {3, 0}, {2, 0}, {2, 29}
	},
	{
		{1, 26}, {0, 26}, {1, 7}, {1, 2}, {0, 2}, {1, 27}, {0, 27},
		{3, 27}, {3, 26}, {2, 26}, {0, 7}, {3, 2}, {2, 2}, {2, 27}
	},
	{
		{1, 24}, {0, 24}, {1, 9}, {1, 8}, {0, 8}, {1, 25}, {0, 25},
		{3, 25}, {3, 24}, {2, 24}, {0, 9}, {3, 8}, {2, 8}, {2, 25}
	},
	{
		{1, 20}, {0, 20}, {1, 11}, {1, 10}, {0, 10}, {1, 21}, {0, 21},
		{3, 21}, {3, 20}, {2, 20}, {0, 11}, {3, 10}, {2, 10}, {2, 21}
	},
	{
		{1, 18}, {0, 18}, {1, 13}, {1, 12}, {0, 12}, {1, 19}, {0, 19},
		{3, 19}, {3, 18}, {2, 18}, {0, 13}, {3, 12}, {2, 12}, {2, 19}
	},
	{
		{1, 17}, {0, 17}, {1, 15}, {1, 14}, {0, 14}, {1, 16}, {0, 16},
		{3, 16}, {3, 17}, {2, 17}, {0, 15}, {3, 14}, {2, 14}, {2, 16}
	}
};

/*
 * Character font (14 segments)
 *
 *  !"#$%&'
 * ()*+,-./
 * 01234567
 * 89:;<=>?
 * @ABCDEFG
 * HIJKLMNO
 * PQRSTUVW
 * XYZ[\]^_
 * `abcdefg
 * hijklmno
 * pqrstuvw
 * xyz{|}~
 */
u16 font[] = {0x0000, 0x0208, 0x0120, 0x154e, 0x156d, 0x2ee4, 0x2a8d, 0x0200,
	      0x0a00, 0x2080, 0x3fc0, 0x1540, 0x2000, 0x0440, 0x1000, 0x2200,
	      0x003f, 0x0006, 0x045b, 0x044f, 0x0466, 0x046d, 0x047d, 0x0007,
	      0x047f, 0x046f, 0x1100, 0x2100, 0x0a00, 0x0448, 0x2080, 0x1403,
	      0x053b, 0x0477, 0x0a79, 0x0039, 0x208f, 0x0479, 0x0471, 0x043d,
	      0x0476, 0x1109, 0x001e, 0x0674, 0x0038, 0x0137, 0x08b6, 0x013f,
	      0x0473, 0x083f, 0x0c73, 0x046d, 0x1101, 0x003e, 0x2230, 0x103e,
	      0x06d4, 0x046e, 0x2209, 0x0039, 0x0880, 0x000f, 0x0202, 0x0008,
	      0x0080, 0x1058, 0x047c, 0x0458, 0x045e, 0x2058, 0x0071, 0x060e,
	      0x0474, 0x1001, 0x000d, 0x0a70, 0x1108, 0x1454, 0x0454, 0x045c,
	      0x0271, 0x0487, 0x0050, 0x0c08, 0x0078, 0x001c, 0x0818, 0x101c,
	      0x2a80, 0x080c, 0x2048, 0x0a40, 0x1100, 0x2480, 0x0452};

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

static void tim_setup(void)
{
	/* Enable TIM7 clock. */
	rcc_enable_clock(RCC_TIM7);

	/* Enable one-pulse mode. */
	/* Generate update interrupt on counter overflow. */
	tim_enable_one_pulse_mode(TIM7);
	tim_disable_update_interrupt_on_any(TIM7);

	/* Load prescaler value (2kHz). */
	tim_load_prescaler_value(TIM7, TIMX_CLK_APB1 / 2000 - 1);
}

/* 1 - 32767 msec */
static void delay_ms(u16 ms)
{
	tim_set_autoreload_value(TIM7, (ms << 1) - 1);

	tim_enable_counter(TIM7);
	while (!tim_get_interrupt_status(TIM7, TIM_UPDATE))
		;
	tim_clear_interrupt(TIM7, TIM_UPDATE);
}

static void lcd_setup(void)
{
	/* Enable GPIOA, GPIOB and GPIOC clock. */
	rcc_enable_clock(RCC_GPIOA);
	rcc_enable_clock(RCC_GPIOB);
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
	lcd_set_contrast(3);

	/* Wait for LCD_FCR register update. */
	lcd_wait_for_sync();

	/* Start display. */
	lcd_enable();

	/* Wait for step-up converter ready. */
	lcd_wait_for_ready();
}

static void write_char(int col, int fn)
{
	int i;
	int com;
	int seg;

	for (i = 0; i < 14; i++) {
		/* Common */
		com = charseg[col][i].com;

		/* Segment */
		seg = charseg[col][i].seg;

		/* Set pixel. */
		if (font[fn] & (1 << i))
			LCD_RAM[com * 2] |= (1 << seg);
		else
			LCD_RAM[com * 2] &= ~(1 << seg);
	}
}

/* frame buffer */
static u8 buf[6] = {' ', ' ', ' ', ' ', ' ', ' '};

/* character pointer (0 - 5) */
static int cp;

/* new line flag */
static bool nlflag;

static int lcd_putchar(int c)
{
	u8 r = c;
	int i;

	if (r < ' ') {		/* control codes */
		/* new line code */
		if (r == '\n')
			/* Set flag. */
			nlflag = true;
	} else if (r < 0x7f) {	/* character codes */
		if (nlflag) {
			for (i = 1; i < 6; i++) {
				/* Clear buffer. */
				buf[i] = ' ';

				/* Clear LCD. */
				write_char(i, 0);
			}
			/* charcacter pointer = 0 */
			cp = 0;

			/* Clear flag. */
			nlflag = false;
		}

		if (cp < 6) {
			/* Write to buffer. */
			buf[cp] = r;

			/* Write to LCD. */
			write_char(cp, r - ' ');

			/* next colmun */
			cp++;
		} else {	/* overflow */
			for (i = 0; i < 5; i++) {
				/* Shift buffer. */
				buf[i] = buf[i + 1];

				/* Shift LCD. */
				write_char(i, buf[i] - ' ');
			}

			/* Write to buffer. */
			buf[5] = r;

			/* Write to LCD. */
			write_char(5, r - ' ');
		}
	}
	return (int)r;
}

int _write(int file, char *ptr, int len)
{
	int i;

	if (file == 1) {
		for (i = 0; i < len; i++)
			lcd_putchar(ptr[i]);

		/* Update display. */
		lcd_update();

		return i;
	}

	errno = EIO;
	return -1;
}

int main(void)
{
	u32 id;
	int flash;
	u32 uid[3];

	clock_setup();
	lcd_setup();
	tim_setup();

	id = dbgmcu_get_device_id();
	printf("%X", (unsigned int)(id & 0xfff));

	/* Wait a bit. */
	delay_ms(2000);

	switch (id & 0xfff) {
	case 0x416:
		printf(" LM\n");
		break;
	case 0x427:
		printf(" M+\n");
		break;
	case 0x436:
		printf("HM+\n");
		break;
	default:
		printf("???\n");
		break;
	}

	/* Wait a bit. */
	delay_ms(2000);

	flash = desig_get_flash_size(id & 0xfff);

	printf("%X", (unsigned int)(id >> 16));

	/* Wait a bit. */
	delay_ms(2000);

	switch (id & 0xfff) {
	case 0x416:
		switch (id >> 16) {
		case 0x1000:
			printf(" A\n");
			break;
		case 0x1008:
			printf(" Y\n");
			break;
		case 0x1018:
			printf(" X\n"); /* ??? */
			break;
		case 0x1038:
			printf(" W\n");
			break;
		case 0x1078:
			printf(" V\n");
			break;
		default:
			printf("??\n");
			break;
		}
		break;
	case 0x427:
		switch (id >> 16) {
		case 0x1018:
			printf(" A\n");
			break;
		default:
			printf("??\n");
			break;
		}
		break;
	case 0x436:
		if (flash == 256 * 1024)
			switch (id >> 16) {
			case 0x1018:
				printf(" A\n");
				break;
			default:
				printf("??\n");
				break;
			}
		else
			switch (id >> 16) {
			case 0x1000:
				printf(" A\n");
				break;
			case 0x1008:
				printf(" Z\n");
				break;
			case 0x1018:
				printf(" Y\n");
				break;
			default:
				printf("??\n");
				break;
			}
		break;
	}

	/* Wait a bit. */
	delay_ms(2000);

	printf("%3d KB\n", flash / 1024);

	/* Wait a bit. */
	delay_ms(2000);

	desig_get_unique_id(id & 0xfff, uid);

	printf("%04X\n", (unsigned int)uid[2] >> 16);

	/* Wait a bit. */
	delay_ms(2000);

	printf("%04X\n", (unsigned int)uid[2] % 0xffff);

	/* Wait a bit. */
	delay_ms(2000);

	printf("%04X\n", (unsigned int)uid[1] >> 16);

	/* Wait a bit. */
	delay_ms(2000);

	printf("%04X\n", (unsigned int)uid[1] % 0xffff);

	/* Wait a bit. */
	delay_ms(2000);

	printf("%04X\n", (unsigned int)uid[0] >> 16);

	/* Wait a bit. */
	delay_ms(2000);

	printf("%04X\n", (unsigned int)uid[0] % 0xffff);

	while (1)
		;

	return 0;
}
