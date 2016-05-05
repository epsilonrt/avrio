/**
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 *
 * @file ax25.h
 * @brief Couche liaison du protocole AX.25.
 *
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version from BeRTOS
*/

#ifndef _AVRIO_AX25_H_
#define _AVRIO_AX25_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include <stdio.h>
#include "avrio-board-ax25.h"
#include <avrio/hdlc.h>

/**
 *  @addtogroup net_group
 *  @{
 *  @defgroup ax25_module Liaison AX25
 *  Ce module gère la couche liaison d'un réseau AX25 et premet l'envoi et la
 *  réception de trames. \n
 *  Ce module a été conçu pour être indépendant de la couche physique qui sera
 *  fournie au moudule au moment de l'initialisation sous forme de fichiers.
 *  @{
 *    @example aprs/demo_aprs.c
 *    Envoie une trame Ax25/APRS de façon périodique et affiche les trames
 *    reçues sur la liaison série. A chaque réception de trame valide, la led
 *    APRS_LED est basculée.
 */

/* constants ================================================================ */
/**
 * Taille minimale d'une trame AX25
 */
#define AX25_MIN_FRAME_LEN 18

/**
 * Identifiant contrôle d'une trame UI
 * Le module gère uniquement ce type de trame
 */
#define AX25_CTRL_UI      0x03

/**
 * Identifiant protocole sans couche 3
 * Le module gère uniquement ce type de trame
 */
#define AX25_PID_NOLAYER3 0xF0

/**
 * Nombre maximal de répéteurs dans une trame AX25
 */
#define AX25_MAX_RPT 8

/* macros =================================================================== */
/**
 * Permet la création d'une structure xAx25Node
 * @param str indicatif de la station au maximum 6 caractères.
 * @param id  ssid identifiant de sous-station.
 */
#define AX25_CALL(str, id) {.call = (str), .ssid = (id) }

/**
 * Permet la création d'un chemin
 * Un chemin AX25 est constitué d'une destination, d'une source et de répéteurs
 * éventuels (entre 0 et )
 * @param dst destination du chemin, @see AX25_CALL
 * @param src source du chemin, @see AX25_CALL
 *
 * Des répéteurs optionnels peuvent être ajoutés à la suite.
 *
 * Cette macro permet de déclarer facilement un chemin pour le fournir à la
 * fonction vAx25SendVia() :
 * @code
 * xAx25Node path[] = AX25_PATH(AX25_CALL("abcdef", 0), AX25_CALL("ghjklm", 0), AX25_CALL("wide1", 1), AX25_CALL("wide2", 2));
 * @endcode
 */
#define AX25_PATH(dst, src, ...) { dst, src, ## __VA_ARGS__ }

/**
 * Envoie une trame AX25
 * @param ax25 Objet Ax25 à utiliser
 * @param dst destination de la trame, @see AX25_CALL
 * @param src source de la trame, @see AX25_CALL
 * @param buf buffer contenant les informations (payload).
 * @param len nombre d'octets du payload.
 *
 * @see vAx25SendVia() pour l'envoi avec répéteurs.
 */
#define vAx25Send(ax25, dst, src, buf, len) vAx25SendVia(ax25, ({static xAx25Node __path[]={dst, src}; __path;}), 2, buf, len)

/* structures =============================================================== */
struct xAx25Frame; // fwd declaration
struct xAx25;

/* types ==================================================================== */
/**
 * Fonction gérant la réception de trame
 */
typedef void (*vAx25CallBack)(struct xAx25Frame *msg);

/**
 * Station AX25
 */
typedef struct xAx25Node
{
  char call[6]; ///< Indicatif composé d'au plus 6 caractères ASCII (lettres et chiffres)
  uint8_t ssid; ///< Identifiant de sous-station (0 à 15)
} xAx25Node;

/**
 * Trame AX25
 */
typedef struct xAx25Frame
{
  xAx25Node src;  ///< Source de la trame
  xAx25Node dst;  ///< Destination de la trame
  #if CONFIG_AX25_RPT_LST
    xAx25Node rpt_lst[AX25_MAX_RPT]; ///< List des répéteurs
    uint8_t rpt_cnt; ///< Nombre de répéteurs dans la trame
    uint8_t rpt_flags; ///< Bits Has-been-repeated pour chaque répéteur
    #define AX25_REPEATED(msg, idx) ((msg)->rpt_flags & _BV(idx))
  #endif
  uint16_t ctrl; ///< Champs contrôle AX25
  uint8_t pid;   ///< Champs d'identification du protocole AX25
  const uint8_t *info; ///< Pointeur sur les informations (payload)
  size_t len;    ///< Nombre d'octets du payload
} xAx25Frame;

/**
 * Objet Ax25
 * Cet objet encapsule toutes les informations nécessaires à la gestion de la
 * réception et de la transmission.
 */
typedef struct xAx25
{
  uint8_t buf[CONFIG_AX25_FRAME_BUF_LEN]; ///< Buffer des octets reçus
  FILE *fin;        ///< Fichier pour accèder à la couche physique en entrée
  FILE *fout;       ///< Fichier pour accèder à la couche physique en sortie
  size_t frm_len;   ///< Nombre d'octets reçus
  uint16_t crc_in;  ///< CRC calculé en entrée
  uint16_t crc_out; ///< CRC calculé en sortie
  vAx25CallBack hook; ///< Fonction de traitement des trames reçues
  bool sync;   ///< Vrai si un flag HDLC de début de trame a été reçu
  bool escape; ///< Vrai si un cacractère d'échappement a été reçu
} xAx25;

/* internal public functions ================================================ */
/**
 * Intialise l'objet Ax25
 *
 * @param ax25 Objet à initialiser
 * @param fin Pointeur sur le fichier d'entrée de la couche physique
 * @param fout Pointeur sur le fichier de sortie de la couche physique
 * @param hook Fonction exécutée à la réception d'une trame (callback)
 */
void vAx25Init (struct xAx25 *ax25, FILE *fin, FILE *fout, vAx25CallBack hook);

/**
 * Vérifies si une trame AX25 peut être traiter.
 *
 * Cette fonction lit les octets sur le media d'entrée à la recherche de trames
 * AX25. \n
 * Si une tame est trouvée, elle est décodée et passée à la fonction de gestion
 * de réception (callback). \n
 * Cette fonction doit être appelée le plus souvent possible dans la boucle du
 * programme principal.  \n
 * Cette fonction peut être bloquante si aucun octet n'est reçu et si le media
 * utilisé en entrée est configuré en mode bloquant.
 *
 * @param ax25 Objet Ax25 utilisé
 */
void vAx25Poll (struct xAx25 *ax25);

/**
 * Envoi d'une trame par un chemin spécifique
 *
 * @param ax25 Objet Ax25 utilisé
 * @param path Tableau de stations utilisé comme chemin, @see AX25_PATH
 * @param path_len Nombre de station dans le path
 * @param _buf Buffer contenant les informations à transmettre (payload)
 * @param len Nombre d'octets du payload.
 */
void vAx25SendVia (struct xAx25 *ax25, const xAx25Node *path, size_t path_len, const void *_buf, size_t len);

/**
 * Affiche une trame AX25 au format TNC-2
 *
 * @param f Fichier à utiliser pour l'affichage
 * @param msg Trame à afficher
 */
void vAx25Print (FILE *f, const xAx25Frame *msg);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * ===========================================================================*/

/**
 * @defgroup ax25_cfg Configuration
 * La configuration du module est effectuée dans le fichier avrio-board-ax25.h
 * Ce fichier doit se trouver dans le chemin de recherche des fichiers en-tête
 * (répertoire du projet ou dossier board normalement). \n
 * @{
 */

/**
 * @brief Taille buffer de réception
 *
 * Configuration de la taille du buffer de réception utilisé par le module.
 * La taille doit être suffisante pour contenir une trame complète (sans les
 * flags HDLC).
 */
#define CONFIG_AX25_FRAME_BUF_LEN

/**
 * @brief Validation répéteurs
 *
 * Valide la prise en charge des répéteurs dans les trames, cela utilise 56
 * octets supplémentaire par trame.
 */
#define CONFIG_AX25_RPT_LST
/**   @} */
/* ========================================================================== */
#endif /* __DOXYGEN__ defined */

/**
 *  @}
 * @}
 */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */

#endif /* _AVRIO_AX25_H_ */
