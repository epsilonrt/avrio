/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 *
 * @file mt8870.h
 * @brief Gestion d'une ligne téléphonique avec décodage DTMF
 *
 * @todo Documentation du module
 */
#ifndef _AVRIO_PHONE_H_
#  define _AVRIO_PHONE_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup dev_group
 * @{
 *
 *  @defgroup mt8870_module Gestion d'une ligne téléphonique avec décodage DTMF
 *  @{
 * Utilisation d'un décodeur MT8870 conformément au schéma de la note
 * d'application de ST Microelectronics :\n
 * <a href="http://www.datasheetcatalog.org/datasheet/SGSThomsonMicroelectronics/mXyzxty.pdf">HULOUX (J.), Patrice MOREL (P.) - AN488/0695 Phone remote system.</a>
 * 
 *
 * <b>Détail de l'implémentation</b>\n
 *
 *  - Décrochage au bout de PHONE_HOOKOFF_DELAY sonneries (8 par défaut) OU
 *  - Décrochage "rapide": après une sonnerie, raccrocher. Attendre 10 sec,
 *    puis rappel dans un délai de 1 minute. On décroche au bout d'une sonnerie.
 * .
 *
 *  <b>Comment détecter une sonnerie ?</b>\n
 *  Arrivée d'une sonnerie = front descendant sur ligne RING (INT0/PD2).\n
 *  Les fronts descendants sont espacés de 20 ms +/- 2%. Si au bout de
 *  22 ms, un nouveau front n'est pas arrivée => fin de la sonnerie.\n
 *  Une sonnerie dure 1,5 s +/- 10% et chaque sonnerie est espacée
 *   de 3,5 s +/- 10 %. Donc si un blanc dure plus de 4 s, il y a
 *  interruption de la séquence des sonneries.\n\n
 *  @verbatim
 *  RING_FLAG CALL_FLAG
 *      0        0  appel terminé (mode attente IDLE)
 *      0        1  appel en cours, sonnerie inactive
 *      1        1  appel en cours, sonnerie active
 *  @endverbatim
 *  - RING_FLAG est mis à 1 par l'arrivée d'un front descendant sur RING.
 *    une tâche 22 ms est alors réarmé
 *  - RING_FLAG est mis à 0 par le déclenchement de la tâche 22 ms.
 *    une tâche 4000 ms est alors réarmé
 *  - CALL_FLAG est mis à 1 dès qu'une sonnerie dépasse une longueur minimale
 *    (RING_LENGTH_MIN).
 *  - CALL_FLAG est mis à 0 par le déclenchement de la tâche 4000 ms.
 *  .
 *  ucRingCount doit être incrémenté lorsqu'une sonnerie dépasse une longueur
 *  minimale de RING_LENGTH_MIN.\n
 *  ucRingCount doit être remis à zéro lorsque CALL_FLAG passe de 0 à 1.
 */
  /* internal public functions ============================================== */
/**
 * @brief Init module phone
 */
void vPhoneInit (void);

/**
 * @brief Décrocher ligne téléphonique
 */
void vPhoneHookoff (void);

/**
 * @brief Indique si la ligne est décrochée
 */
bool xPhoneOffHook (void);

/**
 * @brief Raccrocher ligne téléphonique
 */
void vPhoneHangup (void);

/**
 * @brief Indique qu'une demande de raccrocher est en cours (délai d'inactivité)
 */
bool xPhoneHangupRequest (void);

/**
 * @brief Nombre de sonneries avant décrochage ligne téléphonique
 */
uint8_t ucPhoneHookoffDelay (void);

/**
 * @brief Modification nombre de sonneries avant décrochage ligne téléphonique
 * @return 0 succès, -1 erreur.
 */
int8_t iPhoneSetHookoffDelay (uint8_t ucDelay);

/**
 * @brief Nombre d'octets non lus dans la file de message
 */
uint8_t ucPhoneMsgLength (void);

/**
 *  @brief Lecture d'un octet dans la file de message
 *  Les événements générer dans la file de messages sont les suivants :
 *    - <Message H> - longueur 1 octet (H)
 *      Demande de décrochage ligne si le nombre de sonneries dépasse
 *      PHONE_HOOKOFF_DELAY.
 *    - <Message G> - longueur 1 octet (G)
 *      Demande de racrochage ligne si aucune touche n'est détectée pendant
 *      PHONE_HANGUP_DELAY.
 *    - <Message R> - longueur 2 octets (Rx)
 *      Début d'une sonnerie avec son numéro d'ordre en hexa x.
 *    - <Message K> - longueur 2 octets (Ka)
 *      Appui sur une touche du téléphone avec son code ASCII a.
 *    - <Message F> - longueur 1 octets (F)
 *      Mode décrochage rapide validé suite à une sonnerie puis une attente de
 *      10 secondes .
 *    - <Message f> - longueur 1 octets (f)
 *      Mode décrochage rapide dévalidé 1 minute après sa validation si pas
 *      d'appel.
 */
char cPhoneMsgGetChar (void);

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

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_PHONE_H_ */
