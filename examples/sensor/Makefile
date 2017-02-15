# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
SUBDIRS  = hih4000 hih6130 hsc lmt87 mpx mq135 ntc pot rtd tsl230
CLEANER_SUBDIRS = 

all: $(SUBDIRS)
clean: $(SUBDIRS) $(CLEANER_SUBDIRS)
distclean: $(SUBDIRS) $(CLEANER_SUBDIRS)
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

$(CLEANER_SUBDIRS):
	$(MAKE) -w -C $@ $(MAKECMDGOALS)

.PHONY: all clean distclean rebuild lib cleanlib extcoff program debug elf hex eep lss sym har $(SUBDIRS) $(CLEANER_SUBDIRS)
