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
 * @file kalman.h
 * @brief Filtre de Kalman
 * Copyright © 2012 Kristian Lauszus, TKJ Electronics. All rights reserved.
 *
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * Revision History ------------------------------------------------------------
 *    20121225 - Initial version by epsilonRT
 */
#ifndef _AVRIO_KALMAN_H_
#define _AVRIO_KALMAN_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup math_group
 * @{
 *
 *  @defgroup kalman_module Filtre de Kalman
 *  Ce module permet de gérer des filtres de Kalman. Le filtre de Kalman est un 
 *  filtre à réponse impulsionnelle infinie qui estime les états d'un système 
 *  dynamique à partir d'une série de mesures incomplètes ou bruitées.
 *  @{
 */

/* constants ================================================================ */
#define KALMAN_Q_ANGLE    0.001
#define KALMAN_Q_BIAS     0.003
#define KALMAN_R_MEASURE  0.03

/* structures =============================================================== */
/**
 * @struct xKalmanFilter
 * @brief Données du filtre de Kalman
 */
typedef struct xKalmanFilter {

  double fQAngle; ///< Process noise variance for the accelerometer
  double fQBias; ///< Process noise variance for the gyro bias
  double fRMeasure; ///< Measurement noise variance - this is actually the variance of the measurement noise
  
  double fAngle; ///< The angle calculated by the Kalman filter - part of the 2x1 state matrix
  double fBias; ///< The gyro bias calculated by the Kalman filter - part of the 2x1 state matrix
  double fRate; ///< Unbiased rate calculated from the rate and the calculated bias - you have to call getAngle to update the rate
  
  double fP[2][2]; ///< Error covariance matrix - This is a 2x2 matrix
  double fK[2]; ///< Kalman gain - This is a 2x1 matrix
  double fY; ///< Angle difference - 1x1 matrix
  double fS; ///< Estimate error - 1x1 matrix
} xKalmanFilter;


/* internal public functions ================================================ */
/**
 * @brief Initialise le filtre
 *
 * 
 * @param xFilter Pointeur sur le filtre
 */
void vKalmanInit (xKalmanFilter * xFilter);

/**
 * @brief Processus de filtrage
 *
 * 
 * @param xFilter Pointeur sur le filtre qui sera mis à jour
 * @param fNewAngle Nouvel angle calculé à partir des accélérations en degrés
 * @param fNewRate Nouvelle vitesse angulaire en degrés par secondes
 * @param usDt Ecart de temps avec la mesure précédente en millisecondes
 */
void vKalmanProcess (xKalmanFilter * xFilter, double fNewAngle, double fNewRate, uint16_t usDt);

// -----------------------------------------------------------------------------

#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 *   @}
 * @}
 */

#  else
/* 
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

/* public variables ========================================================= */

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_KALMAN_H_ not defined */
