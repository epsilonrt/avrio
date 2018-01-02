/**
 * Copyright © 20175 epsilonRT. All rights reserved.
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
 * @file tws.h
 * @brief Bus I2C avec coupleur TWS
 *
 *
 * Revision History ------------------------------------------------------------
 *    20170402 - Initial version by epsilonRT from AVR290 8380A-AVR-03/11
 */
#ifndef _AVRIO_TWS_H_
#define _AVRIO_TWS_H_

#include <avrio/defs.h>
#include <avr/interrupt.h>
#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup tws_module Bus I2C esclave avec coupleur TWS
 *
 *  Ce module permet de gérer un esclave I2C avec un coupleur de type TWS.
 *  @{
 * @example net/i2c/slave/tws/sensor/demo_i2c_slave_tws_sensor.c
 * Ce programme simule un capteur I2C. Il utilise le module TWS présent sur 
 * les MCU AtTiny20/40/441/841/1634. \n
 * Le maître peut faire un accès en lecture simple, ou en lecture-écriture de
 * façon à accèder à un registre particulier. Chaque valeur mesurée est stockée
 * dans un registre 16 bits, octet de poids faible en premier. Le programme
 * procède à une incrémentation d'une des valeurs toutes les 2 secondes.
 * @example net/i2c/slave/tws/sensor-sleep/demo_i2c_slave_tws_sensor_sleep.c
 * Ce programme simule un capteur I2C. Il utilise le module TWS présent sur 
 * les MCU AtTiny20/40/441/841/1634. \n
 * Le maître peut faire un accès en lecture simple, ou en lecture-écriture de
 * façon à accèder à un registre particulier. Chaque valeur mesurée est stockée
 * dans un registre 16 bits, octet de poids faible en premier. \n 
 * Le programme utilise le chien de garde pour cadencer les séquences de mise
 * en sommeil. \n
 * Sans traffic I2C, le MCU est réveillé toutes les 8 secondes. \n
 * Le timer chien de garde est remis à zéro à chaque échange I2C avec un délai
 * de 15ms. Le MCU sera donc endormi 15ms après le dernier échange I2C pour 8s. \n
 * Une incrémentation de tous les registres capteurs est effectuée toutes les 4 
 * interruptions chien de garde (simulation d'une évolution capteur). \n
 * LED1 est allumée lorsque le MCU est réveillé. \n
 */

/**
 * @brief Gestionnaire de transmission au maître
 * 
 * Cette fonction doit être implémentée par l'utilisateur, elle est appelée par
 * le module lorsque le maitre de bus demande à lire une valeur. \n
 * Cette fonction est exécutée sous interruption, il faut donc être très bref,
 * typiquement, cette fonction devrait retourner une valeur stockée dans un buffer,
 * cette valeur étant mise à jour de façon asynchrone dans la boucle principale
 * du main(). Il est hors de question de faire appel à des fonctions d'accès
 * au matériel utilisant la scrutation (ADC, USART, SPI, ONEWIRE ...).
 * 
 * @return l'octet à renvoyer au maitre
 */
uint8_t ucTwsToMaster (void);


/**
 * @brief Gestionnaire de réception au maître
 * 
 * Cette fonction doit être implémentée par l'utilisateur, elle est appelée par
 * le module lorsque le maitre de bus transmet une valeur. \n
 * Cette fonction est exécutée sous interruption, il faut donc être très bref,
 * typiquement, cette fonction devrait stocker la valeur dans un buffer,
 * cette valeur étant utilisée de façon asynchrone dans la boucle principale
 * du main(). Il est hors de question de faire appel à des fonctions d'accès
 * au matériel utilisant la scrutation (ADC, USART, SPI, ONEWIRE ...).
 * 
 * @param data octet transmis par le maitre
 */
void ucTwsFromMaster (uint8_t data);

/**
 * @brief Vérification et acquittement des erreurs bus I2C
 * 
 * Cette fonction vérifie si il y a eu des erreurs bus I2C, si c'est le cas, 
 * elle réinitialise le module TWS.
 */
void vTwsBusErrorCheck (void);

/**
 *   @}
 * @}
 */
#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Initialise l'interface TWS
 *
 * Les interruptions doivent être validées pour que le module fonctionne.
 * @param Adresse I2C à laquelle l'esclave doit répondre (alignée à gauche)
 */
static inline void vTwsInit (uint8_t ucOwnAddress);

#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
#include <avr/io.h>
#include <avr/interrupt.h>

// -----------------------------------------------------------------------------
INLINE void 
vTwsInit (uint8_t ucOwnAddress) {
  
  TWSA = ucOwnAddress;    // Set TWI slave address.
  TWSD = 0xFF;            // Default content = SDA released.
  TWSCRB = _BV(TWHNM);
  TWSCRA = (uint8_t) (_BV (TWEN) | _BV (TWSHE) |    // Enable TWI-interface and release TWI pins.
                      _BV (TWDIE) | _BV (TWASIE) | _BV (TWSIE)); // Enable TWI  Interupts.
}
#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif /* _AVRIO_TWS_H_ */
