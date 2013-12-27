/**
 * @file network.c
 * Implémentation réseau Nto1 WirelessUSB
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20111114 - Initial version by epsilonRT
 */
#include <avrio/wusb/net/network.h>
#include <avrio/util.h>
#include <string.h>

/* constants ================================================================ */
#define WNET_PNCODE_SIZE  8
#define WNET_PACKET_SIZE_MAX  WPKT_PACKET_SIZE_MAX(WUSB_CONFIG_NETWORK_PAYLOAD_SIZE)

/* macros =================================================================== */
#define WNET_PNCODETABLE_SIZEOF(t) (sizeof(t)/WNET_PNCODE_SIZE)

/* structures =============================================================== */
typedef struct xWNetByteTable {

  const uint8_t * pTable;
  uint8_t pSize;
} xWNetByteTable;

/* enum ===================================================================== */
typedef enum  {

  WNET_NOPACKET   = 0x00, /**< en attente, aucun octet reçu */
  WNET_INPROGRESS = 0x01, /**< paquet en acquisition, des octets ont été reçus */
  WNET_RECEIVED   = 0x02, /**< paquet complet reçu */
} eWNetStateSet;

/* private variables ======================================================== */
static uint8_t pxTxPacketBuffer[WNET_PACKET_SIZE_MAX];
static uint8_t pxRxPacketBuffer[WNET_PACKET_SIZE_MAX];
static uint8_t pxRxValidBuffer[WNET_PACKET_SIZE_MAX];

/* EEPROM --------------------------------------------------------------------*/
static xWNetConfig pNetConfigEEPROM EEMEM = {
  .ucChannel       = WUSB_CONFIG_NETWORK_SEEDED_BIND_CHANNEL,
  .ucPnCode        = WUSB_CONFIG_NETWORK_SEEDED_BIND_PNCODE,
  .ucChecksumSeed  = WPKT_BIND_CHECKSUM_SEED,
  .ucCrcSeed       = WPKT_BIND_CRC_SEED,
  .ucStatus        = 0
};

/* FLASH ---------------------------------------------------------------------*/
/* Permet de recharger une config propre */
static xWNetConfig pNetConfigFLASH PROGMEM = {
  .ucChannel       = WUSB_CONFIG_NETWORK_SEEDED_BIND_CHANNEL,
  .ucPnCode        = WUSB_CONFIG_NETWORK_SEEDED_BIND_PNCODE,
  .ucChecksumSeed  = WPKT_BIND_CHECKSUM_SEED,
  .ucCrcSeed       = WPKT_BIND_CRC_SEED,
  .ucStatus        = 0
};

/*---- Pseudo noise code tables ------*/
static const uint8_t ucPnCodeTable16kps[] PROGMEM = {
// LSB                                       MSB
  0xD3, 0x64, 0x44, 0x7A, 0x2D, 0xA8, 0xF7, 0x83, // PN Code ID 0  - ucIndex 0
  0xC9, 0x7A, 0x48, 0x71, 0xAA, 0x4E, 0x2C, 0x3F, // PN Code ID 1  - ucIndex 1
  0x82, 0xC7, 0x90, 0x36, 0x21, 0x9E, 0xFF, 0x17, // PN Code ID 2  - ucIndex 2
//0xAD, 0xB6, 0x30, 0x3A, 0x9A, 0xB5, 0x46, 0xA6, // PN Code ID 3
  0xEB, 0x42, 0x7F, 0xEE, 0x5B, 0x9A, 0x5D, 0xBC, // PN Code ID 4  - ucIndex 3
  0xE7, 0x74, 0x77, 0x7A, 0xF8, 0xDD, 0xF5, 0x24, // PN Code ID 5  - ucIndex 4
  0x95, 0xAD, 0x84, 0xDC, 0x94, 0x7C, 0x70, 0x3D, // PN Code ID 6  - ucIndex 5
  0xD4, 0x11, 0x7B, 0x52, 0x37, 0xF0, 0x6A, 0x1E, // PN Code ID 7  - ucIndex 6
  0xAF, 0xBF, 0x33, 0xFC, 0xAA, 0x2B, 0xF5, 0x62, // PN Code ID 8  - ucIndex 7
  0x97, 0x5D, 0xD9, 0x0F, 0xD9, 0x32, 0x56, 0x40, // PN Code ID 9  - ucIndex 8
  0xCA, 0x20, 0xFF, 0xA7, 0xA9, 0xD0, 0x4A, 0x8E, // PN Code ID 10 - ucIndex 9
//0xBD, 0xA6, 0x78, 0x24, 0xAB, 0x31, 0xB3, 0x38, // PN Code ID 11
  0xBE, 0xB5, 0x3D, 0xB8, 0xBF, 0x9D, 0x97, 0x4C, // PN Code ID 12 - ucIndex 10
  0xBD, 0x6D, 0xCA, 0x9F, 0x30, 0x24, 0x5D, 0x0C, // PN Code ID 13 - ucIndex 11
//0x8A, 0x4D, 0xF4, 0xC8, 0xB5, 0x7B, 0x29, 0x29, // PN Code ID 14
  0xAD, 0x95, 0x78, 0xF1, 0xDE, 0x33, 0x14, 0x50, // PN Code ID 15 - ucIndex 12
//0xA7, 0xA6, 0xB3, 0x96, 0x10, 0xAA, 0xA6, 0xD6, // PN Code ID 16
//0x94, 0xA6, 0x66, 0xE3, 0x12, 0x0D, 0xCE, 0xF6, // PN Code ID 17
  0xC9, 0x10, 0xF2, 0xF0, 0xF9, 0xFA, 0x3C, 0x0C, // PN Code ID 18 - ucIndex 13
  0xB3, 0x6F, 0x4E, 0xBF, 0xDB, 0x06, 0xDA, 0xF4, // PN Code ID 19 - ucIndex 14
//0xE6, 0xCF, 0x39, 0x1A, 0x14, 0x4E, 0x9C, 0x93, // PN Code ID 20
//0xD7, 0x9E, 0xB9, 0x86, 0x93, 0x06, 0x2C, 0xC9, // PN Code ID 21
  0xF0, 0xE8, 0x5E, 0x59, 0xAE, 0xD1, 0x08, 0x9E, // PN Code ID 22 - ucIndex 15
  0x8E, 0x2B, 0x8E, 0x7C, 0xBB, 0x8F, 0x90, 0xC0, // PN Code ID 23 - ucIndex 16
  0xE7, 0x49, 0xF8, 0x08, 0x80, 0x26, 0x69, 0x80, // PN Code ID 24 - ucIndex 17
  0xC1, 0x40, 0x80, 0xd0, 0x54, 0x49, 0x2d, 0x7d, // PN Code ID 25 - ucIndex 18
  0xB4, 0x36, 0x5A, 0x80, 0x1B, 0xE6, 0xF2, 0xB6, // PN Code ID 26 - ucIndex 19
  0xF6, 0x05, 0x67, 0xDA, 0x1C, 0x9C, 0xAE, 0x42, // PN Code ID 27 - ucIndex 20
  0x80, 0xB5, 0x8D, 0x14, 0xE0, 0xF7, 0x75, 0x9B, // PN Code ID 28 - ucIndex 21
  0x88, 0x5A, 0x30, 0xB7, 0xB9, 0x98, 0x54, 0xBF, // PN Code ID 29 - ucIndex 22
  0x88, 0xC9, 0xD4, 0x23, 0x97, 0xFC, 0xD1, 0x35, // PN Code ID 30 - ucIndex 23
  0x93, 0x40, 0xBD, 0x5F, 0x26, 0x31, 0xD6, 0xE1, // PN Code ID 31 - ucIndex 24
  0x8C, 0xAF, 0xAE, 0x97, 0x99, 0x08, 0x68, 0xDC, // PN Code ID 32 - ucIndex 25
  0xBA, 0x06, 0x32, 0x0E, 0x16, 0x01, 0x0E, 0xC3, // PN Code ID 33 - ucIndex 26
  0xAC, 0x8F, 0x3E, 0xAB, 0xFA, 0x01, 0x83, 0xE0, // PN Code ID 34 - ucIndex 27
  0x84, 0x7D, 0x9C, 0x46, 0xB8, 0x9C, 0xD5, 0x5C, // PN Code ID 35 - ucIndex 28
  0xB7, 0x4F, 0xA5, 0x9D, 0x5C, 0xFE, 0xC6, 0xF1, // PN Code ID 36 - ucIndex 29
  0xB0, 0xF1, 0x28, 0x0E, 0xDD, 0xB3, 0xB5, 0x58, // PN Code ID 37 - ucIndex 30
//0x9C, 0x32, 0xC5, 0xA4, 0xBA, 0x95, 0xD6, 0x9A, // PN Code ID 38
  0xA1, 0xF4, 0x45, 0x96, 0x56, 0x3B, 0x30, 0x5F, // PN Code ID 39 - ucIndex 31
  0xF8, 0xFE, 0xBD, 0xEF, 0x8A, 0x6E, 0xBC, 0x03, // PN Code ID 40 - ucIndex 32
  0xD6, 0x06, 0xBF, 0x2D, 0x3B, 0x13, 0x17, 0x88, // PN Code ID 41 - ucIndex 33
  0xA9, 0x88, 0x1C, 0xA1, 0x21, 0x30, 0x94, 0xF1, // PN Code ID 42 - ucIndex 34
  0xB4, 0xE3, 0x2F, 0x82, 0xBC, 0x8E, 0xD2, 0xD0, // PN Code ID 43 - ucIndex 35
  0xD0, 0x66, 0xA5, 0x83, 0x9B, 0x47, 0xFA, 0x8C, // PN Code ID 44 - ucIndex 36
  0xB8, 0x0F, 0x31, 0xC8, 0x26, 0x9F, 0xBD, 0x07, // PN Code ID 45 - ucIndex 37
//0x86, 0xAE, 0x89, 0x5E, 0xB1, 0x54, 0xA1, 0xD7, // PN Code ID 46
  0x9D, 0x61, 0x71, 0xB4, 0x89, 0x95, 0x03, 0xEF, // PN Code ID 47 - ucIndex 38
  0xD1, 0xCC, 0x4F, 0x86, 0xD5, 0x97, 0xBA, 0x40, // PN Code ID 48 - ucIndex 39
};

static const uint8_t ucPnCodeTable64kps[] PROGMEM = {
// LSB                                       MSB
  0xD2, 0x13, 0xFD, 0x03, 0xEA, 0x01, 0xE7, 0x6A, // PN Code ID 0 - ucIndex 0
  0xB2, 0xBB, 0x09, 0x2B, 0xB8, 0x6B, 0xC0, 0xDC, // PN Code ID 1 - ucIndex 1
  0xB3, 0x7A, 0x32, 0x31, 0xA4, 0xF2, 0x1E, 0xA3, // PN Code ID 2 - ucIndex 2
  0xC9, 0x8E, 0xCF, 0x14, 0xDD, 0x3B, 0x83, 0x44, // PN Code ID 3 - ucIndex 3
  0xB0, 0x47, 0x52, 0xF3, 0xC5, 0x4E, 0x35, 0x35, // PN Code ID 4 - ucIndex 4
  0xD7, 0x54, 0x5C, 0x45, 0xCE, 0x8A, 0x23, 0x7C, // PN Code ID 5 - ucIndex 5
  0xAC, 0x61, 0x9A, 0x7A, 0x83, 0xFB, 0xAC, 0x81, // PN Code ID 6 - ucIndex 6
  0x8A, 0xF6, 0x98, 0x39, 0x9C, 0x5F, 0x12, 0x3C, // PN Code ID 7 - ucIndex 7
//0x22, 0xB2, 0xE9, 0xE0, 0x3D, 0x6A, 0x8B, 0x1E, // Chip default
};

static const xWNetByteTable ucPnCodeTables[] PROGMEM = {
  { ucPnCodeTable16kps, WNET_PNCODETABLE_SIZEOF(ucPnCodeTable16kps) },
  { ucPnCodeTable64kps, WNET_PNCODETABLE_SIZEOF(ucPnCodeTable64kps) }
};

/*---- Device Registers table ------*/
static const xWDevRegister pDeviceRegistersTable16Kbps[] PROGMEM = {
  { REG_THRESHOLD_L, WUSB_CONFIG_NETWORK_DEFAULT_THRESHOLD_L_16KBPS },
  { REG_THRESHOLD_H, WUSB_CONFIG_NETWORK_DEFAULT_THRESHOLD_H_16KBPS }
};

static const xWDevRegister pDeviceRegistersTable64Kbps[] PROGMEM = {
  { REG_THRESHOLD_L, WUSB_CONFIG_NETWORK_DEFAULT_THRESHOLD_L_64KBPS },
  { REG_THRESHOLD_H, WUSB_CONFIG_NETWORK_DEFAULT_THRESHOLD_H_64KBPS }
};

static const xWDevRegisterTable pDeviceRegistersTables[] PROGMEM = {
  { pDeviceRegistersTable16Kbps, WDEV_REGTABLE_SIZEOF(pDeviceRegistersTable16Kbps) },
  { pDeviceRegistersTable64Kbps, WDEV_REGTABLE_SIZEOF(pDeviceRegistersTable64Kbps) }
};

/* public variables ========================================================= */
WusbNetwork dNet;

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static uint8_t
prvucDataRateIndex (void) {

  return eWDevGetDataRate() - 1; 
}

// ------------------------------------------------------------------------------
static const xWNetByteTable * 
prvpxDataratedPnCodeTable (void) {

  return &ucPnCodeTables[prvucDataRateIndex()];
}

// ------------------------------------------------------------------------------
static void 
prvvFlushRxPacket (void) {

  vWPktClear (&dNet.pxRxPacket);
  vWPktClear (&dNet.pxRxValid);
}

/*
 * Purge le paquet RX et configure pour le prochain
 */ 
static void 
prvvSetToReceive (void) {

  prvvFlushRxPacket ();
  vStateSet (&dNet.xError, WNET_SUCCESS);
  vStateSet (&dNet.xRxStatus, WNET_NOPACKET);
  /* Valide la réception */
  vWDevEnable (bRX_ENABLE);
}

/*
 * Routine Interruption Timeout
 */ 
static void
prvvTimeoutTask (xTaskHandle unused __attribute__ ((unused))) {

  dNet.xError = WNET_TIMEOUT;
}

/*
 * Charge la config réseau à partir de l'EEPROM
 */   
static void 
prvvWNetLoadConfigFromEEPROM(void) {
  xWNetConfig xConfig;
  
  eeprom_read_block (&xConfig, &pNetConfigEEPROM, sizeof(xWNetConfig));
  vWNetSetConfig (&xConfig);
}

/*
 * Charge la config réseau à partir de la FLASH
 */   
static void 
prvvWNetSetupDefaultConfig(void) {
  xWNetConfig xConfig;
  
  memcpy_P (&xConfig, &pNetConfigFLASH, sizeof(xWNetConfig));
  vWNetSetConfig (&xConfig);
  vWNetSaveConfigToEEPROM();
}

/* internal public functions ================================================ */

/*
 * Initialise le réseau
 *  L'initialisation du réseau se fait avec les valeurs sauvegardées en EEPROM.
 *  La configuration initiale est la suivante:
 *  - Canal 1
 *  - PnCode 1
 *  - Crc Seed 0
 *  - Checksum Seed 0
 *  .
 * eDataRate vitesse de transmission. 0 pour initialiser à partir de la config en EEPROM.
 */
void 
vWNetInit(eWDevDataRate eDataRate) {

  vWDevInit (eDataRate);
  vWDevSetupFromTable_P (pDeviceRegistersTables, prvucDataRateIndex());
  
  if (dNet.xTimeoutTask == 0)
    dNet.xTimeoutTask = xTaskCreate (1, prvvTimeoutTask);

  vWPktInit (&dNet.pxTxPacket, pxTxPacketBuffer, WNET_PACKET_SIZE_MAX);
  vWPktInit (&dNet.pxRxPacket, pxRxPacketBuffer, WNET_PACKET_SIZE_MAX);
  vWPktInit (&dNet.pxRxValid,  pxRxValidBuffer,  WNET_PACKET_SIZE_MAX);
  
  prvvWNetLoadConfigFromEEPROM ();
  if (xWNetStatus(WNET_EEPROMBLANK) != false)
    prvvWNetSetupDefaultConfig();
    
  dNet.usCorruptError = 0;  
  dNet.xError = WNET_SUCCESS;
  dNet.xRxStatus = WNET_NOPACKET;
}

/*
 * Attends la réception d'un paquet.
 * Si \c iTimeoutMs négatif, la fonction attends indéfiniment et sort uniquement 
 *  sur réception ou erreur. 
 * iTimeoutMs temps d'attente en millisecondes. 
 */   
eWNetErrorCode 
eWNetRxPktWait(int16_t iTimeoutMs) {


  /* Démarre Timer si Timeout positif, négatif pas de timeout */
  if (iTimeoutMs > 0) {

    vTaskSetInterval (dNet.xTimeoutTask, xTaskConvertMs((time_t)iTimeoutMs));
    vTaskStart (dNet.xTimeoutTask);
  }
  else {

    vTaskStop (dNet.xTimeoutTask);
  }
  prvvSetToReceive();

  do { 
  
    /* Boucle d'attente, sortie sur réception, erreur ou Timeout */
    if ((xStateTrywait(&dNet.xRxStatus, WNET_INPROGRESS) == 0) && (iTimeoutMs > 0)) {
    
        vTaskRewind (dNet.xTimeoutTask);
    }
    
    if (xStateTrywait(&dNet.xRxStatus, WNET_RECEIVED) == 0) {
      bool xPacketIsOk;
    
      /* paquet reçu, vérification et réparation éventuelle  */
      vWDevDisable ();
      xPacketIsOk = xWPktCheckAndRepair (&dNet.pxRxPacket, 
          &dNet.pxRxValid, dNet.xConfig.ucCrcSeed, dNet.xConfig.ucChecksumSeed);
              
      if (xPacketIsOk == true) {
      
        if (dNet.usCorruptError != 0)
          dNet.usCorruptError--; 
        break; /* sortie réception, paquet valide */
      }
      /* paquet non valide : 
        il ne s'agit pas forcément d'une erreur, cela peut être un paquet venant
        d'un autre réseau utilisant le même canal et pncode mais pas le même
        crc seed ou/et checksum seed. Par contre cela ne peut pas continuer
        car 2 réseaux sur le même canal avec le même PnCode, il y aura beaucoup
        de collisions. */
        dNet.usCorruptError += 2;
        if (dNet.usCorruptError >= WUSB_CONFIG_NETWORK_CORRUPTPKT_THRESHOLD) {
        
          /* Le seuil a été franchi, sortie sur erreur, PacketReceivedAndChecked = false */
          vStateSet (&dNet.xError, WNET_CORRUPTED_PACKET_ERROR);
        }
        else {
        
          /* Purge le paquet et retourne en réception */
          prvvSetToReceive();
        }
    }
  } while (xStateTrywait(&dNet.xError, WNET_SUCCESS) == 0);
  /* Sortie sur erreur ou en cas de réception d'une trame correcte */
  
  vWDevDisable ();
  vTaskStop (dNet.xTimeoutTask);
  return (eWNetErrorCode) xStateGet (&dNet.xError);
}

/*
 * Routine Interruption Réception 
 */ 
void
vWDevRxCB (uint8_t * ucRxData) {

  if (ucRxData[WDEV_RXSTAT] & bFULL_A) {
  
    if (xWPktIsFull (&dNet.pxRxPacket) != false) { 
    /* Le buffer est plein et le paquet incomplet, on purge. Le paquet devrait 
       être retransmis car pas d'Ack */

      dNet.xRxStatus = WNET_NOPACKET;
      prvvFlushRxPacket ();
    }
    else {

      if (xWPktIsEmpty (&dNet.pxRxPacket) != false)
        dNet.xRxStatus = WNET_INPROGRESS;
      /* ajoute l'octet reçu et celui de validation */
      vWPktAddByte (&dNet.pxRxPacket, ucRxData[WDEV_RXDATA]);
      vWPktAddByte (&dNet.pxRxValid, ucRxData[WDEV_RXVALID]);
    }
  }

  if (ucRxData[WDEV_RXSTAT] & bEOF_A) {
  
    if (xWPktLength(&dNet.pxRxPacket) >= WPKT_PING_PACKET_SIZE) { 
    /* un paquet a été reçu avec une longueur suffisante */
      vWDevDisable ();
      dNet.xRxStatus = WNET_RECEIVED;
    }
    else { 
    /* Le paquet n'est pas assez long, on purge. Il devrait être retransmis 
       car pas d'Ack */

      dNet.xRxStatus = WNET_NOPACKET;
      prvvFlushRxPacket ();
    }
  }
}

/*
 * Renvoie le Device ID du paquet reçu
 * Cette fonction s'adapte en fonction du bit WPKT_SINGLE reçu. Si ce bit est à 
 * 1 dans le paquet reçu, le Device ID est lu comme un octet et converti en uint16_t.
 * Cette fonction ne doit être utilisée que si le paquet reçu est de type DATA, ACK ou BIND_RESPONSE    
 */   
uint16_t 
usWNetRxPktDeviceId (void) {
  uint16_t usDeviceId;

  if (xWNetRxPktFlag(WPKT_SINGLE) != false) {
    /* Single Byte Device Id */
    usDeviceId = (uint16_t) ucWNetRxPktByte (WPKT_DID_OFFSET);
    if (usDeviceId == 0xFF)
      usDeviceId = WNET_INVALID_DEVICEID;
  }
  else {
  
    usDeviceId = usWNetRxPktWord (WPKT_DID_OFFSET);
  }
    
  return usDeviceId;
}

// -----------------------------------------------------------------------------
void
vWNetTxPktAddTrailer (void) {

  vWNetTxPktSetFlag (WPKT_SINGLE, xWNetSingleByteDeviceId());
  vWPktAddTrailer (&dNet.pxTxPacket, dNet.xConfig.ucCrcSeed, 
                   dNet.xConfig.ucChecksumSeed);
}

/*
 * Initialise une nouveau paquet de type ucType
 */   
void 
vWNetTxPktNew (uint8_t ucType) {

  vWPktPrepare (&dNet.pxTxPacket, (ucType) << 4);
}

/*
 * Ajoute le Device ID au paquet TX
 * L'ajout se fait en respectant le flag WNET_SINGLEBYTE de mode du réseau  
 */   
void 
vWNetTxPktAddDeviceId (uint16_t usDeviceId) {

  if (xWNetSingleByteDeviceId() != false) {

    vWNetTxPktAddByte (usDeviceId);
  }
  else {
  
    vWNetTxPktAddWord (usDeviceId);
  }
}

/*
 * Renvoie un nombre pseudo aléatoire compris entre 1 et 31
 */   
uint8_t 
ucWNetRand (void) {
  static bool xSeeded = false;
  uint8_t ucDelay;

  if (xSeeded == false) {
    /* Init générateur pseudo-aléatoire */
    uint8_t * pucSeed = (uint8_t *)((RAMEND + 0x0060) / 2); // place le pointeur au milieu de la SRAM
    
    /* TODO: Trouver un seed avec une meilleure entropie */
    while (*pucSeed == 0) {
    
      pucSeed++;
    }
    
    srand8 (ucWPktChecksum (pucWDevMid(), WDEV_MID_SIZEOF, *pucSeed));
    xSeeded = true; 
  }
  
  ucDelay = ((rand8() + 1) & 0x1F);
  
  return ucDelay;
}


/*
 * Modifie la configuration à partir d'une structure xWNetConfig
 */   
void 
vWNetSetConfig(const xWNetConfig * pxConfig) {

  xWNetSetChannel (pxConfig->ucChannel); /* go to idle status */
  xWNetSetPnCode (pxConfig->ucPnCode);
  vWNetSetCrcSeed (pxConfig->ucCrcSeed);
  vWNetSetChecksumSeed (pxConfig->ucChecksumSeed);
  dNet.xConfig.ucStatus = pxConfig->ucStatus;
}

/*
 * Sauvegarde la configuration courante en EEPROM
 */   
void 
vWNetSaveConfigToEEPROM(void) {

  eeprom_write_block (&dNet.xConfig, &pNetConfigEEPROM, sizeof(xWNetConfig));
  vWDevSaveConfigToEEPROM ();
}

/*
 * Renvoie le nombre de PnCode possibles
 */
uint8_t
ucWNetNumberOfPnCodes (void) {
  xWNetByteTable Table;

  memcpy_P (&Table, prvpxDataratedPnCodeTable(), sizeof (xWNetByteTable));
  return Table.pSize;
}

/*
 * Modifie le Pn Code du réseau
 * ucIndex ucIndex du PnCode
 * true Succès. False Erreur.
 */   
bool
xWNetSetPnCode(uint8_t ucValue) {
  xWNetByteTable Table;

  memcpy_P (&Table, prvpxDataratedPnCodeTable(), sizeof (xWNetByteTable));
  
  if (ucValue < Table.pSize)  {

    vWDevSetPnCode_P (&Table.pTable[ucValue*WNET_PNCODE_SIZE]);
    dNet.usCorruptError = 0;
    dNet.xConfig.ucPnCode = ucValue;
    return true;
  }
  return false;
}

/*
 * Passe au canal suivant dans le subset utilisé
 */   
void 
vWNetGoToNextChannel(void) {
  
  xWNetSetChannel ((ucWNetChannel() + WNET_NUM_OF_SUBSETS) % WNET_NUM_OF_CH_USED);
}

/*
 * Modifie le canal
 * ucIndex ucIndex du canal
 * true Succès. False Erreur.
 */   
bool
xWNetSetChannel(uint8_t ucValue) {
  
  if (ucValue < WNET_NUM_OF_CH_USED) {

    vWDevSetChannel (ucValue + WUSB_CONFIG_NETWORK_CHANNEL_OFFSET);
    dNet.usCorruptError = 0;
    dNet.xConfig.ucChannel = ucValue;
    return true;
  }
  return false;
}

/* =====================Fin de l'implémentation du réseau==================== */

#ifdef WNET_DEBUG_ONUART
/*
 * Fonctions de DEBUG
 * ========================================================================== 
 */
# include <avrio/term.h>
#define NEWLINE() fputc('\r', stderr)

// -----------------------------------------------------------------------------
void 
vWNetDbgNoResp (void) {

  fputc('X', stderr);
}

// -----------------------------------------------------------------------------
void 
vWNetDbgSend (void) {

  fputc('-', stderr);
}

// -----------------------------------------------------------------------------
void 
vWNetDbgDumpConfig (void) {

  fputs("C>", stderr);
  vTermPutHexBytes ((const uint8_t *)&dNet.xConfig, sizeof(xWNetConfig), stderr);
  NEWLINE();
}

// -----------------------------------------------------------------------------
void 
vWNetDbgDumpTxPacket (void) {

  fputs("T>", stderr);
  vTermPutHexBytes (dNet.pxTxPacket.pData, dNet.pxTxPacket.xLen, stderr);
  NEWLINE();
}

// -----------------------------------------------------------------------------
void 
vWNetDbgDumpRxPacket (void) {
  eWNetErrorCode eError = eWNetError();

  if (eError) {
  
    fputs("E>", stderr);
    vTermPutHexByte (eError, stderr);
    NEWLINE();
  }
  if (dNet.pxRxPacket.xLen) {
  
    fputs("R>", stderr);
    vTermPutHexBytes (dNet.pxRxPacket.pData, dNet.pxRxPacket.xLen, stderr);
    NEWLINE();
    fputs("V>", stderr);
    vTermPutHexBytes (dNet.pxRxValid.pData, dNet.pxRxValid.xLen, stderr);
    NEWLINE();
  }
}
#endif  /* WNET_DEBUG_ONUART defined */

#ifdef WNET_TEST
/*
 * Tests unitaires
 * ========================================================================== 
 */
#define WNET_TEST_CHANNEL 0
#define WNET_TEST_PNCODE  0
#define WNET_TEST_CRCSEED 0x00
#define WNET_TEST_CHECKSUMSEED  0x00
#define WNET_TEST_SENSORDID 0x1234

#define WNET_TEST_RETRY_MAX   10 // Nombre d'essai avant Timeout
#define WNET_TEST_RETRY_DELAY 0 // si <= 0, délai aléatoire
#define WNET_TEST_MAX_BOUND_DEVICES 4
#define WNET_TEST_HUB_NULL_MID 1

#define WNET_ASSERT(__test) \
  if (!(__test)) return iError; \
  iError--

/* private variables ======================================================== */
static int iError = -1;

/* internal private functions =============================================== */
// -----------------------------------------------------------------------------
int 
iWNetTestSetup (void) {
  xWNetConfig Old, New = { .ucChannel = 2, .ucPnCode = 3, .ucCrcSeed = 4, .ucChecksumSeed = 5 };

  vWNetInit (0);
  vWNetDbgDumpConfig();
  WNET_ASSERT (ucWNetChannel() == WUSB_CONFIG_NETWORK_SEEDED_BIND_CHANNEL);
  WNET_ASSERT (ucWNetPnCode() == WUSB_CONFIG_NETWORK_SEEDED_BIND_PNCODE);
  WNET_ASSERT (ucWNetCrcSeed() == WPKT_BIND_CRC_SEED);
  WNET_ASSERT (ucWNetChecksumSeed() == WPKT_BIND_CHECKSUM_SEED);
  memcpy (&Old, pxWNetCurrentConfig(), sizeof(xWNetConfig));
  
  xWNetSetChannel (WNET_TEST_CHANNEL);
  WNET_ASSERT (ucWNetChannel() == WNET_TEST_CHANNEL);
  
  xWNetSetPnCode (WNET_TEST_PNCODE);
  WNET_ASSERT (ucWNetPnCode() == WNET_TEST_PNCODE);

  vWNetSetCrcSeed (WNET_TEST_CRCSEED);
  WNET_ASSERT (ucWNetCrcSeed() == WNET_TEST_CRCSEED);

  vWNetSetChecksumSeed (WNET_TEST_CHECKSUMSEED);
  WNET_ASSERT (ucWNetChecksumSeed() == WNET_TEST_CHECKSUMSEED);
  
  /* modif sans sauvegarde */  
  vWNetSetConfig (&New);
  vWNetDbgDumpConfig();

  WNET_ASSERT (memcmp (pxWNetCurrentConfig(), &New, sizeof(xWNetConfig)) == 0);

  /* restauration */
  vWNetInit (0);
  WNET_ASSERT (memcmp (pxWNetCurrentConfig(), &Old, sizeof(xWNetConfig)) == 0);
  
  /* modif avec sauvegarde */  
  vWNetSetConfig (&New);
  vWNetDbgDumpConfig();
  vWNetSaveConfigToEEPROM ();
  vWNetInit (0);
  vWNetDbgDumpConfig();
  WNET_ASSERT (memcmp (pxWNetCurrentConfig(), &New, sizeof(xWNetConfig)) == 0);

  vWNetSetConfig (&Old);
  vWNetDbgDumpConfig();
  vWNetSaveConfigToEEPROM ();

  return 0;
}

#ifdef WUSB_CONFIG_HUB_ENABLE
/* internal private functions =============================================== */

// -----------------------------------------------------------------------------
static uint16_t
prvusBindDevice(uint8_t * pucMID) {
  static uint8_t ucDeviceCount;
  static uint8_t ucDevicesMID[WNET_TEST_MAX_BOUND_DEVICES*WDEV_MID_SIZEOF];
  uint8_t ucDeviceIndex;
  uint16_t usDID = WNET_INVALID_DEVICEID;
  
  if (ucDeviceCount < WNET_TEST_MAX_BOUND_DEVICES) {
  
    // Recherche du nouveau dans la table
    for (ucDeviceIndex = 0; ucDeviceIndex < ucDeviceCount; ucDeviceCount++) {
    
      if (memcmp (pucMID, &ucDevicesMID[ucDeviceIndex*WDEV_MID_SIZEOF], WDEV_MID_SIZEOF) == 0) {
      
        return ucDeviceIndex; // Device déjà dans la table
      }
    }
    // Mémorise le nouveau
    memcpy (&ucDevicesMID[ucDeviceCount*WDEV_MID_SIZEOF], pucMID, WDEV_MID_SIZEOF);
    usDID = ucDeviceCount++;
  }
  return usDID;
}

/* internal public functions ================================================ */
/*
 * Attends un paquet:
 * - Si paquet correct reçu, vérifie le type:
 *  - Si le type est un WPKT_DATA_PACKET, un WPKT_ACK_PACKET est renvoyé (V=1)
 *  - Si le type est un WPKT_BIND_REQUEST_PACKET, un WPKT_BIND_RESPONSE_PACKET
 *   est renvoyé
 *  - Pour les autres types, ne renvoie rien   
 * - Si paquet incorrect, ne fait rien
 * true si paquet correct reçu     
 */   
eWNetErrorCode  
eWNetTestHub (int16_t iTimeoutMs) {
  uint16_t usDeviceId;
    
  /* attente d'un paquet DATA */  
  if  (eWNetRxPktWait(iTimeoutMs) == WNET_SUCCESS) {
    
    /*  Un paquet correctement formaté a été reçu, le compte à rebourd a été
        déclenché, il faut répondre le plus rapidement possible
     */
    switch (ucWNetRxPktType()) {
      case WPKT_DATA_PACKET: 
        /* Paquet DATA reçu, construction du paquet ACK */
        usDeviceId = usWNetRxPktDeviceId();
        
        vWNetTxPktNew(WPKT_ACK_PACKET);
        vWNetTxPktSetFlag (WPKT_ACK, xWNetRxPktFlag(WPKT_TOGGLE));
        vWNetTxPktSetFlag (WPKT_VALID, (usDeviceId < WNET_TEST_MAX_BOUND_DEVICES ? true : false));
        vWNetTxPktAddDeviceId (usDeviceId);
        vWNetTxPktAddTrailer();
        vWNetTxPktSend(); // Fin du compte à rebourd
        vWNetDbgDumpRxPacket();
        vWNetDbgDumpTxPacket();
        break;

      case WPKT_BIND_REQUEST_PACKET: 
        /* Paquet BIND_REQUEST reçu, construction du paquet BIND_RESPONSE */
        usDeviceId = prvusBindDevice (pucWNetRxPktBytes(WPKT_MID_OFFSET));
        
        if (usDeviceId != WNET_INVALID_DEVICEID) {
        
          vWNetTxPktNew(WPKT_BIND_RESPONSE_PACKET);
          vWNetTxPktAddDeviceId (usDeviceId);
          vWNetTxPktAddByte (ucWNetChannel());
          vWNetTxPktAddByte (ucWNetPnCode());
          // Le MID est utilisé comme CRC et checksum seed
          vWNetTxPktAddWord (0);
          vWNetTxPktAddByte (ucWNetCrcSeed());
          vWNetTxPktAddByte (ucWNetChecksumSeed());
          vWNetTxPktAddTrailer();
          vWNetTxPktSend(); // Fin du compte à rebourd
          vWNetDbgDumpRxPacket();
          vWNetDbgDumpTxPacket();
        }
        
        break;
      default:
          vWNetDbgDumpRxPacket();
        break;
    }
  }
  return eWNetError();
}

#else   /* WUSB_CONFIG_HUB_ENABLE not defined */

/* private variables ======================================================== */
static uint8_t ucSensorFlag = (1 << WPKT_ACK);
static uint16_t usSensorDID = WNET_TEST_SENSORDID;

/* internal private functions =============================================== */
// -----------------------------------------------------------------------------
static inline bool
prvxToggleBit (uint8_t Header) {
 
  return (Header & 1<<WPKT_TOGGLE) != 0;
}

// -----------------------------------------------------------------------------
static void
prvvSetPingPkt (uint16_t usCount) {
  
  vWNetTxPktNew (WPKT_DATA_PACKET);
  vWNetTxPktSetAllFlags (ucSensorFlag);
  vWNetTxPktAddDeviceId (usSensorDID);
  vWNetTxPktAddWord (usCount);
  vWNetTxPktAddTrailer();
}

// -----------------------------------------------------------------------------
static void 
prvvRetryDelay (uint16_t usDelay) {

#if WNET_TEST_RETRY_DELAY > 0
  usDelay += WNET_TEST_RETRY_DELAY;
#else
  /* Attente pseudo aléatoire */
  usDelay += ucWNetRand();
#endif
  delay_ms (usDelay); 
}

/* internal public functions ================================================ */
/*
 * Envoi paquet BIND_REQUEST et attends réponse HUB en recommancant tant que le HUB
 * n'a pas répondu correctement.
 */ 
eWNetErrorCode 
eWNetTestSensorBind (uint16_t usMaxRetries, uint16_t usInterval,
                     xWNetTestResult * pxResult) {
  eWNetErrorCode eError;
  
  /* Prepare le paquet BIND_REQUEST */  
  vWNetTxPktNew (WPKT_BIND_REQUEST_PACKET);
  vWNetTxPktAddBytes (pucWDevMid(), WDEV_MID_SIZEOF);
  vWNetTxPktAddTrailer ();
  
  pxResult->usSended = 0;
  pxResult->usReceived = 0;
  pxResult->usCorrupted = 0;

  while (usMaxRetries--) {
  
    vWNetDbgDumpTxPacket();
    pxResult->usSended++;
    
    /* Envoi paquet BIND_REQUEST */
    vWNetTxPktSend();
    
    /* attente d'un paquet */
    eError = eWNetRxPktWait(ucWNetBindTimeout());
    
    switch (eError) {
    
      case WNET_SUCCESS:
        /* Paquet correct reçu */  
        vWNetDbgDumpRxPacket();
        pxResult->usReceived++;
        if (ucWNetRxPktType() == WPKT_BIND_RESPONSE_PACKET) {
        
          usSensorDID = usWNetRxPktDeviceId ();
          return WNET_SUCCESS;
        }
      break;
      
      case WNET_CORRUPTED_PACKET_ERROR:
        vWNetDbgDumpRxPacket();
        pxResult->usCorrupted++;
        pxResult->usReceived++;
      break;
      
      default:
        vWNetDbgNoResp ();
      break;
    }
    prvvRetryDelay (usInterval);
  }
  return eWNetError();
}

/*
 * Envoi paquet DATA et attends réponse HUB en recommancant tant que le HUB
 * n'a pas répondu correctement.
 */ 
eWNetErrorCode 
eWNetTestSensorPing (uint16_t usCount, uint16_t usInterval, 
                     xWNetTestResult * pxResult) {
  eWNetErrorCode eError;
  
  pxResult->usSended = 0;
  pxResult->usReceived = 0;
  pxResult->usCorrupted = 0;

  while (usCount--) {
  
    prvvSetPingPkt (pxResult->usSended++);
    
    /* Envoi paquet DATA */
    vWNetDbgSend();
    vWNetTxPktSend();
    
    /* attente d'un paquet */
    eError = eWNetRxPktWait(ucWNetAckTimeout());
    
    switch (eError) {
    
      case WNET_SUCCESS:
        /* Paquet correct reçu */  
        pxResult->usReceived++;

        if ((ucWNetRxPktType() == WPKT_ACK_PACKET) &&
            (usWNetRxPktDeviceId() == usSensorDID) &&
            (xWNetRxPktFlag (WPKT_ACK) == prvxToggleBit(ucSensorFlag))) {

          /*  - Si le paquet est un ACK 
              - Si son DID est le mien
              - Si son bit Ack Toggle correspond au bit de séquence précédent
              Alors c'est la réponse au paquet DATA précédement envoyé */
          ucSensorFlag ^= _BV(WPKT_TOGGLE); /* Bascule le bit T */
        }
        delay_ms(usInterval);
      break;
      
      case WNET_CORRUPTED_PACKET_ERROR:
        vWNetDbgDumpTxPacket();
        vWNetDbgDumpRxPacket();
        pxResult->usCorrupted++;
        pxResult->usReceived++;
        prvvRetryDelay (usInterval);
      break;
      
      default:
        vWNetDbgNoResp ();
        prvvRetryDelay (usInterval);
      break;
    }
  }
  return eWNetError();
}
#endif  /* WUSB_CONFIG_HUB_ENABLE not defined */
#endif  /* WNET_TEST defined */

/* ========================================================================== */
