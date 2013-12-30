/* Copyright © 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _AVRIO_BOARD_KEYBOARD_H_
#  define _AVRIO_BOARD_KEYBOARD_H_
/* ========================================================================== */

/* KEYBOARD ================================================================= */
#  include <avrio/defs.h>
#  include <avr/io.h>

/* constants ================================================================ */

/* Adresse IĠC du THMI */
#define KEYB_THMI_ADDR THMI_BASE

/* Taille du buffer de réception */
#define KEYB_THMI_BUFFERSIZE  16

/* Broche reliée au signal HIRQ (à mettre en commentaire si non utilisé) */
#define KEYB_THMI_HIRQ 7

/* Registre PORT de la broche reliée au signal HIRQ */
#define KEYB_THMI_HIRQ_PORT PORTE

/* Registre DDR de la broche reliée au signal HIRQ */
#define KEYB_THMI_HIRQ_DDR  DDRE

/* Registre PIN de la broche reliée au signal HIRQ */
#define KEYB_THMI_HIRQ_PIN  PINE

/* Vecteur d'interruption */
#define KEYB_THMI_HIRQ_vect INT7_vect

/* inline public functions ================================================== */
/* Valide l'interruption */
static inline void
vKeybIrqEnable (void) {

  EIFR  |= _BV(INTF7);
  EIMSK |= _BV(INT7);
}

/* ========================================================================== */
#endif /* _AVRIO_BOARD_KEYBOARD_H_ */
