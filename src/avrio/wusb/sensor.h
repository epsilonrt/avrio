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
 * @file sensor.h
 * @brief Capteur WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20110921 - Initial version by epsilonRT
 */
#ifndef _AVRIO_WUSB_SENSOR_H_
#define _AVRIO_WUSB_SENSOR_H_

#include <avrio/wusb/net/network.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup wusb_module
 * @{
 *
 *  @defgroup wusb_sensor_module Capteur Nto1 WirelessUSB
 *  Modélisation d'un capteur Nto1 WirelessUSB&tm; (sensor)
 *  @{
 */

/* constants ================================================================ */
/**
 * @enum eWSensorFlags
 * @brief Indice des bits de mode et d'état utilisés par un capteur
 */ 
typedef enum {

  WSENSOR_EEPROMBLANK     = 0,  /**< Indique que l'EEPROM n'a pas été programmée */

  /* Mode */
  WSENSOR_AUTOSEEDEDBIND  = 1, /**< Autorise le Seeded Bind auto si le Sensor est invalidé par le Hub (Défaut) */
  /* Bit 2 dispo. */
  /* Status */
  WSENSOR_NACK            = 3, /**< Indique que le hub n'a pas répondu */
  WSENSOR_CHANSEARCH      = 4, /**< Le mode Recherche de canal est actif */
  WSENSOR_BACKDATA        = 5, /**< Le hub a renvoyé des données */
  WSENSOR_BINDING         = 6, /**< Le sensor est en cours d'intégration au réseau */
  WSENSOR_BOUND           = 7  /**< Le sensor est intégré au réseau */
} eWSensorFlags;

/**
 * @enum eWSensorError
 * @brief Codes d'erreurs d'un capteur
 */ 
typedef enum {

  WSENSOR_SUCCESS      =  WNET_SUCCESS,  /**< Pas d'erreur */

  /* Erreur < 0 */
  WSENSOR_NACK_TIMEOUT =  WNET_TIMEOUT, /**< Indique que le hub n'a pas répondu */
  WSENSOR_PACKET_ERROR =  WNET_CORRUPTED_PACKET_ERROR, /**< Trop de paquet corrompus ont été reçus */
  WSENSOR_NODE_UNBOUND   = (WNET_LAST_ERROR - 1), /**< Le capteur a été déconnecté du réseau */
  WSENSOR_NODE_REBOUND   = (WNET_LAST_ERROR - 2), /**< Le capteur a été déconnecté et reconnecté au réseau */
  WSENSOR_SEARCH_TIMEOUT = (WNET_LAST_ERROR - 3), /**< La recherche de canal a échouée */
  WSENSOR_BIND_TIMEOUT   = (WNET_LAST_ERROR - 4) /**< L'intégration au réseau a échouée */
} eWSensorError;

/* == <API> == public application interface  ========class xWusbSensor======== */

/**
 * @brief Initialisation de la couche application capteur WirelessUSB&tm;
 * @param DataRate vitesse de transmission. 0 pour initialiser à partir de la config en EEPROM.
 */   
void vWSensorInit(eWDevDataRate DataRate);

/**
 * @brief Intègre le capteur au réseau WirelessUSB&tm;
 * Envoie des paquets BIND REQUEST à la recherche d'un hub. Passe en revue tous
 * les canaux du sous-ensemble (subset) utilisé pour le BIND (Canal 0 - PnCode 0
 *  si Seeded = false, Canal et PnCode courant si Seeded = true). Cette fonction
 *  effectue plusieurs tentatives sur un canal avant de passer au suivant dans le
 *  subset si aucune réponse n'arrive. Le balayage de tous les canaux est 
 *   effectué plusieurs fois. 
 * Si une réponse avec un DeviceID valide arrive (différent de 0xFFFF), cette 
 * fonction :
 * - Réponds au hub en lui envoyant un ACK sur le canal utilisé pour le BIND
 * - Reconfigure la couche application avec ces nouveaux paramètres
 * - Mémorise les paramètres réseau envoyés par le hub en EEPROM
 * - Valide le mode recherche de canal pour que le capteur trouve le canal utilisé dans le subset pour la transmission de données 
 * - Renvoie true.          
 * @param Seeded true intégration rapide sur le canal DATA en cours, false sur canal de BIND
 * @note Paramètres de compilation:
 * - WUSB_CONFIG_SENSOR_BIND_CYCLES Nombre de fois que la fonction va balayer l'ensemble des canaux du subset (2)
 * - WUSB_CONFIG_SENSOR_NACK_RETRIES Nombre d'essais sur chaque canal  (2)
 * - WUSB_CONFIG_NETWORK_DEFAULT_TIMEOUT_BIND Temps de réponse max à un BIND REQUEST (12 ms en 16KBps, 6 ms en 64 KBps)    
 */   
void vWSensorBind(bool Seeded);

/**
 * @brief Gestionnaire d'endormissement
 * Cette fonction doit être implémentée par l'utilisateur. Elle est appellée
 * lorsque le hub ne répond pas ou plus.
 * @param eError Code erreur ayant provoqué l'appel à la fonction qui peut
 * prendre deux valeurs WSENSOR_SEARCH_TIMEOUT ou WSENSOR_BIND_TIMEOUT
 */   
void vWSensorSleepCB (eWSensorError eError);

#if defined(__DOXYGEN__)
/**
 * @brief Efface les données à envoyer au hub et prépare le prochain paquet DATA
 */   
void vWSensorDataClear(void);

/**
 * @brief Ajoute un octet aux données à transmettre au hub
 */   
inline void vWSensorDataAddByte(uint8_t ucByte);

/**
 * @brief Ajoute un mot de 16 bits aux données à transmettre au hub
 */   
inline void vWSensorDataAddWord(uint16_t ucWord);

/**
 * @brief Ajoute des octets aux données à transmettre au hub
 */   
inline void vWSensorDataAddBytes(const uint8_t * pucData, uint8_t ucLength);

/**
 * @brief Ajoute des octets stockés en mémoire programme aux données à transmettre au hub
 */   
inline void vWSensorDataAddBytes_P(const uint8_t * pucData, uint8_t ucLength);

/**
 * @brief Ajoute une chaîne de caractères aux données à transmettre au hub
 * Le zéro final n'est pas ajouté au paquet 
 */   
inline void vWSensorDataAddStr(const char * pcStr);

/**
 * @brief Ajoute une chaîne de caractères stockée en mémoire programme aux 
 *  données à transmettre au hub
 * Le zéro final n'est pas ajouté au paquet 
 */   
inline void vWSensorDataAddStr_P(const char * pcStr);

/**
 * @brief Transmission/Réception de données vers le hub
 * Cette fonction effectue les opérations suivantes: \n
 * - Formate le paquet DATA en y ajoutant les drapeaux nécessaires (séquence ...) etc.
 * - Envoie le paquet DATA
 * - Attends une réponse du hub (WUSB_CONFIG_NETWORK_DEFAULT_TIMEOUT_ACK)
 * - Si une réponse arrive ACK ou DATA qui correspond à la réponse du paquet envoyé
 *   - Le flag de séquence TX est basculé
 *   - Si le paquet reçu est de type ACK et que le bit V est à zéro, le capteur 
 *     retourne à l'état UNBOUND et, si le drapeau WSENSOR_AUTOSEEDEDBIND est 
 *     levé, passe en BIND rapide. A l'issue WSENSOR_NODE_UNBOUND ou 
 *     WSENSOR_NODE_REBOUND est renvoyé. 
 *   - Si le paquet reçu est de type DATA 
 *      - un paquet ACK est envoyé au hub
 *      - avec une longueur nulle, les flags de séquences sont réinitialisés T=A=0
 *      - avec des octets, le flag de status WSENSOR_BACKDATA est levé (le
 *        d'octets reçus est renvoyé.
 *      .
 *   . 
 * - Si aucune réponse arrive:
 *  - Si Synchronous = true, cette fonction retourne 0
 *  - Si Synchronous = false :
 *    - Elle attent un délai pseudo-aléatoire dépendant du MID capteur et  
 *      réémet le paquet.
 *    - Recommence un certain nombre de fois (WUSB_CONFIG_SENSOR_NACK_RETRIES).
 *    - Si toujours pas de réponse, passe en mode recherche de canal:
 *      - Passe au canal suivant.
 *      - Attends une durée fixe (WUSB_CONFIG_SENSOR_SEARCH_CHANNEL_DELAY)
 *      - Réémet plusieurs fois le paquet sur ce nouveau canal. 
 *       (WUSB_CONFIG_SENSOR_NACK_RETRIES) en attendant la même 
 *        durée entre chaque envoi.
 *      - Recommence cette recherche périodique sur tous les canaux du subset.
 *      .
 *    - Si toujours pas de réponse, renvoie WSENSOR_SEARCH_TIMEOUT.
 *    .        
 *  .        
 * .          
 *     
 * @param Synchronous true indique une transmission synchrone (pas d'attente de réponse du hub)
 * @return
 * - 0 si les données ont été transmises au hub (pas de données en retour).
 * - Une valeur positive si les données ont été transmises au hub et que le hub 
 *   a renvoyé des données en retour (la valeur correspondant au nombre d'octets).
 * - Une valeur négative en cas d'erreur (eWSensorError)
 * .
 * @warning Les données à transmettre sont détruites par iWSensorDataSend(). 
 * (Un appel à vWSensorDataClear() est effectué avec la sortie).
 */   
int iWSensorDataSend(bool Synchronous);

/**
 * @brief Renvoie un pointeur sur les données renvoyées par le hub lors de la 
 *  dernière transmission
 * @return Le pointeur ou 0 si aucune donnée reçue
 */   
uint8_t * pucWSensorBackDataPayload (void);

/**
 * @brief Renvoie le nombre d'octets renvoyés par le hub lors de la 
 *  dernière transmission
 * @return Nombre d'octets ou 0 si aucune donnée reçue
 */   
uint8_t ucWSensorBackDataPayloadLength (void);

/**
 * @brief Indique si le sensor est intégré à un réseau
 */   
inline bool xWSensorBound (void);

/**
 * @brief Autorise le BIND rapide automatique en cas de retrait du réseau par le hub
 */   
inline void vWSensorSetAutoSeededBind (bool xEnable);

/**
 * @brief Indique si le BIND rapide automatique en cas de retrait du réseau par le hub est autorisé.
 */   
inline bool xWSensorAutoSeededBind (void);

/**
 * @brief Teste un bit d'état d'indice eFlag
 */   
inline bool xWSensorFlag(eWSensorFlags eFlag);

/**
 * @brief Renvoie le Device ID attribué par le hub (WNET_INVALID_DEVICEID si invalide)
 */   
inline uint16_t usWSensorDeviceId(void);

/**
 * @brief Endort ou réveille le module WirelessUSB afin d'économiser de l'énergie
 * Le module est à l'état statique. Les registres du module ne sont pas modifiés.
 * @param Sleep true pour endormir, false pour réveiller
 */
inline void vWSensorSleep (bool xSleep);

/* == <API-END> == public application interface  ====class xWusbSensor======== */
/**
 *   @}
 * @}
 */

#else /* __DOXYGEN__ defined */

/* structures =============================================================== */
/*
 * @struct xWSensorConfig
 * @brief Configuration d'un capteur
 */  
typedef struct xWSensorConfig {

  uint16_t usDeviceId; /*< Identifiant attribué par le hub */
  uint8_t ucFlags; /*< Bits d'états et de mode */
} xWSensorConfig;

/*
 * @struct xWusbSensor
 * @brief Classe capteur d'un réseau WirelessUSB&tm;
 */ 
typedef struct xWusbSensor {

  uint8_t ucPktBits; /*< Drapeaux transmis dans l'entête de paquet */
  xWSensorConfig xConfig;  /*< Configuration */
} xWusbSensor;

/* internal public functions ================================================ */
void vWSensorDataClear(void);
int iWSensorDataSend(bool Synchronous);
uint8_t * pucWSensorBackDataPayload (void);
uint8_t ucWSensorBackDataPayloadLength (void);

/* public variables ========================================================= */
extern xWusbSensor dSensor;

/* inline public functions ================================================== */
__STATIC_ALWAYS_INLINE (void 
vWSensorSleep (bool xSleep)) {

  vWDevSleep (xSleep);
}

__STATIC_ALWAYS_INLINE (bool 
xWSensorFlag (eWSensorFlags eFlag)) {

  return xWPktBit (dSensor.xConfig.ucFlags, eFlag);
}

__STATIC_ALWAYS_INLINE (bool 
xWSensorBound (void)) {
  
  return xWSensorFlag (WSENSOR_BOUND);
}

__STATIC_ALWAYS_INLINE (bool 
xWSensorAutoSeededBind (void)) {

  return xWSensorFlag (WSENSOR_AUTOSEEDEDBIND);
}

__STATIC_ALWAYS_INLINE (uint16_t 
usWSensorDeviceId(void)) {

  return  dSensor.xConfig.usDeviceId;
}

__STATIC_ALWAYS_INLINE (void 
vWSensorSetFlag (eWSensorFlags eFlag, bool xValue)) {

    vWPktSetBit (&dSensor.xConfig.ucFlags, eFlag, xValue);
}

__STATIC_ALWAYS_INLINE (void 
vWSensorSetAutoSeededBind (bool xEnable)) {

  vWSensorSetFlag (xEnable, WSENSOR_AUTOSEEDEDBIND);
}

/*------------ Setters Net Tx Packet -------------*/
__STATIC_ALWAYS_INLINE (void 
vWSensorDataAddByte(uint8_t ucByte)) {

  vWNetTxPktAddByte (ucByte);
}

__STATIC_ALWAYS_INLINE (void 
vWSensorDataAddWord(uint16_t usWord)) {

  vWNetTxPktAddWord (usWord);
}

__STATIC_ALWAYS_INLINE (void 
vWSensorDataAddBytes(const uint8_t * pucData, uint8_t ucLength)) {

  vWNetTxPktAddBytes (pucData, ucLength);
}

__STATIC_ALWAYS_INLINE (void 
vWSensorDataAddBytes_P(const uint8_t * pucData, uint8_t ucLength)) {

  vWNetTxPktAddBytes_P (pucData, ucLength);
}

__STATIC_ALWAYS_INLINE (void 
vWSensorDataAddStr(const char * pcStr)) {

  vWNetTxPktAddStr (pcStr);
}

__STATIC_ALWAYS_INLINE (void 
vWSensorDataAddStr_P(const char * pcStr)) {

  vWNetTxPktAddStr_P (pcStr);
}

#ifdef WSENSOR_TEST
  int iSensorTestPing (uint16_t usCount, uint16_t usInterval, 
                        xWNetTestResult * pxResult);
#else   /* WSENSOR_TEST not defined */
# define iSensorTestPing(c,i,r) (0)
#endif  /* WSENSOR_TEST defined */

#endif  /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_WUSB_SENSOR_H_ */
