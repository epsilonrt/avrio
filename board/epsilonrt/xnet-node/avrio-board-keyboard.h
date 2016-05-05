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
