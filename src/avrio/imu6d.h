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
 * @file imu6d.h
 * @brief Inertial Measurement Unit Filter
 * Copyright Aaron Berk © 2010 ARM Limited. All rights reserved.
 *
 *
 * Revision History ------------------------------------------------------------
 *    20121225 - Initial version by epsilonRT
 */
#ifndef _AVRIO_IMU6D_H_
#define _AVRIO_IMU6D_H_

#include <avrio/defs.h>

#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
#include <avrio/vector3f.h>
#include <avrio/quaternionf.h>

/**
 * @addtogroup nav_group
 * @{
 *
 *  @defgroup imu_module Centrale Inertielle 6 axes

 *  Ce module réalise le filtrage des informations fournies par une centrale
 *  intertielle équipée d'un accéléromètre et d'un gyroscope 3 axes.\n
 *  @{
 */

/* structures =============================================================== */
/**
 * @struct xImu6dFilter
 * @brief Données du filtre Imu6d
 */
typedef struct xImu6dFilter {

    // Quatfernion orientation of earth frame relative to auxiliary frame.
    xQuatf xAEq;

    // Estimated orientation quaternionf elements with initial conditions.
    xQuatf xSEq;

    // Attitude Vector (Euler Angles)
    float xAtt[3];

    // Sampling period
    float fDeltaT;

    // Gyroscope measurement error (in degrees per second).
    float fGyroMeasError;

    // Compute fBeta (filter tuning constant...)
    float fBeta;

    // Initializer boolean
    bool bFirstUpdate;

} xImu6dFilter;


/* internal public functions ================================================ */
/**
 * Initializes filter variables.
 *
 * @param xFilter Filter data
 * @param fFilterRate The rate at which the filter should be updated.
 * @param fGyroError The error of the gyroscope in degrees
 *  per second. This used to calculate a tuning constant for the filter.
 *  Try changing this value if there are jittery readings, or they change
 *  too much or too fast when rotating the IMU.
 */
void vImu6dInit (xImu6dFilter * xFilter, float fFilterRate, float fGyroError);

/**
 * Update the filter variables.
 *
 * @param xFilter Filter data
 * @param xAcc X,Y,Z-axis accelerometer reading in m/s/s.
 * @param xRate X,Y,Z-axis gyroscope reading in rad/s.
 */
void vImu6dUpdateFilter(xImu6dFilter * xFilter, float * xAcc, float * xRate);

/**
 * Compute the Euler angles based on the current filter data.
 *
 * @param xFilter Filter data
 */
void vImu6dComputeEuler(xImu6dFilter * xFilter);

/**
 * Reset the filter.
 */
void vImu6dReset(xImu6dFilter * xFilter);

#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/**
 * Get the current roll.
 *
 * @return The current roll angle in radians.
 */
float fImu6dRoll(xImu6dFilter * xFilter);

/**
 * Get the current pitch.
 *
 * @return The current pitch angle in radians.
 */
float fImu6dPitch(xImu6dFilter * xFilter);

/**
 * Get the current yaw.
 *
 * @return The current yaw angle in radians.
 */
float fImu6dYaw(xImu6dFilter * xFilter);

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

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (float 
fImu6dRoll(xImu6dFilter * xFilter)) {

    return xFilter->xAtt[0];
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (float 
fImu6dPitch(xImu6dFilter * xFilter)) {

    return xFilter->xAtt[1];
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (float 
fImu6dYaw(xImu6dFilter * xFilter)) {

    return xFilter->xAtt[2];
}

/* public variables ========================================================= */

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif  /* _AVRIO_IMU6D_H_ not defined */
