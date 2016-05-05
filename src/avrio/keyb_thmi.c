/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
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
 *
 * @file keyb_thmi.c
 * @brief Clavier THMI
 */
#include "avrio-config.h"

#ifdef AVRIO_KEYB_ENABLE
/* ========================================================================== */
#include <avrio/delay.h>
#include <avrio/twi.h>
#include <avrio/keyb.h>
#include <avrio/queue.h>
#include <avr/interrupt.h>
#include "avrio-board-keyboard.h"

/* private variables ======================================================== */
QUEUE_STATIC_DECLARE (xKbBuffer, KEYB_THMI_BUFFERSIZE);

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
ISR(KEYB_THMI_HIRQ_vect) {
  uint8_t ucButton;
  
  (void) eTwiRead (KEYB_THMI_ADDR, &ucButton);
  if ((ucButton) && !xQueueIsFull(&xKbBuffer))
    vQueuePush (&xKbBuffer, ucButton);
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vKeybInit (void) {

  // Configuration de la broche relié à HIRQ en entrée sans R pullup
  KEYB_THMI_HIRQ_DDR  &= ~_BV(KEYB_THMI_HIRQ);
  KEYB_THMI_HIRQ_PORT &= ~_BV(KEYB_THMI_HIRQ);
  // Attente mise à 1 de HIRQ (fin d'initialisation)
  while ((KEYB_THMI_HIRQ_PIN & _BV(KEYB_THMI_HIRQ)) == 0)
    delay_ms(50);

  vKeybIrqEnable();
  sei();
}

// -----------------------------------------------------------------------------
uint16_t
usKeybHit (void) {

  return xQueueLength(&xKbBuffer);
}

// -----------------------------------------------------------------------------
void
vKeybFlush (void) {

  vQueueFlush (&xKbBuffer);
}

// -----------------------------------------------------------------------------
char
cKeybGetChar (void) {
  
  while (usKeybHit () == 0)
    ;

  return (char) ucQueuePull(&xKbBuffer);
}

/* avr-libc stdio interface ================================================= */
static int
prvGetChar (FILE * pxStream) {

  return (unsigned int) cKeybGetChar ();
}

FILE xKeyb = FDEV_SETUP_STREAM (NULL, prvGetChar, _FDEV_SETUP_READ);

#endif /* AVRIO_KEYB_ENABLE defined */
/* ========================================================================== */
