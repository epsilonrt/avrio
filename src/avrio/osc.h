/**
 * Copyright © 2015 epsilonRT. All rights reserved.
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
 * @file delay.h
 * @brief Fonctions de temporisation
 */
#ifndef _AVRIO_OSC_H_
#define _AVRIO_OSC_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ======================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup osc_module Fonctions de réglage de l'oscillateur RC
 *
 *  Ce module fournit des fonctions permettant le réglage de l'oscillateur RC.
 *  @{
 */

/* constants ================================================================ */
#define OSCCAL_EEADDR 0xFF /**< Adresse par défaut de OSCCAL en EEPROM */

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/* internal public functions ================================================ */
/**
 * @brief Retourne la valeur du registre OSCCAL
 */
inline uint8_t ucOscCal (void);

/**
 * @brief Modifie le registre OSCCAL
 */
inline void vOscCalibrate (uint8_t ucOscCal);

/**
 * @brief Réglage du registre OSCCAL avec une valeur stockée en EEPROM
 *
 * @param usEeAddr adresse en EEPROM de la valeur de OSCCAL calculée lors de
 * l'étalonnage.
 * @return la valeur de OSCCAL comme un unsigned, -1 si OSCCAL non modifié
 * @note La macro AVRIO_SET_OSCCAL doit être définie avant l'inclusion de ce
 * fichier sinon cette fonction ne fait rien et renvoie -1.
 */
inline int iOscCalibrateFromEE (uint16_t usEeAddr);

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
#include <avr/io.h>
#include <avr/eeprom.h>

#ifndef OSCCAL
#ifdef OSCCAL0
#define OSCCAL OSCCAL0
#else
#error OSCCAL not defined
#endif
#endif

// ---------------------------------------------------------------------------
INLINE uint8_t
ucOscCal (void) {
  return OSCCAL;
}

// ---------------------------------------------------------------------------
INLINE void
vOscCalibrate (uint8_t ucOscCal) {
  OSCCAL = ucOscCal;
}

// ---------------------------------------------------------------------------
INLINE int
iOscCalibrateFromEE (uint16_t usEeAddr) {
#ifdef AVRIO_SET_OSCCAL
  uint8_t ucOsc = eeprom_read_byte ((const uint8_t *)usEeAddr);
  if (ucOsc != 0xFF) {
    vOscCalibrate (ucOsc);
    return ucOscCal();
  }
#endif
  return -1;
}

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_OSC_H_ */
