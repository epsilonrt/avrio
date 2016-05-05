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
 * @file encoder.h
 * @brief Encodeurs incrémentaux
 *
 * @todo Détail des paramètres des fonctions.
 */
#ifndef _AVRIO_ENCODER_H_
#  define _AVRIO_ENCODER_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup robotic_group
 * @{
 *
 *  @defgroup encoder_module Encodeurs incrémentaux

 *  Ce module permet de gérer des codeurs incrémentaux qui fournissent deux
 *  signaux en quadrature permettant de quantifier le déplacement et le sens de
 *  rotation d'un axe.\n
 *  La macro AVRIO_ENCODER_ENABLE doit être définie dans avrio-config.h et dans ce
 *  cas, la partie interface matérielle doit être implémentée dans le fichier
 *  avrio-board-encoder.h se trouvant dans le même répertoire.
 *  @{
 */
  /* constants ============================================================== */
/**
 * @enum eEncoderDir
 * @brief Direction de déplacement
 */
   typedef enum {

  ENCODER_STOP = 0, /**< Pas de déplacement */
  ENCODER_CW = 1,   /**< Sens des aiguilles d'une montre (positif) */
  ENCODER_CCW = -1  /**< Sens inverse des aiguilles d'une montre (négatif) */
} eEncoderDir;

/* types ==================================================================== */
/**
 * @brief Type de compteur utilisé dans l'implémentation.
 */
typedef int32_t xEncoderCounter;

/* internal public functions ================================================ */
/**
 *  @brief Initialisation des codeurs
 */
void vEncoderInit (void);

/**
 *  @brief Lecture de la valeur du compteur du codeur
 */
xEncoderCounter xEncoderCount (uint8_t ucEncoder);

/**
 *  @brief Lecture de la vitesse du codeur en nb d'impulsions par 100 ms
 */
int16_t xEncoderSpeed (uint8_t ucEncoder);

/**
 * @brief Renvoie le sens de deplacement du codeur
 */
eEncoderDir eEncoderDirection (uint8_t ucEncoder);

/**
 * @brief  Remise à 0 du compteur du codeur
 */
void vEncoderReset (uint8_t ucEncoder);

/**
 * Validation du codeur
 */
void vEncoderEnable (uint8_t ucEncoder, bool xEnable);

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
void vEncoderTask (uint8_t ucEncoder);

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_ENCODER_H_ */
