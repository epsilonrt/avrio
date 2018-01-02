/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file eeprom.h
 * @brief Gestion de la mémoire EEPROM.
 */
#ifndef _AVRIO_EEPROM_H_
#define _AVRIO_EEPROM_H_

#include <avrio/defs.h>
#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
#include <avr/eeprom.h>

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup eeprom_module Mémoire EEPROM
 *
 *  Ce module permet de gérer le stockage en mémoire EEPROM de façon sécurisé.\n
 *  Au moment du stockage, un octet de CRC est ajouté à la suite des octets
 *  mémorisés. Au moment de la lecture, le CRC est vérifié et une erreur est
 *  renvoyée par iEepromLoadBlock().
 *  @{
 * @example sys/memory/eeprom/demo_eeprom.c
 */

/* constants ================================================================ */
/**
 * @brief Taille de la mémoire EEPROM du MCU
 */
#define EEPROM_SIZE (E2END + 1)

/* internal public functions ================================================ */
/**
 * @brief Sauvegarde d'un bloc en EEPROM
 *
 * La sauvegarde est sécurisée par l'ajout d'un CRC 8 bits ajouté après le bloc
 * destination en EEPROM.
 *
 * @param pvSrcRam Adresse du 1er octet du bloc en mémoire vive à sauvegarder
 * @param pvDstEem Adresse du 1er octet du bloc de sauvegarde
 * @param xSize Nombre d'octets du bloc à sauvegarder
 * \note La taille réservée en EEPROM doit être de xSize + 1 (un octet de plus
 * pour le CRC)
 */
void vEepromSaveBlock (const void *pvSrcRam, void *pvDstEem, size_t xSize);

/**
 * @brief Sauvegarde d'un bloc en EEPROM
 *
 * La restauration est sécurisée par l'ajout d'un CRC 8 bits ajouté après le
 * bloc destination en EEPROM.
 *
 * @param pvDstRam Adresse du 1er octet du bloc en mémoire vive de restauration
 * @param pvSrcEem Adresse du 1er octet du bloc à restaurer
 * @param xSize Nombre d'octets du bloc à restaurer
 * @return 0 succès, -1 erreur de CRC
 * \note Le dernier octet du bloc source contient le CRC.
 */
int iEepromLoadBlock (void *pvDstRam, const void *pvSrcEem, size_t xSize);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif /* _AVRIO_EEPROM_H_ */
