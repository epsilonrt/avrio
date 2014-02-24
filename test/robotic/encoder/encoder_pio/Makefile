# Hey Emacs, this is a -*- makefile -*-
#----------------------------------------------------------------------------
# $Id$
# On command line:
#
# make all = Make software.
#
# make clean = Clean out built project files.
#
# make rebuild = rebuilt project.
#
# make coff = Convert ELF to AVR COFF.
#
# make extcoff = Convert ELF to AVR Extended COFF.
#
# make program = Download the hex file to the device, using avrdude.
#                Please customize the avrdude settings below first!
#
# make debug = Start either simulavr or avarice as specified for debugging,
#              with avr-gdb or avr-insight as the front end for debugging.
#
# make filename.s = Just compile filename.c into the assembler code only.
#
# make filename.i = Create a preprocessed source file for use in submitting
#                   bug reports to the GCC project.

# Nom du fichier cible (sans extension).
TARGET = test_encoder_pio

# Chemin relatif du répertoire racine d'AVRIO
AVRIO_TOPDIR = ../../../..

# Nom de la carte cible (voir le fichier avrio/boards.txt pour la liste)
# Si BOARD est vide, il faut un fichier board.mk pour définir les propriétés de
# la carte dans le répertoire où se trouve ce Makefile !
BOARD = MP2011_3
# Normalement F_CPU est définie dans board.mk ... Il est possible de la modifier
# dans le cas où la fréquence n'est pas fixée par un quartz (horloge interne...)
# F_CPU = 8000000

# Chemin relatif du répertoire racine du projet de l'utilisateur
PROJECT_TOPDIR = .

# Normalement le MCU est défini dans board.mk Il est possible de le modifier
# dans le cas où le modèle implanté (cas des cartes avec support) ne correspond
# pas.
# MCU = atmega32

# Valide le noyau multitâche temps réel AVRX (sous forme de bibliothèque)
AVRX = OFF

# Niveau d'optimisation de GCC =  [0, 1, 2, 3, s].
#     0 = pas d'optimisation (pour debug).
#     s = optimisation de la taille du code (pour release).
#     (Note: 3 n'est pas toujours le meilleur niveau. Voir la FAQ avr-libc.)
OPT = s

# Liste des fichiers source C. (Les dépendances sont automatiquement générées.)
# Le chemin d'accès des fichiers sources systèmes a été ajouté au chemin de 
# recherche du compilateur, il n'est donc pas nécessaire de préciser le chemin 
# d'accès complet du fichier mais seulement le nom du projet (avrio, avrx, ...)
SRC = $(TARGET).c avrio/encoder_irq.c avrio/task.c avrio/bdcm.c avrio/serial.c

# Liste des fichiers source C++. (Les dépendances sont automatiquement générées.)
# Le chemin d'accès des fichiers sources systèmes a été ajouté au chemin de 
# recherche du compilateur, il n'est donc pas nécessaire de préciser le chemin 
# d'accès complet du fichier mais seulement le nom du projet (avrio, avrx, ...)
CPPSRC =

# Liste des fichiers source assembleur
#   L'extenson doit toujours être .S (en majuscule). En effet, les fichiers .s
#   ne sont pas consédérés comme des fichiers sources mais comme des fichiers
#   générés par le compilateur et seront supprimés lors d'un make clean.
#   Cela est valable aussi sous DOS/Windows (bien que le système d'exploitation
#   ne soit pas sensible à la casse).
ASRC =

# Place -D or -U options here for C sources
CDEFS = -DAVRIO_TASK_ENABLE -DAVRIO_SERIAL_PORT=0

# Place -D or -U options here for ASM sources
ADEFS = 

# Place -D or -U options here for C++ sources
CPPDEFS = 

# List any extra directories to look for include files here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRA_INCDIRS = 

#---------------- Library Options ----------------
# List any extra directories to look for libraries here.
#     Each directory must be seperated by a space.
#     Use forward slashes for directory separators.
#     For a directory that has spaces, enclose it in quotes.
EXTRA_LIBDIRS = 

# STANDARD, MIN or FLOAT
PRINTF_VERSION = STANDARD

# STANDARD, MIN or FLOAT
SCANF_VERSION = STANDARD

# ON/OFF
MATH_LIB_ENABLE = OFF

#---------------- Programming Options (avrdude) ----------------

# Programming hardware
# Type: avrdude -c ?
# to get a full listing.
#
# STK200 programmer on parallel port
#AVRDUDE_PROGRAMMER = stk200
#AVRDUDE_PORT = lpt1

# JTAG ICE MkII
#AVRDUDE_PROGRAMMER  = jtag2
#AVRDUDE_PORT = usb


# AVR Dragon
AVRDUDE_PROGRAMMER = dragon_isp
#AVRDUDE_PROGRAMMER = dragon_jtag
#AVRDUDE_PROGRAMMER = dragon_dw
AVRDUDE_PORT = usb

# Arduino OnBoard Programmer
#AVRDUDE_PROGRAMMER = arduino
#AVRDUDE_PORT = /dev/ttyACM0
#AVRDUDE_PORT = COM4
#AVRDUDE_BAUDRATE = 115200

# ON/OFF
AVRDUDE_WRITE_EEPROM_ENABLE = OFF

#---------------- Debugging Options ----------------

# Set the DEBUG_UI to either gdb or insight.
# DEBUG_UI = gdb
DEBUG_UI = insight

# Set the debugging back-end to either avarice, simulavr.
DEBUG_BACKEND = avarice
#DEBUG_BACKEND = simulavr

# When using avarice settings for the JTAG
JTAG_DEV = usb

#------------------------------------------------------------------------------
include $(AVRIO_TOPDIR)/script/common.mk 
