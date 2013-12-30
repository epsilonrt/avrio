# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
# Note:
# If BOARD is empty in the Makefile, the user can describe his board in a
# board.mk file that will be in the same directory as the Makefile.
#----------------------------------------------------------------------------

#############################################################################
#                         TEMPLATE BOARD                             #
#############################################################################

# AVRIO BOARD directory
AVRIOBRDDIR = $(PROJECT_TOPDIR)/board
# MCU name
ifeq ($(MCU),)
MCU = atmega328p
endif

# Processor frequency.
#     This will define a symbol, F_CPU, in all source code files equal to the
#     processor frequency. You can then use this symbol in your source code to
#     calculate timings. Do NOT tack on a 'UL' at the end, this will be done
#     automatically to create a 32-bit value in your source code.
#     Typical values are:
#         F_CPU =  1000000
#         F_CPU =  1843200
#         F_CPU =  2000000
#         F_CPU =  3686400
#         F_CPU =  4000000
#         F_CPU =  7372800
#         F_CPU =  8000000
#         F_CPU = 11059200
#         F_CPU = 14745600
#         F_CPU = 16000000
#         F_CPU = 18432000
#         F_CPU = 20000000
ifeq ($(F_CPU),)
F_CPU = 16000000
endif
#----------------------------------------------------------------------------
