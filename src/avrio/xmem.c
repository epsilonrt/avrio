/**
 * @file xmem.c
 * @brief Interface mémoire externe (Implémentation)
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120328 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_XMEM_ENABLE
/* ========================================================================== */
#include "xmem.h"
#include <avr/io.h>

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * Partie de code exécutée lors de la phase 3 de l'init.
 */
void
vXmemEnable (void) __attribute__ ((naked))
   __attribute ((section (".init3")));
void
vXmemEnable (void) {
 
#if __AVR_ARCH__ == 5 || __AVR_ARCH__ == 51

  /* Wait states */
  XMCRA = ((XMEM_SECTOR_LIMIT & 0x07) << SRL0) | 
          ((XMEM_LOWER_WAIT_STATES & 0x03) << SRW00) |
          ( XMEM_UPPER_WAIT_STATES & _BV(SRW11));
  
  /* Largeur du bus d'adresses et buskeeper */
  XMCRB = XMEM_BUS_SETUP & (_BV(XMBK)|_BV(XMM2)|_BV(XMM1)|_BV(XMM0));
  
  /* Configure les résistances de pull-up sur le bus de données */
#if XMEM_BUS_SETUP & XMEM_BUS_PULLUP_RESISTOR
# if   (XMEM_BUS_SETUP & 0x07) == XMEM_BUS_WIDTH_A15
  PORTA = 0xFF;
# elif (XMEM_BUS_SETUP & 0x07) == XMEM_BUS_WIDTH_A14
  PORTA = 0x7F;
# elif (XMEM_BUS_SETUP & 0x07) == XMEM_BUS_WIDTH_A13
  PORTA = 0x3F;
# elif (XMEM_BUS_SETUP & 0x07) == XMEM_BUS_WIDTH_A12
  PORTA = 0x1F;
# elif (XMEM_BUS_SETUP & 0x07) == XMEM_BUS_WIDTH_A11
  PORTA = 0x0F;
# elif (XMEM_BUS_SETUP & 0x07) == XMEM_BUS_WIDTH_A10
  PORTA = 0x07;
# elif (XMEM_BUS_SETUP & 0x07) == XMEM_BUS_WIDTH_A9
  PORTA = 0x03;
# endif /* (XMEM_BUS_SETUP & 0x07) == XMEM_BUS_WIDTH_A15 */
#endif /*  XMEM_BUS_SETUP & XMEM_BUS_PULLUP_RESISTOR */

  /* Validation de l'interface mémoire externe */
#if XMEM_UPPER_WAIT_STATES == 3 || XMEM_UPPER_WAIT_STATES == 2
  MCUCR = _BV (SRE) | _BV(SRW10);
#else
  MCUCR = _BV (SRE) | _BV(SRW10);
#endif /* XMEM_UPPER_WAIT_STATES == 3 || XMEM_UPPER_WAIT_STATES == 2 */

#elif __AVR_ARCH__ == 6
#error "This part of the source code is not finalized... TO DO !"

#endif /* __AVR_ARCH__ == 6 */

}


#endif /* AVRIO_XMEM_ENABLE defined */
/* ========================================================================== */
