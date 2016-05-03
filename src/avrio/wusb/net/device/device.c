/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file device.c
 * @brief Implémentation d'un coupleur WirelessUSB CYWUSB6935
 *
 * Revision History ------------------------------------------------------------
 *    20111002 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_WUSB_ENABLE
/* ========================================================================== */
# include <avrio/wusb/net/device/device.h>
# include <avr/pgmspace.h>

/* public variables ========================================================= */
/* Bloc de contrôle de la couche Device */
xWDevice dDevice;

/* private variables ======================================================== */

/* FLASH ---------------------------------------------------------------------*/
/* Configuration des registres communs à toutes les vitesses */
static const xWDevRegister 
xSetupBase[] PROGMEM = {

  { REG_CONTROL, 0 },
  { REG_CONFIG, (bIRQ_CMOS | bIRQ_ACTIVE_LOW) },
  { REG_CRYSTAL_ADJ, (bCLOCK_DISABLE | (WUSB_CONFIG_DEVICE_DEFAULT_XTL & mCRYSTAL_ADJ)) },
  { REG_PA, (WUSB_CONFIG_DEVICE_DEFAULT_PA_BIAS & mPA_BIAS) },
  { REG_RX_INT_EN, (bFULL_A | bEOF_A) },
  { REG_SERDES_CTL, (bSERDES_ENABLE | (mEND_OF_FRAME_LEN & WUSB_CONFIG_DEVICE_DEFAULT_EOF_BITS)) },
  { REG_TX_VALID, 0xFF },
  { REG_ANALOG_CTL, (bAGC_DISABLE | bPA_OUTPUT_ENABLE) },
  { REG_CHANNEL, WUSB_CONFIG_DEVICE_DEFAULT_CHANNEL },
  { REG_VCO_CAL, WUSB_CONFIG_DEVICE_DEFAULT_VCO_ADJUST },
  { REG_PWR_CTL, bPWR_CTL },
  { REG_SYN_LOCK_CNT, WUSB_CONFIG_DEVICE_DEFAULT_SYNTH_SETTLE_COUNT }, // wait for the synth to settle
  { REG_CLOCK_MANUAL, CM_DEFAULT },
  { REG_CLOCK_ENABLE, CE_DEFAULT }
};

/* Configuration des registres spécifiques au 16 Kbps */
static const xWDevRegister 
xSetup16kbps[] PROGMEM = {

  { REG_DATA_RATE, b64_CHIP_BITS | bNORMAL_DATA_RATE }
};

/* Configuration des registres spécifiques au 64 Kbps */
static const xWDevRegister 
xSetup64kbps[] PROGMEM = {

  { REG_DATA_RATE, b32_CHIP_BITS | bDOUBLE_DATA_RATE }
};

/* Tableau des configurations */
static const xWDevRegisterTable 
xSetupDefault[] PROGMEM = {

  { xSetup16kbps, WDEV_REGTABLE_SIZEOF(xSetup16kbps) },
  { xSetup64kbps, WDEV_REGTABLE_SIZEOF(xSetup64kbps) },
  { xSetupBase  , WDEV_REGTABLE_SIZEOF(xSetupBase)   } // xSetupBase must be the last
};

/* constants ================================================================ */
#define WDEV_CONFIG_BASE_INDEX (sizeof(xSetupDefault) / sizeof(xWDevRegisterTable))

/* internal private functions =============================================== */
/*
 * Recopie le Mid du module en RAM
 */
static inline void
prvvReadMid (void) {
  uint8_t reg;

  reg = ucWDevGetReg (REG_ANALOG_CTL);
  vWDevSetReg (REG_ANALOG_CTL, reg | bMIDREAD_ENABLE);
  vWIfcBurstRead (REG_MID, dDevice.pMid, WDEV_MID_SIZEOF);
  vWDevSetReg (REG_ANALOG_CTL, reg);
}

static uint8_t ucRxBuffer[3]; /* REG_RX_INT_STAT, REG_RX_DATA_A, REG_RX_VALID_A */

/*
 * Routine d'interruption Transmission
 */
static inline void
prvvRxIsr (void) {

  vWIfcBurstRead (REG_RX_INT_STAT, ucRxBuffer, 3);
  vWDevRxCB (ucRxBuffer);
}

/*
 * Routine d'interruption Transmission
 */
static inline void
prvvTxIsr (void) {
  uint8_t IrqSource;

  IrqSource = ucWDevGetReg (REG_TX_INT_STAT);

  if (IrqSource & bTX_DONE) {
  
    dDevice.pState = WDEV_TX;
  } 
  else if (IrqSource & bTX_EMPTY) {
  
    if (dDevice.pTxLength--) {
    
      vWDevSetReg (REG_TX_DATA, *dDevice.pTxData++);
    }
    else {
   
      IrqSource = 0;
      vWDevSetReg (REG_TX_INT_EN, bTX_DONE);
    }    
  }
}

#ifdef WUSB_CONFIG_EEPROM_ENABLE
/* private variables ======================================================== */

/* FLASH ---------------------------------------------------------------------*/
/* Permet de recharger une config propre en EEPROM en cas de corruption */
static xWDevConfig pDeviceConfigFLASH PROGMEM = {
  .pDataRate = WUSB_CONFIG_DEVICE_DEFAULT_DATARATE,
  .pXtalAdjust = -1
};

/* EEPROM --------------------------------------------------------------------*/
/* Copie en EEPROM du bloc de contrôle */
static xWDevConfig dDeviceConfig EEMEM = {
  .pDataRate = WUSB_CONFIG_DEVICE_DEFAULT_DATARATE,
  .pXtalAdjust = -1
};

/* internal private functions =============================================== */
// -----------------------------------------------------------------------------
static inline void
prvvSetDeviceControlBlock (eWDevDataRate DataRate) {
  /* lecture config en EEPROM */
  eeprom_read_block (&dDevice.xConfig, &dDeviceConfig, sizeof(xWDevConfig));
  
  /* Vérif si lecture EEPROM valide */
  if (dDevice.xConfig.pDataRate > WDEV_RATE_64KBPS) {
    /* et si EEPROM invalide, recharge la config usine et la sauve en EEPROM */
    memcpy_P (&dDevice.xConfig, &pDeviceConfigFLASH, sizeof(xWDevConfig));
    vWDevSaveConfigToEEPROM();
  }
  /* Si DataRate transmis par l'utilisateur, modif */  
  if (DataRate > WDEV_RATE_EEPROM)
    dDevice.xConfig.pDataRate = DataRate;
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
void
vWDevSaveConfigToEEPROM (void) {

  eeprom_write_block (&dDevice.xConfig, &dDeviceConfig, sizeof(xWDevConfig));
}

#else /* WUSB_CONFIG_EEPROM_ENABLE not defined */
/* internal private functions =============================================== */

// -----------------------------------------------------------------------------
static inline void
prvvSetDeviceControlBlock (eWDevDataRate DataRate) {

  if ((DataRate < WDEV_RATE_16KBPS) || (DataRate > WDEV_RATE_64KBPS))
    DataRate = WUSB_CONFIG_DEVICE_DEFAULT_DATARATE;
  dDevice.xConfig.pDataRate = DataRate;
}

// -----------------------------------------------------------------------------
#endif /* WUSB_CONFIG_EEPROM_ENABLE not defined */

/* == <API> == public application interface  ================================ */
/*
 * @brief Initialise le module radio
 * @param DataRate vitesse de transmission. 0 pour utiliser celle sotckée en EEPROM
 */
void 
vWDevInit (eWDevDataRate DataRate) {

  vWIfcInit ();  /* Init interface matérielle vers le module CYPRESS */
  prvvSetDeviceControlBlock (DataRate);

  /* Charge les registres du module avec les valeurs par défaut */
  vWDevSetupFromTable_P (xSetupDefault, WDEV_CONFIG_BASE_INDEX);
  vWDevSetupFromTable_P (xSetupDefault, dDevice.xConfig.pDataRate);
#if defined(WUSB_CONFIG_DEVICE_ADJUST_XTAL)
  vWDevSetCrystal (dDevice.xConfig.pXtalAdjust);
#endif
  prvvReadMid (); /* Lecture MID dans le module */
}

/*
 * @brief Transmission d'un message 
 *  Passe le module en transmission et attend la fin de l'envoi du message avant 
 *  de libérer le module.
 * @param Data pointeur sur les octets
 * @param Length nombre d'octets
 */
void 
vWDevTransmit (const uint8_t * Data, uint8_t Length) {

  vWDevEnable (bTX_ENABLE);
  dDevice.pTxLength = Length;
  dDevice.pTxData = Data;
  vStateSwitch (&dDevice.pState, WDEV_TX, WDEV_TXBUSY);
  vWusbIoIrqEnable ();
  (void) ucWDevGetReg (REG_TX_INT_STAT);
  vWDevSetReg (REG_TX_INT_EN, bTX_EMPTY);
  vStateWait (&dDevice.pState, WDEV_TX);  /* Attente fin transmission */

  vWDevDisable ();
}

/*
 * @brief Charge les registres du module radio à partir d'un tableau en RAM
 * @param Regs pointeur sur le tableau des registres
 * @param Size nombre de registres contenus dans le tableau 
 */
void 
vWDevSetup (const xWDevRegister * Regs, uint8_t Size) {
  uint8_t Index;

  for (Index = 0; Index < Size; Index++) {
  
    vWDevSetReg (Regs[Index].Address, Regs[Index].Value);
  }
}

/*
 * @brief Charge les registres du module radio à partir d'un tableau de configuration en FLASH
 * @param SetupTable pointeur sur le tableau de structures \c xWDevRegisterTable
 * @param Index Index  1 correspond au premier élément du tableau
 */
void 
vWDevSetupFromTable_P (const xWDevRegisterTable * SetupTable, uint8_t Index) {
  xWDevRegisterTable Setup;
  
  if (Index < 1) {
  
    Index = 1;
  }
  memcpy_P (&Setup, &SetupTable[Index-1], sizeof (xWDevRegisterTable));
  vWDevSetup_P (Setup.Regs, Setup.Size);
}

/*
 * @brief Charge les registres du module radio à partir d'un tableau en FLASH
 * @param Regs pointeur sur le tableau des registres
 * @param Size nombre de registres contenus dans le tableau 
 */
void 
vWDevSetup_P (const xWDevRegister * Regs, uint8_t Size) {
  uint8_t Index;

  for (Index = 0; Index < Size; Index++) {
  
    vWDevSetReg (pgm_read_byte_near (&Regs[Index].Address),
                 pgm_read_byte_near (&Regs[Index].Value));
  }
}

/*
 * @brief Passe le module à l'état dispo
 */
void
vWDevIdle (void) {

  vWDevSetReg (REG_CONTROL, 0);
  vStateSet (&dDevice.pState, WDEV_IDLE);
}

/*
 * @brief Arrête la transmission ou la réception
 */
void 
vWDevDisable (void) {

  vWusbIoIrqDisable ();
  vWDevIdle ();
}

/*
 * @brief Valide le module en attente de réception ou de transmission
 * @param rxOrTx bTX_ENABLE pour valider la transmission, bRX_ENABLE pour la réception
 */
void 
vWDevEnable (uint8_t RxOrTx) {

  vWDevDisable();
  if (RxOrTx == bTX_ENABLE) { 

    /* Transmission */
    vStateSwitch (&dDevice.pState, WDEV_IDLE, WDEV_TX);
    vWDevSetReg (REG_CONTROL, (bTX_ENABLE | bBYPASS_INTERNAL_SYN_LOCK_SIGNAL));
    /* Bit-shift fix - force 200us synch settle */
    delay_us (200);
  }
  else { 

    /* réception */
    vStateSwitch (&dDevice.pState, WDEV_IDLE, WDEV_RX);
    vWDevSetReg (REG_CONTROL, (bRX_ENABLE | bBYPASS_INTERNAL_SYN_LOCK_SIGNAL));
    ucWDevGetReg (REG_RX_INT_STAT); /* clear des flags d'irq */
    vWusbIoIrqEnable ();
  }
}

/*
 * @brief Endort le module radio afin d'économiser de l'énergie
 * Le module est à l'état statique. Les registres du module ne sont pas modifiés.
 */
void 
vWDevSleep(bool Enable) {

  if (Enable != false) {
#if defined(WUSB_CONFIG_DEVICE_ADJUST_XTAL)
    /* Régle la capacité à zéro afin de ne pas ralentir le réveil */
    vWDevSetCrystal (0);
#endif
    vWIfcSleep ();
  }
  else {
  
    vWIfcWakeup ();  /* reveil ! */
#if defined(WUSB_CONFIG_DEVICE_ADJUST_XTAL)
    /* Re-règle le quartz */
    vWDevSetCrystal (dDevice.xConfig.pXtalAdjust);
    /* attend la stabilisation du quartz */
    delay_ms (2);
#endif
  }
}

/**
 * @brief Lecture du PN Code
 */
const uint8_t * 
pucWDevGetPnCode (void) {
  static uint8_t PnCode[8];
  
  vWIfcBurstRead (REG_PN_CODE, PnCode, 8);
  return PnCode;
}

/*
 * Routine d'interruption appelée par le driver
 */
void
vWDevIsr (void) {

  if (dDevice.pState & WDEV_TX) {
  
    prvvTxIsr ();
  }
  else {
  
    prvvRxIsr ();
  }
}

#if defined(WUSB_CONFIG_DEVICE_ADJUST_XTAL)
// -----------------------------------------------------------------------------
/*
 * @brief Ajustement de la fréquence du quartz (REG_CRYSTAL_ADJ)
 *  Permet d'ajuster la capacité parallèle du quartz 13 Mhz afin de le régler précisement
 *  la fréquence à 13 MHz.
 *  La capacité peut être ajustée par pas de 0.135 pF entre 8.65 et 17.15 pF.
 *  
 * @param Valeur de la capacité à ajouter entre 0 et 63. 0 correspond à 8.65 pf, 63 à 17.15 pF.
 * @note 
 * - Toute valeur non comprise entre 0 et 63 est ignorée. 
 * - Pour utiliser cette fonction, il faut avoir validé 
 *  WUSB_CONFIG_DEVICE_ADJUST_XTAL lors de la compilation.
 * - L'activation ou non de la sortie X13 n'est pas modifiée.
 * .  
 */
void 
vWDevSetCrystal (uint8_t Value) {

  if ((Value >0) && (Value < 64))
  {
    dDevice.xConfig.pXtalAdjust = Value | 
                                (ucWDevGetReg (REG_CRYSTAL_ADJ) & bCLOCK_DISABLE);
    vWDevSetReg (REG_CRYSTAL_ADJ, dDevice.xConfig.pXtalAdjust);
  }
}

/*
 * @brief Active ou désactive la broche X13OUT du module radio
 * @param flag bCLOCK_DISABLE pour désactiver, bCLOCK_ENABLE pour activer
 * @note Pour utiliser cette fonction, il faut avoir validé WUSB_CONFIG_DEVICE_ADJUST_XTAL lors de la
 *  compilation
 */
void 
vWDevSetX13Out(uint8_t flag) {

  flag = (flag & bCLOCK_DISABLE) | (ucWDevGetReg (REG_CRYSTAL_ADJ) & mCRYSTAL_ADJ);
  vWDevSetReg (REG_CRYSTAL_ADJ, flag);
}

// -----------------------------------------------------------------------------
#endif  /* defined(WUSB_CONFIG_DEVICE_ADJUST_XTAL) */

#if defined(WUSB_CONFIG_HUB_ENABLE) || defined(WUSB_CONFIG_DEVICE_RSSI_ENABLE)
// -----------------------------------------------------------------------------
/*
 * @brief Evaluation de la qualité de la transmission
 *  La valeur renvoyée, entre 0 et 31 permet d'évaluer le bruit présent sur 
 *  le canal utilisé pour la transmission. \n
 *  Une valeur inférieure à 10 indique un canal relativement calme, entre 10 et 20 
 *  un canal probablement déjà utilisé et une valeur supérieure à 28 indique la 
 *  présence d'un brouillage.
 * @return une valeur entre 0 et 31.
 * @note Normalement cette fonction ne devrait être utilisée que par un 
 * contrôleur de liaison (hub).
 */
int8_t
iWDevRssi (void) {
  int8_t iRssi;
  
/*
  Extrait de la doc:
    To check for a quiet channel before transmitting, first set up receive mode 
    properly and read the RSSI register (Reg 0x22).
    If the valid bit is zero, then force the Carrier Detect register 
    (Reg 0x2F, bit 7=1) to initiate an ADC conversion. 
    Then, wait greater than 50 µs and read the RSSI register again.
    Next, clear the Carrier Detect Register (Reg 0x2F, bit 7=0) and turn the 
    receiver OFF. 
    Measuring the noise floor of a quiet channel is inherently a 'noisy' process 
    so, for best results, this
    procedure should be repeated several times (~20) to compute an average noise 
    floor level.
    A RSSI register value of 0-10 indicates a channel that is relatively quiet. 
    A RSSI register value greater than 10 indicates the channel is probably being used. 
    A RSSI register value greater than 28 indicates the presence of a strong signal.
*/
  do {

    /* Vérifie si le module est en mode réception */
    if ((eWDevGetStatus() & WDEV_RX) == 0) {
    
      /* si ce n'est pas le cas, passe en réception */
      vWDevEnable (bRX_ENABLE);
      delay_us (300);
    }
  
    iRssi = ucWDevGetReg (REG_RSSI);
    if ((iRssi & bRSSI_VALID) == 0) {
    
      /* force la détection de porteuse */
      vWDevSetReg (REG_CARRIER_DETECT, bCDET_OVERRIDE);
      delay_us (60); /* attend la fin de conversion */
      iRssi = ucWDevGetReg (REG_RSSI);
      vWDevSetReg (REG_CARRIER_DETECT, 0);
      vWDevDisable ();
    }
  } while ((iRssi & bRSSI_VALID) == 0);

  return iRssi & mRSSI;
}
// -----------------------------------------------------------------------------
#endif /* WUSB_CONFIG_HUB_ENABLE defined */

#ifdef WDEV_TEST
// -----------------------------------------------------------------------------
#include <avrio/semaphore.h>

/* private variables ======================================================== */
static volatile uint8_t * ucRxPacket;
static volatile uint8_t * ucRxValid;
static volatile int iRxLen;
static volatile uint16_t usOverRunCount;
static volatile uint16_t usUnvalidCount;
static xSem xRxPacketReceived;
static int iRxLastIndex;

// -----------------------------------------------------------------------------
// Gestionnaire réception (sous interruption !)
void
vWDevRxCB (uint8_t * ucRxData) {

  if (ucRxData[WDEV_RXSTAT] & bFULL_A) {
  
    if (ucRxData[WDEV_RXSTAT] & bRX_VALID_A) {
    
      /* Octet valide ...*/
      if (iRxLen == iRxLastIndex) {
      
        /* Dépassement de la taille du buffer de réception */
        iRxLen = -iRxLen;
        vWDevDisable ();
        vSemPost (&xRxPacketReceived);
      } 
      else {
      
        /* Mémorisation octet */
        *ucRxPacket++ = ucRxData[WDEV_RXDATA];
        *ucRxValid++ = ucRxData[WDEV_RXVALID];
        iRxLen++;
      }
    } 
    else {
    
      /* Octet invalide ...*/
      usUnvalidCount++;
    }

  }

  if (ucRxData[WDEV_RXSTAT] & bOVER_A) {
  
    /* Recouvrement d'octet */
    usOverRunCount++;
  }

  if (ucRxData[WDEV_RXSTAT] & bEOF_A) {
  
    if (iRxLen > 0) {
      
      /* Fin de trame détectée */
      vWDevDisable ();
      vSemPost (&xRxPacketReceived);
    }
  }
}

/* internal public functions ================================================ */
int 
iWDevReceive (uint8_t * ucUserPacket, uint8_t * ucUserValid, 
                    int iUserBufferSize, int usTimeout) {

    vWDevDisable ();
    
    if (usTimeout > 0) {
    
      usTimeout /= WUSB_CONFIG_SENSOR_SEARCH_CHANNEL_DELAY;
      if (usTimeout == 0)
        usTimeout++;
    }
    iRxLen = 0;
    iRxLastIndex = iUserBufferSize - 1;
    ucRxPacket = ucUserPacket;
    ucRxValid = ucUserValid;
    
    vWDevEnable (bRX_ENABLE);
    
    while ((xSemTryWait (&xRxPacketReceived) != 0) && (usTimeout != 0)) {
    
      delay_ms (WUSB_CONFIG_SENSOR_SEARCH_CHANNEL_DELAY);
      if (usTimeout > 0)
        --usTimeout;
    }
    if (usTimeout == 0) {

      vWDevDisable ();
    }
    return iRxLen;    
}
// -----------------------------------------------------------------------------
#endif /* WDEV_TEST defined */

#endif /* AVRIO_WUSB_ENABLE defined */
/* ========================================================================== */
