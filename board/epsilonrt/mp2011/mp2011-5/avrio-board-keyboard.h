/*
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
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
