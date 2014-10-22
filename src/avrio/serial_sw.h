/**
 * @file serial_sw.h
 * @brief Liaison série asynchrone logiciele
 * @author Atmel Corporation: http://www.atmel.com (AVR274 application note)
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
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
 * @brief Envoie une chaîne caractères sur la liaison série
 * @param pcString chaîne caractères
 */
void vSerialSwPutString (const char *pcString);

/**
 * @brief Reçoit un caractère sur la liaison série
 * @return le caractère ou EOF
 */
int iSerialSwGetChar (void);

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
 * @brief Renvoie le nombre de caractères reçus
 */
inline uint16_t usSerialSwHit (void);

/**
 * Renvoie l'état de l'uart (true si la transmission est terminée, false sinon)
 */
bool xSerialSwReady (void);

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

// -----------------------------------------------------------------------------
INLINE bool
bSerialSwReady (void) {

  return (ucStatus & (1 << SERIAL_SW_TX_BUFFER_FULL)) == 0;
}

// -----------------------------------------------------------------------------
INLINE uint16_t
usSerialSwHit (void) {

  return (ucStatus & (1 << SERIAL_SW_RX_BUFFER_FULL)) != 0;
}
#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_SERIAL_SW_H_ */
