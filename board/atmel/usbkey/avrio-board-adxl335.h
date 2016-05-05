/*
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
#define ADXL335_ADC_LIST { 1, 0, 2 } /* Voies ADC utilisées pour les gyros */
#define ADXL335_VOLT_PER_UNIT  0.333

/* Signe des axes */
#define ADXL335_AXIS_SIGN   ADXL335_AXIS_SIGN_PPP

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
#endif  /* _AVRIO_BOARD_ADXL335_H_ not defined */
