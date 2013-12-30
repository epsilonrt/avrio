# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$

#############################################################################
#                              USER BOARDS                                  #
#############################################################################
include $(AVRIO_TOPDIR)/board/epsilonrt/mp2011/mp2011.mk
#include $(AVRIO_TOPDIR)/../pp2012/pp2012.mk
#include $(AVRIO_TOPDIR)/../msl/msl.mk
#include $(AVRIO_TOPDIR)/../thmi/thmi.mk
include $(AVRIO_TOPDIR)/../solarPi/tracker/solarpi-tracker.mk
