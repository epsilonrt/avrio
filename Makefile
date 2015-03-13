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
	@-mkdir -p $(prefix)/bin
	@-install -m 0755 utils/avrio-make $(prefix)/bin
	@-install -m 0755 utils/avrio-cl $(prefix)/bin
	@-install -m 0755 utils/import-arduino $(prefix)/bin
	@echo Installed for $(OS)
uninstall:
	@-rm -f $(prefix)/bin/avrio-cl
	@-rm -f $(prefix)/bin/avrio-make
	@-rm -f $(prefix)/bin/import-arduino
set-profile: unset-profile
	@sed -i -e "\$$aexport AVRIO_ROOT=${PWD}" ${HOME}/.profile
	@echo "AVRIO_ROOT=${PWD}  was added in ${HOME}/.profile"
	@echo "You must log off for this to take effect."
unset-profile:
	@echo "Remove AVRIO_ROOT=... in ${HOME}/.profile"
	@sed -i -e "/^export\ AVRIO_ROOT=.*/d" ${HOME}/.profile

elf: $(SUBDIRS)
hex: $(SUBDIRS)
eep: $(SUBDIRS)
lss: $(SUBDIRS)
sym: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -w -C $@ $(MAKECMDGOALS)

.PHONY: all clean distclean rebuild lib cleanlib extcoff program debug install uninstall set-profile unset-profile elf hex eep lss sym $(SUBDIRS)
