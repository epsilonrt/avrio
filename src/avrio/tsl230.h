/**
 * @file tsl230.h
 * @brief Capteur de luminosité TSL230
 *
 * Copyright © 2015 epsilonRT. All rights reserved.
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
 * @brief Plage de mesure capteur
 * 
 * eTsl230RangeOff permet de mettre le capteur en mode sommeil (conso. 5 µA). En
 * sortie du mode sommeil, il faut 100 µs au capteur pour être opérationnel.
 */
typedef enum {
  eTsl230RangeOff   = 0,  /**< Mode Power Down */
  eTsl230Range2000  = 1,  /**< 2000 W/m² */
  eTsl230Range200   = 2,  /**< 200  W/m² */
  eTsl230Range20    = 3,  /**< 20   W/m² */
} eTsl230Range;


/**
 *  @defgroup tsl230_module_low Réglage bas niveau
 * 
 * Les fonctions de ce groupe sont utilisées en interne, elles ne devraient pas
 * être utilisées directement.
 * @{
 */

/**
 * @brief Sensibilité du capteur
 * 
 * eTsl230Off permet de mettre le capteur en mode sommeil (conso. 5 µA). En
 * sortie du mode sommeil, il faut 100 µs au capteur pour être opérationnel.
 */
typedef enum {
  eTsl230Off            = 0,  /**< Mode Power Down */
  eTsl230Sensitivity1   = 1,  /**< 7.9  Hz/(µw/cm²) (eTsl230ScaleDiv1) */
  eTsl230Sensitivity10  = 2,  /**< 79   Hz/(µw/cm²) (eTsl230ScaleDiv1) */
  eTsl230Sensitivity100 = 3,  /**< 790  Hz/(µw/cm²) (eTsl230ScaleDiv1) */
} eTsl230Sensitivity;

/**
 * @brief Division de fréquence du capteur
 * 
 * La division est effectuée grâce à des divisieurs de fréquence interne au
 * circuit. Cela revient a effectuer un moyennage sur 2, 10 ou 100. \n
 * Sans division (Div1) la sortie génère des impulsions très courtes de durée 
 * fixe (entre 125 et 600 ns), avec division (2,10,100) la sortie génère un 
 * signal rectangulaire de rapport cyclique 50%.
 */
typedef enum {
  
  eTsl230ScaleDiv1      = 0, /**< Division fréquence par 1 (Imp. 300 ns)*/
  eTsl230ScaleDiv2      = 1, /**< Division fréquence par 2 (50%) */
  eTsl230ScaleDiv10     = 2, /**< Division fréquence par 10 (50%)*/
  eTsl230ScaleDiv100    = 3, /**< Division fréquence par 100 (50%)*/
#ifndef __DOXYGEN__
  /* Obsolete */
  eTsl230Scale1   = eTsl230ScaleDiv100,
  eTsl230Scale10  = eTsl230ScaleDiv10,
  eTsl230Scale50  = eTsl230ScaleDiv2,
  eTsl230Scale100 = eTsl230ScaleDiv1,
#endif
} eTsl230Scale;

/**
 * @}
 */

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
 * @return Irradiance en µW/cm² (100 µW/cm² = 1 W/m²)
 */
double dTsl230FreqToIrradiance (double dFreq);

/**
 * @brief Réglage la plage de mesure du capteur
 * 
 * Les valeurs possibles sont:
 * - 0 W/m² (Mode power-down)
 * - 20 W/m²
 * - 200 W/m²
 * - 2000 W/m²
 * .
 * @param usRange plage de mesure {0, 20, 200, 2000}, une valeur incorrecte est
 * ignorée.
 */
void vTsl230SetRange (uint16_t usRange);

/**
 * @brief Lecture de la plage de mesure du capteur
 * @return  plage de mesure {0, 20, 200, 2000}
 */
uint16_t usTsl230Range (void);

/**
 * @brief Réglage automatique de la plage de mesure du capteur
 * 
 * Effectue le réglage automatique de la plage de mesure en fonction
 * @return  plage de mesure effective {0, 20, 200, 2000}
 */
uint16_t usTsl230AutoRange (void);

/**
 * @brief Mesure bloquante de la fréquence du capteur
 * 
 * Effectue une mesure et renvoie la fréquence mesurée.
 * 
 * dTsl230FreqToIrradiance() peut être utilisée pour convertir la fréquence en
 * irradiance.
 * @param bAutoRange valide le réglage automatique de plage de mesure
 * @return fréquence en Hertz correspondant à l'irradiance mesurée
 */
double dTsl230ReadFreq (bool bAutoRange);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * @brief Mesure bloquante de l'irradiance du capteur
 * 
 * Effectue une mesure et renvoie l'irradiance mesurée.
 * 
 * @param bAutoRange valide le réglage automatique de plage de mesure
 * @return fréquence en Hertz correspondant à l'irradiance mesurée
 */
inline double dTsl230ReadIrradiance (bool bAutoRange);

/**
 * @brief Valide la broche OE du capteur
 *
 * @note Il faut que TSL230_OE soit défini dans avrio-board-tsl230.h, c'est à
 * dire qu'une broche de port soit reliée au circuit.
 */
inline void vTsl230Enable (void);

/**
 * @brief Dévalide la broche OE du capteur
 *
 * @note Il faut que TSL230_OE soit défini dans avrio-board-tsl230.h, c'est à
 * dire qu'une broche de port soit reliée au circuit.
 */
inline void vTsl230Disable (void);

/**
 * @brief Lit la dernière mesure d'irradiance en Hertz
 * 
 * Cette fonction ne lance pas de mesure.
 *
 * @return fréquence en Hertz correspondant à l'irradiance mesurée
 */
inline double dTsl230Freq (void);

/**
 * @brief Lit la dernière mesure d'irradiance en µW/cm²
 * 
 * Cette fonction ne lance pas de mesure.
 *
 * @return Irradiance en µW/cm²
 */
inline double dTsl230Irradiance (void);

/**
 * @brief Modifie la fréquence capteur dans le noir
 * 
 * D'après le datasheet, la fréquence dans le noir typique est de 0.4 Hz, cette 
 * fonction permet de modifier cette valeur d'étalonnage.
 */
inline void vTsl230SetDarkFreq (double dDarkFreq);

/**
 * @brief Fréquence capteur dans le noir
 */
inline double dTsl230GetDarkFreq (void);

/**
 * @brief Modifie la réceptivité du capteur
 * 
 * D'après le datasheet, la réceptivité est de 790 Hz/(µW/cm²), cette fonction
 * permet de modifier cette valeur d'étalonnage.
 */
inline void vTsl230SetResponsivity (double dResponsivity);

/**
 * @brief Réceptivité du capteur
 */
inline double dTsl230GetResponsivity (void);

/**
 *  @defgroup tsl230_module_rt Utilisation non bloquante
 * 
 * Les fonctions dTsl230ReadFreq() et dTsl230ReadIrradiance() sont bloquantes.
 * Une mesure peut prendre de 75 à 750 ms ce qui peut être problématique dans
 * certains cas. \n
 * Les fonctions de ce groupe permettent de gérer une mesure non bloquante.
 * Voici un exemple typique d'utilisation:
 * \code
 * for(;;) { // Boucle principale
 *   if (bTsl230IsComplete()) { // Mesure terminée
 *     // Affichage irradiance en W/m2
 *     printf ("Irr.=%.1f W/m2\n", dTsl230Irradiance() / 100.);
 *     // Redémarre la mesure
 *     vTsl230Start();
 *   }
 *   // Ici on peut faire autre chose ...
 * }
 * \endcode
 * @{
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
 * @}
 */

/**
 * @addtogroup tsl230_module_low
 * @{
 */
 
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
 * @brief Modifie la durée de la fenêtre de mesure de la fréquence
 * 
 * La fenêtre par défaut est de 1s ce qui permet de mesurer une fréquence
 * jusqu'à 65535 Hz.
 *
 * @param usWindowMs durée en ms de la fenêtre de mesure
 */
inline void vTsl230SetWindow (uint16_t usWindowMs);

/**
 * @}
 */

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
// -----------------------------------------------------------------------------
#include "avrio-board-tsl230.h"

#if ! defined(AVRIO_TASK_ENABLE)
#error AVRIO_TASK_ENABLE should be defined for TSL230 module
#endif

#ifdef TSL230_INT
/*******************************************************************************
 * Version avec comptage interruption
 */
#include "icounter.h"

#if ! defined(AVRIO_IRQ_ENABLE) && defined(TSL230_INT)
#error AVRIO_IRQ_ENABLE should be defined for TSL230 module with icounter
#endif

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
INLINE 
void vTsl230SetWindow (uint16_t usWindowMs) {
  
  vICounterSetWindow (&xTsl230.xCounter, usWindowMs);
}

#else /* TSL230_INT not defined */
/*******************************************************************************
 * Version avec comptage d'événements (Timer en horloge externe généralement)
 */
#include "counter.h"

/* structures =============================================================== */
typedef struct xTsl230Context {
  eTsl230Sensitivity eSensitivity;
  eTsl230Scale eScale;
  xCounter xCounter;
  double dDarkFreq;
  double dResponsivity;
} xTsl230Context;

/* public variables ========================================================= */
extern xTsl230Context xTsl230;

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
INLINE void
vTsl230Start (void) {

  vCounterStart (&xTsl230.xCounter);
}

// -----------------------------------------------------------------------------
INLINE bool
bTsl230IsComplete (void) {

  return bCounterIsComplete (&xTsl230.xCounter);
}

// -----------------------------------------------------------------------------
INLINE void
vTsl230WaitForComplete (void) {

  vCounterWaitForComplete (&xTsl230.xCounter);
}

// -----------------------------------------------------------------------------
INLINE double
dTsl230Freq (void) {

  return dCounterFreq (&xTsl230.xCounter);
}

// -----------------------------------------------------------------------------
INLINE 
void vTsl230SetWindow (uint16_t usWindowMs) {
  
  vCounterSetWindow (&xTsl230.xCounter, usWindowMs);
}

#endif /* TSL230_INT not defined */

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
INLINE double 
dTsl230ReadIrradiance (bool bAutoRange) {
  
  return dTsl230FreqToIrradiance (dTsl230ReadFreq (bAutoRange));
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
