/*
 * @file avrio-board-adxl335.h
 * @brief Configuration ADXL335
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version by epsilonRT
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
