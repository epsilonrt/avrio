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
 * @file serial_sw.h
 * @brief Liaison série asynchrone logiciele
 * @author Atmel Corporation: http://www.atmel.com (AVR274 application note)
 */
#ifndef _AVRIO_SERIAL_SW_H_
#  define _AVRIO_SERIAL_SW_H_
/* *INDENT-OFF* */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup serial_sw_module Liaison série asynchrone logicielle
 *
 *  Ce module permet de gérer la communication sur une liaison série asynchrone
 *  à l'aide d'une borche d'interruption externe et d'un timer 8 bits.
 *  La transmission s'effectue sur un fil de port quelconque mais la réception
 *  doit se faire sur une broche d'interruption (INTX) sur front descendant
 *  Il n'est pas possible de faire du FULL DUPLEX !
 *  @{
 *  @example serial/demo_serial.c
 *  Renvoie chaque caractère reçu sur la liaison série en basculant la LED1.
 */
#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */

#include <stdio.h>
#include <avr/interrupt.h>

/* Status register defines. */
#define SERIAL_SW_TX_BUFFER_FULL        4     ///< Set if data is ready to be sent from the Tx buffer.
#define SERIAL_SW_RX_BUFFER_FULL        5     ///< Set if data is ready to be received from the Rx buffer.
#define SERIAL_SW_RX_BUFFER_OVERFLOW    6     ///< Set if receive buffer is overflowed. Indicates data loss.
#define SERIAL_SW_FRAME_ERROR           7     ///< Set if a frame error has occured.

/* internal public functions ================================================ */

/**
 * @brief Envoie un caractère sur la liaison série
 * @param c caractère
 */
void vSerialSwPutChar (char c);

/**
 * Renvoie l'état de l'uart (true si la transmission est possible, false sinon)
 */
bool xSerialSwReady (void);

/**
 * @brief Envoie une chaîne caractères sur la liaison série
 * @param pcString chaîne caractères
 */
void vSerialSwPutString (const char *pcString);

/**
 * @brief Reçoit un caractère sur la liaison série
 * @return le caractère ou _FDEV_EOF
 */
int iSerialSwGetChar (void);

/**
 * @brief Renvoie le nombre de caractères reçus
 */
uint16_t usSerialSwHit (void);

/**
 * @brief Valide l'uart
 */
void vSerialSwEnable (void);

/**
 * @brief Valide l'uart
 */
void vSerialSwDisable (void);

/* public variables ========================================================= */
/**
 * @brief Descripteur de fichier de l'uart permettant son utilisation avec les fonctions de la libc.
 *
 * Pour utiliser l'uart comme entrée et sortie par défaut, on mettra au début
 * du main() :
 * @code
 * int main(void) {
 *   vSerialSwInit ();
 *   stdout = &xSerialSwPort;
 *   stdin  = &xSerialSwPort;
 * ...
 * @endcode
 * On pourra dans le cas contraire passer &xSerialSwPort au fonction fprintf, fscanf :
 * @code
 * int choix;
 * ...
 * fprintf (&xSerialSwPort, "Choix ? ");
 * fscanf (&xSerialSwPort, "%d", &choix);
 * ...
 * @endcode
 */
extern FILE xSerialSwPort;

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Initialise l'SERIAL
 */
inline void vSerialSwInit (void);

/**
 * @brief Renvoie les drapeaux de l'uart
 */
inline uint8_t ucSerialSwGetFlags (void);

  /**
   *   @}
   * @}
   */

#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
#include <avr/interrupt.h>
extern volatile uint8_t ucStatus; //< Byte holding status flags.

// -----------------------------------------------------------------------------
INLINE void
vSerialSwInit (void) {

  vSerialSwEnable();
}

// -----------------------------------------------------------------------------
INLINE uint8_t
ucSerialSwGetFlags (void) {

  return ucStatus;
}
#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_SERIAL_SW_H_ */
