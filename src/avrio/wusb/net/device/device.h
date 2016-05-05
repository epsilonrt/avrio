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
 * @file device.h
 * @brief Modélisation d'un coupleur WirelessUSB CYWUSB6935
 *
 * Revision History ------------------------------------------------------------
 *    20111002 - Initial version by epsilonRT
 */
#ifndef _AVRIO_WUSB_DEVICE_H_
#define _AVRIO_WUSB_DEVICE_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup wusb_module
 * @{
 *
 *  @defgroup wusb_device Coupleur WirelessUSB CYWUSB6935
 *  Couche matérielle - OSI1 Physical Layer. \n
 *  @{
 */

/* macros =================================================================== */
/**
 * @brief Cacul du nombre de registres dans un tableau de registres \c xWDevRegister
 */
#define WDEV_REGTABLE_SIZEOF(r) (sizeof(r)/sizeof(xWDevRegister))

/* constants ================================================================ */
#define WDEV_MID_SIZEOF 4
#define WDEV_RXSTAT 0
#define WDEV_RXDATA 1
#define WDEV_RXVALID 2

/* enum ===================================================================== */
/**
 * @brief Différentes vitesse de transmission
 */
typedef enum {
  WDEV_RATE_EEPROM = 0,
  WDEV_RATE_16KBPS = 1,
  WDEV_RATE_64KBPS = 2
} eWDevDataRate;

/**
 * @brief Différents états du module
 */
typedef enum {
  WDEV_IDLE   = 0x00,  /**< Disponible */
  WDEV_TX     = 0x01,  /**< Transmission validée */
  WDEV_RX     = 0x02,  /**< Réception validée */
  WDEV_BUSY   = 0x04,  /**< Occupé */
  WDEV_TXBUSY = (WDEV_TX | WDEV_BUSY)  /**< Transmission en cours */
} eWDevStatus;

/* structures =============================================================== */
/**
 * @brief Structure de stockage d'un registre du module
 */
typedef struct xWDevRegister {

  uint8_t Address;  /**< Adresse du registre */
  uint8_t Value;  /**< Valeur du registre */
} xWDevRegister;

/**
 * @brief Structure de stockage d'un tableau de registres du module
 */
typedef struct xWDevRegisterTable {

  const struct xWDevRegister * Regs; /**< Ensemble des registres */
  uint8_t Size; /**< Nombre de registres dans Regs */
} xWDevRegisterTable;

/**
 * @brief Structure de stockage de la configuration du module
 */
typedef struct xWDevConfig {

  eWDevDataRate pDataRate;  /**< Vitesse de transmission */
  int8_t pXtalAdjust;  /**< Ajustement de la fréquence du quartz. Valeur entre 0 et 63, -1 pas d'ajustement */
} xWDevConfig;

/* == <API> == public application interface  ================================ */
/**
 * @brief Initialise le module radio
 *  L'initialisation se fait avec les valeurs par défaut correspondants à la 
 *  vitesse de transmission \c DataRate passée en paramètre
 *
 * @param DataRate vitesse de transmission. 0 pour initialiser à partir de la config en EEPROM.
 */
void vWDevInit (eWDevDataRate DataRate);

/**
 * @brief Transmission d'un message 
 * @param Data pointeur sur les octets
 * @param Length nombre d'octets
 */
void vWDevTransmit (const uint8_t * Data, uint8_t Length);

/**
 * @brief Réception d'un message 
 * @param ucUserPacket Buffer pour stocker les octets reçus
 * @param ucUserValid Buffer pour stocker les octets de validation
 * @param iUserBufferSize Taille des buffers ucUserPacket et ucUserValid
 * @param usTimeout Temps d'attente avant sortie. -1 attente infinie
 * @return Le nombre d'octets reçus ou un nombre négatif en cas d'erreur
 * @note Cette fonction est prévue pour être utilisée que dans le cas où la 
 * couche N to 1 n'est pas utilisée. La macro WDEV_TEST doit être définie lors
 * de la compilation.
 */
int iWDevReceive (uint8_t * ucUserPacket, uint8_t * ucUserValid, 
                    int iUserBufferSize, int usTimeout);
/**
 * @brief Gestionnaire de réception radio
 * Renvoie l'information reçue à la couche supérieure.
 * @param Data Le pointeur désigne 3 octets qui sont dans l'ordre le contenu des 
 *  registres REG_RX_INT_STAT, REG_RX_DATA_A et REG_RX_VALID_A.
 */
void vWDevRxCB (uint8_t * Data);

/**
 * @brief Passe le module en attente de réception ou de transmission
 * @param rxOrTx bTX_ENABLE pour valider la transmission, bRX_ENABLE pour la réception
 */
void vWDevEnable (uint8_t Tx);

/**
 * @brief Arrête la transmission ou la réception
 */
void vWDevDisable (void);

/**
 * @brief Endort ou réveille le module afin d'économiser de l'énergie
 * Le module est à l'état statique. Les registres du module ne sont pas modifiés.
 * @param Sleep true pour endormir, false pour réveiller
 */
void vWDevSleep (bool Sleep);

/**
 * @brief Charge les registres du module radio à partir d'un tableau en RAM
 * @param Regs pointeur sur le tableau des registres
 * @param Size nombre de registres contenus dans le tableau 
 */
void vWDevSetup (const xWDevRegister * Regs, uint8_t Size);

/**
 * @brief Charge les registres du module radio à partir d'un tableau en FLASH
 * @param Regs pointeur sur le tableau des registres
 * @param Size nombre de registres contenus dans le tableau 
 */
void vWDevSetup_P (const xWDevRegister * Regs, uint8_t Size);

/**
 * @brief Charge les registres du module radio à partir d'un tableau de configuration en FLASH
 * @param SetupTable pointeur sur le tableau de structures \c xWDevRegisterTable
 * @param Index Index  1 correspond au premier élément du tableau
 */
void vWDevSetupFromTable_P(const xWDevRegisterTable * SetupTable, uint8_t Index);

/**
 * @brief Evaluation de la qualité de la transmission
 *  La valeur renvoyée, entre 0 et 31 permet d'évaluer le bruit présent sur 
 *  le canal utilisé pour la transmission. \n
 *  Une valeur inférieure à 10 indique un canal relativement calme, entre 10 et 20 
 *  un canal probablement déjà utilisé et une valeur supérieure à 28 indique la 
 *  présence d'un brouillage.
 * @return une valeur entre 0 et 31.
 * @note Cette fonction ne peut être utilisée que par un hub, il faut que la
 * macro WUSB_CONFIG_HUB_ENABLE soit définie à la compilation. 
 */
int8_t iWDevRssi (void);

/**
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
void vWDevSetCrystal (uint8_t Capacitance);

/**
 * @brief Active ou désactive la broche X13OUT du module radio
 * @param flag bCLOCK_DISABLE pour désactiver, bCLOCK_ENABLE pour activer
 * @note Pour utiliser cette fonction, il faut avoir validé 
 *  WUSB_CONFIG_DEVICE_ADJUST_XTAL lors de la compilation.
 */
void vWDevSetX13Out (uint8_t flag);

/**
 * @brief Sauvegarde la configuration courante en EEPROM
 * @note Pour utiliser cette fonction, il faut avoir validé 
 *  WUSB_CONFIG_EEPROM_ENABLE lors de la compilation.
 */
void vWDevSaveConfigToEEPROM (void);

/**
 * @brief Lecture du PN Code courant
 */
const uint8_t * pucWDevGetPnCode (void);

#if defined(__DOXYGEN__)
/**
 * @brief Modification du PN Code
 * @param PnCode pointeur sur les 8 octets en RAM du PN Code (octet de poids faible en premier)
 */
inline void vWDevSetPnCode (const uint8_t * PnCode);

/**
 * @brief Modification du PN Code
 * @param PnCode pointeur sur les 8 octets en FLASH du PN Code (octet de poids faible en premier)
 */
inline void vWDevSetPnCode_P (const uint8_t * PnCode);

/**
 * @brief Lecture des 4 octets de l'identifiant de fabrication (REG_MID)
 */
inline const uint8_t * pucWDevMid (void);

/**
 * @brief Modifie les 4 octets de l'identifiant de fabrication
 *  La modificiation se fait en RAM. Elle est perdue lors d'un RESET.
 *  Attention, normalement le MID est fourni par le fabricant du module et n'est
 *  pas accessible en écriture.
 *  La modification du MID risque de provoquer des conflits sur le réseau (si
 *  un autre node a le même MID)
 *  \param Mid les 4 octets du nouveau MID rangés en little endian
  */
inline void vWDevSetMid (const uint8_t * Mid);

/**
 * @brief Lecture de l'identifiant de revision (REG_ID)
 */
inline uint8_t ucWDevRevId (void);

/**
 * @brief Renvoie la vitesse de transmission
 */
inline eWDevDataRate eWDevGetDataRate (void);

/**
 * @brief Réinitialise le module radio avec une nouvelle vitesse de transmission
 * @param DataRate vitesse de transmission
 */
inline void vWDevSetDataRate (eWDevDataRate DataRate);

/**
 * @brief Modification du canal de réception (REG_CHANNEL)
 */
inline void vWDevSetChannel (uint8_t Value);

/**
 * @brief Lecture du canal de réception (REG_CHANNEL)
 */
inline uint8_t ucWDevGetChannel (void);

/**
 * @brief Modification du niveau d'amplification à l'emission (REG_PA)
 */
inline void vWDevSetPaBias (uint8_t Value);

/**
 * @brief Lecture du niveau d'amplification à l'emission (REG_PA)
 */
inline uint8_t ucWDevGetPaBias (void);

/**
 * @brief Modification du seuil de réception du niveau bas (REG_THRESHOLD_L)
 */
inline void vWDevSetThresholdLow (uint8_t Value);

/**
 * @brief Lecture du seuil de réception du niveau bas (REG_THRESHOLD_L)
 */
inline uint8_t ucWDevGetThresholdLow (void);

/**
 * @brief Modification du seuil de réception du niveau haut (REG_THRESHOLD_H)
 */
inline void vWDevSetThresholdHigh (uint8_t Value);

/**
 * @brief Lecture du seuil de réception du niveau haut (REG_THRESHOLD_H)
 */
inline uint8_t ucWDevGetThresholdHigh (void);

/**
 * @brief Modification du délai de verrouillage du synthetiseur par pas de 2 us(REG_SYN_LOCK_CNT)
 */
inline void vWDevSetSynthLockCount (uint8_t Value);

/**
 * @brief Lecture du délai de verrouillage du synthetiseur par pas de 2 us(REG_SYN_LOCK_CNT)
 */
inline uint8_t ucWDevGetSynthLockCount (void);

/**
 * @brief Renvoie l'état du module
 */
inline eWDevStatus eWDevGetStatus (void);

/**
 * @brief Attends que le module passe à l'état \c Status
 */
inline void vWDevWaitStatus (eWDevStatus eStatus);

/**
 * @brief Lecture de la capacité d'ajustement de la fréquence du quartz (REG_CRYSTAL_ADJ)
 * @return valeur de la capacité entre 0 et 63. 0 correspond à 8.65 pf, 63 à 17.15 pF.
 */
inline uint8_t ucWDevGetCrystal (void);

/**
 * @brief Lecture de l'activation de la broche X13OUT (REG_PA)
 * @return bCLOCK_DISABLE si désactivée, bCLOCK_ENABLE si activée.
 */
inline uint8_t ucWDevGetX13Out (void);

/**
 * @brief Ecriture dans un registre octet du module radio
 * @param Address adresse du registre
 * @param Value valeur du registre
 */
inline void vWDevSetReg (uint8_t Address, uint8_t Value);

/**
 * @brief Lecture d'un registre octet du module radio
 * @param Address adresse du registre
 * @return Value valeur du registre
 */
inline uint8_t ucWDevGetReg (uint8_t Address);

/* == <API-END> == public application interface  ============================ */
/**@} */

#else   /* ! defined(__DOXYGEN__) */
#include <avrio/state.h>
#include <avrio/wusb/net/device/iface.h>

#ifdef AVRIO_WUSB_ENABLE
/* ========================================================================== */
/*                  Implémentation inline (non documentée)                    */
/* ========================================================================== */
#include <string.h>
#include <avr/eeprom.h>

/* structures =============================================================== */
typedef struct xWDevice {

/* private: */
  xState pState; /*< état du module */
  const uint8_t * pTxData; /*< pointeur sur les octets à envoyer */
  uint8_t pTxLength; /*< nombre d'octets pointés par \c pTxData */
  xWDevConfig xConfig; /*< config actuelle du module */
  uint8_t pMid[4]; /*< copie en ram du Mid du module */
} xWDevice;

/* internal public functions ================================================ */
extern void vWDevIdle (void);

/* public variables ========================================================= */
extern xWDevice dDevice;

/* inline public functions ================================================== */

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void
vWDevSetReg (uint8_t Reg, uint8_t Value)) {

  vWIfcWrite (Reg, Value);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucWDevGetReg (uint8_t Reg)) {

  return ucWIfcRead (Reg);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (eWDevStatus 
eWDevGetStatus (void)) {

  return (eWDevStatus) xStateGet (&dDevice.pState);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vWDevWaitStatus (eWDevStatus Status)) {

  vStateWait (&dDevice.pState, Status);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (const uint8_t * 
pucWDevMid (void)) {

  return dDevice.pMid;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (eWDevDataRate 
eWDevGetDataRate (void)) {

  return dDevice.xConfig.pDataRate;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vWDevSetDataRate (eWDevDataRate DataRate)) {

  vWDevInit (DataRate);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucWDevGetCrystal (void)) {

  return ucWDevGetReg (REG_CRYSTAL_ADJ) & mCRYSTAL_ADJ;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucWDevGetX13Out (void)) {

  return ucWDevGetReg (REG_CRYSTAL_ADJ) & bCLOCK_DISABLE;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void
vWDevSetPnCode_P (const uint8_t * PnCode)) {

  vWIfcBurstWrite_P (REG_PN_CODE, PnCode, 8);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void
vWDevSetPnCode (const uint8_t * PnCode)) {

  vWIfcBurstWrite (REG_PN_CODE, PnCode, 8);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucWDevRevId (void)) {

  return ucWDevGetReg (REG_ID);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucWDevGetThresholdLow (void)) {

  return ucWDevGetReg (REG_THRESHOLD_L) & mTHRESHOLD_L;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucWDevGetThresholdHigh (void)) {

  return ucWDevGetReg (REG_THRESHOLD_H) & mTHRESHOLD_H;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucWDevGetChannel (void)) {

  return ucWDevGetReg (REG_CHANNEL) & mCHANNEL;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucWDevGetPaBias (void)) {

  return ucWDevGetReg (REG_PA) & mPA_BIAS;
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (uint8_t 
ucWDevGetSynthLockCount (void)) {

  return ucWDevGetReg (REG_SYN_LOCK_CNT);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vWDevSetThresholdLow (uint8_t Value)) {

  vWDevSetReg (REG_THRESHOLD_L, Value & mTHRESHOLD_L);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vWDevSetThresholdHigh (uint8_t Value)) {

  vWDevSetReg (REG_THRESHOLD_H, Value & mTHRESHOLD_H);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vWDevSetChannel (uint8_t Value)) {

  vWDevIdle ();
  vWDevSetReg (REG_CHANNEL, Value & mCHANNEL);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void 
vWDevSetPaBias (uint8_t Value)) {

  vWDevSetReg (REG_PA, Value & mPA_BIAS);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void
vWDevSetSynthLockCount (uint8_t Value)) {

  vWDevSetReg (REG_SYN_LOCK_CNT, Value);
}

// -----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void
vWDevSetMid (const uint8_t * Mid)) {

  memcpy (&dDevice.pMid, Mid, WDEV_MID_SIZEOF);
}
#endif /* AVRIO_WUSB_ENABLE defined */
#endif  /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_WUSB_DEVICE_H_ */
