/**
 * Copyright © 2017 epsilonRT. All rights reserved.
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
 * @file
 * @brief I²C Esclave avec module USI
 */
#ifndef _AVRIO_USI_SLAVE_H_
#define _AVRIO_USI_SLAVE_H_
#include <avrio/defs.h>
#include <avr/interrupt.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *  @defgroup usi_slave_module Bus I2C avec coupleur USI
 *
 *  Ce module permet de gérer un bus I2C avec un coupleur de type USI.
 *  @{
 * @example net/i2c/slave/usi/demo_i2c_slave_usi.c
 * Ce programme réalise un circuit esclave I2C qui peut être adressé par un
 * maître I2C (comme un Raspberry Pi). \n
 * Il utilise le module USI présent sur les MCU AtTiny.
 * Le maître va écrire sur le bus un octet à l'adresse 0x10, l'esclave
 * mémorise cet octet et bascule la LED1. Le maître peut alors lire un octet
 * à cette même adresse, sa valeur devrait être la même que l'octet écrit
 * précédement.
 */
/* constants ================================================================ */
/**
 * Le flag USIF_USE_SLEEP est utilisé pour activer l'endormissment
 */
#define USIF_USE_SLEEP 0

/* types ==================================================================== */
/**
 * @brief Prototype d'une fonction de gestion de réception
 * @param rx_buffer pointeur sur les octets reçus
 * @param rx_buffer_len nombre d'octets reçus
 */
typedef void (*ucUsiSlaveRxCallback) (const uint8_t *rx_buffer, uint8_t rx_buffer_len);

/* internal public functions ================================================ */
/**
 * @brief Initialisation du module
 *
 * Initialise le module USI en I²C esclave.
 *
 * @param slave_addr notre adresse de esclave alignée à droite (sans bit R/W)
 * @param flags flags de configuration
 * @param tx_buffer buffer utilisé pour la transmission, l'utilisateur devra
 * stocker dans ce buffer les informations à transmettre au maître de bus I²C.
 * La tranmission de ces informations étant effectuée sous interruption, la
 * modification de ce buffer doit de faire de façon atomique.
 * @param tx_buffer_len taille du buffer de transmission
 * @param rx_cb fonction de gestion appelée lorsque le maître de bus transmet
 * des informations à notre esclave. Mettre NULL si aucune fonction n'est
 * prévue.
 */
void vUsiSlaveInit (uint8_t slave_addr, uint8_t flags, uint8_t *tx_buffer, uint8_t tx_buffer_len, ucUsiSlaveRxCallback rx_cb);

/**
 * @brief Fonction de scrutation
 *
 * Le module USI utilise les interruptions, la transmission des informations vers
 * le maître est automatique. Cette fonction est chargée
 * d'interroger le module USI pour savoir si des informations ont été reçues, si
 * c'est le cas, la fonction de gestion est appelée. Si le flag USIF_USE_SLEEP a
 * été activé lors de l'initialisation et qu'aucune information n'a été reçu, le
 * MCU est endormi. \n
 * Cette fonction doit être appelée aussi souvent que possible dans la boucle
 * du programme principal.
 */
void vUsiSlavePoll (void);

/**
 * @brief Activation du module
 *
 * Le module est activé à l'issue de l'initialisation, il peut être désactivé
 * puis réactivé à l'aide de cette fonction
 * @param enable true pour activer
 */
void vUsiSlaveEnable (bool enable);

/**
 * @brief Modification de l'index du buffer de transmission
 *
 * L'index du buffer de transmission est incrémenté automatiquement à chaque
 * transmission d'un octet au maître. Arrivé à la fin du buffer de transmission,
 * l'index est remis à 0. Cette fonction permet de modifier la valeur de l'index
 * du buffer de transmission, en particulier, si le mécanisme write-restart-read
 * doit être implémenté.
 * @param index nouvel index, la modification se fera modulo la taille du buffer
 * de transmission
 */
void vUsiSlaveSetTxBufferIndex (uint8_t index);

/**
 *   @}
 * @}
 */
/* ========================================================================== */
__END_C_DECLS

#endif
