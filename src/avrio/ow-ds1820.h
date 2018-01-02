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
 * @brief
 */
#ifndef _AVRIO_OW_DS1820_H_
#define _AVRIO_OW_DS1820_H_

#include <avrio/defs.h>
#include <avrio/ow.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
/**
 * @addtogroup ow_module
 * @{
 *
 *  @defgroup ow_ds1820 Circuit DS1820 et DS18B20
 *  @{
 */

/* constants ================================================================ */
#define DS1820_FAMILY_ID   0x10 ///< Identifiant DS1820
#define DS1820_LSB         0.5  ///< Résolution en °C du DS1820

#define DS18S20_FAMILY_ID  DS1820_FAMILY_ID
#define DS18S20_LSB        DS18B20_LSB

#define DS18B20_FAMILY_ID  0x28   ///< Identifiant DS18B20
#define DS18B20_LSB        0.0625 ///< Résolution en °C du DS18B20

#define DS1820_ERROR       INT16_MIN   ///< Code erreur

/* internal public functions ================================================ */

/** 
 * @brief Lecture de la température
 *
 * Cette fonction démarre une conversion, attend sa fin et retourne la
 * température.
 *
 * @param device pointeur sur le circuit
 * @return valeur brute de la température lue dans le registre du DS1820 ou 
 * DS1820_ERROR si erreur.
 */
int16_t iDs1820Read (xOwDevice * device);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif /* _AVRIO_OW_DS1820_H_ */
