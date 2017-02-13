###############################################################################
# (c) Copyright 2015 epsilonRT, All rights reserved.                          #
# Licensed under the Apache License, Version 2.0 (the "License")              #
###############################################################################

#---------------- Install Options ----------------
prefix=/usr/local

INSTALL_BINDIR=$(prefix)/bin
INSTALL_DATDIR=$(prefix)/share
MSG_INSTALL = [INSTALL]
MSG_UNINSTALL = [UNINSTALL]

all:
	@echo "shell script project, only targets install and uninstall are available !"

install: uninstall
	@echo "$(MSG_INSTALL) utils in $(prefix)"
	@-install -m 0755 $(CURDIR)/git-version $(INSTALL_BINDIR)
	@-install -m 0755 $(CURDIR)/avrio-make $(INSTALL_BINDIR)
	@-install -m 0755 $(CURDIR)/import-arduino $(INSTALL_BINDIR)
	@-install -m 0755 $(CURDIR)/avrio-prj $(INSTALL_BINDIR)
	@sed -i -e "s#INSTALLED_TEMPLATE_DIR#$(INSTALL_DATDIR)/avrio/template#g" $(INSTALL_BINDIR)/avrio-prj

	@-install -d -m 0755 $(INSTALL_DATDIR)/avrio
	@-install -d -m 0755 $(INSTALL_DATDIR)/avrio/template

	@-install -d -m 0755 $(INSTALL_DATDIR)/avrio/template/subdir
	@-install -m 0644 $(CURDIR)/../template/subdir/* $(INSTALL_DATDIR)/avrio/template/subdir

	@-install -d -m 0755 $(INSTALL_DATDIR)/avrio/template/avrio
	@-install -m 0644 $(CURDIR)/../template/avrio/* $(INSTALL_DATDIR)/avrio/template/avrio

	@-install -d -m 0755 $(INSTALL_DATDIR)/avrio/template/arduino
	@-install -m 0644 $(CURDIR)/../template/arduino/* $(INSTALL_DATDIR)/avrio/template/arduino

	@-install -d -m 0755 $(INSTALL_DATDIR)/avrio/template/project
	@-install -m 0644 $(CURDIR)/../template/project/board.mk $(INSTALL_DATDIR)/avrio/template/project
	@-install -m 0644 $(CURDIR)/../template/project/template.atsln $(INSTALL_DATDIR)/avrio/template/project
	@-install -m 0644 $(CURDIR)/../template/project/template.workspace $(INSTALL_DATDIR)/avrio/template/project

	@-install -d -m 0755 $(INSTALL_DATDIR)/avrio/template/project/board
	@-install -m 0644 $(CURDIR)/../template/project/board/* $(INSTALL_DATDIR)/avrio/template/project/board

	@-install -d -m 0755 $(INSTALL_DATDIR)/avrio/template/project/test
	@-install -m 0644 $(CURDIR)/../template/project/test/Makefile $(INSTALL_DATDIR)/avrio/template/project/test
	@-install -d -m 0755 $(INSTALL_DATDIR)/avrio/template/project/test/led
	@-install -m 0644 $(CURDIR)/../template/project/test/led/* $(INSTALL_DATDIR)/avrio/template/project/test/led


uninstall:
	@echo "$(MSG_UNINSTALL) utils from $(prefix)"
	@-rm -fr $(INSTALL_DATDIR)/avrio
	@-rm -f $(INSTALL_BINDIR)/git-version
	@-rm -f $(INSTALL_BINDIR)/avrio-prj
	@-rm -f $(INSTALL_BINDIR)/avrio-make
	@-rm -f $(INSTALL_BINDIR)/import-arduino

.PHONY: all install uninstall
