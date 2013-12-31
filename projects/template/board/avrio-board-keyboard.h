/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_KEYBOARD_H_
#  define _AVRIO_BOARD_KEYBOARD_H_
/* ========================================================================== */

/* KEYBOARD ================================================================= */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */
/*
 * Configuration du driver
 */
#  define  KEYB_DRIVER_SETUP (KEYB_IFACE_PIO + KEYB_IFACE_USORTED)

/*
 * Nombre de lignes du clavier
 */

#  define KEYB_HEIGHT 4
/*
 * Nombre de colonnes du clavier
 */
#  define KEYB_WIDTH  4

/*
 * Tableau de decodage du clavier
 * Le tableau est organisé en lignes et en colonnes
 * Le code en haut à gauche correspond à la colonne 0 et à la ligne 0
 */
#  define KEYB_ARRAY { \
  { '1',         '2',   '3',        KEY_HOME  },\
  { '4',         '5',   '6',        KEY_UP    },\
  { '7',         '8',   '9',        KEY_DOWN  },\
  { KEY_LEFT,    '0',    KEY_RIGHT, KEY_ENTER }}

/*
 * Durée de l'anti-rebond du clavier en ms
 * Une touche doit être maintenue enfoncée sur une durée supérieure
 */
#  define KEYB_DEBOUNCE_DELAY 30

/*
 * Durée de répétition du clavier en ms
 */
#  define KEYB_REPEAT_DELAY   250

/*
 * Taille du buffer clavier
 * Une taille de 0 dévalide l'utilisation du buffer
 */
#  define KEYB_BUFSIZE 4

/*
 * Lignes et colonnes dans le désordre
 * KEYB_IFACE_USORTED définie
 */
#  define KEYB_C1_DDR  DDRD
#  define KEYB_C1_PORT PORTD
#  define KEYB_C1_PIN  PIND
#  define KEYB_C1_BIT  0

#  define KEYB_C2_DDR  DDRD
#  define KEYB_C2_PORT PORTD
#  define KEYB_C2_PIN  PIND
#  define KEYB_C2_BIT  2

#  define KEYB_C3_DDR  DDRD
#  define KEYB_C3_PORT PORTD
#  define KEYB_C3_PIN  PIND
#  define KEYB_C3_BIT  4

#  define KEYB_C4_DDR  DDRD
#  define KEYB_C4_PORT PORTD
#  define KEYB_C4_PIN  PIND
#  define KEYB_C4_BIT  5

#  define KEYB_R1_DDR  DDRD
#  define KEYB_R1_PORT PORTD
#  define KEYB_R1_PIN  PIND
#  define KEYB_R1_BIT  7

#  define KEYB_R2_DDR  DDRD
#  define KEYB_R2_PORT PORTD
#  define KEYB_R2_PIN  PIND
#  define KEYB_R2_BIT  6

#  define KEYB_R3_DDR  DDRD
#  define KEYB_R3_PORT PORTD
#  define KEYB_R3_PIN  PIND
#  define KEYB_R3_BIT  3

#  define KEYB_R4_DDR  DDRD
#  define KEYB_R4_PORT PORTD
#  define KEYB_R4_PIN  PIND
#  define KEYB_R4_BIT  1

/* ========================================================================== */
#endif /* _AVRIO_BOARD_KEYBOARD_H_ */
