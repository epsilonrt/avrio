/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file sensor.c
 * @brief Implémentation capteur WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111121 - Initial version by epsilonRT
 */
#include <avrio/wusb/sensor.h>

/* constants ================================================================ */
#define WSENSOR_DATA_FLAGS  (_BV(WSENSOR_TX_SEQN)|_BV(WSENSOR_TX_SYNC)|_BV(WSENSOR_RX_SEQN))
#define WSENSOR_ACK_FLAGS   _BV(WSENSOR_RX_SEQN)
#define WSENSOR_SEQN_FLAGS  (_BV(WSENSOR_TX_SEQN)|_BV(WSENSOR_RX_SEQN))

/*
 * Indice des bits de flag de l'entête des paquets utilisés par les capteurs
 */ 
typedef enum {

  WSENSOR_RX_SEQN     = 1,  // Bit A: SENSOR <-> HUB
  WSENSOR_TX_SYNC     = 2,  // Bit S: SENSOR  -> HUB
  WSENSOR_VALID_DID   = 2,  // Bit V: SENSOR <-  HUB
  WSENSOR_TX_SEQN     = 3   // Bit T: SENSOR  -> HUB
} eWSensorPktBit;

/* private variables ======================================================== */
/* EEPROM --------------------------------------------------------------------*/
static xWSensorConfig pSensorConfigEEPROM EEMEM = {
  .usDeviceId  = WNET_INVALID_DEVICEID,
  .ucFlags    = _BV(WSENSOR_AUTOSEEDEDBIND)
};

/* FLASH ---------------------------------------------------------------------*/
/* Permet de recharger une config propre */
static xWSensorConfig pSensorConfigFLASH PROGMEM = {
  .usDeviceId  = WNET_INVALID_DEVICEID,
  .ucFlags    = _BV(WSENSOR_AUTOSEEDEDBIND)
};

/* public variables ========================================================= */
xWusbSensor dSensor;

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static void 
  prvvSaveConfigToEEPROM (void) {

  eeprom_write_block (&dSensor.xConfig, &pSensorConfigEEPROM, sizeof(xWSensorConfig));
  vWNetSaveConfigToEEPROM ();
}

// -----------------------------------------------------------------------------
static inline void
  prvvSetDeviceId (uint16_t usDeviceId) {

  dSensor.xConfig.usDeviceId = usDeviceId;
}

// -----------------------------------------------------------------------------
static inline void 
  prvvTogglePktBit (eWSensorPktBit eBit) {

  dSensor.ucPktBits ^= _BV(eBit);
}

// -----------------------------------------------------------------------------
static inline bool 
  prvxPktBit (eWSensorPktBit eBit) {

  return xWPktBit (dSensor.ucPktBits, eBit);
}

// -----------------------------------------------------------------------------
static inline void 
  prvvSetPktBit (eWSensorPktBit eBit, bool xValue) {

  vWPktSetBit (&dSensor.ucPktBits, eBit, xValue);
}

// -----------------------------------------------------------------------------
static inline void 
  prvvSendAck (void) {

  vWNetTxPktNew (WPKT_ACK_PACKET);
  vWNetTxPktSetAllFlags (dSensor.ucPktBits & WSENSOR_ACK_FLAGS);
  vWNetTxPktAddDeviceId (usWSensorDeviceId());
  vWNetTxPktAddTrailer ();
  vWNetTxPktSend ();
}

// -----------------------------------------------------------------------------
static inline void 
  prvvLoadConfigFromEEPROM (void) {

  eeprom_read_block (&dSensor.xConfig, &pSensorConfigEEPROM, sizeof(xWSensorConfig));
}

// -----------------------------------------------------------------------------
static inline void 
  prvvSetupDefaultConfig(void) {
  
  memcpy_P (&dSensor.xConfig, &pSensorConfigFLASH, sizeof(xWSensorConfig));
  prvvSaveConfigToEEPROM();
}

#ifdef AVRIO_DEBUG_STREAM
/*
 * Fonctions de DEBUG
 * ========================================================================== 
 */
# include <avrio/debug.h>

// -----------------------------------------------------------------------------
static void 
prvvDbgDumpChannel (char cPrefix) {

  vDbgNewLine();
  vDbgPutc(cPrefix);
  vDbgPutDec (ucWNetChannel());
  vDbgPutc('>');
}

// -----------------------------------------------------------------------------
static void 
prvvDbgDumpConfig (void) {

  vDbgPuts_P(PSTR("\rN>"));
  vDbgPutHexBytes ((const uint8_t *)&dNet.xConfig, sizeof(xWNetConfig));
  vDbgNewLine();
  vDbgPuts_P(PSTR("S>"));
  vDbgPutHexBytes ((const uint8_t *)&dSensor.xConfig, sizeof(xWSensorConfig));
  vDbgNewLine();
}

// -----------------------------------------------------------------------------
static void 
prvvDbgDumpTxPacket (void) {

  vDbgPuts_P(PSTR("T>"));
  vDbgPutHexBytes (dNet.pxTxPacket.pData, dNet.pxTxPacket.xLen);
  vDbgNewLine();
}

// -----------------------------------------------------------------------------
static void 
prvvDbgDumpRxPacket (void) {

  if (dNet.pxRxPacket.xLen) {
  
    vDbgPuts_P(PSTR("R>"));
    vDbgPutHexBytes (dNet.pxRxPacket.pData, dNet.pxRxPacket.xLen);
    vDbgNewLine();
    vDbgPuts_P(PSTR("V>"));
    vDbgPutHexBytes (dNet.pxRxValid.pData, dNet.pxRxValid.xLen);
    vDbgNewLine();
  }
}
#else
# define prvvDbgDumpConfig()
# define prvvDbgDumpRxPacket()
# define prvvDbgDumpTxPacket()
# define prvvDbgDumpChannel(c)
#endif  /* AVRIO_DEBUG_STREAM defined */

// -----------------------------------------------------------------------------
/*  Attente réponse du hub
    Sorties possibles :
      - WSENSOR_SUCCESS: rien à faire
        - Ack normal 
        - Data vide
      - Valeur positive: nombre d'octets reçus du hub
        - Data non vide : process data WSENSOR_BACKDATA = 1
      - WSENSOR_NODE_UNBOUND
        - Ack invalid device id : bind necessaire WSENSOR_BOUND = 0
      - WSENSOR_NACK_TIMEOUT
      - WSENSOR_PACKET_ERROR
 */ 
static inline int
  prviWaitHubResponse (void) {
  int iReturn;
  
  iReturn = eWNetRxPktWait (ucWNetAckTimeout());
  if (iReturn == WNET_SUCCESS) {
    /* Paquet correctement formaté reçu */    

    if ((ucWNetRxPktType() == WPKT_ACK_PACKET || 
                      ucWNetRxPktType() == WPKT_DATA_PACKET) &&
                (usWNetRxPktDeviceId() == usWSensorDeviceId()) &&
                (xWNetRxPktFlag (WPKT_ACK) == prvxPktBit (WSENSOR_TX_SEQN))) {

      /* Le paquet reçu est bien la réponse au dernier paquet envoyé */
      prvvTogglePktBit (WSENSOR_TX_SEQN);
      
      dSensor.xConfig.ucFlags &= ~(   _BV(WSENSOR_BINDING)    |
                                      _BV(WSENSOR_CHANSEARCH) |
                                      _BV(WSENSOR_NACK)       |
                                      _BV(WSENSOR_BACKDATA) );

      if (ucWNetRxPktType() == WPKT_ACK_PACKET) {
        /* Paquet ACK --------------------------------------------------------*/

        if (xWNetRxPktFlag (WPKT_VALID) == false) {
          /* Invalid Device Id */

          vWSensorSetFlag (WSENSOR_BOUND, false);
          prvvSetDeviceId (WNET_INVALID_DEVICEID);
          return WSENSOR_NODE_UNBOUND; /* Sortie sur erreur */
        }
        /* Ack correct reçu, sortie normale */
        /* Fin Paquet ACK ----------------------------------------------------*/
      }
      else {
        /* Paquet DATA -------------------------------------------------------*/
        /* Envoi ACK au hub: le paquet à transmettre est écrasé */
        prvvSetPktBit (WSENSOR_RX_SEQN, xWNetRxPktFlag(WPKT_TOGGLE));
        prvvSendAck();

        iReturn = ucWNetRxPktDataPayloadLength();
        if (iReturn) {
          /* Réception de données venant du Hub */

          vWSensorSetFlag (WSENSOR_BACKDATA, true);
        }
        else {
          /* Paquet NULL DATA -> Sequence Bit Reset */
          
          dSensor.ucPktBits &= ~WSENSOR_SEQN_FLAGS;
        }
        /* Data correct reçu, sortie normale */
        /* Fin Paquet DATA ---------------------------------------------------*/
      }
    } /* Fin paquet reçu est bien la réponse au dernier paquet envoyé */ 
    else {
    
      /* Le paquet reçu n'est pas la réponse au dernier paquet envoyé */
      iReturn = WSENSOR_NACK_TIMEOUT; /* sortie sur erreur */
    }
  } /* Fin Paquet correctement formaté reçu */

  return iReturn;
}
  
/* == <API> == public application interface  ========class xWusbSensor======== */
// -----------------------------------------------------------------------------
void 
  vWSensorInit(eWDevDataRate DataRate) {

  vWNetInit (DataRate);
  prvvLoadConfigFromEEPROM ();
  if (xWSensorFlag(WSENSOR_EEPROMBLANK) == true)
    prvvSetupDefaultConfig();  

  dSensor.ucPktBits = _BV(WSENSOR_RX_SEQN);
}

// -----------------------------------------------------------------------------
void 
  vWSensorBind(bool Seeded) {

  if (xWSensorBound())
    return;
  
  vWSensorSetFlag (WSENSOR_BINDING, true);
    
  /* Prepare le paquet BIND_REQUEST */  
  vWNetTxPktNew (WPKT_BIND_REQUEST_PACKET);
  vWNetTxPktAddBytes (pucWDevMid(), WDEV_MID_SIZEOF);
  vWNetTxPktAddTrailer ();
  
  prvvDbgDumpConfig();
  prvvDbgDumpTxPacket();
  
  for (;;) {
    uint8_t ucLoopCount = 0;
    uint8_t ucRetryCount;

    if (Seeded == false) {
    
      (void) xWNetSetChannel (WNET_BIND_CHANNEL);
      (void) xWNetSetPnCode (WNET_BIND_PNCODE);
    }
    
    while (ucLoopCount < 
                (WUSB_CONFIG_SENSOR_BIND_CYCLES * WNET_NUM_OF_CH_PER_SUBSET)) {
      
      prvvDbgDumpChannel('B');
      ucRetryCount = 0;
      while (ucRetryCount < WUSB_CONFIG_SENSOR_NACK_RETRIES) {
        
        vWNetTxPktSend ();
        if (eWNetRxPktWait (ucWNetBindTimeout()) == WNET_SUCCESS) {
          
          if (ucWNetRxPktType() == WPKT_BIND_RESPONSE_PACKET) {
          
            prvvSetDeviceId (usWNetRxPktDeviceId ());
            if (usWSensorDeviceId () != WNET_INVALID_DEVICEID) {
            
              /* Envoi de l'ack */
              dSensor.ucPktBits = _BV(WSENSOR_RX_SEQN);
              vWNetSetSingleByteDeviceId (xWNetRxPktFlag (WPKT_SINGLE));
              vWNetSetCrcSeed (ucWNetRxPktByte (7)); /* MID3 */
              vWNetSetChecksumSeed (ucWNetRxPktByte (8)); /* MID4 */
              prvvSendAck(); /* Envoi l'ack sur le canal de BIND */
              /* Enregistrement des paramètres réseau */
              vDbgPutc('-');
              (void) xWNetSetChannel (ucWNetRxPktByte(3));
              (void) xWNetSetPnCode (ucWNetRxPktByte(4));
              vWSensorSetFlag (WSENSOR_BOUND, true);
              prvvSaveConfigToEEPROM ();
              vWSensorSetFlag (WSENSOR_CHANSEARCH, true);
              prvvDbgDumpConfig();
              return;
            }
          }
          else {
          
            // Le paquet n'est pas un BIND_RESP: on l'affiche...
            prvvDbgDumpRxPacket();
          }
        }
        else {
        
          vDbgPutc('X');
        }
        ucRetryCount++;
        delay_ms (ucWNetRand());
      }
      vWNetGoToNextChannel ();
      ucLoopCount++;      
    }
    vDbgNewLine();
    vWSensorSleepCB (WSENSOR_BIND_TIMEOUT);
  }
}

// -----------------------------------------------------------------------------
void 
  vWSensorDataClear(void) {

  vWNetTxPktNew (WPKT_DATA_PACKET);
  vWNetTxPktAddDeviceId (usWSensorDeviceId());
}

// -----------------------------------------------------------------------------
int 
  iWSensorDataSend (bool Synchronous) {
  int iReturn;
  uint8_t ucNackLoop = WUSB_CONFIG_SENSOR_NACK_RETRIES;
  uint8_t ucSearchChannelLoop = WNET_NUM_OF_CH_PER_SUBSET;
  uint8_t ucSearchPacketLoop = 0;

  if (xWSensorBound() == false)
    return WSENSOR_NODE_UNBOUND;

  prvvSetPktBit (WSENSOR_TX_SYNC, Synchronous); /* met à jour le flag TX_SYNC */
  /* Recopie les flags du sensor dans le paquet */
  vWNetTxPktSetAllFlags (dSensor.ucPktBits & WSENSOR_DATA_FLAGS);
  /* Ajoute le crc et le checksum */
  vWNetTxPktAddTrailer ();

  vWSensorSetFlag (WSENSOR_BACKDATA, false);
  vWSensorSetFlag (WSENSOR_NACK, true);
  
  for (;;) {
    uint8_t ucDelay = 0;

    /* Boucle de transmission */
    vWNetTxPktSend (); /* Envoi paquet */
    iReturn = prviWaitHubResponse(); /* Attente réponse du hub */
    
    if (iReturn >= WSENSOR_SUCCESS) {

      /* Réponse correcte reçue, on passe à la suite */
      vDbgPutc('-');
      vWSensorSetFlag (WSENSOR_CHANSEARCH, false);
      break;  /* Sortie normale */
    }
    
    vDbgPutc('X');
    if (iReturn == WSENSOR_NODE_UNBOUND) {

      /* Le capteur a été deconnecté du réseau */
      vDbgPuts_P (PSTR("Sensor was unbound !\r"));
      if (xWSensorFlag (WSENSOR_AUTOSEEDEDBIND) == true) {

        /* 
          Paquet ACK avec Invalid Device ID Bit et SEDDED BIND Auto autorisé
          <Attention> 
            Le paquet à transmettre va être détruit par le bind ! il est donc
            nécessaire de sortir de la boucle de transmission
         */
        vWSensorBind (true);
        iReturn = WSENSOR_NODE_REBOUND;
        vDbgPuts_P (PSTR("Sensor was rebound !\r"));
      }
      break; /* Sortie sur erreur WSENSOR_NODE_REBOUND ou WSENSOR_NODE_UNBOUND */
    }

    /*=====================================================* 
     * Le Hub n'a pas répondu ou trop de paquets corrompus *
     *=====================================================*/
    if (xWSensorFlag (WSENSOR_CHANSEARCH) == false) {

      /* 
        Le Hub n'a pas répondu, Pas de recherche canal en cours:
          - Si transmission synchrone sortie normale
          - Si transmission asynchrone, le paquet doit être retransmis plusieurs 
            fois (WUSB_CONFIG_SENSOR_NACK_RETRIES)
            - Si toujours pas de réponse, passage en recherche canal
       */
      if (Synchronous == true) {

        /* pas de réponse mais ce n'est pas grave, paquet synchrone */
        vWSensorSetFlag (WSENSOR_NACK, false);
        iReturn = WSENSOR_SUCCESS;
        break;  /* Sortie normale */
      }

      /* Retransmission nécessaire ? */
      if (ucNackLoop-- != 0) {

        /* Oui, Attente aléatoire avant retransmission */
        ucDelay = ucWNetRand();
      }
      else {

        /* Non, Passage en recherche de canal */
        vWSensorSetFlag (WSENSOR_CHANSEARCH, true);
        vDbgPuts_P (PSTR("\rChannel scan mode was enabled !"));
      }
    }
    
    if (xWSensorFlag (WSENSOR_CHANSEARCH) == true) {

      /* 
        Le Hub n'a pas répondu, Recherche canal en cours:
        - Renvoie WUSB_CONFIG_SENSOR_NACK_RETRIES fois le paquet sur
          le nouveau canal
        - Si toujours pas de réponse, passe au canal suivant
        - Quand tous les canaux ont été passés en revue et toujours pas de 
          réponse, s'endort puis recommence.
       */
      if (ucSearchPacketLoop == 0) {

        if (ucSearchChannelLoop-- == 0) {

          /* Channel Search Timeout > Echec de la recherche */
          vDbgPuts_P (PSTR("\rThe channel scan failed !\r"));
          ucSearchChannelLoop = WNET_NUM_OF_CH_PER_SUBSET;
          vWSensorSleepCB (WSENSOR_SEARCH_TIMEOUT);
        }
        /* Passage au canal suivant */
        vWNetGoToNextChannel ();
        ucSearchPacketLoop = WUSB_CONFIG_SENSOR_NACK_RETRIES;
        ucDelay = WUSB_CONFIG_SENSOR_SEARCH_CHANNEL_DELAY;
        prvvDbgDumpChannel('S');
      }
      else {

        /* Retransmission sur canal en cours */
        ucSearchPacketLoop--;
        ucDelay = ucWNetRand();
      }
    }
    if (ucDelay)
      delay_ms (ucDelay);
  } /* Fin de la Boucle de transmission */
  
  return iReturn;
}

// -----------------------------------------------------------------------------
uint8_t *
  pucWSensorBackDataPayload (void) {

  return (xWSensorFlag (WSENSOR_BACKDATA) == true ? pucWNetRxPktDataPayload() : 0); 
}

// -----------------------------------------------------------------------------
uint8_t 
  ucWSensorBackDataPayloadLength (void) {

  return (xWSensorFlag (WSENSOR_BACKDATA) == true ? ucWNetRxPktDataPayloadLength() : 0); 
}
/* == <API-END> == public application interface  ====class xWusbSensor======== */

#ifdef WSENSOR_TEST
/*
 * Tests unitaires
 * ========================================================================== 
 */

/*
 * Transmission de paquets asynchrone avec comptage des ACK
 */ 
int 
iSensorTestPing (uint16_t usCount, uint16_t usInterval, 
                     xWNetTestResult * pxResult) {
  int iReturn = 0;
  
  pxResult->usSended = 0;
  pxResult->usReceived = 0;
  pxResult->usCorrupted = 0;

  while (usCount--) {
  
    vWSensorDataClear ();
    vWSensorDataAddWord (pxResult->usSended++);
    
    /* Envoi paquet DATA */
    iReturn = iWSensorDataSend (false); /* Transmission asynchrone */
    
    if (iReturn >= WNET_SUCCESS) {
    
      pxResult->usReceived++;
    }
    else if (iReturn == WSENSOR_PACKET_ERROR) {
    
      pxResult->usCorrupted++;
    }
    delay_ms(usInterval);
  }
  return iReturn;
}

#endif  /* WSENSOR_TEST defined */
/* ========================================================================== */
