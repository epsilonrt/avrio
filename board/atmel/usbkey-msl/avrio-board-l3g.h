/*
 * @file avrio-board-l3g.h
 * @brief Configuration L3G
 *
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version by epsilonRT
 */

#ifndef _AVRIO_BOARD_L3G_H_
#define _AVRIO_BOARD_L3G_H_

#include <avr/io.h>

/* 
 *          -------- Convention des axes utilisée --------
 * La convention utilisée est celle des aéronefs :
 * - X axe longitudinal (dans le sens de la marche), c'est l'axe du roulis (roll). 
 *   Positif lorsque l'aile droite descend (virage droite)
 * - Y axe transversal, c'est l'axe du tangage ou piqué (pitch). 
 *   Positif à  monter.
 * - Z axe normal au plan XY (vertical), c'est l'axe de lacel (yaw)
 *   Positif vers la droite.
 *
 * http://en.wikipedia.org/wiki/Flight_dynamics
 * Il faut définir correctement L3G_AXIS_ORDER et L3G_AXIS_SIGN pour respecter
 * ce cahier des charges.
 */

/* constants ================================================================ */
/* Ordre des axes  */
#define L3G_AXIS_ORDER_XYZ  0
#define L3G_AXIS_ORDER_YXZ  1
#define L3G_AXIS_ORDER_YZX  2
#define L3G_AXIS_ORDER_ZYX  3

/* Signe des axes */
#define L3G_AXIS_SIGN_PPP   0 /* Tous les axes sont positifs */
#define L3G_AXIS_SIGN_PPN   1 /* L'axe 3 sera inversé */  
#define L3G_AXIS_SIGN_PNP   2 /* L'axe 2 sera inversé */     
#define L3G_AXIS_SIGN_PNN   3 /* Les axes 2 et 3 seront inversés */     
#define L3G_AXIS_SIGN_NPP   4 /* L'axe 1 sera inversé */  
#define L3G_AXIS_SIGN_NPN   5 /* Les axes 1 et 3 seront inversés */    
#define L3G_AXIS_SIGN_NNP   6 /* Les axes 1 et 2 seront inversés */   
#define L3G_AXIS_SIGN_NNN   7 /* Les axes 1, 2 et 3 seront inversés */      

/* Adresse I2C */
#define L3G4200D_ADDRESS_SA0_LOW  (0xD0)
#define L3G4200D_ADDRESS_SA0_HIGH (0xD2)
#define L3GD20_ADDRESS_SA0_LOW    (0xD4)
#define L3GD20_ADDRESS_SA0_HIGH   (0xD6)

/* Output Data Rates */
#define L3G_ODR_100HZ 0x00
#define L3G_ODR_200HZ 0x40
#define L3G_ODR_400HZ 0x80
#define L3G_ODR_800HZ 0xC0

/* Fréquences de coupure du filtre passe-bas */
/*                      ODR       100 200 400 800 */
#define L3G_LPBW_12HZ5  0x00  /*   X   X          */
#define L3G_LPBW_20HZ   0x00  /*           X      */
#define L3G_LPBW_25HZ   0x10  /*   X   X   X      */
#define L3G_LPBW_30HZ   0x00  /*               X  */
#define L3G_LPBW_35HZ   0x10  /*               X  */
#define L3G_LPBW_50HZ   0x20  /*       X   X   X  */
#define L3G_LPBW_70HZ   0x30  /*       X          */
#define L3G_LPBW_110HZ  0x30  /*           X   X  */

/* setup ==================================================================== */
/* Adresse I2C */
#define L3G_ADDRESS L3G4200D_ADDRESS_SA0_HIGH

/* Pleine Echelle (250,500 ou 2000) */
#define L3G_FULL_SCALE 250

/* Ordre des axes dans les registres */
#define L3G_AXIS_ORDER  L3G_AXIS_ORDER_XYZ

/* Signe des axes */
#define L3G_AXIS_SIGN   L3G_AXIS_SIGN_PPP

/* Output Data Rate */
#define L3G_ODR L3G_ODR_200HZ

/* Fréquence de coupure du filtre passe-bas */
#define L3G_LPBW L3G_LPBW_25HZ

/* Nombre d'échantillons pour la calibartion */
#define L3G_CALIBRATE_SAMPLES 512

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_L3G_H_ not defined */
