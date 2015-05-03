/**
 * @file sensor_database.h
 * @brief Base de données de capteurs WirelessUSB
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20111204 - Initial version by epsilonRT
 */
#ifndef _AVRIO_WUSB_SENSOR_DATABASE_H_
#define _AVRIO_WUSB_SENSOR_DATABASE_H_

#include <avrio/wusb/sensor.h>
#include "wusb-config.h"

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup wusb_hub_module
 * @{
 *
 *  @defgroup wusb_sensor_database
 *  Modélisation d'une base de données permettant à un hub WirelessUSB de stoker
 *  les paramètres des capteurs du réseau.   
 *  @{
 */

/* constants ================================================================ */
/**
 * @brief Indice des bits de flag et d'état d'un capteur
 */ 
typedef enum {

  /* Flags */
  WSDBASE_VALID_DID   = 3, /**< Bit V à transmettre au capteur lors d'un paquet Ack */
  WSDBASE_TX_SEQN     = 4, /**< Bit T à transmettre au capteur lors d'un paquet pucData */
  WSDBASE_RX_SEQN     = 5, /**< Dernier Bit T transmis par le capteur  */

  /* Status */
  WSDBASE_BACKDATA    = 6, /**< Des données doivent être transmises au capteur */
  WSDBASE_NOTIFY      = 7  /**< Valide la notification lors de la réception d'un ACK suite à l'envoi de données au capteur */
} eWSensorRecordFlags;

/**
 * @brief Valeurs des bits de flag et d'état lors de l'ajout d'un capteur
 */ 
#define WSDBASE_SENSOR_INIT_FLAGS (0)

/**
 * @brief Renvoie le nombre d'octets nécessaires au stockage d'un certains nombre de 
 * capteurs dans la base de données
 */  
#define WSDBASE_SENSOR_FILE_SIZE(DbaseSize) (sizeof(uint16_t)+sizeof(xWSensorRecord)*(DbaseSize))

/**
 * @brief Renvoie le nombre le nombre de capteurs correspondant à une taille de 
 *  fichier.
 */  
#define WSDBASE_SENSOR_DBASE_SIZE(FileSize) ((FileSize-sizeof(uint16_t))/sizeof(xWSensorRecord))

/* structures =============================================================== */
/**
 * @brief Enregistrement d'un capteur
 */ 
typedef struct xWSensorRecord {

  uint32_t ulMid;  /**< MID du capteur */ 
  uint8_t ucFlag;  /**< Bit de flag et d'état */
  uint8_t ucBackDataLength; /**< Nombre d'octets à transmettre au capteur */
  uint8_t pucBackData[WUSB_CONFIG_HUB_BACKDATA_SIZE];  /**< Octets à transmettre au capteur */
} xWSensorRecord;

/**
 * @brief Base de données de capteurs
 */ 
typedef struct xWSensorDataBase {

  uint16_t usSize; /**< Nombre max de capteurs dans la base */
  uint16_t usNextFreeDeviceId; /**< Prochain DeviceID libre dans la base */
} xWSensorDataBase;

/* == <API> == public application interface  =====class WusbSensorDataBase=== */
/**
 * @brief Cette fonction doit être implémentée par l'utilisateur et fait partie de 
 * l'interface logicielle entre le module WusbSensorDataBase et le fichier de 
 * stockage. Elle est automatiquement appellée par \c WHubInit().
 * Elle est chargée d'initialiser le fichier (ou la mémoire) dans lequel la base 
 * de données est stockée. Les paramètres iArg1 et pvArg2, passés à \c WHubInit() 
 * sont destinés à l'initialisation de ce fichier et dépndent donc de celui-ci.
 * Typiquement iArg1 sera la taille et pvArg2 un pointeur sur une zone de mémoire
 * ou un fichier.
 * Cette fonction doit se terminer par un appel à \c iWSdBaseInit() et renvoyer
 * sa valeur.
 * @return 0 succès. une valeur négative en cas d'erreur.
 */ 
int iWSdBaseOpen (int iArg1, void * pvArg2);

/**
 * @brief Ajout d'un nouveau capteur dans la base
 * @param ulMid du capteur
 * @return DeviceID du capteur ou 0xFFFF si la base est pleine 
 */   
uint16_t usWSdBaseAddSensor (uint32_t ulMid);

/**
 * @brief Retire un capteur de la base
 * @param usDeviceId DeviceID du capteur
 * @return true si le retrait a réussi 
 */   
bool xWSdBaseDeleteSensor (uint16_t usDeviceId);

/**
 * @brief Efface la base
 */
void vWSdBaseClear (void);

#if defined(__DOXYGEN__)
/**
 * @brief Renvoie la capacité de la base en nombre de capteurs
 */   
inline uint16_t usWSdBaseSize (void);

/**
 * @brief Renvoie le nombre de capteurs actuellement dans la base
 */   
uint16_t usWSdBaseLength (void);

/**
 * @brief Modifie le nombre de capteurs actuellement dans la base
 */   
inline void vWSdBaseSetLength (uint16_t usLength);

/**
 * @brief Lecture des bits de flag et d'état d'un capteur enregistré
 */   
uint8_t ucWSdBaseSensorFlag (uint16_t usDeviceId);

/**
 * @brief Modification des bits de flag et d'état d'un capteur enregistré
 */   
inline void vWSdBaseSensorSetFlag (uint16_t usDeviceId, uint8_t ucFlag);

/**
 * @brief Lecture du nombre d'octets à transmettre à un capteur enregistré
 */   
uint8_t ucWSdBaseSensorBackDataLength (uint16_t usDeviceId);

/**
 * @brief Modification du nombre d'octets à transmettre à un capteur enregistré
 */   
void vWSdBaseSensorSetBackDataLength (uint16_t usDeviceId, uint8_t ucBackDataLength);

/**
 * @brief Lecture des octets à transmettre à un capteur enregistré
 * @param pucBackData Pointeur vers une zone mémoire où seront stockés les octets lus.
 * @return Nombre d'octets lus. 
 */   
uint8_t ucWSdBaseSensorBackData (uint16_t usDeviceId, uint8_t * pucBackData);

/**
 * @brief Ecriture des octets à transmettre à un capteur enregistré
 * @param ucBackDataLength Nombre d'octets à écrire
 * @param pucBackData Pointeur vers une zone mémoire où seront stockés les octets à écrire.
 */
inline void vWSdBaseSensorSetBackData ( uint16_t usDeviceId, 
                                        const uint8_t * pucBackData,
                                        uint8_t ucBackDataLength);                                       );

/**
 * @brief Lecture du MID d'un capteur enregistré
 */   
uint32_t ulWSdBaseSensorMid (uint16_t usDeviceId);

/**
 * @brief Modification du MID d'un capteur enregistré
 */   
inline void vWSdBaseSensorSetMid (uint16_t usDeviceId, uint32_t ulMid);

/**
 * @brief Initialise la base de données 
 * @param xFileSize Taille en octets du fichier binaire à utiliser pour le stockage de la base
 * @return 0 succès. -1 en cas d'erreur.
 * Cette fonction doit être obligatoirement appellée par iWSdBaseOpen() 
 * @c xFileSize doit être suffisant pour y stocker au moins un \c xWSensorRecord 
 *  et un entier \c uint8_t (nombre de xWSensorRecord stockés). 
 */   
int iWSdBaseInit (size_t xFileSize);

/**
 * @brief Cette fonction doit être implémentée par l'utilisateur et fait partie de 
 * l'interface logicielle entre le module WusbSensorDataBase et le fichier de 
 * stockage. Elle est utilisée par le module WusbHub.
 * Elle est chargée de lire le fichier (ou la mémoire) dans lequel la base 
 * de données est stockée. 
 * @param pvAddress Adresse du premier octet à lire dans le fichier
 * @param pucData Pointeur vers une zone mémoire où seront stockés les octets lus.
 * @param xDataLength nombre d'octets à lire.
 * @return le nombre d'octets effectivement lus.
 */   
int iWSdBaseRead (void * pvAddress, uint8_t * pucData, size_t xDataLength);

/**
 * @brief Cette fonction doit être implémentée par l'utilisateur et fait partie de 
 * l'interface logicielle entre le module WusbSensorDataBase et le fichier de 
 * stockage. Elle est utilisée par le module WusbHub.
 * Elle est chargée d'écrire dans le fichier (ou la mémoire) dans lequel la base 
 * de données est stockée. 
 * @param pvAddress Adresse du premier octet à écrire dans le fichier
 * @param pucData Pointeur vers une zone mémoire où sont stockés les octets à écrire.
 * @param xDataLength nombre d'octets à écrire.
 * @return le nombre d'octets effectivement écrits.
 */   
int iWSdBaseWrite (void * pvAddress, const uint8_t * pucData, size_t xDataLength); 

/**
 * @brief Met à jour le prochain DeviceID libre
 */ 
void vWSdBaseFindNextFreeDeviceId (void);

/**
 * @brief Renvoie le prochain Device ID  libre dans la base ou 0xFFFF si la base est pleine.
 */   
inline uint16_t usWSdBaseNextFreeDeviceId (void);
/* == <API-END> == public application interface  ===class WusbSensorDataBase= */
/**@} */

#else /* __DOXYGEN__ defined */
/* internal public functions ================================================ */
int iWSdBaseRead (void * pvAddress, uint8_t * pucData, size_t xDataLength);
int iWSdBaseWrite (void * pvAddress, const uint8_t * pucData, size_t xDataLength); 
int iWSdBaseInit (size_t xFileSize);
uint16_t usWSdBaseLength (void);
void vWSdBaseFindNextFreeDeviceId (void);
uint32_t ulWSdBaseSensorMid (uint16_t usDeviceId);
uint8_t ucWSdBaseSensorFlag (uint16_t usDeviceId);
uint8_t ucWSdBaseSensorBackDataLength (uint16_t usDeviceId);
uint8_t ucWSdBaseSensorBackData (uint16_t usDeviceId, uint8_t * pucBackData);
void vWSdBaseSensorSetField (  uint16_t usDeviceId, uint8_t ucFieldOffset, 
                              const void * pvValue, uint8_t ucSizeOfValue);                              
/* public variables ========================================================= */
extern xWSensorDataBase dSensorDbase;

/* inline public functions ================================================== */
__STATIC_ALWAYS_INLINE (uint16_t 
usWSdBaseSize (void)) {

  return dSensorDbase.usSize;
}

__STATIC_ALWAYS_INLINE (uint16_t 
usWSdBaseNextFreeDeviceId (void)) {

  return dSensorDbase.usNextFreeDeviceId;
}

__STATIC_ALWAYS_INLINE (void
vWSdBaseSetLength (uint16_t usLength)) {

  iWSdBaseWrite (0, (const uint8_t *)&usLength, sizeof(uint16_t));
}

__STATIC_ALWAYS_INLINE (void 
vWSdBaseSensorSetMid (uint16_t usDeviceId, uint32_t ulMid)) {

  vWSdBaseSensorSetField (usDeviceId, 
                         STRUCT_FIELD_OFFSET(xWSensorRecord, ulMid),
                         &ulMid, sizeof(uint32_t));
}

__STATIC_ALWAYS_INLINE (void
vWSdBaseSensorSetFlag (uint16_t usDeviceId, uint8_t ucFlag)) {

  vWSdBaseSensorSetField (usDeviceId, 
                         STRUCT_FIELD_OFFSET(xWSensorRecord, ucFlag),
                         &ucFlag, sizeof(uint8_t));
}

__STATIC_ALWAYS_INLINE (void
vWSdBaseSensorSetBackDataLength (uint16_t usDeviceId, uint8_t ucBackDataLength)) {

  vWSdBaseSensorSetField (usDeviceId, 
                         STRUCT_FIELD_OFFSET(xWSensorRecord, ucBackDataLength),
                         &ucBackDataLength, sizeof(uint8_t));
}


__STATIC_ALWAYS_INLINE (void
vWSdBaseSensorSetBackData (uint16_t usDeviceId, const uint8_t * pucBackData, uint8_t ucBackDataLength)) {

  vWSdBaseSensorSetField (usDeviceId, 
                         STRUCT_FIELD_OFFSET(xWSensorRecord, pucBackData),
                         pucBackData, ucBackDataLength);
  vWSdBaseSensorSetBackDataLength (usDeviceId, ucBackDataLength);
}

#endif  /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_WUSB_SENSOR_DATABASE_H_ */
