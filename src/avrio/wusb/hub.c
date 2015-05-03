/**
 * @file hub.c
 * @brief Implémentation Hub WirelessUSB
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20121212 - Initial version by epsilonRT
 */
#include <avrio/wusb/hub.h>
#include <avrio/delay.h>
#include <avrio/debug.h>
/*
  Messages de débogage vers stderr si AVRIO_DEBUG_STREAM est défini:
  - + : Passage en mode DATA
  - - : Passage en mode BIND
  - : : Envoi d'un broadcast PING
  - CXX : Changement de canal vers XX
  - BXXXXXXXX : Bind d'un nouveau capteur de MID XXXXXXXX avec ACK du capteur
  - bXXXXXXXX : Bind d'un nouveau capteur de MID XXXXXXXX sans ACK du capteur
  - DXXXX>DDDDD... : Réception de données DDDDD du capteur DID=XXXX
  - EXXXX : Eviction d'un capteur DID=XXXX du réseau
  - AXXXX : ACK d'un capteur DID=XXXX suite à l'envoi de données BACKDATA
 */
/* constants ================================================================ */
#define MODE_LED  LED_LED5

/* private variables ======================================================== */
/* EEPROM --------------------------------------------------------------------*/
static xWHubConfig pHubConfigEEPROM EEMEM = {
  .ucStatus      = _BV(WHUB_SEARCHCHANNEL)|_BV(WHUB_AUTOBIND),
  .ucDataChannel = WUSB_CONFIG_NETWORK_SEEDED_BIND_CHANNEL,
  .ucDataPnCode  = WUSB_CONFIG_NETWORK_SEEDED_BIND_PNCODE,
  .ucBindChannel = WNET_BIND_CHANNEL
};

/* FLASH ---------------------------------------------------------------------*/
/* Permet de recharger une config propre */
static xWHubConfig pHubConfigFLASH PROGMEM = {
  .ucStatus      = _BV(WHUB_SEARCHCHANNEL)|_BV(WHUB_AUTOBIND),
  .ucDataChannel = WUSB_CONFIG_NETWORK_SEEDED_BIND_CHANNEL,
  .ucDataPnCode  = WUSB_CONFIG_NETWORK_SEEDED_BIND_PNCODE,
  .ucBindChannel = WNET_BIND_CHANNEL
};

/* public variables ========================================================= */
xWusbHub dHub;

/* private functions ======================================================== */

/* Configuration --------------------*/
static inline void
prvvLoadConfigFromEEPROM (void) {

  eeprom_read_block (&dHub.xConfig, &pHubConfigEEPROM, sizeof(xWHubConfig));
}

/*
 * Sauvegarde la configuration courante en EEPROM
 */
static void
prvvSaveConfigToEEPROM (void) {

  eeprom_write_block (&dHub.xConfig, &pHubConfigEEPROM, sizeof(xWHubConfig));
  vWNetSaveConfigToEEPROM();
}

/*
 * Sauvegarde le canal Data courant en EEPROM
 */
static void
prvvSaveDataChannelToEEPROM (void) {

  eeprom_write_byte (&pHubConfigEEPROM.ucDataChannel, dHub.xConfig.ucDataChannel);
}

/*
 * Sauvegarde le PnCode Data courant en EEPROM
 */
static void
prvvSaveDataPnCodeToEEPROM (void) {

  eeprom_write_byte (&pHubConfigEEPROM.ucDataPnCode, dHub.xConfig.ucDataPnCode);
}

/*
 * Sauvegarde le status courant en EEPROM
 */
static void
prvvSaveStatusToEEPROM (void) {

  eeprom_write_byte (&pHubConfigEEPROM.ucStatus, dHub.xConfig.ucStatus);
}

/*
 * Charge la config réseau à partir de la FLASH
 */   
static inline void
prvvSetupDefaultConfig(void) {
  
  memcpy_P (&dHub.xConfig, &pHubConfigFLASH, sizeof(xWHubConfig));
  prvvSaveConfigToEEPROM();
}

/* Mode -----------------------------*/
/*
 * Bascule d'un mode de fonctionnement à l'autre DATA<->BIND
 */
static void
prvvToggleMode (void) {

  vWHubToggleStatusFlag (WHUB_BIND);
  if (xWHubStatusFlag (WHUB_BIND) == true) {

    /* Passage en mode BIND */
    xWNetSetChannel (dHub.xConfig.ucBindChannel);
    vWNetGoToNextChannel(); /* le canal change à chaque phase de BIND */
    dHub.xConfig.ucBindChannel = ucWNetChannel();
    xWNetSetPnCode (WNET_BIND_PNCODE);
    /* DEBUG */
    vDbgPutc('-');
    vDbgLedClear(MODE_LED);
    /* FIN DEBUG */
  }
  else {

    /* Passage en mode DATA */
    xWNetSetChannel (ucWHubDataChannel());
    xWNetSetPnCode (ucWHubDataPnCode());
    /* DEBUG */
    vDbgPutc('+');
    vDbgLedSet(MODE_LED);
    /* FIN DEBUG */
  }
}

// -----------------------------------------------------------------------------
static void
prvvSetDataMode (void) {

  if (ucWHubStatus() & _BV(WHUB_BIND)) {
  
    prvvToggleMode();
  }
}

/* Services -------------------------*/
// -----------------------------------------------------------------------------
static void
prvvServiceTask (xTaskHandle unused __attribute__ ((unused))) {

  vMutexUnlock (&dHub.xService.xModeMutex);
  vTaskStart (dHub.xService.xTask);
}

// -----------------------------------------------------------------------------
static inline void
prvvServiceInit (void) {

  if (dHub.xService.xTask == 0) {
  
    dHub.xService.xTask =  
                  xTaskCreate (xTaskConvertMs(WUSB_CONFIG_HUB_SERVICE_TICK), 
                               prvvServiceTask);
  }
  else {
  
    vTaskStop (dHub.xService.xTask);
  }
  dHub.xService.xModeMutex = MUTEX_INITIALIZER;
  dHub.xService.ucRssiCounter = WUSB_CONFIG_HUB_SERVICE_PERIOD_NOISE;
  vTaskStart (dHub.xService.xTask);
}

/* Envoi des paquets ----------------*/
// -----------------------------------------------------------------------------
static inline void
prvvSendPingResponsePacket (void) {

  vWNetTxPktNew(WPKT_PING_PACKET);
  vWNetTxPktSetFlag (WPKT_BCAST, xWNetRxPktFlag(WPKT_BCAST));
  vWNetTxPktSetFlag (WPKT_RESP, true);
  vWNetTxPktAddTrailer();
  vWNetTxPktSend();
}

// -----------------------------------------------------------------------------
static void
prvvSendAckPacket (bool xDeviceValid) {

  vWNetTxPktNew(WPKT_ACK_PACKET);
  vWNetTxPktSetFlag (WPKT_ACK, xWNetRxPktFlag(WPKT_TOGGLE));
  vWNetTxPktSetFlag (WPKT_VALID, xDeviceValid);
  vWNetTxPktAddDeviceId (dHub.xMsg.usDeviceId);
  vWNetTxPktAddTrailer();
  vWNetTxPktSend();
}

/* Traitement des paquets reçus -----*/

/*
 * Traitement d'un paquet BIND_REQUEST
 */ 
static inline void
prvvProcessBindPacket (void) {

  if (ucWHubStatus() & (_BV(WHUB_BIND) | _BV(WHUB_SEEDEDBIND))) {
    uint8_t SensorFlag = 0;
    uint32_t SensorMid;
    eWNetErrorCode eError;
  
    /* Réponse uniquement si Mode BIND en cours ou SEEDED BIND autorisé */
    SensorMid = ulWNetRxPktDword(WPKT_MID_OFFSET);
    dHub.xMsg.usDeviceId = usWSdBaseAddSensor(SensorMid);
    /* si dHub.xMsg.usDeviceId == 0xFFFF, le capteur est refusé */
    vWNetTxPktNew(WPKT_BIND_RESPONSE_PACKET);
    vWNetTxPktAddDeviceId (dHub.xMsg.usDeviceId);
    vWNetTxPktAddByte (dHub.xConfig.ucDataChannel);
    vWNetTxPktAddByte (dHub.xConfig.ucDataPnCode);
    vWNetTxPktAddBytes (pucWDevMid(), WDEV_MID_SIZEOF);
    vWNetTxPktAddTrailer();
    vWNetTxPktSend();
    
    /* Attente Ack, nous pouvons être sur le canal de BIND... */
    do {
      
      /* ... donc, ignore la réception de tout autre paquet que le ACK */
      eError = eWNetRxPktWait(ucWNetAckTimeout());
    } while ((eError == WNET_SUCCESS) && (ucWNetRxPktType() != WPKT_ACK_PACKET));

    /* Un paquet correct reçu ou une erreur s'est produite */
    if ((ucWNetRxPktType() == WPKT_ACK_PACKET) &&
        (usWNetRxPktDeviceId() == dHub.xMsg.usDeviceId)) {

      /* Paquet ACK venant bien du récipiendaire */
      SensorFlag = ucWSdBaseSensorFlag (dHub.xMsg.usDeviceId);
      SensorFlag |= _BV(WSDBASE_VALID_DID); /* Capteur validé pour échange */
      vWSdBaseSensorSetFlag (dHub.xMsg.usDeviceId, SensorFlag);
    }
    /*
     * Fin du process BIND
     */
     
    /*
     * Compte rendu à l'hôte
     * WHUB_RSP_BIND_INFO - Bind Information Response
     */
    dHub.xMsg.ucId = WHUB_RSP_BIND_INFO;
    dHub.xMsg.pucData = (uint8_t *)&SensorMid;
    vDbgPutc('\r');
    if ((SensorFlag & _BV(WSDBASE_VALID_DID)) != 0) {
    
      dHub.xMsg.ucStatus = WHUB_STA_SUCCESS;
      vDbgPutc('B');  /* BIND avec ACK sensor */ 
    }
    else {
    
      dHub.xMsg.ucStatus = WHUB_STA_UNKNOWN_ID;
      vDbgPutc('b');  /* BIND sans ACK sensor */
    }
    
    vDbgPutHexDword(SensorMid);
    vDbgPutc('\r');
    
    /* Mets à jour le prochain DeviceId */
    vWSdBaseFindNextFreeDeviceId();
  }
}

/*
 * Traitement d'un paquet DATA
 */ 
static inline void
prvvProcessDataPacket (void) {
  uint8_t SensorFlag;
  bool xPacketIsNew = true;

  dHub.xMsg.usDeviceId = usWNetRxPktDeviceId(); 
  if (xWHubNodeFlag (dHub.xMsg.usDeviceId, &SensorFlag) == true) {

    /* ucDeviceId valide, Le capteur est connu du hub */
    if  (xWPktBit (SensorFlag, WSDBASE_BACKDATA) == true) {
      size_t BackDataLength;
      uint8_t BackData[WUSB_CONFIG_HUB_BACKDATA_SIZE];
    
      /* Données à transmettre au sensor */
      BackDataLength = ucWSdBaseSensorBackData (dHub.xMsg.usDeviceId, BackData);
      
      /* Envoi Paquet DATA */
      vWNetTxPktNew(WPKT_DATA_PACKET);
      vWNetTxPktSetFlag (WPKT_ACK, xWNetRxPktFlag(WPKT_TOGGLE));
      vWNetTxPktSetFlag (WPKT_TOGGLE, xWPktBit(SensorFlag, WSDBASE_TX_SEQN));
      vWNetTxPktSetFlag (WPKT_SYNC, false); /* Pas de paquet synchrone en back data */
      vWNetTxPktAddDeviceId (dHub.xMsg.usDeviceId);
      vWNetTxPktAddBytes (BackData, BackDataLength);
      vWNetTxPktAddTrailer();
      vWNetTxPktSend();
    }
    else {
    
      /* Pas de données à transmettre, ACK pour ucDeviceId Valide */
      prvvSendAckPacket (true);
    }
    
    vDbgPutc('\r');
    vDbgPutc('D');
    vDbgPutHexWord(dHub.xMsg.usDeviceId);
    vDbgPutc('>');
    vDbgPutHexBytes (pucWNetRxPktDataPayload(), ucWNetRxPktDataPayloadLength());    
    vDbgPutc('\r');
    
    if (xWNetRxPktFlag(WPKT_SYNC) == false) { /* Paquet asynchrone */
    
      if (xWNetRxPktFlag(WPKT_TOGGLE) == xWPktBit(SensorFlag, WSDBASE_RX_SEQN)) {
          
        /* Duplicate Packet, ne pas prévenir l'hôte */
        xPacketIsNew = false;
      }
      else {
      
        /* Basculement et mémorisation du toggle sequence bit */
        SensorFlag ^= _BV(WSDBASE_RX_SEQN);
        vWSdBaseSensorSetFlag (dHub.xMsg.usDeviceId, SensorFlag);
      }
    }
    
    /*
     * Compte rendu Données reçues
     * 0x86 - Incoming message     
     */
    if (xPacketIsNew == true) {
    
      dHub.xMsg.ucId = WHUB_RSP_INCOMING_MSG;
      dHub.xMsg.pucData = pucWNetRxPktDataPayload();
      dHub.xMsg.ucDataLength = ucWNetRxPktDataPayloadLength();
    }
  }
  else {

    /* Si le ucDeviceId est invalide, un ACK avec V = 0 est envoyé */
    prvvSendAckPacket (false);
    vDbgPutc('\r');
    vDbgPutc('E');
    vDbgPutHexWord(dHub.xMsg.usDeviceId);
    vDbgPutc('\r');
  }
}

/*
 * Traitement d'un paquet ACK en réponse à un paquet BACK DATA
 */ 
static inline void
prvvProcessAckPacket (void) {
  uint8_t SensorFlag;
    
  dHub.xMsg.usDeviceId = usWNetRxPktDeviceId(); 
  
  if (xWHubNodeFlag(dHub.xMsg.usDeviceId, &SensorFlag) == true) {

    /* ucDeviceId valide, Le capteur est connu du hub et valide */
    if ((xWPktBit(SensorFlag, WSDBASE_BACKDATA) == true) &&
        (xWNetRxPktFlag (WPKT_TOGGLE) == xWPktBit(SensorFlag, WSDBASE_TX_SEQN))) {
        
      /* Ce paquet est bien la réponse du dernier paquet DATA envoyé */
      SensorFlag ^= _BV(WSDBASE_TX_SEQN); /* Toggle TX Seqn Flag */
      SensorFlag &= ~_BV(WSDBASE_BACKDATA); /* Données transmises */
      vWSdBaseSensorSetFlag (dHub.xMsg.usDeviceId, SensorFlag);
      
      if (SensorFlag & _BV(WSDBASE_NOTIFY)) {

        /*
         * WHUB_RSP_SEND_MSG - Send Message Response > Success
         */
        dHub.xMsg.ucId = WHUB_RSP_SEND_MSG;
        dHub.xMsg.ucStatus = WHUB_STA_SUCCESS;

        /* DEBUG */
        vDbgPutc('A');
        vDbgPutHexWord(dHub.xMsg.usDeviceId);
        vDbgPutc('\r');
        /* FIN DEBUG */
      }
    }
  }
}

/*
 * Aiguille en fonction du type de paquet reçu
 */ 
static inline void
prvvProcessRxPacket (void) {
  
  switch (ucWNetRxPktType()) {
  
    case WPKT_DATA_PACKET:
      prvvProcessDataPacket();
      break;
      
    case WPKT_ACK_PACKET:
      prvvProcessAckPacket();
      break;
      
    case WPKT_PING_PACKET:
      prvvSendPingResponsePacket();
      break;
      
    case WPKT_BIND_REQUEST_PACKET:
      prvvProcessBindPacket();
      break;
      
    default:
      /* Les autres paquets sont ignorés */
      break;
  }
}

/* internal public functions ================================================ */
/*
 * Initialisation du Hub
 * Cette fonction initialise tous les élèments nécessaires au fonctionnement 
 *  du hub :
 *  - La couche réseau WusbNetwork
 *  - La base de données des capteurs du réseau WusbSensorDataBase  
 * @param DataRate vitesse de transmission. 0 pour initialiser à partir de la config en EEPROM.
 * @param SensorDbaseFileArg1 premier paramètre à transmettre à \c iWSdBaseOpen()
 * @param SensorDbaseFileArg2 deuxième paramètre à transmettre à \c iWSdBaseOpen()
 * @return la valeur renvoyée par iWSdBaseOpen(): 0 succès, -1 erreur.  
 */ 
int
iWHubInit (eWDevDataRate DataRate, 
            int SensorDbaseFileArg1, void *SensorDbaseFileArg2) {

  vWNetInit (DataRate);
  prvvLoadConfigFromEEPROM();
  if (xWHubStatusFlag (WNET_EEPROMBLANK) == true) {
  
    prvvSetupDefaultConfig(); 
  }
  if (xWHubStatusFlag (WHUB_CONFIGSAVED) == false) {
    
    xWNetSetChannel (dHub.xConfig.ucDataChannel);
    xWNetSetPnCode (dHub.xConfig.ucDataPnCode);
    vWNetSetCrcSeed (pucWDevMid()[2]);
    vWNetSetChecksumSeed (pucWDevMid()[3]);
    vWHubSetStatusFlag (WHUB_CONFIGSAVED, true);
    prvvSaveConfigToEEPROM();
  }
  dHub.ucFlag = 0;
  dHub.ucNoiseThreshold = 0;
  prvvServiceInit();
  return iWSdBaseOpen (SensorDbaseFileArg1, SensorDbaseFileArg2); 
}

/*
 * Fonction qui gère l'intégralité du réseau WirelessUSB
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
const xWHubMessage * 
pxWHubLoop (void) {
  const xWHubMessage * pxMsg = 0;
  eWNetErrorCode eRxError;

  dHub.xMsg.ucId = 0;
  eRxError = eWNetRxPktWait(ucWNetAckTimeout());
  if (eRxError == WNET_SUCCESS) {

    /* Un paquet correctement formaté a été reçu, il faut le traiter */
    prvvProcessRxPacket();
  }
  else {

    /* - Pas de paquet ou erreur */
    if (xWHubStatusFlag (WHUB_BIND) == false) {

      /* Actions à effectuer en mode DATA */
      if (eRxError == WNET_CORRUPTED_PACKET_ERROR) {

        /* Trop de paquets corrompus, changement de canal demandé */
        vWHubSetStatusFlag (WHUB_SEARCHCHANNEL, true);
      }
      
      if (dHub.xService.ucRssiCounter == 0) {
        int8_t Rssi = iWDevRssi();
        
        /* Vérifie la qualité du canal */
        dHub.xService.ucRssiCounter = WUSB_CONFIG_HUB_SERVICE_PERIOD_NOISE;
        if (Rssi >= 0) {

          /* Noise = (Noise * 3 + Rssi) / 4 */
          dHub.ucNoiseThreshold = (dHub.ucNoiseThreshold * 3 + Rssi) / 4;
          if (dHub.ucNoiseThreshold >= WUSB_CONFIG_HUB_NOISE_THRESHOLD_MAX) {
          
            /* Qualité insuffisante, changement de canal demandé */
            vWHubSetStatusFlag (WHUB_SEARCHCHANNEL, true);
          }
        }
      }
      
      /* Recherche de Canal si nécessaire */
      if (xWHubStatusFlag (WHUB_SEARCHCHANNEL) == true) {
        
        vWHubFindAnAvailableChannel();
      }
      /* Fin Actions à effectuer en mode DATA */
    }
    
    /* On lance les vérifications et les services */  
    if (xMutexTryLock (&dHub.xService.xModeMutex) == 0) {
    
      /* Exécution périodique des services */
      if (xWHubStatusFlag (WHUB_AUTOBIND) == true) {
      
        /* Si le AUTOBIND est validé, basculer de mode */
        prvvToggleMode();
      }
      else {
      
        /* Sinon, rester en mode DATA */
        prvvSetDataMode();
      }
        
      /* Mettre à jour le compte à rebours pour la surveillance de qualité */
      if (dHub.xService.ucRssiCounter != 0) {
      
        dHub.xService.ucRssiCounter--;
      }
    }
  
  }
  
  if (dHub.xMsg.ucId) {
  
    pxMsg = &dHub.xMsg;
  }  
  return pxMsg;
}

/*
 * Lecture des flags d'un capteur avec vérification de sa validité
 * Si le capteur correspondant au ucDeviceId est enregistré dans la
 *  base :
 * - Copie ses flags depuis la base vers SensorFlag, vérifie si son bit V = 1 et
 * renvoie true, si V = 0 renvoie false
 * Sinon renvoie false et laisse SensorFlag inchangé.
 */
bool
xWHubNodeFlag (uint16_t ucDeviceId, uint8_t * SensorFlag) {

  if (ucDeviceId < usWSdBaseLength()) {

    *SensorFlag = ucWSdBaseSensorFlag (ucDeviceId);
    if ((*SensorFlag & _BV(WSDBASE_VALID_DID)) != 0)
      return true;
  }
  return false;
}

/*
 * Modifie le canal
 * La modification n'est effectuée que si Index est dans les limites autorisées
 * @param Index Index du canal
 */
bool
xWHubSetDataChannel (uint8_t Index) {

  if (Index != WNET_BIND_CHANNEL) {

    if ((Index < ucWNetNumberOfSubsets()) && (Index != dHub.xConfig.ucDataChannel)) {

      prvvSetDataMode();
      xWNetSetChannel (Index);
      dHub.xConfig.ucDataChannel = Index;
      prvvSaveDataChannelToEEPROM();
      return true;
    }
  }
  return false;
}

/*
 * Modifie le Pn Code du réseau
 * La modification n'est effectuée que si Index est dans les limites autorisées
 * @param Index Index du PnCode
 */
bool
xWHubSetDataPnCode (uint8_t Index) {

  if (Index != WNET_BIND_PNCODE) {

    if ((Index < ucWNetNumberOfPnCodes()) && (Index != dHub.xConfig.ucDataPnCode)) {
    
      prvvSetDataMode();
      xWNetSetPnCode (Index);
      dHub.xConfig.ucDataPnCode = Index;
      prvvSaveDataPnCodeToEEPROM();
      return true;
    }
  }
  return false;
}

/*
 * Modifie les 4 octets de l'identifiant de fabrication
 *  La modificiation se fait en RAM. Elle est perdue lors d'un RESET.
 *  Attention, normalement le MID est fourni par le fabricant du module et n'est
 *  pas accessible en écriture.
 *  La modification du MID risque de provoquer des conflits sur le réseau (si
 *  un autre node a le même MID)
 *  \param Mid les 4 octets du nouveau MID rangés en little endian
  */
void
vWHubSetMid (const uint8_t * Mid) {

  prvvSetDataMode();
  vWDevSetMid (Mid);
  vWNetSetCrcSeed (Mid[2]);
  vWNetSetChecksumSeed (Mid[3]);
  vWNetSaveConfigToEEPROM();
}

/*
 * Passe de canal en canal dans le subset à la recherche le premier canal disponible
 */
void
vWHubFindAnAvailableChannel (void) {
  uint8_t ucRetry = WUSB_CONFIG_HUB_SEARCH_PING_REPEATS;
  eWNetErrorCode eError;
  
  prvvSetDataMode();
  vWNetGoToNextChannel();
  
  do {

    vDbgPutc(':');
    --ucRetry;
    
    vWNetTxPktNew(WPKT_PING_PACKET);
    vWNetTxPktSetFlag (WPKT_BCAST, true);
    vWNetTxPktAddTrailer();
    vWNetTxPktSend();
    
    do {
    
      eError = eWNetRxPktWait(ucWNetPingTimeout());
    } while ((eError == WNET_SUCCESS) && (ucWNetRxPktType() != WPKT_PING_PACKET));
    
    /* Timeout ou PING reçu */
    if (ucWNetRxPktType() == WPKT_PING_PACKET) {

      /* Réponse PING, canal non dispo */
      vWNetGoToNextChannel();
      ucRetry = WUSB_CONFIG_HUB_SEARCH_PING_REPEATS;
    }
    else if (ucRetry) {
    
      delay_ms (WUSB_CONFIG_HUB_SEARCH_PING_PERIOD);
    }
    
  } while (ucRetry);
  
  dHub.ucNoiseThreshold = 0;
  dHub.xConfig.ucDataChannel = ucWNetChannel();
  prvvSaveDataChannelToEEPROM();
  
  /* Un nouveau canal a été trouvé */
  vWHubSetStatusFlag (WHUB_SEARCHCHANNEL, false);
  
  /*
   * WHUB_RSP_CHANGE_CHANNEL - Channel Information
   * Changement de canal
   */
  dHub.xMsg.ucId = WHUB_RSP_CHANGE_CHANNEL;
  dHub.xMsg.pucData = &dHub.xConfig.ucDataChannel;
  dHub.xMsg.ucStatus = WHUB_STA_SUCCESS;
  
  /* DEBUG */
  vDbgPutc('C');
  vDbgPutDec(dHub.xConfig.ucDataChannel);
  vDbgPutc('\r');
  /* FIN DEBUG */
  
}

/*
 * Active ou désactive un bit d'état du hub
 */
void
vWHubSetStatusFlag (eWHubStatus Flag, bool Value) {

  vWPktSetBit (&dHub.xConfig.ucStatus, Flag, Value);
  prvvSaveStatusToEEPROM();
}

/*
 * Bascule l'état d'un bit d'état du hub
 */
void
vWHubToggleStatusFlag (eWHubStatus Flag) {

  dHub.xConfig.ucStatus ^= _BV(Flag);
  prvvSaveStatusToEEPROM();
}

/* ========================================================================== */
