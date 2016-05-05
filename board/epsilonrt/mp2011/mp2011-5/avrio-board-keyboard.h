/*
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
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
#  define  KEYB_DRIVER_SETUP (KEYB_IFACE_PIO + KEYB_IFACE_SORTED)

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
  { KEY_LEFT,    '0',   KEY_RIGHT,  KEY_ENTER }}

/*
 * Durée de l'anti-rebond du clavier en ms
 * Une touche doit être maintenue enfoncée sur une durée supérieure
 */
#  define KEYB_DEBOUNCE_DELAY 20

/*
 * Durée de répétition du clavier en ms
 */
#  define KEYB_REPEAT_DELAY   150

/*
 * Taille du buffer clavier
 * Une taille de 0 dévalide l'utilisation du buffer
 */
#  define KEYB_BUFSIZE 4

/*
 * Lignes et colonnes dans l'ordre
 * KEYB_LINES_UNSORTED non définie
 */
#  define KEYB_ROW_PORT PORTA
#  define KEYB_ROW_DDR  DDRA
#  define KEYB_ROW_PIN  PINA
#  define KEYB_ROW_LSB  0

#  define KEYB_COL_PORT PORTA
#  define KEYB_COL_DDR  DDRA
#  define KEYB_COL_PIN  PINA
#  define KEYB_COL_LSB  4

/* ========================================================================== */
#endif /* _AVRIO_BOARD_KEYBOARD_H_ */
