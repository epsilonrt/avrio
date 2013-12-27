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
#  define  KEYB_DRIVER_SETUP (KEYB_IFACE_PCF8574 + KEYB_IFACE_SORTED)

/*
 * Nombre de lignes du clavier
 */

#  define KEYB_HEIGHT 4

/*
 * Nombre de colonnes du clavier
 */
#  define KEYB_WIDTH  3

/*
 * Indice du premier bit des lignes sur le port (dans l'ordre)
 */
#  define KEYB_ROW_LSB  0

/*
 * Indice du premier bit des colonnes sur le port (dans l'ordre)
 */
#  define KEYB_COL_LSB  4

/*
 * Vitesse du bus I2c en KHz, si définie, le module LCD initialise le module
 * TWI maître avec cette vitesse, sinon, il est du ressort de l'utilisateur
 * d'initialiser le module TWI maître AVANT d'initialiser le module LCD.
 */
//#  define KEYB_TWI_MASTER_INIT 400

/*
 * Adresse I2c du PCF8574 contrôlant le clavier 
 */
#  define KEYB_TWI_DEVICE_ADDR 0x70

/*
 * Tableau de decodage du clavier
 * Le tableau est organisé en lignes et en colonnes
 * Le code en haut à gauche correspond à la colonne 0 et à la ligne 0
 */
#  define KEYB_ARRAY { \
  { '1', '2', '3' },\
  { '4', '5', '6' },\
  { '7', '8', '9' },\
  { '*', '0', '#' }}

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

/* ========================================================================== */
#endif /* _AVRIO_BOARD_KEYBOARD_H_ */
