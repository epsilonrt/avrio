# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
# Current BOARD list :
#
# ------------- manufactured by BTS SE1 2010-2011 
# from Rouviere High School, Toulon, France - http://www.ac-nice.fr/rouviere/
# The high school where epsilonRT works...
#  * MP2011_2
#  * MP2011_3
#  * MP2011_4
#  * MP2011_5
#  * MP2011_6
#  * MP2011_7
#
# Note:
# If BOARD is empty in the Makefile, the user can describe his board in a 
# board.mk file that will be in the same directory as the Makefile.
#----------------------------------------------------------------------------

#############################################################################
#                             MP2011 BOARDS                                 #
#############################################################################

#----------------------------------------------------------------------------
ifeq ($(BOARD),MP2011_2)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/mp2011/mp2011-2

# MCU name
MCU = atmega32u4

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

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),MP2011_3)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/mp2011/mp2011-3

# MCU name
ifeq ($(MCU),)
MCU = atmega32
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
F_CPU = 14745600
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),MP2011_4)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/mp2011/mp2011-4

# MCU name
ifeq ($(MCU),)
MCU = atmega32
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
F_CPU = 14745600
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),MP2011_5)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/mp2011/mp2011-5

# MCU name
ifeq ($(MCU),)
MCU = atmega32
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
F_CPU = 14745600
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),MP2011_6)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/mp2011/mp2011-6

# MCU name
ifeq ($(MCU),)
MCU = attiny84
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
F_CPU = 8000000
endif

endif

#----------------------------------------------------------------------------
ifeq ($(BOARD),MP2011_7)

# AVRIO BOARD directory
AVRIOBRDDIR = $(AVRIO_TOPDIR)/board/epsilonrt/mp2011/mp2011-7

# MCU name
ifeq ($(MCU),)
MCU = atmega32
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

endif

#----------------------------------------------------------------------------
