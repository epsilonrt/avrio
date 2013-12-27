# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$

SUBDIRS = idwarf  
LIBS = avrx

all: $(SUBDIRS) $(LIBS)
clean: $(SUBDIRS) $(LIBS)
rebuild: $(SUBDIRS) $(LIBS)
distclean: $(SUBDIRS) $(LIBS)

lib: $(LIBS)
cleanlib: $(LIBS)
rebuildlib: $(LIBS)
distcleanlib: $(LIBS)

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
	
$(LIBS):
ifeq ($(MAKECMDGOALS),)
	$(MAKE) -w -C $@ lib
else ifeq ($(MAKECMDGOALS),all)
	$(MAKE) -w -C $@ lib
else ifeq ($(MAKECMDGOALS),clean)
	$(MAKE) -w -C $@ cleanlib
else ifeq ($(MAKECMDGOALS),rebuild)
	$(MAKE) -w -C $@ rebuildlib
else ifeq ($(MAKECMDGOALS),distclean)
	$(MAKE) -w -C $@ distcleanlib
endif

.PHONY: all clean distclean rebuild lib cleanlib rebuildlib distcleanlib extcoff program debug elf hex eep lss sym $(SUBDIRS) $(LIBS)
