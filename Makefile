# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
OS := $(firstword $(subst _, ,$(shell uname -s)))

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

install: uninstall
	$(MAKE) -w -C util/bin $(MAKECMDGOALS)
	@echo Installed for $(OS)

uninstall:
	$(MAKE) -w -C util/bin $(MAKECMDGOALS)

set-profile: unset-profile
	@touch ${HOME}/.profile
	@echo "export AVRIO_ROOT=${PWD}" >> ${HOME}/.profile
	@echo "AVRIO_ROOT=${PWD}  was added in ${HOME}/.profile"
	@echo "You must log off for this to take effect."

unset-profile:
	@touch ${HOME}/.profile
	@echo "Remove AVRIO_ROOT=... in ${HOME}/.profile"
	@sed -i -e "/^export\ AVRIO_ROOT=.*/d" ${HOME}/.profile

elf: $(SUBDIRS)
hex: $(SUBDIRS)
eep: $(SUBDIRS)
lss: $(SUBDIRS)
sym: $(SUBDIRS)
har: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -w -C $@ $(MAKECMDGOALS)

.PHONY: all clean distclean rebuild lib cleanlib extcoff program debug install uninstall set-profile unset-profile elf hex eep lss sym $(SUBDIRS)
