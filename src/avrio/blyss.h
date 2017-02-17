/**
 * Copyright © 2016 epsilonRT. All rights reserved.
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
 * @file
 * @brief Modules Blyss™
 */
#ifndef _AVRIO_BLYSS_H_
#define _AVRIO_BLYSS_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
#include <stdio.h>
#include <avr/interrupt.h>

/**
 * @addtogroup net_group
 * @{
 * @defgroup sysio_blyss Modules Blyss™
 * Ce module permet de gérer la transmission vers des modules domotiques
 * Blyss™ commerciailisés par Castorama (protocole LiveEz™ 433.92 MHz). \n
 * La plateforme doit disposer d'un GPIO. \n
 * Ce code est basé sur le travail de « reverse engineering » de 7 membres du
 * forum arduino.cc: SkyWodd, Artouste, Al1fch, Barbudor, Bunyman, Trigger et
 * Bidle (https://skyduino.wordpress.com/2012/07/19/hack-partie-2-reverse-engineering-des-interrupteurs-domotique-blyss) \n
 * La configuration du module se fait dans le fichier avrio-board-blyss.h,
 * la partie réception utilise un timer 16 bits en mode capture sous interruption,
 * la broche reliée au récepteur devra être une broche de capture ICP.
 * @{
 * @example net/blyss/rx/demo_blyss_rx.c
 * Demo réception Blyss (module blyss)
 * Ce programme allume et éteint une led à distance par liaison Blyss T0.
 * 
 * @example net/blyss/tx/demo_blyss_tx.c
 * Demo transmission de commandes Blyss (module blyss)
 * Ce programme permet la transmission de commande Blyss à partie d'ordre
 * donnés sur la liaison série (Réalisation d'une télécommande) : \n
 * @code
 *   ** Demo Transmission Blyss **
 *   0..8 : Canal
 *     M  : Marche
 *     A  : Arret
 *     B  : Boucle
 *
 *   Canal 4 -> Loop On Off On Off On Off On Off On Off On On
 * @endcode
 * 
 * @example net/blyss/sniffer/demo_blyss_sniffer.c
 * Demo sniffer de liaison Blyss (module blyss)
 * Ce programme affiche les trames Blyss reçues sur la liaison série :
 * @code
 * RF frame          : FE5082411671B
 * RF footprint      : FE - Ok
 * RF global channel : 5
 * RF adress         : 0824
 * RF channel        : 4
 * Light status      : Off
 * Rolling code      : 67 - Ok
 * Token             : 1B
 * Echo 1
 * Echo 2
 * @endcode
 */
/* constants ================================================================ */
#define BLYSS_BROADCAST 0 /***< Canal d'appel général */

/* macros =================================================================== */
/**
 * @brief Affiche le contenu de la trame pointée par f sur stdout
 *
 * @param f pointeur sur une trame de type \ref xBlyssFrame
 */
#define vBlyssPrintFrame(f) vBlyssPrintFrameToFile (f, stdout)

/* structures =============================================================== */

/**
 * @class xBlyssFrame
 * @brief Trame LiveEz
 *
 * La trame RF est précédée d'un état haut qui dure 2.4 ms et suivie d'un état
 * bas de 24 ms, elle contient les informations suivant:
 * - drapeau de début de trame 0xFE (8 bits),
 * - canal global (4 bits) de la télécommande,
 * - adresse (16 bits) de la télécommande,
 * - sous-canal (4 bits) de l'élément commandé,
 * - état de l'élément commandé (état logique) (4 bits),
 * - rolling code, MSBFIRST (8 bits),
 * - timestamp incrémentiel (0 ~ 255), MSBFIRST (8 bits),
 * - .
 *
 * Une trame contient donc 13 quartets soit 52 bits stockés dans 7 octets.
 */
struct xBlyssFrame {

  uint8_t raw[7];
};


/* types ==================================================================== */
/**
 * @brief xBlyss
 */
typedef struct xBlyss xBlyss;

/**
 * @brief xBlyssFrame
 */
typedef struct xBlyssFrame xBlyssFrame;

/* internal public functions ================================================ */
/**
 * @brief Initialisation du module Blyss
 * 
 * Si la réception est activée, sei() devra être appelé pour valider les
 * interruptions.
 */
void vBlyssInit (void);

/**
 * @brief Envoi d'une trame
 *
 * @warning Cette fonction ne fait rien si la macro BLYSS_TX_DISABLE a été 
 * définie à la compilation.
 * 
 * @param frame pointeur sur la trame
 * @param repeat nombre de répétition d'nevoi
 */
void vBlyssSend (xBlyssFrame * frame, uint8_t repeat);

/**
 * @brief Réception d'une trame
 * 
 * La réception et le décodage de trames reçues se fait sous interruption,
 * si une nouvelle trame a été reçue, elle est copiée dans frame et la fonction
 * retourne true, sinon elle retourne false et frame n'est pas modifiée. \n
 * La réception d'une nouvelle trame est bloquée tant que la précédente n'a pas
 * été lue avec cette fonction, il faut donc appeler cette fonction le plus 
 * souvent possible afin de ne pas perdre de trame.
 * 
 * @warning Cette fonction ne fait rien et retourne false si la macro 
 * BLYSS_RX_DISABLE a été définie à la compilation.
 * 
 * @param frame pointeur sur la trame
 * @return true si f a été modifiée
 */
bool bBlyssReceive (xBlyssFrame * frame);

/**
 * @brief Initialisation d'une trame LiveEz
 *
 * @param tx_id pointeur sur un tableau de 3 octets contenant dans l'ordre
 * le canal global, le poids fort de l'adresse et le poids faible de l'adresse
 * identifiant l'emetteur. Si NULL, la trame
 */
void vBlyssFrameInit (xBlyssFrame * frame, const uint8_t * tx_id);

/**
 * @brief Modification du canal destinataire d'une trame
 *
 * @param frame pointeur sur la trame
 * @param channel canal destinataire codé sur les 4 bits de poids faible
 * uniquement, valeur comprise entre 0 et 8, 0 correspondant à tous les canaux
 * (broadcast), 1 au canal 1, 2 au canal 2 ...
 */
void vBlyssFrameSetChannel (xBlyssFrame * frame, uint8_t channel);

/**
 * @brief Canal destinataire de la trame
 *
 * @param frame pointeur sur la trame
 * @return canal destinataire codé sur les 4 bits de poids faible
 * uniquement, valeur comprise entre 0 et 8, 0 correspondant à tous les canaux
 * (broadcast), 1 au canal 1, 2 au canal 2 ...
 */
uint8_t ucBlyssFrameChannel (const xBlyssFrame * frame);

/**
 * @brief Test si un buffer contient une trame LiveEz valide
 *
 * @param buffer pointeur sur une zone contenant les octets
 * @return true si valide
 */
bool bBlyssIsValidFrame (const void * buffer);

/**
 * @brief Test si un canal est valide
 *
 * @param channel canal à tester
 * @return true si valide
 */
bool bBlyssIsValidChannel (uint8_t channel);

/**
 * @brief Imprime le contenu d'une trame dans un fichier
 *
 * Le contenu imprimé pour chaque trame est formaté comme ci-dessous:
 * @code
 * RF frame          : FE5082410987D
 * RF footprint      : FE - Ok
 * RF global channel : 05
 * RF adress         : 0824
 * RF channel        : Ch4
 * Light status      : On
 * Rolling code      : 98 - Ok
 * Token             : 7D
 * @endcode
 * @param frame pointeur sur la trame
 * @param out pointeur sur le fichier ouvert en écriture
 */
void vBlyssPrintFrameToFile (const xBlyssFrame * frame, FILE * out);

/**
 *  @}
 * @}
 */

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * ===========================================================================*/
/**
 * @brief Modification de l'état du destinataire
 *
 * @param frame pointeur sur la trame
 * @param state état du destinataire ON = true
 */
static inline void vBlyssFrameSetState (xBlyssFrame * frame, bool state);

/**
 * @brief Etat du destinataire de la trame
 *
 * @param frame pointeur sur la trame
 * @return état du destinataire ON = true
 */
static inline bool bBlyssFrameState (const xBlyssFrame * frame);

/**
 * @brief Canal global d'une trame
 *
 * @param frame pointeur sur la trame
 * @return canal global (4 bits de poids faible)
 */
static inline uint8_t ucBlyssFrameGlobalChannel (const xBlyssFrame * frame);

/**
 * @brief Adresse source d'une trame
 *
 * @param frame pointeur sur la trame
 * @return adresse de l'émetteur de la trame sur 16 bits
 */
static inline uint16_t usBlyssFrameAddress (const xBlyssFrame * frame);

/**
 * @brief Compare deux trames
 * 
 * @param f1 pointeur sur une trame
 * @param f2 pointeur sur une trame
 * @return true si identiques
 */
static inline bool bBlyssFrameMatch (const xBlyssFrame * f1, const xBlyssFrame * f2);

/**
 * @brief Copie d'une trame
 * 
 * @param dest trame destination
 * @param src trame source
 */
static inline void vBlyssFrameCopy (xBlyssFrame * dest, const xBlyssFrame * src);

/* ========================================================================== */
#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
#include <string.h>

/* constants ================================================================ */
#define BLYSS_IDX_FLAG  0
#define BLYSS_IDX_GCHAN 8
#define BLYSS_IDX_ADDR  12
#define BLYSS_IDX_CHAN  28
#define BLYSS_IDX_STATE 32
#define BLYSS_IDX_ROLL  36
#define BLYSS_IDX_TOKEN 44

/* internal private functions ================================================ */
void vBlyssFrameSetBits (xBlyssFrame * f, uint8_t index, uint8_t len, uint16_t value);
uint16_t vBlyssFrameGetBits (const xBlyssFrame * f, uint8_t index, uint8_t len);

/* internal inline functions ================================================ */
// -----------------------------------------------------------------------------
INLINE void
vBlyssFrameSetState (xBlyssFrame * f, bool state) {

  vBlyssFrameSetBits (f, BLYSS_IDX_STATE, 4, ! state);
}

// -----------------------------------------------------------------------------
INLINE uint8_t
ucBlyssFrameGlobalChannel (const xBlyssFrame * f) {

  return vBlyssFrameGetBits (f, BLYSS_IDX_GCHAN, 4);
}

// -----------------------------------------------------------------------------
INLINE uint16_t
usBlyssFrameAddress (const xBlyssFrame * f) {

  return vBlyssFrameGetBits (f, BLYSS_IDX_ADDR, 16);
}

// -----------------------------------------------------------------------------
INLINE bool
bBlyssFrameState (const xBlyssFrame * f) {

  return ! vBlyssFrameGetBits (f, BLYSS_IDX_STATE, 4);
}

// -----------------------------------------------------------------------------
INLINE bool
bBlyssFrameMatch (const xBlyssFrame * f1, const xBlyssFrame * f2) {

  return memcmp (f1, f2, sizeof(xBlyssFrame)) == 0;
}

// -----------------------------------------------------------------------------
INLINE void
vBlyssFrameCopy (xBlyssFrame * dest, const xBlyssFrame * src) {

   memcpy (dest, src, sizeof(xBlyssFrame));
}

/* ========================================================================== */
#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
#endif /*_AVRIO_BLYSS_H_ defined */
