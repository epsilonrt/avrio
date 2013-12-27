# Hey Emacs, this is a -*- makefile -*-
#-------------------------------------------------------------------------------
# $Id$

AVRIO_CONFIG += AVRIO_MBMASTER_ENABLE

MBM_TOPDIR  = $(AVRIOSRCDIR)/mbmaster
MBM_SRCDIR  = $(MBM_TOPDIR)/mbmaster
MBM_PORTDIR = $(AVRIOSRCDIR)/avrio/mbmaster
MBM_TSTDIR = $(MBM_PORTDIR)/mbmtest

MBM_TOP_SRCS 	   = mbm.c common/mbutils.c
MBM_ASCII_SRCS   = ascii/mbmascii.c
MBM_RTU_SRCS 	   = rtu/mbmrtu.c
MBM_FUNC_SRCS 	 = functions/mbmfunccoils.c functions/mbmfuncinput.c
MBM_FUNC_SRCS 	+= functions/mbmfuncholding.c functions/mbmfuncraw.c
MBM_FUNC_SRCS 	+= functions/mbmfunccustom1.c functions/mbmfuncdisc.c
MBM_FUNC_SRCS 	+= functions/mbmfuncslaveid.c 
MBM_TCP_SRCS 	   = tcp/mbmtcp.c

MBM_PORT_SRCS = mbcrc.c mbportevent.c mbportother.c mbporttimer.c
# conditionnal port sources
ifeq ($(MCU),atmega128)
MBM_PORT_SRCS += mbportserial_m128.c
else
ifeq ($(MCU),atmega168)
MBM_PORT_SRCS += mbportserial_m168.c
else
ifeq ($(MCU),atmega2561)
MBM_PORT_SRCS += mbportserial_m2561.c
else
ifeq ($(MCU),atmega32)
MBM_PORT_SRCS += mbportserial_m32.c
else
ifeq ($(MCU),atmega328p)
MBM_PORT_SRCS += mbportserial_m328p.c
else
ifeq ($(MCU),atmega644p)
MBM_PORT_SRCS += mbportserial_m644p.c
else
$(error Error MODBUS Master does not support $(MCU))
erreur
endif
endif
endif
endif
endif
endif

MBM_SRCDIRS = $(MBM_SRCDIR):$(MBM_PORTDIR)

MBM_SRCS  = $(MBM_TOP_SRCS) $(MBM_ASCII_SRCS) $(MBM_RTU_SRCS) $(MBM_FUNC_SRCS) 
MBM_SRCS += $(MBM_TCP_SRCS) $(MBM_PORT_SRCS)

MBM_INCDIRS  = $(MBM_SRCDIR)/include $(MBM_SRCDIR)/ascii $(MBM_SRCDIR)/rtu
MBM_INCDIRS +=	$(MBM_SRCDIR)/tcp $(MBM_PORTDIR) $(MBM_TSTDIR)

#------------------------------------------------------------------------------
