/*
 * @file avrio-board-wusb.h
 * @brief Partie matérielle WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111002 - Initial version by epsilonRT
 */
#ifndef _AVRIO_BOARD_WUSBIO_H_
#define _AVRIO_BOARD_WUSBIO_H_

#include <avrio/defs.h>
#include <avrio/spi.h>
#include <avr/io.h>
#include <avr/interrupt.h>

/* constants ================================================================ */
/*
  Compatible iDwaRF-Node
  SPI sur PORTB
  SS    = PB2
  PD    = PC0
  RESET = PC1
  IRQ   = PD2/INT0
 */
#define WUSBIO_NO_PIN (0)

/*
 * Signal nSS
 */
#define WUSBIO_SS_DDR   SPI_DDR
#define WUSBIO_SS_PORT  SPI_PORT
#define WUSBIO_SS       _BV(SPI_SS_BIT)

/*
 * Signaux nPD et nRESET
 */
#define WUSBIO_CTL_DDR  DDRC
#define WUSBIO_CTL_PORT PORTC
#define WUSBIO_RESET    _BV(0) /* Reset: Actif à 0 */
#define WUSBIO_PD       _BV(1) /* Power Down: Actif à 0 */

/* 
 * Signal IRQ
 */
#define WUSBIO_IRQ_DDR  DDRD
#define WUSBIO_IRQ_PORT PORTD
#define WUSBIO_IRQ   _BV(2)

/*
 * Vecteur d'interruption utilisé par le module WirelessUSB
 * Voir le paragraphe "Choosing the vector: Interrupt vector names" de la
 * documentation avr-libc :
 * http://www.nongnu.org/avr-libc/user-manual/group__avr__interrupts.html
 */
#  define WUSBIO_vect INT0_vect

/* 
 * Facteur de division d'horloge du bus SPI, si définie, le module WirelessUSB
 * initialise le module SPI maître avec cette valeur, sinon, il est du ressort 
 * de l'utilisateur d'initialiser le module SPI maître AVANT d'initialiser 
 * le module WirelessUSB avec une valeur conforme :
 * - t(SCK_CYC) = 476 ns min. donc fsck max. = 2.1 MHz
 */
#define WUSBIO_SPI_INIT SPI_DIV4

/* types ==================================================================== */
typedef uint8_t xWusbIoMask;

/* inline public functions ================================================== */
#define WUSBIO_CTL_PINS (WUSBIO_RESET | WUSBIO_PD)
#define WUSBIO_ALL_PINS (WUSBIO_SS | WUSBIO_CTL_PINS)

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(void
vWusbIoClear (xWusbIoMask xMask)) {

  WUSBIO_CTL_PORT |=  (xMask & WUSBIO_CTL_PINS);
  WUSBIO_SS_PORT  |=  (xMask & WUSBIO_SS);

}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(void
vWusbIoInit (void)) {

  /* Signaux de contrôle du module CYWM6935 */
  WUSBIO_SS_DDR  |=  (WUSBIO_SS);
  WUSBIO_CTL_DDR |=  (WUSBIO_CTL_PINS);
  vWusbIoClear (WUSBIO_ALL_PINS);

  /* Broche d'interruption du module CYWM6935 */
  WUSBIO_IRQ_DDR  &= ~WUSBIO_IRQ;  // en entrée
  WUSBIO_IRQ_PORT &= ~WUSBIO_IRQ; // sans résistance de pullup
  
  EICRA &= ~_BV(ISC00); // déclenche sur front descendant
  EICRA |=  _BV(ISC01); // déclenche sur front descendant
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(void
vWusbIoSet (xWusbIoMask xMask)) {

  WUSBIO_CTL_PORT &= ~(xMask & WUSBIO_CTL_PINS);
  WUSBIO_SS_PORT  &= ~(xMask & WUSBIO_SS);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(void
vWusbIoToggle (xWusbIoMask xMask)) {

  WUSBIO_CTL_PORT ^= (xMask & WUSBIO_CTL_PINS);
  WUSBIO_SS_PORT  ^= (xMask & WUSBIO_SS);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(void
vWusbIoIrqEnable (void)) {

  sbi (EIFR, INTF0); /* clear le flag d'it */
  sbi (EIMSK, INT0);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(void
vWusbIoIrqDisable (void)) {

  cbi (EIMSK, INT0);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE(bool
vWusbIoIrqIsTriggered (void)) {

  return bit_is_set (EIFR, INTF0);
}

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_WUSBIO_H_ not defined */
