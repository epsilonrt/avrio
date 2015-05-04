# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$

SUBDIRS  = adc adc_sensor bisrelay aprs hsc_sensor button i2c-twi-slave
SUBDIRS +=  keyboard lcd led ledrgb melody mmc rtc serial spi switch task
SUBDIRS += mq135
# The examples below are dependents of external projects, you may uncomment if
# necessary after installing this projects in src directory
# SUBDIRS += avrx
# SUBDIRS += arduino
# SUBDIRS += cmg
# SUBDIRS += fatfs
# SUBDIRS += idwarf
# SUBDIRS += lufa
# SUBDIRS += mbmaster
# SUBDIRS += modbus

all: $(SUBDIRS)
clean: $(SUBDIRS)
distclean: $(SUBDIRS)
rebuild: $(SUBDIRS)
lib: $(SUBDIRS)
cleanlib: $(SUBDIRS)
extcoff: $(SUBDIRS)
program: $(SUBDIRS)
debug: $(SUBDIRS)

elf: $(SUBDIRS)
hex: $(SUBDIRS)
eep: $(SUBDIRS)
lss: $(SUBDIRS)
sym: $(SUBDIRS)
har: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -w -C $@ $(MAKECMDGOALS)

.PHONY: all clean distclean rebuild lib cleanlib extcoff program debug elf hex eep lss sym har $(SUBDIRS)
