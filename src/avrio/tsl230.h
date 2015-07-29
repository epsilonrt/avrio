/**
 * @file tsl230.h
 * @brief Capteur de luminosité TSL230
 *
 * Copyright © 2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 */
#ifndef _AVRIO_TSL230_H_
#define _AVRIO_TSL230_H_
#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */

/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup tsl230_module Capteur de luminosité TSL230
 *
 *  Ce module permet la mesure de l'éclairement lumineux (irradiance à l'aide
 *  d'un capteur TSL230. Ce capteur est composé d'un réseau de photodiodes et
 *  d'un convertisseur courant-fréquence qui fournit un signal de fréquence
 *  proportionnelle à l'irradiance. \n
 *  Le rapport de proportionnalité Re en Hz/(µw/cm²) est modifiable grâce à un
 *  réglage de sensibilité et d'échelle de fréquence. \n
 *  @{
 *  @example tsl230/demo_tsl230.c
 *  description
 */

/* constants ================================================================ */
/**
 * @brief Sensibilté du capteur
 */
typedef enum {
  eTsl230Off            = 0,  /**< Mode Power Down */
  eTsl230Sensitivity1   = 1,  /**< 7.9  Hz/(µw/cm²) (eTsl230Scale100) */
  eTsl230Sensitivity10  = 2,  /**< 79   Hz/(µw/cm²) (eTsl230Scale100) */
  eTsl230Sensitivity100 = 3,  /**< 790  Hz/(µw/cm²) (eTsl230Scale100) */
} eTsl230Sensitivity;

/**
 * @brief Echelle de fréquence du capteur
 */
typedef enum {
  eTsl230Scale1   = 3,  /**< 7.9  Hz/(µw/cm²) (eTsl230Sensitivity100) */
  eTsl230Scale10  = 2,  /**< 79   Hz/(µw/cm²) (eTsl230Sensitivity100) */
  eTsl230Scale50  = 1,  /**< 395  Hz/(µw/cm²) (eTsl230Sensitivity100) */
  eTsl230Scale100 = 0,  /**< 790  Hz/(µw/cm²) (eTsl230Sensitivity100) */
} eTsl230Scale;

/* structures =============================================================== */

/* types ==================================================================== */

/* internal public functions ================================================ */
/**
 * @brief Initialisation du capteur et de son fréquencemètre
 *
 * La configuration est modifiable dans avrio-board-tsl230.h
 */
void vTsl230Init (void);

/**
 * @brief Convertit une fréquence en irradiance
 *
 * @param fréquence en sortie du capteur en Hertz
 * @return Irradiance en µW/cm²
 */
double dTsl230FreqToIrradiance (double dFreq);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Démarre la mesure
 */
inline void vTsl230Start (void);

/**
 * @brief Teste si la mesure est terminée
 *
 * Retourne true après l'initialisation
 * @return true si la mesure est terminée
 */
inline bool bTsl230IsComplete (void);

/**
 * @brief Attends que la mesure se termine
 */
inline void vTsl230WaitForComplete (void);

/**
 * @brief Lit la dernière mesure d'irradiance en Hertz
 *
 * @return fréquence en Hertz correspondant à l'irradiance mesurée
 */
inline double dTsl230Freq (void);

/**
 * @brief Lit la dernière mesure d'irradiance en µW/cm²
 *
 * @return Irradiance en µW/cm²
 */
inline double dTsl230Irradiance (void);

/**
 * @brief Modifie la sensibilité du capteur
 */
inline void vTsl230SetSensitivity (eTsl230Sensitivity eSensitivity);

/**
 * @brief Sensibilité du capteur
 */
inline eTsl230Sensitivity eTsl230GetSensitivity (void);

/**
 * @brief Modifie l'échelle de fréquence du capteur
 */
inline void vTsl230SetScale (eTsl230Scale eScale);

/**
 * @brief Sensibilité du capteur
 */
inline eTsl230Scale eTsl230GetScale (void);

/**
 * @brief Modifie la fréquence capteur dans le noir
 */
inline void vTsl230SetDarkFreq (double dDarkFreq);

/**
 * @brief Fréquence capteur dans le noir
 */
inline double dTsl230GetDarkFreq (void);

/**
 * @brief Modifie la réceptivité du capteur
 */
inline void vTsl230SetResponsivity (double dResponsivity);

/**
 * @brief Réceptivité du capteur
 */
inline double dTsl230GetResponsivity (void);

/**
 * @brief Valide la broche OE du capteur
 *
 * @note Il faut que TSL230_OE soit défini dans avrio-board-tsl230.h
 */
inline void vTsl230Enable (void);

/**
 * @brief Dévalide la broche OE du capteur
 *
 * @note Il faut que TSL230_OE soit défini dans avrio-board-tsl230.h
 */
inline void vTsl230Disable (void);
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
#include "avrio-config.h"
#ifdef AVRIO_TSL230_ENABLE
#if ! defined(AVRIO_IRQ_ENABLE) || ! defined(AVRIO_TASK_ENABLE)
#error AVRIO_IRQ_ENABLE and AVRIO_TASK_ENABLE should be defined for TSL230 module
#endif

#include "icounter.h"
#include "avrio-board-tsl230.h"

/* structures =============================================================== */
typedef struct xTsl230Context {
  eTsl230Sensitivity eSensitivity;
  eTsl230Scale eScale;
  xICounter xCounter;
  double dDarkFreq;
  double dResponsivity;
} xTsl230Context;

/* public variables ========================================================= */
extern xTsl230Context xTsl230;

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
INLINE void
vTsl230Start (void) {

  vICounterStart (&xTsl230.xCounter);
}

// -----------------------------------------------------------------------------
INLINE bool
bTsl230IsComplete (void) {

  return bICounterIsComplete (&xTsl230.xCounter);
}

// -----------------------------------------------------------------------------
INLINE void
vTsl230WaitForComplete (void) {

  vICounterWaitForComplete (&xTsl230.xCounter);
}

// -----------------------------------------------------------------------------
INLINE double
dTsl230Freq (void) {

  return dICounterFreq (&xTsl230.xCounter);
}

// -----------------------------------------------------------------------------
INLINE double
dTsl230Irradiance (void) {

  return dTsl230FreqToIrradiance (dTsl230Freq());
}

// -----------------------------------------------------------------------------
INLINE void
vTsl230SetSensitivity (eTsl230Sensitivity eSensitivity) {

  xTsl230.eSensitivity = eSensitivity;
  vTsl230PinSetSensitivity (eSensitivity);
}

// -----------------------------------------------------------------------------
INLINE eTsl230Sensitivity 
eTsl230GetSensitivity (void) {
  
  return xTsl230.eSensitivity;
}

// -----------------------------------------------------------------------------
INLINE void
vTsl230SetScale (eTsl230Scale eScale) {

  xTsl230.eScale = eScale;
  vTsl230PinSetScale (eScale);
}

// -----------------------------------------------------------------------------
INLINE eTsl230Scale 
eTsl230GetScale (void) {
  
  return xTsl230.eScale;
}

// -----------------------------------------------------------------------------
INLINE void 
vTsl230SetDarkFreq (double dDarkFreq) {
  xTsl230.dDarkFreq = dDarkFreq;
}

// -----------------------------------------------------------------------------
INLINE double 
dTsl230GetDarkFreq (void) {
  return xTsl230.dDarkFreq;
}

// -----------------------------------------------------------------------------
INLINE void 
vTsl230SetResponsivity (double dResponsivity) {
   xTsl230.dResponsivity = dResponsivity;
}

// -----------------------------------------------------------------------------
INLINE double 
dTsl230GetResponsivity (void) {
  
  return  xTsl230.dResponsivity;
}

#endif  /* AVRIO_TSL230_ENABLE defined */

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_TSL230_H_ */
