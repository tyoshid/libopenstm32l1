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

PREFIX		?= arm-none-eabi

DESTDIR		?= $(shell dirname $(shell readlink -f $(shell which $(PREFIX)-gcc)))/..

INCDIR		= $(DESTDIR)/$(PREFIX)/include
LIBDIR		= $(DESTDIR)/$(PREFIX)/lib
SHAREDIR	= $(DESTDIR)/$(PREFIX)/share
INSTALL		= install

LIBS		= lib/stm32/l1/libstm32l1.a
TARGETS		= stm32/l1

# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q := @
# Do not print "Entering directory ...".
MAKEFLAGS += --no-print-directory
endif

all: $(LIBS)

$(LIBS):
	@echo "  BUILD   $(@D)";
	$(Q)$(MAKE) -C $(@D)

install: $(LIBS)
	@printf "  INSTALL headers\n"
	$(Q)$(INSTALL) -d $(INCDIR)/stm32
	$(Q)cp -r include/stm32/* $(INCDIR)/stm32
	$(Q)cp -r include/usb $(INCDIR)
	$(Q)cp include/*.h $(INCDIR)
	@printf "  INSTALL libs\n"
	$(Q)$(INSTALL) -d $(LIBDIR)/stm32/l1
	$(Q)$(INSTALL) -m 0644 lib/stm32/l1/*.a $(LIBDIR)/stm32/l1
	@printf "  INSTALL ldscripts\n"
	$(Q)$(INSTALL) -m 0644 lib/stm32/l1/*.ld $(LIBDIR)/stm32/l1
	$(Q)$(INSTALL) -m 0644 lib/*.specs $(LIBDIR)
	@printf "  INSTALL scripts\n"
	$(Q)$(INSTALL) -d $(SHAREDIR)/libopencm3/scripts
	$(Q)$(INSTALL) -m 0644 share/libopencm3/scripts/* $(SHAREDIR)/libopencm3/scripts

# Bleh http://www.makelinux.net/make3/make3-CHP-6-SECT-1#make3-CHP-6-SECT-1
clean:
	$(Q)for i in $(dir $(LIBS)); do \
		if [ -d $$i ]; then \
			echo "  CLEAN   $$i"; \
			$(MAKE) -C $$i clean || exit $?; \
		fi; \
	done

EXAMPLE_DIRS:=$(sort $(dir $(wildcard $(addsuffix /*/*/Makefile,$(addprefix examples/,$(TARGETS))))))
$(EXAMPLE_DIRS):
	@echo "  BUILD   $@";
	$(Q)$(MAKE) -C $@

examples: $(EXAMPLE_DIRS)
	$(Q)true

clean_examples:
	$(Q)for i in $(EXAMPLE_DIRS); do \
		if [ -d $$i ]; then \
			echo "  CLEAN   $$i"; \
			$(MAKE) -C $$i clean || exit $?; \
		fi; \
	done

.PHONY: examples $(EXAMPLE_DIRS) install clean clean_examples
