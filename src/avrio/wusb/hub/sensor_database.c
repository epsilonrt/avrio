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
 * @file sensor_database.c
 * @brief Implémentation base de données capteurs WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111204 - Initial version by epsilonRT
 */
#include <avrio/wusb/hub/sensor_database.h>

/* public variables ========================================================= */
xWSensorDataBase dSensorDbase;

/* private functions ======================================================== */
static void *
prvpvSensorAddress (uint16_t usDeviceId, uint8_t ucFieldOffset) {

  return (void *) ( usDeviceId * sizeof(xWSensorRecord) + ucFieldOffset + 
                    sizeof(uint16_t) );
}

#if 0
/*
 * Recherche si le capteur est déjà enregistré, si oui renvoie son DID, sinon,
 * renvoie le prochain DID libre
 * Pour le debug 
 */  
static uint16_t 
FindSensor (uint32_t ulMid) {
  uint16_t usDeviceId = 0;
  uint16_t usLoop = usWSdBaseLength();

  while (usLoop-- != 0) {

    if (ulWSdBaseSensorMid(usDeviceId) == ulMid)
      return usDeviceId;
    usDeviceId++;
  }
  return dSensorDbase.usNextFreeDeviceId;
}

#else
#define FindSensor(ulMid) (dSensorDbase.usNextFreeDeviceId)

#endif

/* internal public functions ================================================ */
/*
 * @brief Initialise la base de données 
 * @param xFileSize Taille en octets du fichier binaire à utiliser pour le stockage de la base
 * @return 0 succès. -1 en cas d'erreur.
 * Cette fonction doit être obligatoirement appellée par iWSdBaseOpen() 
 * @c xFileSize doit être suffisant pour y stocker au moins un \c xWSensorRecord 
 *  et un entier uint16_t (nombre de xWSensorRecord stockés). 
 */   
int 
iWSdBaseInit (size_t xFileSize) {
  
  if (xFileSize >= (sizeof(uint16_t) + sizeof(xWSensorRecord))) {

    dSensorDbase.usSize = (xFileSize - sizeof(uint16_t)) / sizeof(xWSensorRecord);
    vWSdBaseFindNextFreeDeviceId();
    return 0;
  }
  return -1;
}

/*
 * @brief Efface la base
 */
void
vWSdBaseClear (void) {
  uint16_t usDeviceId;
  
  vWSdBaseSetLength (0);
  dSensorDbase.usNextFreeDeviceId = 0;
  for (usDeviceId = 0; usDeviceId < usWSdBaseSize(); usDeviceId++) {
  
    vWSdBaseSensorSetFlag (usDeviceId, 0);
  }
}

/*
 * @brief Met à jour le prochain DeviceID libre
 */ 
void
vWSdBaseFindNextFreeDeviceId (void) {
  uint16_t usLoop = usWSdBaseSize();
  uint16_t DeviceIdMax = usLoop - 1; 
  uint8_t SensorFlag;
  
  while (usLoop-- != 0) {

    if (dSensorDbase.usNextFreeDeviceId > DeviceIdMax)
      dSensorDbase.usNextFreeDeviceId = 0;
    SensorFlag = ucWSdBaseSensorFlag (dSensorDbase.usNextFreeDeviceId);
    if ((SensorFlag & _BV(WSDBASE_VALID_DID)) == 0)
      return;
    dSensorDbase.usNextFreeDeviceId++;
  }
  dSensorDbase.usNextFreeDeviceId = 0xFFFF;
}

/*
 * @brief Ajout d'un nouveau capteur dans la base
 * @param ulMid du capteur
 * @return DeviceID du capteur ou 0xFFFF si la base est pleine 
 */   
uint16_t 
usWSdBaseAddSensor (uint32_t ulMid) {
  uint16_t usDeviceId;
  
  usDeviceId = dSensorDbase.usNextFreeDeviceId;

  if (usDeviceId != 0xFFFF) {
    /* Ajout d'un Sensor */
    
    if (usDeviceId == usWSdBaseLength()) {
    
      vWSdBaseSetLength (usDeviceId + 1); // Ajout en dernière position
    }
    vWSdBaseSensorSetMid (usDeviceId, ulMid);
    vWSdBaseSensorSetFlag (usDeviceId, WSDBASE_SENSOR_INIT_FLAGS);
    vWSdBaseSensorSetBackDataLength (usDeviceId, 0);
  }
  return usDeviceId;
}

/*
 * @brief Retire un capteur de la base
 * @param usDeviceId DeviceID du capteur
 */   
bool
xWSdBaseDeleteSensor (uint16_t usDeviceId) {

  if (usDeviceId < usWSdBaseLength()) {
  
    vWSdBaseSensorSetFlag (usDeviceId, 0);
    return true;
  }
  return false;
}

/*
 * @brief Renvoie le nombre de capteurs actuellement dans la base
 */   
uint16_t 
usWSdBaseLength (void) {
  uint16_t usLength;
    
  iWSdBaseRead (0, (uint8_t *)&usLength, sizeof(uint16_t));
  return usLength;
}

/*
 * Modif d'un champ d'enregistrement
 */ 
void
vWSdBaseSensorSetField ( uint16_t usDeviceId, uint8_t ucFieldOffset, 
                        const void * pvValue, uint8_t ucSizeOfValue) {

    iWSdBaseWrite (prvpvSensorAddress (usDeviceId, ucFieldOffset), 
                    (const uint8_t *)pvValue, ucSizeOfValue);
}

/*
 * Lecture d'un champ d'enregistrement
 */ 
void
vWSdBaseSensorGetField ( uint16_t usDeviceId, uint8_t ucFieldOffset, 
                        void * pvValue, uint8_t ucSizeOfValue) {

    iWSdBaseRead (prvpvSensorAddress (usDeviceId, ucFieldOffset), 
                  (uint8_t *)pvValue, ucSizeOfValue);
}

/*
 * @brief Lecture du MID d'un capteur enregistré
 */   
uint32_t 
ulWSdBaseSensorMid (uint16_t usDeviceId) {
  uint32_t ulMid;
 
  vWSdBaseSensorGetField (usDeviceId, 
                         STRUCT_FIELD_OFFSET(xWSensorRecord, ulMid),
                         &ulMid, sizeof(uint32_t));
  return ulMid;
}

/*
 * @brief Lecture des bits de flag et d'état d'un capteur enregistré
 */   
uint8_t
ucWSdBaseSensorFlag (uint16_t usDeviceId) {
  uint8_t Flag;
  
  vWSdBaseSensorGetField (usDeviceId, 
                         STRUCT_FIELD_OFFSET(xWSensorRecord, ucFlag),
                         &Flag, sizeof(uint8_t));
  return Flag;
}

/*
 * @brief Lecture du nombre d'octets à transmettre à un capteur enregistré
 */   
uint8_t
ucWSdBaseSensorBackDataLength (uint16_t usDeviceId) {
  uint8_t ucBackDataLength;
  
  vWSdBaseSensorGetField (usDeviceId, 
                         STRUCT_FIELD_OFFSET(xWSensorRecord, ucBackDataLength),
                         &ucBackDataLength, sizeof(uint8_t));
  return ucBackDataLength;
}

/*
 * @brief Lecture des octets à transmettre à un capteur enregistré
 * @param pucBackData Pointeur vers une zone mémoire où seront stockés les octets lus.
 * @return Nombre d'octets lus. 
 */   
uint8_t
ucWSdBaseSensorBackData (uint16_t usDeviceId, uint8_t * pucBackData) {
  uint8_t ucBackDataLength;

  ucBackDataLength = ucWSdBaseSensorBackDataLength (usDeviceId);
  vWSdBaseSensorGetField (usDeviceId, 
                         STRUCT_FIELD_OFFSET(xWSensorRecord, pucBackData),
                         pucBackData, ucBackDataLength);
  return ucBackDataLength;
}
/* ========================================================================== */
