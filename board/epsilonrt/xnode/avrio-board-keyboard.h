/* Copyright � 2009-2012 epsilonRT. All rights reserved.
 *  $Id: avrio-board-keyboard.h 24 2013-02-09 14:57:22Z pjean $ */
#ifndef _AVRIO_BOARD_KEYBOARD_H_
#  define _AVRIO_BOARD_KEYBOARD_H_
/* ========================================================================== */

/* KEYBOARD ================================================================= */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */

/* Adresse I�C du THMI */
#define KEYB_THMI_ADDR THMI_BASE

/* Taille du buffer de r�ception */
#define KEYB_THMI_BUFFERSIZE  16

/* Broche reli�e au signal HIRQ (� mettre en commentaire si non utilis�) */
#define KEYB_THMI_HIRQ 2

/* Registre PORT de la broche reli�e au signal HIRQ */
#define KEYB_THMI_HIRQ_PORT PORTD

/* Registre DDR de la broche reli�e au signal HIRQ */
#define KEYB_THMI_HIRQ_DDR  DDRD

/* Registre PIN de la broche reli�e au signal HIRQ */
#define KEYB_THMI_HIRQ_PIN  PIND

/* Vecteur d'interruption */
#define KEYB_THMI_HIRQ_vect INT0_vect

/* inline public functions ================================================== */
/* Valide l'interruption */
static inline void
vKeybIrqEnable (void) {

  EIFR  |= _BV(INTF0);
  EIMSK |= _BV(INT0);
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_KEYBOARD_H_ */
