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
 * @file network.h
 * @brief Modélisation d'un réseau Nto1 WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111114 - Initial version by epsilonRT
 */
#ifndef _AVRIO_WUSB_NETWORK_H_
#define _AVRIO_WUSB_NETWORK_H_

#include <avrio/task.h>
#include <avrio/state.h>
#include <avrio/wusb/net/device/device.h>
#include <avrio/wusb/net/packet.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup wusb_module
 * @{
 *
 *  @defgroup wusb_network Modélisation d'un réseau WirelessUSB
 *  Couche Liaison - OSI2 Data Link Layer
 *  @{
 */

/* constants ================================================================ */

/**
 * @brief Majeur de la version de l'implémentation du réseau Nto1
 */
#define WNET_MAJOR  1

/**
 * @brief Majeur de la version de l'implémentation du réseau Nto1
 */
#define WNET_MINOR  0

/**
 * @brief Version de l'implémentation du réseau Nto1
 */
#define WNET_VERSION  "1.0"

/* == <API> == public application interface  ========class WusbNetwork======= */
/**
 * @brief PnCode utilisé pour la procédure de BIND
 */
#define WNET_BIND_PNCODE  0

/**
 * @brief Canal utilisé pour la procédure de BIND
 */
#define WNET_BIND_CHANNEL 0

/**
 * @brief Valeur indiquant un device ID invalide
 */
#define WNET_INVALID_DEVICEID 0xFFFF

/**
 * @enum eWNetErrorCode
 * @brief Codes d'erreurs et d'état du réseau Nto1
 * 0 signifie "Aucune erreur", une valeur négative indique une erreur, une 
 * valeur positive un état.
 */ 
typedef enum {

  WNET_SUCCESS  = 0, /**< Pas d'erreur */
  WNET_TIMEOUT  = -1, /**< Temps d'attente dépassé */
  WNET_CORRUPTED_PACKET_ERROR = -2, /**< Trop de paquet corrompus ont été reçus */
  WNET_LAST_ERROR = WNET_CORRUPTED_PACKET_ERROR
} eWNetErrorCode;

/**
 *    @defgroup wusb_network_config Configuration du réseau Nto1
 *    Fonctions permettant d'initialiser, de configurer et de connaître l'état
 *    du réseau Nto1 WirelessUSB.
 *    @{
 *******************************************************************************
 */

/**
 * @enum eWNetStatusCode
 * @brief Indice des bits de mode et d'état utilisés par le réseau
 */ 
typedef enum  {

  /* Etat  */
  WNET_EEPROMBLANK  = 0,  /**< Indique que l'EEPROM n'a pas été programmée */

  /* Contrôle  */
  WNET_SINGLEBYTE   = 1  /**< Valide le mode usDeviceId sur un octet */

} eWNetStatusCode;

/**
 * @struct xWNetConfig
 * @brief Configuration du réseau Nto1
 */
typedef struct xWNetConfig {

  uint8_t ucChannel;      /**< Canal */
  uint8_t ucPnCode;       /**< PnCode */
  uint8_t ucChecksumSeed; /**< Valeur initiale pour le calcul du checksum */
  uint8_t ucCrcSeed;      /**< Valeur initiale pour le calcul du CRC */
  uint8_t ucStatus;       /**< Bit de contrôle et d'état */
} xWNetConfig;

#if defined(__DOXYGEN__)
/**
 * @brief Initialise le réseau
 *  L'initialisation du réseau se fait avec les valeurs sauvegardées en EEPROM.
 *  La configuration initiale est la suivante:
 *  - Canal 1
 *  - PnCode 1
 *  - Crc Seed 0
 *  - Checksum Seed 0
 *  .
 * @param eDataRate vitesse de transmission. 0 pour initialiser à partir de la config en EEPROM.
 */
void vWNetInit(eWDevDataRate eDataRate);

/**
 * @brief Configuration courante
 */   
inline const xWNetConfig * pxWNetCurrentConfig(void);

/**
 * @brief Modifie la configuration à partir d'une structure xWNetConfig
 */   
void vWNetSetConfig(const xWNetConfig * pxConfig);

/**
 * @brief Sauvegarde la configuration courante en EEPROM
 */   
void vWNetSaveConfigToEEPROM(void);

/**
 *******************************************************************************
 *    @}
 *    @defgroup wusb_network_tx Transmission sur le réseau Nto1
 *    Fonctions permettant de modifier et d'envoyer le paquet de transmission
 *    sur le réseau.
 *    @{
 *******************************************************************************
 */

/**
 * @brief Envoie le paquet Tx sur le réseau
 * Le paquet doit être préparé avec les différentes fonctions d'assertion. Cette
 * fonction ne modifie pas le paquet qui est restitué en l'état.
 */   
inline void vWNetTxPktSend(void);

/**
 * @brief Ajoute le CRC et le checksum en queue de paquet
 * Cette fonction modifie le bit Signle Byte Device ID (D) en fonction de la 
 * configuration du réseau.
 */   
void vWNetTxPktAddTrailer (void);

/**
 * @brief Initialise une nouveau paquet de type ucPacketType
 */   
void vWNetTxPktNew (uint8_t ucPacketType);

/**
 * @brief Modifie le type du paquet TX 
 */   
inline void vWNetTxPktSetType (uint8_t ucPacketType);

/**
 * @brief Modifie l'entête du paquet TX
 */   
inline void vWNetTxPktSetHeader (uint8_t ucHeader);

/**
 * @brief Modifie l'état d'un bit de flag d'index \c ucIndex dans l'entête du paquet TX 
 */   
inline void vWNetTxPktSetFlag (uint8_t ucIndex, bool ucValue);

/**
 * @brief Bascule le flag d'index \c ucIndex dans l'entête du paquet TX 
 */   
inline void vWNetTxPktToggleFlag (uint8_t ucIndex);

/**
 * @brief  
 */   
inline void vWNetTxPktSetAllFlags (uint8_t ucFlags);

/**
 * @brief Ajoute le Device ID au paquet TX
 * L'ajout se fait en respectant le flag WNET_SINGLEBYTE de mode du réseau  
 */   
void vWNetTxPktAddDeviceId (uint16_t usDeviceId);

/**
 * @brief Ajoute un octet au paquet TX
 */   
inline void vWNetTxPktAddByte (uint8_t ucByte);

/**
 * @brief Ajoute un mot de 16 bits au paquet TX
 */   
inline void vWNetTxPktAddWord (uint16_t usWord);

/**
 * @brief Ajoute une suite d'octets au paquet TX
 */   
inline void vWNetTxPktAddBytes (const uint8_t * pucBytes, uint8_t ucLength);

/**
 * @brief Ajoute une suite d'octets résidant en mémoire programme au paquet TX
 */   
inline void vWNetTxPktAddBytes_P (const uint8_t * pucBytes, uint8_t ucLength);

/**
 * @brief Ajoute une chaîne de caractères à zéro terminal au paquet TX
 * Le zéro final n'est pas ajouté au paquet 
 */   
inline void vWNetTxPktAddStr (const char * pcStr);

/**
 * @brief Ajoute une chaîne de caractères à zéro terminal résidant en mémoire programme au paquet TX
 * Le zéro final n'est pas ajouté au paquet 
 */   
inline void vWNetTxPktAddStr_P (const char * pcStr);

/**
 * @brief Renvoie un pointeur sur le paquet de transmission
 */   
inline xWusbPacket * pxWNetTxPacket (void);

/**
 *******************************************************************************
 *    @}
 *    @defgroup wusb_network_rx Réception sur le réseau Nto1
 *    Fonctions permettant de recevoir et de lire le paquet de reçu
 *    sur le réseau.
 *    @{
 *******************************************************************************
 */

/**
 * @brief Attends la réception d'un paquet.
 * Si \c iTimeoutMs négatif, la fonction attends indéfiniment et sort uniquement 
 *  sur réception d'un paquet correct ou sur erreur. 
 * @param iTimeoutMs temps d'attente en millisecondes. -1 infini. 
 * @return 0 (WNET_SUCCESS) en cas de succès ou le code d'erreur
 */   
eWNetErrorCode eWNetRxPktWait(int16_t iTimeoutMs);

/**
 * @brief Renvoie le délai d'attente pour la réception d'un paquet ACK
 * La valeur renvoyée tient compte des paramètres du réseau.
 * @note Prévue pour être utilisée pour l'appel de eWNetRxPktWait()
 */   
inline uint8_t ucWNetAckTimeout (void);

/**
 * @brief Renvoie le délai d'attente pour la réception d'un paquet BIND
 * La valeur renvoyée tient compte des paramètres du réseau.
 * @note Prévue pour être utilisée pour l'appel de eWNetRxPktWait()
 */   
inline uint8_t ucWNetBindTimeout (void);

/**
 * @brief Renvoie le délai d'attente pour la réception d'un paquet PING
 * La valeur renvoyée tient compte des paramètres du réseau.
 * @note Prévue pour être utilisée pour l'appel de eWNetRxPktWait()
 */   
inline uint8_t ucWNetPingTimeout (void);

/**
 * @brief Type du paquet reçu 
 */   
inline uint8_t ucWNetRxPktType (void);

/**
 * @brief Entête du paquet reçu 
 */   
inline uint8_t ucWNetRxPktHeader (void);

/**
 * @brief Etat du flag d'index \c ucIndex de l'entête du paquet reçu 
 */   
inline bool xWNetRxPktFlag (uint8_t ucIndex);

/**
 * @brief Octet d'offset \c ucOffset du paquet reçu
 * L'offset 0 correspond à l'entête du paquet  
 */   
inline uint8_t ucWNetRxPktByte (uint8_t ucOffset);

/**
 * @brief Mot de 16 bits d'offset \c ucOffset du paquet reçu
 */   
inline uint16_t usWNetRxPktWord (uint8_t ucOffset);

/**
 * @brief Mot de 32 bits d'offset \c ucOffset du paquet reçu
 */   
inline uint32_t ulWNetRxPktDword (uint8_t ucOffset);

/**
 * @brief Pointeur sur l'octet d'offset \c ucOffset du paquet reçu
 */   
inline uint8_t * pucWNetRxPktBytes (uint8_t ucOffset);

/**
 * @brief Device ID du paquet reçu
 * Cette fonction s'adapte en fonction du bit WPKT_SINGLE reçu. Si ce bit est à 
 * 1 dans le paquet reçu, le Device ID est lu comme un octet et converti en uint16_t.   
 * Cette fonction ne doit être utilisée que si le paquet reçu est de type DATA, ACK ou BIND_RESPONSE    
 */   
uint16_t usWNetRxPktDeviceId (void);

/**
 * @brief Pointeur sur le premier octet du payload du paquet DATA reçu
 * Si le paquet reçu n'est pas un paquet DATA, cette fonction renvoie 0.  
 */   
inline uint8_t * pucWNetRxPktDataPayload (void);

/**
 * @brief Longueur du payload du paquet DATA reçu 
 * Si le paquet reçu n'est pas un paquet DATA, cette fonction renvoie 0.  
 */   
inline uint8_t ucWNetRxPktDataPayloadLength (void);

/**
 * @brief Pointeur sur le paquet de réception
 */   
inline xWusbPacket * pxWNetRxPacket (void);

/**
 *******************************************************************************
 *    @}
 *    @addtogroup wusb_network_config
 *    @{
 */

/**
 * @brief Index du canal courant
 */   
inline uint8_t ucWNetChannel(void);

/**
 * @brief Modifie le canal
 * @param ucIndex ucIndex du canal
 * @return true Succès. False Erreur.
 */
bool xWNetSetChannel(uint8_t ucIndex);

/**
 * @brief Passe au canal suivant dans le subset utilisé
 */   
void vWNetGoToNextChannel(void);

/**
 * @brief Nombre de canaux possibles
 */
inline uint8_t ucWNetNumberOfChannelsUsed (void);

/**
 * @brief Nombre de sous-ensembles de canaux
 */
inline uint8_t ucWNetNumberOfSubsets (void);

/**
 * @brief Index du Pn Code courant
 */   
inline uint8_t ucWNetPnCode(void);

/**
 * @brief Modifie le Pn Code du réseau
 * @param ucIndex ucIndex du PnCode
 * @return true Succès. False Erreur.
 */
bool xWNetSetPnCode(uint8_t ucValue);

/**
 * @brief Nombre de PnCode possibles
 */
uint8_t ucWNetNumberOfPnCodes (void);

/**
 * @brief Checksum seed courant
 */   
inline uint8_t ucWNetChecksumSeed(void);

/**
 * @brief Modifie le checksum seed
 */   
inline void vWNetSetChecksumSeed(uint8_t ucValue);

/**
 * @brief CRC seed courant
 */   
inline uint8_t ucWNetCrcSeed(void);

/**
 * @brief Modifie le crc seed
 */   
inline void vWNetSetCrcSeed(uint8_t ucValue);

/**
 * @brief Etat logique d'un bit d'état ou de contrôle de la configuration du réseau
 */ 
inline bool xWNetStatus (eWNetStatusCode eFlag);

/**
 * @brief Modifie l'état d'un bit d'état ou de contrôle de la configuration du réseau
 */ 
void vWNetSetStatus (eWNetStatusCode eFlag, bool ucValue);

/**
 * @brief Bascule l'état d'un bit d'état ou de contrôle de la configuration du réseau
 */ 
void vWNetToggleStatus (eWNetStatusCode eFlag);

/**
 * @brief Renvoie true si les Device ID sont codés sur un octet
 */   
inline bool xWNetSingleByteDeviceId (void);

/**
 * @brief Active le codage des Device ID sur un octet
 */   
inline void vWNetSetSingleByteDeviceId (bool xEnable);

/**
 *******************************************************************************
 *    @}
 */
 
/**
 * @brief Renvoie la dernière erreur
 */   
inline eWNetErrorCode eWNetError(void);

/**
 * @brief Attends que le réseau passe à l'état Disponible
 */
inline void vWNetWaitIdle (void);

/**
 * @brief Renvoie un nombre pseudo aléatoire compris entre 1 et 31
 * Le générateur pseudo aléatoire est initilialisé lors du premier appel avec 
 *  le MID et le temps système   
 */   
uint8_t ucWNetRand (void);

/* == <API-END> == public application interface  ====class WusbNetwork======= */
/**
 *    @} 
 * @} 
 */

#else /* __DOXYGEN__ defined */
# include "wusb-config.h"

/* constants ================================================================ */
/*---- Channel configuration table -------------------------------------------*/
#if WUSB_CONFIG_NETWORK_CHANNEL_CONFIG == 1
 #define WNET_NUM_OF_SUBSETS          6
 #define WNET_NUM_OF_CH_PER_SUBSET    13
 #define WNET_NUM_OF_CH_USED          78
#elif WUSB_CONFIG_NETWORK_CHANNEL_CONFIG == 2
 #define WNET_NUM_OF_SUBSETS          7
 #define WNET_NUM_OF_CH_PER_SUBSET    11
 #define WNET_NUM_OF_CH_USED          77
#elif WUSB_CONFIG_NETWORK_CHANNEL_CONFIG == 3
 #define WNET_NUM_OF_SUBSETS          8
 #define WNET_NUM_OF_CH_PER_SUBSET    9
 #define WNET_NUM_OF_CH_USED          72
#elif WUSB_CONFIG_NETWORK_CHANNEL_CONFIG == 4
 #define WNET_NUM_OF_SUBSETS          9
 #define WNET_NUM_OF_CH_PER_SUBSET    8
 #define WNET_NUM_OF_CH_USED          72
#elif WUSB_CONFIG_NETWORK_CHANNEL_CONFIG == 5
 #define WNET_NUM_OF_SUBSETS          10
 #define WNET_NUM_OF_CH_PER_SUBSET    7
 #define WNET_NUM_OF_CH_USED          70
#elif WUSB_CONFIG_NETWORK_CHANNEL_CONFIG == 6
 #define WNET_NUM_OF_SUBSETS          11
 #define WNET_NUM_OF_CH_PER_SUBSET    7
 #define WNET_NUM_OF_CH_USED          77
#elif WUSB_CONFIG_NETWORK_CHANNEL_CONFIG == 7
 #define WNET_NUM_OF_SUBSETS          12
 #define WNET_NUM_OF_CH_PER_SUBSET    6
 #define WNET_NUM_OF_CH_USED          72
#elif WUSB_CONFIG_NETWORK_CHANNEL_CONFIG == 8
 #define WNET_NUM_OF_SUBSETS          13
 #define WNET_NUM_OF_CH_PER_SUBSET    6
 #define WNET_NUM_OF_CH_USED          78
#else
 #error Unsupported CHANNEL_CONFIG value
#endif

/* structures =============================================================== */
typedef struct WusbNetwork {

  xWusbPacket pxRxPacket;
  xWusbPacket pxRxValid;
  xWusbPacket pxTxPacket;
  uint16_t usCorruptError;
  xTaskHandle xTimeoutTask;
  xState xRxStatus;
  xState xError;
  xWNetConfig xConfig;
} WusbNetwork;

/* internal public functions ================================================ */
void vWNetInit(eWDevDataRate eDataRate);
void vWNetSetConfig(const xWNetConfig * pxConfig);
void vWNetSaveConfigToEEPROM(void);
bool xWNetSetChannel(uint8_t ucValue);
void vWNetGoToNextChannel(void);
bool xWNetSetPnCode(uint8_t ucValue);
void vWNetTxPktNew (uint8_t ucType);
void vWNetTxPktAddDeviceId (uint16_t usDeviceId);
void vWNetTxPktAddTrailer (void);
eWNetErrorCode eWNetRxPktWait(int16_t iTimeoutMs);
uint16_t usWNetRxPktDeviceId (void);
uint8_t ucWNetRand (void);
uint8_t ucWNetNumberOfPnCodes (void);

/* public variables ========================================================= */
extern WusbNetwork dNet;

/* inline public functions ================================================== */
// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktSend(void)) {

  vWDevTransmit (pucWPktRaw (&dNet.pxTxPacket), xWPktLength (&dNet.pxTxPacket));
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t
  ucWNetAckTimeout (void)) {

  return (WUSB_CONFIG_NETWORK_DEFAULT_TIMEOUT_ACK * (3-eWDevGetDataRate()));
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetBindTimeout (void)) {

  return (WUSB_CONFIG_NETWORK_DEFAULT_TIMEOUT_BIND * (3-eWDevGetDataRate()));
}

__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetPingTimeout (void)) {

  return (WUSB_CONFIG_NETWORK_DEFAULT_TIMEOUT_PING * (3-eWDevGetDataRate()));
}

/*------------ Getters Net Config -------------*/
// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetWaitIdle (void)) {

  vWDevWaitStatus (WDEV_IDLE);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (const xWNetConfig * 
  pxWNetCurrentConfig(void)) {

  return &dNet.xConfig;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetChannel(void)) {

  return dNet.xConfig.ucChannel;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetChecksumSeed(void)) {

  return dNet.xConfig.ucChecksumSeed;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetCrcSeed(void)) {

  return dNet.xConfig.ucCrcSeed;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetPnCode(void)) {

  return dNet.xConfig.ucPnCode;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (bool 
  xWNetStatus (eWNetStatusCode eFlag)) {

  return xWPktBit (dNet.xConfig.ucStatus, eFlag);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (bool 
  xWNetSingleByteDeviceId (void)) {

  return xWNetStatus (WNET_SINGLEBYTE);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t
  ucWNetNumberOfChannelsUsed (void)) {

  return WNET_NUM_OF_CH_USED;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t
  ucWNetNumberOfSubsets (void)) {

  return WNET_NUM_OF_SUBSETS;
}

/*------------ Getters Net Error -------------*/
__STATIC_ALWAYS_INLINE (eWNetErrorCode 
  eWNetError(void)) {

  return (eWNetErrorCode) xStateGet(&dNet.xError);
}

/*------------ Getters Net Tx Packet -------------*/
// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (xWusbPacket * 
  pxWNetTxPacket(void)) {
  
  return &dNet.pxTxPacket;
}

/*------------ Getters Net Rx Packet -------------*/
// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (xWusbPacket * 
  pxWNetRxPacket(void)) {

  return &dNet.pxRxPacket;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetRxPktType (void)) {

  return ucWPktType (pxWNetRxPacket());
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetRxPktHeader (void)) {

  return ucWPktHeader (pxWNetRxPacket());
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (bool 
  xWNetRxPktFlag (uint8_t ucIndex)) {

  return xWPktFlag (pxWNetRxPacket(), ucIndex);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetRxPktByte (uint8_t ucOffset)) {

  return ucWPktByte (pxWNetRxPacket(), ucOffset);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint16_t 
  usWNetRxPktWord (uint8_t ucOffset)) {

  return usWPktWord (pxWNetRxPacket(), ucOffset);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint32_t 
  ulWNetRxPktDword (uint8_t ucOffset)) {

  return ulWPktDword (pxWNetRxPacket(), ucOffset);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t * 
  pucWNetRxPktBytes (uint8_t ucOffset)) {

  return &pucWPktRaw(pxWNetRxPacket())[ucOffset];

}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t * 
  pucWNetRxPktDataPayload (void)) {

  return pucWPktDataPayload (pxWNetRxPacket());
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
  ucWNetRxPktDataPayloadLength (void)) {

  return xWPktDataPayloadLength (pxWNetRxPacket());
}

/*------------ Setters Net Config -------------*/
// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void
  vWNetSetChecksumSeed(uint8_t ucValue)) {

  dNet.xConfig.ucChecksumSeed = ucValue;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vWNetSetCrcSeed(uint8_t ucValue)) {

  dNet.xConfig.ucCrcSeed = ucValue;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetSetStatus (eWNetStatusCode eFlag, bool ucValue)) {

  vWPktSetBit (&dNet.xConfig.ucStatus, eFlag, ucValue);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetSetSingleByteDeviceId (bool xEnable)) {

  vWNetSetStatus (WNET_SINGLEBYTE, xEnable);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetToggleStatus (eWNetStatusCode eFlag)) {

  dNet.xConfig.ucStatus ^= 1<<eFlag;
}

/*------------ Setters Net Tx Packet -------------*/
// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktAddByte (uint8_t ucByte)) {

  vWPktAddByte (pxWNetTxPacket(), ucByte);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktAddBytes (const uint8_t * pucBytes, uint8_t ucLength)) {

  vWPktAddBytes (pxWNetTxPacket(), pucBytes, ucLength);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktAddBytes_P (const uint8_t * pucBytes, uint8_t ucLength)) {

  vWPktAddBytes_P (pxWNetTxPacket(), pucBytes, ucLength);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktAddStr (const char * pcStr)) {

  vWPktAddStr (pxWNetTxPacket(), pcStr);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktAddStr_P (const char * pcStr)) {

  vWPktAddStr_P (pxWNetTxPacket(), pcStr);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktAddWord (uint16_t usWord)) {

  vWPktAddWord (pxWNetTxPacket(), usWord);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktSetAllFlags (uint8_t ucFlags)) {

  xWPktSetAllFlags (pxWNetTxPacket(), ucFlags);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktSetFlag (uint8_t ucIndex, bool ucValue)) {

  vWPktSetFlag (pxWNetTxPacket(), ucIndex, ucValue);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vWNetTxPktSetHeader (uint8_t ucHeader)) {

  vWPktSetHeader (pxWNetTxPacket(), ucHeader);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktSetType (uint8_t ucPacketType)) {

  vWPktSetType (pxWNetTxPacket(), ucPacketType);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
  vWNetTxPktToggleFlag (uint8_t ucIndex)) {

  vWPktToggleFlag (pxWNetTxPacket(), ucIndex);
}

/* ========================================================================== */
/*                     Tests unitaires (non documentés)                       */
/* ========================================================================== */
/* structures =============================================================== */
  typedef struct xWNetTestResult {

    uint16_t usSended;
    uint16_t usReceived;
    uint16_t usCorrupted;
  } xWNetTestResult;

#ifdef WNET_TEST
/* internal public functions ================================================ */
  int iWNetTestSetup (void);
  
  /*
   * Attends un paquet:
   * - Si paquet correct reçu, vérifie le type:
   *  - Si le type est un WPKT_DATA_PACKET, un WPKT_ACK_PACKET est renvoyé (V=1)
   *  - Si le type est un WPKT_BIND_REQUEST_PACKET, un WPKT_BIND_RESPONSE_PACKET
   *   est renvoyé
   *  - Pour les autres types, ne renvoie rien   
   * - Si paquet incorrect, ne fait rien
   * @return true si paquet correct reçu     
   */   
  eWNetErrorCode eWNetTestHub (int16_t iTimeoutMs);
  /*
   * Envoi paquet DATA et attends la réponse HUB en recommancant tant que le HUB
   * n'a pas répondu correctement.
   */ 
  eWNetErrorCode eWNetTestSensorPing (uint16_t usCount, uint16_t usInterval, 
                                                  xWNetTestResult * pxResult);
  /*
   * Envoi paquet BIND_REQUEST et attends réponse HUB en recommancant tant que 
   * le HUB n'a pas répondu correctement.
   */ 
  eWNetErrorCode eWNetTestSensorBind (uint16_t usMaxRetries, uint16_t usInterval, 
                                                  xWNetTestResult * pxResult);
#else   /* WNET_TEST defined */
  #define iWNetTestSetup() (0)
  #define eWNetTestHub(t) (WNET_SUCCESS)
  #define eWNetTestSensorPing(c,i,r) (WNET_SUCCESS)
  #define eWNetTestSensorBind(m,i,r) (WNET_SUCCESS)
#endif  /* WNET_TEST defined */

#ifdef WNET_DEBUG_ONUART
  void vWNetDbgDumpConfig(void);
  void vWNetDbgDumpRxPacket(void);
  void vWNetDbgDumpTxPacket(void);
  void vWNetDbgNoResp(void);
  void vWNetDbgSend(void);
#else
  #define vWNetDbgDumpConfig()
  #define vWNetDbgDumpRxPacket()
  #define vWNetDbgDumpTxPacket()
  #define vWNetDbgNoResp()
  #define vWNetDbgSend()
#endif  /* WNET_DEBUG_ONUART defined */

#endif  /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_WUSB_NETWORK_H_ */
