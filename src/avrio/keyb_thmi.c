/**
 * @file keyb_thmi.c
 * @brief Clavier THMI
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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
