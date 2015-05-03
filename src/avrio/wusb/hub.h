/**
 * @file hub.h
 * @brief Hub WirelessUSB
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20110921 - Initial version by epsilonRT
 */
#ifndef _AVRIO_WUSB_HUB_H_
#define _AVRIO_WUSB_HUB_H_

#include <avrio/mutex.h>
#include <avrio/task.h>
#include <avrio/wusb/net/network.h>
#include <avrio/wusb/hub/sensor_database.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup wusb_module
 * @{
 *
 *  @defgroup wusb_hub_module Hub WirelessUSB
 *  Modélisation d'un concentrateur WirelessUSB&tm; (hub)
 *  @{
 */

/* macros =================================================================== */
/* constants ================================================================ */
#define WHUB_MAJOR  (0x01)
#define WHUB_MINOR  (0x00)
#define WHUB_BUILD  (0x00)

/**
 * @enum eWHubStatus
 * @brief Indice des bits de mode et d'état utilisés par un hub
 */ 
typedef enum  {

  WHUB_EEPROMBLANK    = 0,  /**< Indique que l'EEPROM n'a pas été programmée */
  
  /* Mode */
  WHUB_AUTOBIND       = 1, /**< Valide le mode d'intégration automatique */
  WHUB_SEEDEDBIND     = 2, /**< Valide le mode d'intégration rapide */
  WHUB_SEARCHCHANNEL  = 3, /**< Valide le mode recherche de canal */

  
  /* Sensor Status */
  WHUB_BACKDATA       = 4, /**< Indique que des données doivent être transmise à un capteur */
  WHUB_NODEBOUND      = 5, /**< Indique qu'un capteur est intégré au réseau */

  /* Hub Status */
  WHUB_BIND           = 6, /**< Indique que le hub est en mode intégration */
  WHUB_CONFIGSAVED    = 7  /**< Indique que la configuration a été sauvée en EEPROM */
   
} eWHubStatus;

/**
 * @enum eWHubMsgId
 * @brief Identifiants des messages échangées du hub vers l'hôte
 */ 
typedef enum {

  WHUB_RESPONSE             = 0x80,
  WHUB_RSP_HUB_INFO         = 0x81,  
  WHUB_RSP_BIND             = 0x82,  
  WHUB_RSP_BIND_INFO        = 0x83,  
  WHUB_RSP_DELETE_NODE      = 0x84,  
  WHUB_RSP_SEND_MSG         = 0x85,  
  WHUB_RSP_INCOMING_MSG     = 0x86,  
  WHUB_RSP_ENUM_DEVICES     = 0x87,  
  WHUB_RSP_NETWORK_CONFIG   = 0x88,  
  WHUB_RSP_NETWORK_STATUS   = 0x89,  
  WHUB_RSP_RESET            = 0x8A,  
  WHUB_RSP_CHANGE_CHANNEL   = 0x8B,  
  WHUB_RSP_UNKNOWN_CMD      = 0xFF  
} eWHubMsgId;

/**
 * @enum eWHubMsgStatus
 * @brief Comptes rendus échangées entre hub et hôte
 */ 
typedef enum  {

  WHUB_STA_SUCCESS                = 0x00,
  WHUB_STA_UNKNOWN_ID             = 0x01,
  WHUB_STA_BUFFER_OVERFLOW        = 0x02,
  WHUB_STA_PRIOR_PAYLOAD_LOST     = 0x03,
  WHUB_STA_PN_CODE_INDEX_INVALID  = 0x04,
  WHUB_STA_CHANNEL_INVALID        = 0x05,
  WHUB_STA_MSG_QUEUED             = 0x06,

  WHUB_STA_SEARCH_CHANNEL_FAILURE = 0x08
} eWHubMsgStatus;

/* structures =============================================================== */
/**
 * @struct xWHubMessage
 * @brief Classe Message entre hub et hôte
 */ 
typedef struct xWHubMessage {

  uint8_t ucId; /**< Identifiant du message */
  uint8_t ucStatus;  /**< Compte rendu */
  uint16_t usDeviceId; /**< ucDeviceId de l'origine du message */
  uint8_t ucDataLength; /**< Longueur des données transmises */
  uint8_t * pucData;  /**< Pointeur sur les données transmises */
} xWHubMessage;

/**
 * @struct xWHubConfig
 * @brief Configuration d'un hub
 */ 
typedef struct xWHubConfig {

  uint8_t ucStatus; /**< Bits d'états du hub */
  uint8_t ucDataChannel; /**< Canal utilisé pour la transmission des données */
  uint8_t ucBindChannel; /**< Canal utilisé pour l'association des capteurs au réseau */
  uint8_t ucDataPnCode;   /**< PnCode utilisé pour la transmission des données */
} xWHubConfig;

/**
 * @struct xWHubService
 * @brief Eléments pour la gestion des services du hub
 */ 
typedef struct xWHubService {

  xTaskHandle xTask;
  xMutex xModeMutex;  /**< Mutex qui cadence le basculement de mode BIND/DATA */
  uint8_t ucRssiCounter;
} xWHubService;

/**
 * @struct xWusbHub
 * @brief Classe WirelessUSB&tm; Hub
 */ 
typedef struct xWusbHub {

  uint8_t ucFlag; /**< Bits de flag du hub */
  uint8_t ucNoiseThreshold;
  xWHubService xService; /**< Classe pour la gestion des services */
  xWHubConfig xConfig; /**< Configuration du Hub */
  xWHubMessage xMsg;
} xWusbHub;

/* == <API> == public application interface  ========class xWusbHub=========== */
/**
 * @brief Initialisation du Hub
 * Cette fonction initialise tous les élèments nécessaires au fonctionnement 
 *  du hub :
 *  - La couche réseau WusbNetwork
 *  - La base de données des capteurs du réseau WusbSensorDataBase  
 * @param eDataRate vitesse de transmission. 0 pour initialiser à partir de la config en EEPROM.
 * @param SensorDbaseFileArg1 premier paramètre à transmettre à \c iWSdBaseOpen()
 * @param SensorDbaseFileArg2 deuxième paramètre à transmettre à \c iWSdBaseOpen()
 * @return la valeur renvoyée par iWSdBaseOpen(): 0 succès, -1 erreur.  
 */ 
int iWHubInit (  eWDevDataRate eDataRate, 
                int iSensorDbaseFileArg1, 
                void * pvSensorDbaseFileArg2);
/**
 * @brief Fonction qui gère l'intégralité du réseau WirelessUSB&tm;
 * Cette fonction doit être appellée le plus souvent possible par l'application.
 * Elle gère l'intégration des capteurs au réseau, la transmission et la réception
 * de données des capteurs, l'envoi de compte rendu à l'hôte qui gère le hub.
 * Elle choisit automatiquement le canal en fonction des conditions de 
 * transmission (bruit, présence d'autres réseaux ....) et en change lorsque cela
 * est nécessaire en validant automatiquement le mode recherche de canal.
 * Elle permute de mode entre la réception de données et l'intégration automatique
 * des capteurs si ce mode est validé (Bit d'état WHUB_AUTOBIND) avec une période
 * de 50 ms. Elle intègre les capteurs qui lui en font la demande sur le canal
 * de données si le mode d'intégration rapide est validé (Bit d'état WHUB_SEEDEDBIND)      
 * @return Message pour la couche application. 0 si aucun message.    
 */ 
const xWHubMessage * pxWHubLoop (void);

#if defined(__DOXYGEN__)
/**
 * @brief Renvoie tous bits d'état ou de mode du hub
 */
inline uint8_t ucWHubStatus (void);

/**
 * @brief Renvoie l'état d'un bit d'état ou de mode du hub
 */
inline bool xWHubStatusFlag (eWHubStatus eFlag);

/**
 * @brief Active ou désactive un bit d'état du hub
 */
void vWHubSetStatusFlag (eWHubStatus eFlag, bool xValue);

/**
 * @brief Bascule l'état d'un bit d'état du hub
 */ 
void vWHubToggleStatusFlag (eWHubStatus eFlag);

/**
 * @brief Renvoie le canal actuellement utilisé pour la transmission de données
 */ 
inline uint8_t ucWHubDataChannel (void);

/**
 * @brief Modifie le canal actuellement utilisé pour la transmission de données
 * La modification n'est effectuée que si ucIndex est dans les limites autorisées
 * entre 1 et vWNetNumberOfChannels()
 * @param ucIndex Index du canal
 * @return true Succès. False Erreur.
 */
bool xWHubSetDataChannel (uint8_t ucIndex);

/**
 * @brief Renvoie le canal actuellement utilisé pour l'intégration automatique des capteurs au réseau
 */ 
inline uint8_t ucWHubBindChannel (void);

/**
 * @brief Renvoie le PnCode actuellement utilisé pour la transmission de données
 */ 
inline uint8_t ucWHubDataPnCode (void);

/**
 * @brief Modifie le PnCode actuellement utilisé pour la transmission de données
 * La modification n'est effectuée que si iIndex est dans les limites autorisées
 * entre 1 et ucWNetNumberOfPnCodes()
 * @param ucIndex ucIndex du PnCode
 * @return true Succès. False Erreur.
 */
bool xWHubSetDataPnCode (uint8_t ucIndex);

/**
 * @brief Lecture des 4 octets de l'identifiant de fabrication
  */
inline const uint8_t * pucWHubMid (void);

/**
 * @brief Modifie les 4 octets de l'identifiant de fabrication
 *  La modificiation se fait en RAM. Elle est perdue lors d'un RESET.
 *  Attention, normalement le MID est fourni par le fabricant du module et n'est
 *  pas accessible en écriture.
 *  La modification du MID risque de provoquer des conflits sur le réseau (si
 *  un autre node a le même MID). Le Crc Seed et le Checksum Seed du réseau sont
 *  mis à jour par cette fonction.
 *  \param Mid les 4 octets du nouveau MID rangés en little endian
  */
void vWHubSetMid (const uint8_t * pucMid);

/*
 * @brief Passe de canal en canal dans le subset à la recherche le premier canal disponible
 */
void vWHubFindAnAvailableChannel (void);

/**
 * @brief Renvoie le nombre de capteurs actuellement enregistrés sur le réseau
 */
inline uint16_t usWHubNumberOfNodes (void);

/**
 * @brief Renvoie le nombre de capteurs maximal
 */
inline uint16_t usWHubMaxNumberOfNodes (void);

/**
 * @brief Retire tous les capteurs du réseau
 */
inline uint16_t usWHubDeleteAllNodes (void);

/**
 * @brief Retire un capteur du réseau
 * @param ucDeviceId DeviceID du capteur
 * @return true si le retrait a réussi 
 */ 
inline bool WHubNodeDelete (uint16_t ucDeviceId);

/**
 * @brief Lecture des bits de flag et d'état d'un capteur avec vérification de sa validité
 * Si le capteur correspondant au ucDeviceId est enregistré dans la
 *  base, copie ses flags depuis la base vers NodeFlag, vérifie si son bit V = 1
 *  et renvoie true, si V = 0 renvoie false.
 * Sinon renvoie false et laisse SensorFlag inchangé.
 */
bool xWHubNodeFlag (uint16_t ucDeviceId, uint8_t * pucNodeFlag);

/**
 * @brief Modification des bits de flag et d'état d'un capteur enregistré
 */
inline void vWHubNodeSetFlag (uint16_t ucDeviceId, uint8_t ucNodeFlag);

/**
 * @brief Renvoie le Mid d'un capteur enregistré
 * Aucune vérification n'est effectué sur ucDeviceId
 */
inline uint32_t ulWHubNodeMid (uint16_t ucDeviceId);

/**
 * @brief Renvoie le nombre d'octets enregistrés à transmettre à un capteur
 * Aucune vérification n'est effectué sur ucDeviceId
 */
inline uint8_t ucWHubNodeDataLength (uint16_t ucDeviceId);

/**
 * @brief Enregistre les octets à transmettre à un capteur
 * Aucune vérification n'est effectué sur ucDeviceId
 */
inline void vWHubNodeSetData (uint16_t ucDeviceId, uint8_t ucDataLength, const uint8_t * pucData);

/* == <API-END> == public application interface  ====class xWusbHub=========== */
/**
 *   @}
 * @}
 */

#else /* __DOXYGEN__ defined */
/* internal public functions ================================================ */
bool xWHubNodeFlag (uint16_t ucDeviceId, uint8_t * pucNodeFlag);
bool xWHubSetDataPnCode (uint8_t ucIndex);
bool xWHubSetDataChannel (uint8_t ucIndex);
void vWHubSetMid (const uint8_t * pucMid);
void vWHubFindAnAvailableChannel (void);
void vWHubSetStatusFlag (eWHubStatus eFlag, bool xValue);
void vWHubToggleStatusFlag (eWHubStatus eFlag);

/* public variables ========================================================= */
extern xWusbHub dHub;

/* inline public functions ================================================== */

/*------------ Getters Hub Config -------------*/
__STATIC_ALWAYS_INLINE (uint8_t
ucWHubStatus (void)) {

  return dHub.xConfig.ucStatus;
}

__STATIC_ALWAYS_INLINE (bool
xWHubStatusFlag (eWHubStatus eFlag)) {

  return xWPktBit (dHub.xConfig.ucStatus, eFlag);
}

__STATIC_ALWAYS_INLINE (const uint8_t *
pucWHubMid (void)) {

  return pucWDevMid();
}

__STATIC_ALWAYS_INLINE (uint8_t
ucWHubDataChannel (void)) {

  return dHub.xConfig.ucDataChannel;
}

__STATIC_ALWAYS_INLINE (uint8_t 
ucWHubBindChannel (void)) {

  return dHub.xConfig.ucBindChannel;
}

__STATIC_ALWAYS_INLINE (uint8_t 
ucWHubDataPnCode (void)) {

  return dHub.xConfig.ucDataPnCode;
}

/*------------ Getters Node -------------*/
__STATIC_ALWAYS_INLINE (uint16_t
usWHubNumberOfNodes (void)) {

  return usWSdBaseLength();
}

__STATIC_ALWAYS_INLINE (uint16_t
usWHubMaxNumberOfNodes (void)) {

  return usWSdBaseSize();
}

__STATIC_ALWAYS_INLINE (size_t
ucWHubNodeDataLength (uint16_t ucDeviceId)) {

  return ucWSdBaseSensorBackDataLength (ucDeviceId);
}

__STATIC_ALWAYS_INLINE (uint32_t
ulWHubNodeMid (uint16_t ucDeviceId)) {

  return ulWSdBaseSensorMid (ucDeviceId);
}

/*------------ Setters Node -------------*/
__STATIC_ALWAYS_INLINE (void
usWHubDeleteAllNodes (void)) {

  vWSdBaseClear();
}

__STATIC_ALWAYS_INLINE (bool
WHubNodeDelete (uint16_t ucDeviceId)) {

  return xWSdBaseDeleteSensor (ucDeviceId);
}

__STATIC_ALWAYS_INLINE (void
vWHubNodeSetData (uint16_t ucDeviceId, uint8_t DataLength, const uint8_t * Data)) {

   vWSdBaseSensorSetBackData (ucDeviceId, Data, DataLength);
}

__STATIC_ALWAYS_INLINE (void
vWHubNodeSetFlag (uint16_t ucDeviceId, uint8_t NodeFlag)) {

  vWSdBaseSensorSetFlag (ucDeviceId, NodeFlag);
}

#endif  /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_WUSB_HUB_H_ */
