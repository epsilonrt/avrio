/**
 * @file serial.h
 * @brief Liaison série asynchrone
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * @todo Intégration de eSerialFlag dans les fonctions et documentation des paramètres.
 */
#ifndef _AVRIO_SERIAL_H_
#  define _AVRIO_SERIAL_H_

#  include <avrio/defs.h>
#  include <avr/interrupt.h>

/* *INDENT-OFF* */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup serial_module Liaison série asynchrone
 *
 *  Ce module permet de gérer la communication sur une liaison série asynchrone
 *  à l'aide du coupleur USART.
 *  @{
 *  @example serial/demo_serial.c
 *  Renvoie chaque caractère reçu sur la liaison série en basculant la LED1.
 */
  /* constants ============================================================== */
#  define SERIAL_NOECHO   0x0000 /**< Pas d'écho (Défaut) */
#  define SERIAL_ECHO     0x0001 /**< Tout caractère reçu est renvoyé en écho */
#  define SERIAL_5BIT     0x0000 /**< 5 bits de données */
#  define SERIAL_6BIT     0x0002 /**< 6 bits de données */
#  define SERIAL_7BIT     0x0004 /**< 7 bits de données */
#  define SERIAL_8BIT     0x0006 /**< 8 bits de données (Défaut) */
#  define SERIAL_2STP     0x0008 /**< 2 bits de stop */
#  define SERIAL_1STP     0x0000 /**< 1 bit de stop (Défaut) */
#  define SERIAL_NONE     0x0000 /**< Pas de parité (Défaut) */
#  define SERIAL_EVEN     0x0020 /**< Parité paire */
#  define SERIAL_ODD      0x0030 /**< Parité impaire */
#  define SERIAL_WR       0x0040 /**< Validation en transmission */
#  define SERIAL_RD       0x0080 /**< Validation en réception */
#  define SERIAL_RW       0x00C0 /**< Validation en transmission et réception */
#  define SERIAL_NOBLOCK  0x0100 /**< Lecture non-bloquante */
#  define SERIAL_RTSCTS   0x0200 /**< Contrôle de flux matériel */

  /**
   * @def SERIAL_DEFAULT
   * @brief Configuration par défaut de la liaison série pour vSerialInit()
   */
#  define SERIAL_DEFAULT \
  (SERIAL_8BIT + SERIAL_1STP + SERIAL_NONE + SERIAL_NOECHO)

  /* macros ================================================================= */

  /**
   * @def SERIAL_BAUD_X1
   * @brief Calcule la valeur du registre BAUD à partir de la vitesse
   *
   * Il s'agit de la formule par défaut sans division de la fréquence (U2X=0)
   @ param usBaud Vitesse de transmission en KBd (96 = 9600 Bd)
   */
#  define SERIAL_BAUD_X1(usBaud) (AVRIO_CPU_FREQ / (1600UL * usBaud) - 1)

  /**
   * @def SERIAL_BAUD_X2
   * @brief Calcule la valeur du registre BAUD à partir de la vitesse
   *
   * Il s'agit de la formule avec division de la fréquence par 2 (U2X=1)
   @ param usBaud Vitesse de transmission en KBd (96 = 9600 Bd)
   */
#  define SERIAL_BAUD_X2(usBaud) (AVRIO_CPU_FREQ / (800UL * usBaud) - 1)

#  ifndef __ASSEMBLER__
  /* ======================================================================== */
__BEGIN_C_DECLS

#  include <stdio.h>

/* internal public functions ================================================ */
/**
 * @brief Initialise l'SERIAL
 * @param usBaud vitesse de transmission en centaines de bauds (usBaud = 96 pour 9600 bds par exemple)
 * @param ucFlags drapeaux de configuration
 */
void vSerialInit (uint16_t usBaud, uint16_t usFlags);

/**
 * @brief Modifie les drapeaux de configuration de l'uart
 */
void vSerialSetFlags (uint16_t usFlags);

/**
 * @brief Renvoie les drapeaux de configuration de l'uart
 */
uint16_t usSerialGetFlags (void);

/**
 * Renvoie l'état de l'uart (true si la transmission est terminée, false sinon)
 */
bool xSerialReady (void);

/**
 * @brief Modifie les drapeaux de validation en lecture et/ou écriture
 *
 * Cette fonction attend que l'uart est terminée la transmission en cours avant
 * de basculer son état.
 */
void vSerialEnable (uint16_t usFlags);

/**
 * @brief Envoie un caractère sur la liaison série
 * @param c caractère
 * @return le caractère écrit ou EOF (dans le cas d'une version non-bloquante)
 */
int iSerialPutChar (char c);

/**
 * @brief Envoie une chaîne caractères sur la liaison série
 * @param pcString chaîne caractères
 */
void vSerialPutString (const char *pcString);

/**
 * @brief Reçoit un caractère sur la liaison série
 * @return le caractère ou EOF
 */
int iSerialGetChar (void);

/**
 * @brief Renvoie le nombre de caractères reçus
 */
uint16_t usSerialHit (void);

/* public variables ========================================================= */
/**
 * @brief Descripteur de fichier de l'uart permettant son utilisation avec les fonctions de la libc.
 *
 * Pour utiliser l'uart comme entrée et sortie par défaut, on mettra au début
 * du main() :
 * @code
 * int main(void) {
 *   vSerialInit (96, SERIAL_DEFAULT);
 *   stdout = &xSerialPort;
 *   stdin  = &xSerialPort;
 * ...
 * @endcode
 * On pourra dans le cas contraire passer &xSerialPort au fonction fprintf, fscanf :
 * @code
 * int choix;
 * ...
 * fprintf (&xSerialPort, "Choix ? ");
 * fscanf (&xSerialPort, "%d", &choix);
 * ...
 * @endcode
 */
extern FILE xSerialPort;

/* ========================================================================== */
__END_C_DECLS
#  endif /* __ASSEMBLER__ not defined */

  /**
   *   @}
   * @}
   */
/* *INDENT-ON* */
#endif /* _AVRIO_SERIAL_H_ */
