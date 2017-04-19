/*
 * Copyright © 2017 epsilonRT, All rights reserved.
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
#ifndef _AVRIO_BOARD_OW_H_
#define _AVRIO_BOARD_OW_H_

#include <avrio/defs.h>
#include <avr/io.h>

#ifdef OW_DRIVER_POLLED_PIO
/*******************************************************************************
 *                  Driver with PIO pin in polling                             *
 ******************************************************************************/
//#define OW_USE_INTERNAL_PULLUP

/**
 * @brief Tirer le bus à l'état bas
 */
INLINE void 
vOwBusPullLow (void) {
  
  DDRB |= _BV(1);
  PORTB &= ~_BV(1);
}

/**
 * @brief Libère le bus OneWire
 * 
 * Remets le bus en entrée avec résistance de pull-up activée si OWI_USE_INTERNAL_PULLUP
 * est défini.
 */
INLINE void 
vOwBusRelease (void) {
  DDRB &= ~_BV(1);
#ifdef OWS_USE_INTERNAL_PULLUP
  PUEB |= _BV(1);
#endif
  PORTB |= _BV(1);
}

/**
 * @brief Lecture de l'état binaire du bus OneWire
 * 
 * @return true si à l'état haut, false sinon
 */
INLINE bool
bOwBusRead (void) {
  
  return (PINB & _BV(1)) != 0;
}

// Timing parameters
#define     OWS_DELAY_OFFSET    (13 * 1E6 / F_CPU)   //!< Délai lors de la mise à l'état bas ou du relachement

// Bit timing delays in us
#define     OWS_DELAY_A_STD_MODE    (6   - OWS_DELAY_OFFSET)
#define     OWS_DELAY_B_STD_MODE    (64  - OWS_DELAY_OFFSET)
#define     OWS_DELAY_C_STD_MODE    (60  - OWS_DELAY_OFFSET)
#define     OWS_DELAY_D_STD_MODE    (10  - OWS_DELAY_OFFSET)
#define     OWS_DELAY_E_STD_MODE    (9   - OWS_DELAY_OFFSET)
#define     OWS_DELAY_F_STD_MODE    (55  - OWS_DELAY_OFFSET)
//#define     OWS_DELAY_G_STD_MODE  (0   - OWS_DELAY_OFFSET)
#define     OWS_DELAY_H_STD_MODE    (480 - OWS_DELAY_OFFSET)
#define     OWS_DELAY_I_STD_MODE    (70  - OWS_DELAY_OFFSET)
#define     OWS_DELAY_J_STD_MODE    (410 - OWS_DELAY_OFFSET)

#endif /* OW_DRIVER_POLLED_PIO defined */

#ifdef OW_DRIVER_POLLED_UART
/*******************************************************************************
 *            Driver with UART in polling or interrupt                         *
 ******************************************************************************/

/**
 * @brief Active le mode double vitesse
 * Les valeurs OWU_UBRR_115200 et OWU_UBRR_9600 devront être modifiées en fonction
 */
#define     OWU_UART_2X         1 ///< F_CPU 8MHz

/**
 * @brief Active le déplacement des broches TXD/RXD
 * Cette fonctionnalité n'est présente que sur certains MCU récents (tiny841...)
 */
#define OWU_UART_REMAP  1

/**
 * Valeur du registre UBRR pour un baudrate de 115200 bd
 */
#define     OWU_UBRR_115200  8 ///< F_CPU 8MHz, U2X, Error -3.5% 


/**
 * Valeur du registre UBRR pour un baudrate de 9600 bd
 */
#define     OWU_UBRR_9600   103 ///< F_CPU 8MHz, U2X, Error 0.2% 

// -----------------------------------------------------------------------------
// Mappage des registres UART en fonction du MCU
// Inutile de modifier quelque chose en dessous de cette ligne sauf si
// vous souhaitez ajouter la prise en charge d'un nouveau MCU
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#if defined(__AVR_ATtiny2313__)
// -----------------------------------------------------------------------------
#define OWU_UCSRA    UCSRA
#define OWU_UCSRB    UCSRB
#define OWU_UCSRC    UCSRC
#define OWU_UDR      UDR
#define OWU_UBRRL    UBRRL
#define OWU_UBRRH    UBRRH

#define OWU_U2X                     U2X
#define OWU_RXEN                    RXEN
#define OWU_TXEN                    TXEN
#define OWU_RXCIE                   RXCIE
#define OWU_UCSZ1                   UCSZ1
#define OWU_UCSZ0                   UCSZ0
#define OWU_UDRIE                   UDRIE
#define OWU_FE                      FE
#define OWU_RXC                     RXC

#define OWU_UART_RXC_vect           USART0_RX_vect
#define OWU_UART_UDRE_vect          USART0_UDRE_vect


#elif defined(__AVR_ATmega8__) | defined(__AVR_ATmega16__) | \
    defined(__AVR_ATmega32__)
// -----------------------------------------------------------------------------
#define OWU_UCSRA     UCSRA
#define OWU_UCSRB     UCSRB
#define OWU_UCSRC     UCSRC
#define OWU_UDR      UDR
#define OWU_UBRRL    UBRRL
#define OWU_UBRRH    UBRRH

#define OWU_U2X                     U2X
#define OWU_RXEN                    RXEN
#define OWU_TXEN                    TXEN
#define OWU_RXCIE                   RXCIE
#define OWU_UCSZ1                   UCSZ1
#define OWU_UCSZ0                   UCSZ0
#define OWU_UDRIE                   UDRIE
#define OWU_FE                      FE
#define OWU_URSEL                   URSEL
#define OWU_RXC                     RXC

#define OWU_UART_RXC_vect           USART_RXC_vect
#define OWU_UART_UDRE_vect          USART_UDRE_vect

#elif defined(__AVR_ATmega48__) | defined(__AVR_ATmega88__)   | \
    defined(__AVR_ATmega168__)  | defined(__AVR_ATmega168P__) | \
    defined(__AVR_ATmega328__)  | defined(__AVR_ATmega328P__) | \
    defined(__AVR_ATmega328PB__)
// -----------------------------------------------------------------------------
#define OWU_UCSRA    UCSR0A
#define OWU_UCSRB    UCSR0B
#define OWU_UCSRC    UCSR0C
#define OWU_UDR      UDR0
#define OWU_UBRRL    UBRR0L
#define OWU_UBRRH    UBRR0H

#define OWU_U2X                     U2X0
#define OWU_RXEN                    RXEN0
#define OWU_TXEN                    TXEN0
#define OWU_RXCIE                   RXCIE0
#define OWU_UCSZ1                   UCSZ01
#define OWU_UCSZ0                   UCSZ00
#define OWU_UDRIE                   UDRIE0
#define OWU_FE                      FE0
#define OWU_RXC                     RXC0

#define OWU_UART_RXC_vect           USART_RX_vect
#define OWU_UART_UDRE_vect          USART_UDRE_vect

#elif defined(__AVR_ATmega169__)
// -----------------------------------------------------------------------------
#define OWU_UCSRA     UCSR0A
#define OWU_UCSRB     UCSR0B
#define OWU_UCSRC     UCSR0C
#define OWU_UDR      UDR0
#define OWU_UBRRL    UBRR0L
#define OWU_UBRRH    UBRR0H

#define OWU_U2X                     U2X0
#define OWU_RXEN                    RXEN0
#define OWU_TXEN                    TXEN0
#define OWU_RXCIE                   RXCIE0
#define OWU_UCSZ1                   UCSZ01
#define OWU_UCSZ0                   UCSZ00
#define OWU_UDRIE                   UDRIE0
#define OWU_FE                      FE0
#define OWU_RXC                     RXC0

#define OWU_UART_RXC_vect           USART0_RXC_vect
#define OWU_UART_UDRE_vect          USART0_UDRE_vect

#elif defined(__AVR_ATmega64__) | defined(__AVR_ATmega128__) | \
    defined(__AVR_ATtiny441__) | defined(__AVR_ATtiny841__)
// -----------------------------------------------------------------------------
#define OWU_UCSRA    UCSR0A
#define OWU_UCSRB    UCSR0B
#define OWU_UCSRC    UCSR0C
#define OWU_UDR      UDR0
#define OWU_UBRRL    UBRR0L
#define OWU_UBRRH    UBRR0H

#define OWU_U2X                     U2X0
#define OWU_RXEN                    RXEN0
#define OWU_TXEN                    TXEN0
#define OWU_RXCIE                   RXCIE0
#define OWU_UCSZ1                   UCSZ01
#define OWU_UCSZ0                   UCSZ00
#define OWU_UDRIE                   UDRIE0
#define OWU_FE                      FE0
#define OWU_RXC                     RXC0

#define OWU_UART_RXC_vect           USART0_RX_vect
#define OWU_UART_UDRE_vect          USART0_UDRE_vect

#if defined(__AVR_ATtiny441__) | defined(__AVR_ATtiny841__)
#define OWU_UMAP U0MAP
#endif

#else
#error You could update this file to support this MCU ?
#endif

#endif /* OW_DRIVER_POLLED_UART defined */

/* ========================================================================== */
#endif /* _AVRIO_BOARD_OW_H_ defined */

