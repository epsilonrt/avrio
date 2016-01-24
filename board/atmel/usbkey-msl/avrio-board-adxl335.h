/*
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
#ifndef _AVRIO_BOARD_ADXL335_H_
#define _AVRIO_BOARD_ADXL335_H_

#include <avr/io.h>

/* constants ================================================================ */
/* Signe des axes */
#define ADXL335_AXIS_SIGN_PPP   0 /* Tous les axes sont positifs */
#define ADXL335_AXIS_SIGN_PPN   1 /* L'axe 3 sera inversé */  
#define ADXL335_AXIS_SIGN_PNP   2 /* L'axe 2 sera inversé */     
#define ADXL335_AXIS_SIGN_PNN   3 /* Les axes 2 et 3 seront inversés */     
#define ADXL335_AXIS_SIGN_NPP   4 /* L'axe 1 sera inversé */  
#define ADXL335_AXIS_SIGN_NPN   5 /* Les axes 1 et 3 seront inversés */    
#define ADXL335_AXIS_SIGN_NNP   6 /* Les axes 1 et 2 seront inversés */   
#define ADXL335_AXIS_SIGN_NNN   7 /* Les axes 1, 2 et 3 seront inversés */      

/* setup ==================================================================== */
#define ADXL335_ADC_LIST { 1, 0, 2 } /* Voies ADC utilisées  */
#define ADXL335_ACC_LSB  (3.3 / (1024.0 * 0.333))

/* Signe des axes */
#define ADXL335_AXIS_SIGN   ADXL335_AXIS_SIGN_PPP

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADXL335_H_ not defined */
