# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
prefix=/usr/local

SUBDIRS = src test examples


all: $(SUBDIRS)
clean: $(SUBDIRS)
distclean: $(SUBDIRS)
rebuild: $(SUBDIRS)
lib: $(SUBDIRS)
cleanlib: $(SUBDIRS)
extcoff: $(SUBDIRS)
program: $(SUBDIRS)
debug: $(SUBDIRS)
install:
	install -m 0755 projects/avrio-make $(prefix)/bin
uninstall:
	-rm -f $(prefix)/bin/avrio-make

elf: $(SUBDIRS)
hex: $(SUBDIRS)
eep: $(SUBDIRS)
lss: $(SUBDIRS)
sym: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -w -C $@ $(MAKECMDGOALS)

.PHONY: all clean distclean rebuild lib cleanlib extcoff program debug install uninstall elf hex eep lss sym $(SUBDIRS)
