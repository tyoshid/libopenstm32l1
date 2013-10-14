##
## This file is part of the libopencm3 project.
##
## Copyright (C) 2009 Uwe Hermann <uwe@hermann-uwe.de>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
##

LIBNAME		= libstm32l1

SRCLIBDIR	?= $(realpath ../..)
PREFIX		?= arm-none-eabi
CC		= $(PREFIX)-gcc
AR		= $(PREFIX)-ar
CFLAGS		= -Os -g \
		  -Wall -Wextra -Wimplicit-function-declaration \
		  -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes \
		  -Wundef -Wshadow \
		  -I../../../include -ffreestanding -fno-common \
		  -mcpu=cortex-m3 -mthumb \
		  -ffunction-sections -fdata-sections -MD
# ARFLAGS	= rcsv
ARFLAGS		= rcs

OBJS		= crc.o pwr.o rcc.o gpio.o ri.o syscfg.o nvic.o vector.o \
                  exti.o dma.o adc.o dac.o comp.o opamp.o lcd.o tim.o rtc.o \
                  iwdg.o wwdg.o aes.o  usbdevfs.o fsmc.o i2c.o usart.o spi.o \
                  sdio.o dbgmcu.o desig.o scb.o systick.o flash.o

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
endif

all: $(LIBNAME).a

$(LIBNAME).a: $(OBJS)
	@echo "  AR      $(@F)"
	$(Q)$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	@echo "  CC      $(<F)"
	$(Q)$(CC) $(CFLAGS) -o $@ -c $<

clean:
	@echo "  CLEAN   $(subst $(dir $(SRCLIBDIR)),,$(shell pwd))"
	$(Q)rm -f *.o *.d *.a

.PHONY: all clean

-include $(OBJS:.o=.d)
