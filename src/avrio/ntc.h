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
#ifndef _AVRIO_NTC_H_
#define _AVRIO_NTC_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup dev_group
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
 *  git://git.epsilonrt.com/ntc
 *  @{
 *    @example ntc/demo_ntc.c
 *    Exemple qui affiche la température sur la liaison série.
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
__END_C_DECLS
#endif /* _AVRIO_NTC_H_ defined */
