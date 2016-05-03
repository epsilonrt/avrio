/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
 *
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
