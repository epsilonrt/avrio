/**
 * @file compass3d.h
 * @brief Compas magnétique 3 axes
 * @author Copyright © 2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20121218 - Initial version by epsilonRT
 */
#ifndef _AVRIO_COMPASS3D_H_
#define _AVRIO_COMPASS3D_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include <stdio.h>
#include <avrio/button.h>
#include <avrio/vector3f.h>
/**
 * @addtogroup robotic_group
 * @{
 *
 *  @defgroup comp3d_module Compas magnétique 3 axes

 *  Ce module permet de gérer un compas magnétique 3 axes. \n
 * -------- Convention des axes utilisée -------- \n
 * La convention utilisée est celle des aéronefs : \n
 * - X axe longitudinal (dans le sens de la marche), c'est l'axe du roulis (roll). 
 *   Positif lorsque l'aile droite descend (virage droite)
 * - Y axe transversal, c'est l'axe du tangage ou piqué (pitch). 
 *   Positif à monter.
 * - Z axe normal au plan XY (vertical), c'est l'axe de lacel (yaw)
 *   Positif vers la droite (dans le sens horaire).
 * .
 * En ce qui converne l'accélaration, la convention est que tout axe aligné sur
 * la gravité terrestre vers le bas aura une valeur de +1g. Ainsi les 
 * accélérations en g seront : \n
 * - à l'horizontal {0,0,1}
 * - en piqué à descendre {1,0,0}
 * - en virage sur l'aile droite {0,1,0)
 * .
 *  @{
 */
/* constants ================================================================ */

/**
 * @brief Indique que le compas n'a pas été calibré
 */
#define COMPASS3D_NOT_CALIBRATED (1)

/* structures =============================================================== */
typedef struct xCompass3dCalibration {

  int iMin[3];
  int iMax[3];
} xCompass3dCalibration;

/* internal public functions ================================================ */
/**
 * @brief Initialise le module Compass
 *
 * @return 0 en cas de succès, COMPASS3D_NOT_CALIBRATED si une calibration est 
 * nécessaire, une valeur négative sur erreur.
 */
int  iCompass3dInit (void);

/**
 * @brief Calibrage du compas
 *
 * Procède au calibrage. Le résultat étant stocké en EEPROM et rechargé
 * automatiquement à chaque initialisation iCompass3dInit(), il n'est pas
 * nécessaire de recalibrer à chaque fois.
 * @param[in] xOutStream Flux de sortie pour les messages
 * @param xBut Bouton pour la gestion du calibrage
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iCompass3dCalibrate (FILE * xOutStream, xButMask xBut);

/**
 * @brief Sauvegarde de l'étalonnage du compas
 *
 * @param[in] xCalibration Pointeur sur l'étalonnage courant.
 */
void vCompass3dSetCalibration (const xCompass3dCalibration * xCalibration);

/**
 * @brief Remets à zéro l'étalonnage du compas
 *
 */
void vCompass3dClearCalibration (void);

/**
 * @brief Lecture de l'étalonnage du compas
 *
 * @param[out] xCalibration Pointeur sur l'étalonnage courant.
 */
void vCompass3dCalibration (xCompass3dCalibration * xCalibration);

/**
 * @brief Lecture des champs magnétiques d'un compas
 *
 * Cette fonction nécessite que le compas ait été calibré une fois
 * @param[out] fMag Pointeur sur le vecteur dans lequel sont stockées les 
 * valeur normées de champs magnétique (entre -1 et +1)
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iCompass3dRead (float * fMag);

/**
 * @brief Retourne la route ou le cap du compas
 *
 * @return Cap entre 0 et 359°
 */
int iCompass3dHeading (const float * fAtt, const float * fMag, float fMagDeclinaison);

/**
 * @brief Retourne la route ou le cap du compas en radians
 *
 * @param[in] fAtt Pointeur sur le vecteur d'attitude (Roll/Pitch)
 * @param[in] fMag Pointeur sur le vecteur de mesure magnétique
 * @param fMagDeclinaison Déclinaison magnétique du lieu
 * @return Cap entre 0 et 2PI
 */
float fCompass3dHeadingRadian (const float * fAtt, const float * fMag, float fMagDeclinaison);

/**
 * @brief Retourne la route ou le cap du compas à partir du repère xFrom
 *
 * @param[in] fFrom Pointeur sur le vecteur de référence du calcul
 * @return Cap entre 0 et 359° ou -1 en cas de compas non calibré
 */
int iCompass3dHeadingFrom (const float * fFrom);

/**
 * @brief Lecture brute des champs magnétiques d'un compas
 *
 * @param[out] iMag Pointeur sur le vecteur dans lequel sont stockées les 
 * valeur de champs magnétique (lecture brute dans le compas en LSB)
 * @return 0 en cas de succès, une valeur négative sur erreur.
 */
int iCompass3dReadRaw (int * iMag);

/**
 *   @}
 * @}
 */

__END_C_DECLS
/* *INDENT-ON* */
/* ========================================================================== */
#endif  /* _AVRIO_COMPASS3D_H_ not defined */

