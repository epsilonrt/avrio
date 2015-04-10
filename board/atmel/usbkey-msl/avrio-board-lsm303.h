/*
 * @file avrio-board-lsm303.h
 * @brief Configuration LSM303
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version by epsilonRT
 */

#ifndef _AVRIO_BOARD_LSM303_H_
#define _AVRIO_BOARD_LSM303_H_

#include <avr/io.h>

/* 
 *          -------- Convention des axes utilisée --------
 * La convention utilisée est celle des aéronefs :
 * - X axe longitudinal (dans le sens de la marche), c'est l'axe du roulis (roll). 
 *   Positif lorsque l'aile droite descend (virage droite)
 * - Y axe transversal, c'est l'axe du tangage ou piqué (pitch). 
 *   Positif à  monter.
 * - Z axe normal au plan XY (vertical), c'est l'axe de lacel (yaw)
 *   Positif vers la droite (dans le sens horaire).
 * En ce qui converne l'accélaration, la convention est que tout axe aligné sur
 * la gravité terrestre vers le bas aura une valeur de +1g. Ainsi les 
 * accélérations en g seront :
 * - à l'horizontal {0,0,1}
 * - en piqué à descendre {1,0,0}
 * - en virage sur l'aile droite {0,1,0)
 * .
 *
 * http://en.wikipedia.org/wiki/Flight_dynamics
 * Il faut définir correctement LSM303_AXIS_ORDER et LSM303_AXIS_SIGN_A pour respecter
 * ce cahier des charges.
 */

/* constants ================================================================ */
/* Ordre des axes  */
#define LSM303_AXIS_ORDER_XYZ  0
#define LSM303_AXIS_ORDER_YXZ  1
#define LSM303_AXIS_ORDER_YZX  2
#define LSM303_AXIS_ORDER_ZYX  3

/* Signe des axes */
#define LSM303_AXIS_SIGN_PPP   0 /* Tous les axes sont positifs */
#define LSM303_AXIS_SIGN_PPN   1 /* L'axe 3 sera inversé */  
#define LSM303_AXIS_SIGN_PNP   2 /* L'axe 2 sera inversé */     
#define LSM303_AXIS_SIGN_PNN   3 /* Les axes 2 et 3 seront inversés */     
#define LSM303_AXIS_SIGN_NPP   4 /* L'axe 1 sera inversé */  
#define LSM303_AXIS_SIGN_NPN   5 /* Les axes 1 et 3 seront inversés */    
#define LSM303_AXIS_SIGN_NNP   6 /* Les axes 1 et 2 seront inversés */   
#define LSM303_AXIS_SIGN_NNN   7 /* Les axes 1, 2 et 3 seront inversés */      

/* Type de circuit */
#define LSM303DLH_DEVICE   0
#define LSM303DLM_DEVICE   1
#define LSM303DLHC_DEVICE  2

/* Adresse I2C */
#define LSM303_ADDRESS_A_SA0_LOW  (0x30)
#define LSM303_ADDRESS_A_SA0_HIGH (0x32)
#define LSM303_ADDRESS_M          (0x3C)

/* Output Data Rates Accelero */
#define LSM303_ODR_A_50HZ   (0)
#define LSM303_ODR_A_100HZ  (_BV(LSM303_DR0))
#define LSM303_ODR_A_400HZ  (_BV(LSM303_DR1))
#define LSM303_ODR_A_1000HZ (_BV(LSM303_DR1)|_BV(LSM303_DR0))

/* Full Scale Accelero */
#define LSM303_FS_A_2G  (0)
#define LSM303_FS_A_4G  (_BV(LSM303_FS0))
#define LSM303_FS_A_8G  (_BV(LSM303_FS1)|_BV(LSM303_FS0))

/* Output Data Rates Magneto */
#define LSM303_ODR_M_0HZ75 (0)
#define LSM303_ODR_M_1HZ5  (_BV(LSM303_DO0))
#define LSM303_ODR_M_3HZ   (_BV(LSM303_DO1))
#define LSM303_ODR_M_7HZ5  (_BV(LSM303_DO1)|_BV(LSM303_DO0))
#define LSM303_ODR_M_15HZ  (_BV(LSM303_DO2))  /* default */
#define LSM303_ODR_M_30HZ  (_BV(LSM303_DO2)|_BV(LSM303_DO0))
#define LSM303_ODR_M_70HZ  (_BV(LSM303_DO2)|_BV(LSM303_DO1))
#define LSM303_ODR_M_220HZ (_BV(LSM303_DO2)|_BV(LSM303_DO1)|_BV(LSM303_DO0))

/* Full Scale Magneto */
#define LSM303_FS_M_1G3  (_BV(LSM303_GN0))  /* default */
#define LSM303_FS_M_1G9  (_BV(LSM303_GN1))
#define LSM303_FS_M_2G5  (_BV(LSM303_GN1)|_BV(LSM303_GN0))
#define LSM303_FS_M_4G0  (_BV(LSM303_GN2))
#define LSM303_FS_M_4G7  (_BV(LSM303_GN2)|_BV(LSM303_GN0))
#define LSM303_FS_M_5G6  (_BV(LSM303_GN2)|_BV(LSM303_GN1))
#define LSM303_FS_M_8G1  (_BV(LSM303_GN2)|_BV(LSM303_GN1)|_BV(LSM303_GN0))

/* setup ==================================================================== */
/* Type de circuit */
#define LSM303_DEVICE LSM303DLM_DEVICE

/* Adresse I2C */
#define LSM303_ADDRESS_A LSM303_ADDRESS_A_SA0_LOW

/* Ordre des axes dans les registres */
#define LSM303_AXIS_ORDER  LSM303_AXIS_ORDER_XYZ

/* Nombre d'essai de lecture avant de déclencher un timeout */
#define LSM303_TIMEOUT 40
// -----------------------------------------------------------------------------

/* Pleine Echelle  */
#define LSM303_FULL_SCALE_A LSM303_FS_A_2G

/* Signe des axes Accelero */
#define LSM303_AXIS_SIGN_A   LSM303_AXIS_SIGN_NNN

/* Nombre d'échantillons pour la calibartion Accelero */
#define LSM303_CALIBRATE_SAMPLES_A 128

/* Output Data Rate Accelero */
#define LSM303_ODR_A LSM303_ODR_A_100HZ

/* Output Data Rate Accelero */
#define LSM303_ODR_A LSM303_ODR_A_100HZ
// -----------------------------------------------------------------------------

/* Output Data Rate Magneto */
#define LSM303_ODR_M LSM303_ODR_M_30HZ

/* Pleine Echelle Magneto */
#define LSM303_FULL_SCALE_M LSM303_FS_M_1G3

/* Signe des axes Magneto */
#define LSM303_AXIS_SIGN_M   LSM303_AXIS_SIGN_PPP
// -----------------------------------------------------------------------------

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_LSM303_H_ not defined */
