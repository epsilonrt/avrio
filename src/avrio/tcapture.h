/**
 * @file tcapture.h
 * @brief Entrée de capture timer 16 bits
 *
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
 */
#ifndef _AVRIO_TCAPTURE_H_
#define _AVRIO_TCAPTURE_H_
#include <avrio/mutex.h>
#include <avr/interrupt.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup tcapture_module Entrée de capture timer 16 bits
 *
 *  Ce module permet de gérer les entrées de capture de timer 16 bits.
 *  @{
 */

/* constants ================================================================ */
/**
 * @brief Type d'évement de déclenchement
 */
typedef enum {
  eTCaptureFalling  = 0, /**< Front descendant*/
  eTCaptureRising   = 1, /**< Front montant*/
} eTCaptureEvent;

/**
 * @brief Division d'horloge
 */
typedef enum {
  eTCaptureDivStop  = 0, /**< Timer arrêté */
  eTCaptureDiv1  = 1, /**< Divsion par 1 */
  eTCaptureDiv8  = 2, /**< Divsion par 8 */
  eTCaptureDiv64  = 3, /**< Divsion par 64 */
  eTCaptureDiv256  = 4, /**< Divsion par 256 */
  eTCaptureDiv1024  = 5, /**< Divsion par 1024 */
  eTCaptureDivExtFall  = 6, /**< Horloge externe sur front descendant */
  eTCaptureDivExtRise  = 7, /**< Horloge externe sur front montant */
} eTCapturePrescaler;

#define TCAPTURE1  0 /**< Entrée de capture TIMER1 */
#define TCAPTURE3  1 /**< Entrée de capture TIMER3 */
#define TCAPTURE4  2 /**< Entrée de capture TIMER4 */
#define TCAPTURE5  3 /**< Entrée de capture TIMER5 */


/* types ==================================================================== */
/** @brief Désigne le numéro du timer */
typedef int8_t xTCaptureHandle;

/* internal public functions ================================================ */
/**
 * @brief Initialise le module
 */
void vTCaptureInit(void);

/**
 * @brief Règle la fréquence d'horloge
 *
 * @param t timer
 * @param ePrescaler facteur de division de l'horloge
 */
void vTCaptureSetPrescaler (xTCaptureHandle t, eTCapturePrescaler ePrescaler);

/**
 * @brief Démarre la mesure entre 2 fronts
 *
 * @param t timer
 */
void vTCaptureStart (xTCaptureHandle t);

/**
 * @brief Teste si la mesure est terminée
 *
 * @param t timer
 * @return true si la mesure est terminée
 */
bool bTCaptureIsComplete (xTCaptureHandle t);

/**
 * @brief Attends que la mesure se termine
 *
 * @param t timer
 */
void vTCaptureWaitForComplete (xTCaptureHandle t);

/**
 * @brief Dernière valeur mesurée entre 2 fronts
 *
 * @param t timer
 * @return le nombre de période d'horloge timer
 */
uint16_t usTCaptureValue (xTCaptureHandle t);

/**
 * @brief Convertit un nombre de périodes d'horloge en durée
 *
 * @param t timer
 * @param usValue nombre de période d'horloge timer
 * @return durée en s
 */
double dTCaptureValueToTime (xTCaptureHandle t, uint16_t usValue);

/**
 * @brief Convertit un nombre de périodes d'horloge en fréquence
 *
 * @param t timer
 * @param usValue nombre de période d'horloge timer
 * @return fréquence en Hertz
 */
double dTCaptureValueToFreq (xTCaptureHandle t, uint16_t usValue);

/**
 * @brief Valide le filtrage du bruit
 *
 * @param t timer
 * @param bEnable true pour valider
 */
void vTCaptureSetNoiseFilter (xTCaptureHandle t, bool bEnable);

/**
 * @brief Règle l'événement de déclenchement
 *
 * @param t timer
 * @param eEvent événement de déclenchement
 */
void vTCaptureSetEvent (xTCaptureHandle t, eTCaptureEvent eEvent);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Dernière durée mesurée entre 2 fronts
 *
 * @param t timer
 * @return durée en s
 */
inline double dTCaptureTime (xTCaptureHandle t);

/**
 * @brief Dernière fréquence mesurée entre 2 fronts
 *
 * @param t timer
 * @return fréquence en Hertz
 */
inline double dTCaptureFreq (xTCaptureHandle t);
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

// -----------------------------------------------------------------------------
INLINE double
dTCaptureTime (xTCaptureHandle t) {
  return dTCaptureValueToTime (t, usTCaptureValue (t));
}

// -----------------------------------------------------------------------------
INLINE double
dTCaptureFreq (xTCaptureHandle t) {
  return dTCaptureValueToFreq (t, usTCaptureValue (t));
}

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_TCAPTURE_H_ */
