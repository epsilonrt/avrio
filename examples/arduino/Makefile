# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$

SUBDIRS = 01.Basics libraries

.PHONY: $(SUBDIRS)

all: $(SUBDIRS)
clean: $(SUBDIRS)
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

$(SUBDIRS):
	$(MAKE) -w -C $@ $(MAKECMDGOALS)

