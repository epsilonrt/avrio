/**
 * @file ntc.h
 * @brief NTC thermistor
 * Copyright © 2015 epsilonRT. All rights reserved.
 * Copyright © 2007, 2013 - SoftQuadrat GmbH, Germany
 * Contact: thermistor (at) softquadrat.de
 * Web site: thermistor.sourceforge.net
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
#ifndef _AVRIO_NTC_H_
#define _AVRIO_NTC_H_

#include <avrio/defs.h>
#if defined(__cplusplus)
  extern "C" {
#endif
/* ========================================================================== */
/**
 * @addtogroup sensor_group
 * @{
 *
 *  @defgroup ntc_module Capteur de température CTN
 *  Ce module permet de gérer des capteurs de température CTN en corrigeant
 *  leur non-linéarité grâce à des coefficients de Steinhart-Hart. \n
 *  L'implémentation est dérivée du projet Thermistor http://thermistor.sourceforge.net
 *  qui fournit un utilitaire permettant de calculer ses coefficients à partir
 *  du tableau de valeurs de résistances en fonction de la température fourni
 *  dans le datasheet de la CTN ou mesuré expérimentalement. \n
 *  Le projet initial a été modifié et disponible par git sur
 *  git://git.epsilonrt.fr/ntc
 *  @{
 */

/* internal public functions ================================================ */
/**
 * Conversion from temperature to resistance
 * Calculates and returns resistance for given temperature
 * @param dT temperature (in degree Celsius)
 * @param dCoeff Steinhart-Hart coefficients, calculates with ntc-coeff utility
 * @return corresponding resistance
 */
double dNtcTempToRes (double dT, double dCoeff[]);

/**
 * Conversion from resistance to temperature
 * Calculates and returns temperature for given resistance
 * @param dR resistance (in Ohm)
 * @param dCoeff Steinhart-Hart coefficients, calculates with ntc-coeff utility
 * @return corresponding temperature
 */
double dNtcResToTemp(double dR, double dCoeff[]);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
#if defined(__cplusplus)
  }
#endif
#endif /* _AVRIO_NTC_H_ defined */
