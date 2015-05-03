/**
 * @file twi.c
 * @brief Module I2C avec le coupleur natif sous interruption
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>

 */
#include "avrio-config.h"

#ifdef AVRIO_TWI_ENABLE
/* ========================================================================== */
#  include <avrio/twi.h>
#  include <avrio/queue.h>
#  include <util/atomic.h>
#  include <avr/io.h>
#  include <avr/interrupt.h>
#  include <util/twi.h>

#  include "avrio-board-twi.h"
#  if !defined(TWI_MASTER_ENABLE) && !defined(TWI_SLAVE_ENABLE)
#    error Il faut definier TWI_MASTER_ENABLE et/ou TWI_SLAVE_ENABLE a la compilation
#  endif

#  ifndef __SIZEOF_ENUM__
#    define __SIZEOF_ENUM__ 1
#    warning __SIZEOF_ENUM__ = 1 defini par defaut, veuillez verifier que -fshort-enums est fourni a la compilation
#  endif

/* constants ================================================================ */
#  define TWI_RD  _BV(0)// bit RW dans l'adresse circuit

/* macros =================================================================== */
#  ifdef TWI_LED_DEBUG
#    include <avrio/led.h>
#    define TWI_LED_TOGGLE(__ledmask) vLedToggle(__ledmask)
#  else
#    define TWI_LED_TOGGLE(__ledmask)
#  endif

/*
 * Enumération des états du driver I2c
 */
typedef enum {
  TWI_STATE_READY = 0,
  TWI_STATE_MASTER_TX = 0x01,
  TWI_STATE_MASTER_RX = 0x02,
  TWI_STATE_MASTER_SLA_NACK = 0x04,
  TWI_STATE_MASTER_ARB_LOST = 0x08,
  TWI_STATE_SLAVE_TX = 0x10,
  TWI_STATE_SLAVE_RX = 0x20,
} eTwiSate;


/* private variables ======================================================== */
static volatile uint8_t eTwiCurrentState;
static eTwiStatus eTwiLastStatus;


/* private functions ======================================================== */
// -----------------------------------------------------------------------------
// Modifie le registre de contrôle
static inline void
prvvSetCR (uint8_t ucCmd) {

  TWCR = _BV (TWEN) | _BV (TWIE) | ucCmd;
}

// -----------------------------------------------------------------------------
// Génération d'une condition de start sur le bus
static inline void
prvvStart (void) {

  prvvSetCR (_BV (TWINT) | _BV (TWSTA));
}

// -----------------------------------------------------------------------------
// Génération d'une condition de stop sur le bus
static inline void
prvvStop (void) {

  prvvSetCR (_BV (TWINT) | _BV (TWEA) | _BV (TWSTO));
}

// -----------------------------------------------------------------------------
// Valide le bit ACK et clear le flag d'interruption
static inline void
prvvSetAck (void) {

  prvvSetCR (_BV (TWINT) | _BV (TWEA));
}

// -----------------------------------------------------------------------------
// Dévalide le bit ACK et clear le flag d'interruption
static inline void
prvvSetNack (void) {

  prvvSetCR (_BV (TWINT));
}

#  ifdef TWI_MASTER_ENABLE
/*-----------------------------Mode Maître------------------------------------*/

/* constants ================================================================ */
#    define TWI_IDLE_FRAME_ID 0
#    define TWI_MASTER_CTRL_LEN ( TWI_ID_SIZEOF + TWI_DEVICE_ADDR_SIZEOF + \
                                TWI_LENGTH_SIZEOF )
#    define TWI_MASTER_STAT_LEN ( TWI_ID_SIZEOF + sizeof(eTwiStatus) + \
                                TWI_LENGTH_SIZEOF)

/* private variables ======================================================== */
static xTwiId xTwiMstCurFrameId;
static xTwiDeviceAddr xTwiMstDeviceAddr;
static xTwiLength xTwiMstDeviceLength;
static xTwiLength xTwiMstDeviceCounter;
static int8_t iTwiMstAddrNackCount;

QUEUE_STATIC_DECLARE (xTwiMstTxQueue, TWI_MASTER_TXBUFSIZE);
QUEUE_STATIC_DECLARE (xTwiMstRxQueue, TWI_MASTER_RXBUFSIZE);
QUEUE_STATIC_DECLARE (xTwiMstCtrlQueue, TWI_MASTER_CTRLBUFSIZE);
QUEUE_STATIC_DECLARE (xTwiMstStatusQueue, TWI_MASTER_STATUSBUFSIZE);

/* inline private functions ================================================= */

// ----------------------------------------------------------------------------
// Ecrit un octet dans le registre et valide les interruptions
static inline void
prvvMstSendByte (uint8_t ucByte) {

  TWDR = ucByte;
  prvvSetCR (_BV (TWINT));// Clear Interrupt Flag
}

// ----------------------------------------------------------------------------
// Libère le bus
// <<<< ATTENTION >>>> Exécuté sous interruption !
static inline void
prvvMstReleaseBus (void) {

  prvvSetNack ();
}

// ----------------------------------------------------------------------------
static inline void
prvvMstPushId (xQueue * pxQueue, xTwiId xId) {

#    if   TWI_ID_SIZEOF == 1
  vQueuePush (pxQueue, xId);
#    elif TWI_ID_SIZEOF == 2
  vQueuePushWord (pxQueue, xId);
#    else
#      warning La taille du type xTwiId est incorrecte !
#    endif
}

// ----------------------------------------------------------------------------
static inline void
prvvMstPushDeviceAddr (xQueue * pxQueue, xTwiDeviceAddr xDeviceAddr) {

#    if   TWI_DEVICE_ADDR_SIZEOF == 1
  vQueuePush (pxQueue, xDeviceAddr);
#    elif TWI_DEVICE_ADDR_SIZEOF == 2
  vQueuePushWord (pxQueue, xDeviceAddr);
#    else
#      warning La taille du type xTwiDeviceAddr est incorrecte !
#    endif
}

// ----------------------------------------------------------------------------
static inline void
prvvMstPushLength (xQueue * pxQueue, xTwiLength xLength) {

#    if   TWI_LENGTH_SIZEOF == 1
  vQueuePush (pxQueue, xLength);
#    elif TWI_LENGTH_SIZEOF == 2
  vQueuePushWord (pxQueue, xLength);
#    else
#      warning La taille du type xTwiLength est incorrecte !
#    endif
}

// ----------------------------------------------------------------------------
static inline void
prvvMstPushStatus (xQueue * pxQueue, eTwiStatus eStatus) {

#    if   __SIZEOF_ENUM__ == 1
  vQueuePush (pxQueue, eStatus);
#    elif __SIZEOF_ENUM__ == 2
  vQueuePushWord (pxQueue, eStatus);
#    else
#      warning La taille du type __SIZEOF_ENUM__ est incorrecte !
#    endif
}

// ----------------------------------------------------------------------------
static inline xTwiId
prvxMstPullId (xQueue * pxQueue) {

#    if   TWI_ID_SIZEOF == 1
  return (xTwiId) ucQueuePull (pxQueue);
#    elif TWI_ID_SIZEOF == 2
  return (xTwiId) usQueuePullWord (pxQueue);
#    else
#      warning La taille du type xTwiId est incorrecte !
#    endif
}

// ----------------------------------------------------------------------------
static inline void
prvvMstDropId (xQueue * pxQueue) {

  vQueueDrop (pxQueue);
#    if TWI_ID_SIZEOF == 2
  vQueueDrop (pxQueue);
#    elif TWI_ID_SIZEOF != 1
#      warning La taille du type xTwiId est incorrecte !
#    endif
}

// ----------------------------------------------------------------------------
static inline xTwiDeviceAddr
prvxMstPullDeviceAddr (xQueue * pxQueue) {

#    if   TWI_DEVICE_ADDR_SIZEOF == 1
  return (xTwiDeviceAddr) ucQueuePull (pxQueue);
#    elif TWI_DEVICE_ADDR_SIZEOF == 2
  return (xTwiDeviceAddr) usQueuePullWord (pxQueue);
#    else
#      warning La taille du type xTwiDeviceAddr est incorrecte !
#    endif
}

// ----------------------------------------------------------------------------
static inline xTwiDeviceAddr
prvxMstReadDeviceAddr (xQueue * pxQueue) {

#    if   TWI_DEVICE_ADDR_SIZEOF == 1
  return (xTwiDeviceAddr) ucQueueRead (pxQueue, sizeof (xTwiId));
#    elif TWI_DEVICE_ADDR_SIZEOF == 2
  return (xTwiDeviceAddr) usQueueReadWord (pxQueue, sizeof (xTwiId));
#    else
#      warning La taille du type xTwiDeviceAddr est incorrecte !
#    endif
}

// ----------------------------------------------------------------------------
static inline xTwiLength
prvxMstPullLength (xQueue * pxQueue) {

#    if   TWI_LENGTH_SIZEOF == 1
  return (xTwiLength) ucQueuePull (pxQueue);
#    elif TWI_LENGTH_SIZEOF == 2
  return (xTwiLength) usQueuePullWord (pxQueue);
#    else
#      warning La taille du type xTwiLength est incorrecte !
#    endif
}

// ----------------------------------------------------------------------------
static inline eTwiStatus
prveMstPullStatus (xQueue * pxQueue) {

#    if   __SIZEOF_ENUM__ == 1
  return (eTwiStatus) ucQueuePull (pxQueue);
#    elif __SIZEOF_ENUM__ == 2
  return (eTwiStatus) usQueuePullWord (pxQueue);
#    else
#      warning La taille du type __SIZEOF_ENUM__ est incorrecte !
#    endif
}

/* private functions ======================================================== */

// ----------------------------------------------------------------------------
static void
prvvMstInit (void) {

  vQueueFlush (&xTwiMstTxQueue);
  vQueueFlush (&xTwiMstRxQueue);
  vQueueFlush (&xTwiMstCtrlQueue);
  vQueueFlush (&xTwiMstStatusQueue);
}

// ----------------------------------------------------------------------------
// Renvoie un nouveau identifiant de trame
static xTwiId
prvxMstNewId (void) {
  static xTwiId xTwiMstIdCounter;

  xTwiMstIdCounter++;
  if (xTwiMstIdCounter <= 0) {

    xTwiMstIdCounter = 1;
  }

  return xTwiMstIdCounter;
}

// ----------------------------------------------------------------------------
// Nouvelle Trame
// <<<< ATTENTION >>>> Exécuté sous interruption !
static void
prvvMstNextFrame (void) {

  iTwiMstAddrNackCount = TWI_MASTER_ADDR_NACK_RETRY;
  xTwiMstCurFrameId = prvxMstPullId (&xTwiMstCtrlQueue);
  xTwiMstDeviceAddr = prvxMstPullDeviceAddr (&xTwiMstCtrlQueue);
  xTwiMstDeviceLength = prvxMstPullLength (&xTwiMstCtrlQueue);
  xTwiMstDeviceCounter = 0;
  if (xTwiMstDeviceAddr & TWI_RD) {

    eTwiCurrentState = TWI_STATE_MASTER_RX;
  } else {

    eTwiCurrentState = TWI_STATE_MASTER_TX;
  }
}

// ----------------------------------------------------------------------------
// Empile le résultat de l'opération
// <<<< ATTENTION >>>> Exécuté sous interruption !
static void
prvvMstPushResult (void) {

  prvvMstPushId (&xTwiMstStatusQueue, xTwiMstCurFrameId);
  prvvMstPushStatus (&xTwiMstStatusQueue, eTwiLastStatus);
  prvvMstPushLength (&xTwiMstStatusQueue, xTwiMstDeviceCounter);
}

// ----------------------------------------------------------------------------
// Remet le module maître au repos
// <<<< ATTENTION >>>> Exécuté sous interruption !
static void
prvvMstSetIdle (void) {

  xTwiMstCurFrameId = TWI_IDLE_FRAME_ID;
  eTwiCurrentState = TWI_STATE_READY;
  prvvStop ();
}

// ----------------------------------------------------------------------------
static inline void
prvvMstSetErrorAndStop (eTwiStatus eError) {

  if (eTwiCurrentState & TWI_STATE_MASTER_TX) {

  // Détruit les octets restants dans le buffer de transmission
    vQueueDropBytes (&xTwiMstTxQueue,
                     xTwiMstDeviceLength - xTwiMstDeviceCounter);
  } else if (eTwiCurrentState & TWI_STATE_MASTER_RX) {

  // Détruit les octets reçus dans le buffer de réception
    vQueueDropBytes (&xTwiMstRxQueue, xTwiMstDeviceCounter);
  }

  eTwiLastStatus = eError;
  prvvMstPushResult ();
  prvvMstSetIdle ();
}

// ----------------------------------------------------------------------------
// Envoie une condition de RESTART si une trame concernant le même circuit
// est
// présente dans la pile de contrôle.
// Sinon, envoie une condition de STOP et mets le module maître au repos.
// <<<< ATTENTION >>>> Exécuté sous interruption !
static void
prvvMstRestartOrStop (void) {

  prvvMstPushResult ();

  if (xQueueLength (&xTwiMstCtrlQueue) >= TWI_MASTER_CTRL_LEN) {
    xTwiDeviceAddr xNextDeviceAddr;

    xNextDeviceAddr = prvxMstReadDeviceAddr (&xTwiMstCtrlQueue) & ~TWI_RD;
    if (xNextDeviceAddr == (xTwiMstDeviceAddr & ~TWI_RD)) {

      /* Restart */
      prvvStart ();
      return;
    }
  }

  /* Stop */
  prvvMstSetIdle ();
}

// ----------------------------------------------------------------------------
// Demande le prochain octet à l'esclave
static void
prvvMstAskNextByte (void) {

// Vérification si d'autres octets doivent être lus ?
  if ((xTwiMstDeviceLength - xTwiMstDeviceCounter) == 1) {

    prvvSetNack (); // donnée reçue, demande dernier octet
  } else {

    prvvSetAck ();// donnée reçue, demande octet suivant
  }
}

/* internal public functions ================================================ */

// ----------------------------------------------------------------------------
eTwiStatus
eTwiSend (xTwiFrame * pxFrame) {
  xTwiId xId;
  eTwiStatus eStatus;

  NONATOMIC_BLOCK (NONATOMIC_RESTORESTATE) {

    do {

      xId = xTwiTrySend (pxFrame);
    } while (xId == 0);

    do {

      eStatus = eTwiGetStatus (xId, &pxFrame->xLen);
    } while (eStatus == TWI_STATUS_BUSY);

  }

  return eStatus;
}

// ----------------------------------------------------------------------------
eTwiStatus
eTwiReceive (xTwiFrame * pxFrame) {
  xTwiId xId;
  eTwiStatus eStatus;

  NONATOMIC_BLOCK (NONATOMIC_RESTORESTATE) {

    do {

      xId = xTwiTryReceive (pxFrame);
    } while (xId == 0);

    do {

      eStatus = eTwiGetStatus (xId, &pxFrame->xLen);
    } while (eStatus == TWI_STATUS_BUSY);

    if (eStatus == TWI_SUCCESS) {

      eStatus = eTwiGetRxFrame (pxFrame);
    }
  }

  return eStatus;
}

// ----------------------------------------------------------------------------
xTwiId
xTwiTrySend (const xTwiFrame * pxFrame) {
  xTwiId xId = 0;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    if ((xQueueFree (&xTwiMstCtrlQueue) >= TWI_MASTER_CTRL_LEN) &&
        (xQueueFree (&xTwiMstTxQueue) >= pxFrame->xLen)) {

      xId = prvxMstNewId ();
      prvvMstPushId (&xTwiMstCtrlQueue, xId);
      prvvMstPushDeviceAddr (&xTwiMstCtrlQueue, pxFrame->xAddr & ~TWI_RD);
      prvvMstPushLength (&xTwiMstCtrlQueue, pxFrame->xLen);
      (void) xQueuePushBytes (&xTwiMstTxQueue, pxFrame->pxData, pxFrame->xLen);

    }
  }

  if (xId) {

    prvvStart ();
  }
  return xId;
}

// ----------------------------------------------------------------------------
xTwiId
xTwiTryReceive (xTwiFrame * pxFrame) {
  xTwiId xId = 0;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    if (xQueueFree (&xTwiMstCtrlQueue) >= TWI_MASTER_CTRL_LEN) {

      xId = prvxMstNewId ();
      prvvMstPushId (&xTwiMstCtrlQueue, xId);
      prvvMstPushDeviceAddr (&xTwiMstCtrlQueue, pxFrame->xAddr | TWI_RD);
      prvvMstPushLength (&xTwiMstCtrlQueue, pxFrame->xLen);
    }
  }

  if (xId) {

    prvvStart ();
  }
  return xId;
}

 // ----------------------------------------------------------------------------
eTwiStatus
eTwiRead (xTwiDeviceAddr xDeviceAddr, uint8_t * pucByte) {
  xTwiFrame xFrame;

  xFrame.xLen = 1;
  xFrame.xAddr = xDeviceAddr;
  xFrame.pxData = pucByte;
  return eTwiReceive (&xFrame);
}

 // ----------------------------------------------------------------------------
eTwiStatus
eTwiWrite (xTwiDeviceAddr xDeviceAddr, uint8_t ucByte) {
  xTwiFrame xFrame;

  xFrame.xLen = 1;
  xFrame.xAddr = xDeviceAddr;
  xFrame.pxData = &ucByte;
  return eTwiSend (&xFrame);
}

// ----------------------------------------------------------------------------
eTwiStatus
eTwiGetStatus (xTwiId xFrameId, xTwiLength * pxLength) {
  eTwiStatus eStatus = TWI_STATUS_BUSY;
  xTwiLength xDeviceLength = 0;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    if (xQueueLength (&xTwiMstStatusQueue) >= TWI_MASTER_STAT_LEN) {

      if (ucQueueRead (&xTwiMstStatusQueue, 0) == xFrameId) {

        prvvMstDropId (&xTwiMstStatusQueue);
        eStatus = prveMstPullStatus (&xTwiMstStatusQueue);
        xDeviceLength = prvxMstPullLength (&xTwiMstStatusQueue);
      }
    }
  }

  if (pxLength) {

    *pxLength = xDeviceLength;
  }

  return eStatus;
}

// ----------------------------------------------------------------------------
eTwiStatus
eTwiGetRxFrame (xTwiFrame * pxFrame) {
  eTwiStatus eStatus = TWI_SUCCESS;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    if (xQueueLength (&xTwiMstRxQueue) >= pxFrame->xLen) {

      pxFrame->xLen = xQueuePullBytes (&xTwiMstRxQueue, pxFrame->pxData,
                                                          pxFrame->xLen);
    } else {

      eStatus = TWI_ERROR_INVALID_LENGTH;
    }
  }

  return eStatus;
}

#if 0
/* TODO */
/*=============================================================================
 |                          Mémoires I2C                                      |
 =============================================================================*/
// ----------------------------------------------------------------------------
eTwiStatus
eTwiMem8Write (uint8_t ucMemAddr, xTwiFrame * pxFrame) {
  eTwiStatus eStatus = TWI_SUCCESS;

  return eStatus;
}

// ----------------------------------------------------------------------------
eTwiStatus
eTwiMem8Read (uint8_t ucMemAddr, xTwiFrame * pxFrame) {
  eTwiStatus eStatus = TWI_SUCCESS;

  return eStatus;
}
// ----------------------------------------------------------------------------
eTwiStatus
eTwiMem16Write (uint16_t usMemAddr, xTwiFrame * pxFrame) {
  eTwiStatus eStatus = TWI_SUCCESS;

  return eStatus;
}

// ----------------------------------------------------------------------------
eTwiStatus
eTwiMem16Read (uint16_t usMemAddr, xTwiFrame * pxFrame) {
  eTwiStatus eStatus = TWI_SUCCESS;

  return eStatus;
}

// ----------------------------------------------------------------------------
eTwiStatus
eTwiSendReceive (xTwiFrame * pxTxFrame, xTwiFrame * pxRxFrame) {
  xTwiId xId;
  eTwiStatus eStatus;

  NONATOMIC_BLOCK (NONATOMIC_RESTORESTATE) {

    do {

      xId = xTwiTrySendReceive (pxTxFrame, pxRxFrame);
    } while (xId == 0);

    do {

      eStatus = eTwiGetStatus (xId, &pxTxFrame->xLen);
    } while (eStatus == TWI_STATUS_BUSY);

    if (eStatus == TWI_SUCCESS) {

      xId++;
      if (xId <= 0) {

        xId = 1;
      }
      do {

        eStatus = eTwiGetStatus (xId, &pxRxFrame->xLen);
      } while (eStatus == TWI_STATUS_BUSY);

      if (eStatus == TWI_SUCCESS) {

        eStatus = eTwiGetRxFrame (pxRxFrame);
      }
    }
  }

  return eStatus;
}

// ----------------------------------------------------------------------------
xTwiId
xTwiTrySendReceive (const xTwiFrame * pxTxFrame, xTwiFrame * pxRxFrame) {

  xTwiId xId = 0;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    if ((xQueueFree (&xTwiMstCtrlQueue) >= (TWI_MASTER_CTRL_LEN * 2)) &&
        (xQueueFree (&xTwiMstTxQueue) >= pxTxFrame->xLen)) {

      xId = prvxMstNewId ();
      prvvMstPushId (&xTwiMstCtrlQueue, xId);
      prvvMstPushDeviceAddr (&xTwiMstCtrlQueue, pxTxFrame->xAddr & ~TWI_RD);
      prvvMstPushLength (&xTwiMstCtrlQueue, pxTxFrame->xLen);
      (void) xQueuePushBytes (&xTwiMstTxQueue, pxTxFrame->pxData,
                                                          pxTxFrame->xLen);

      prvvMstPushId (&xTwiMstCtrlQueue, prvxMstNewId ());
      prvvMstPushDeviceAddr (&xTwiMstCtrlQueue, pxRxFrame->xAddr | TWI_RD);
      prvvMstPushLength (&xTwiMstCtrlQueue, pxRxFrame->xLen);

    }
  }

  if (xId) {

    prvvStart ();
  }
  return xId;
}
#endif /* 0 TODO */

#  else
      /* TWI_MASTER_ENABLE not defined */
// -----------------------------------------------------------------------------
#    define prvvMstInit()

// -----------------------------------------------------------------------------
#  endif
       /* TWI_MASTER_ENABLE defined */

#  ifdef TWI_SLAVE_ENABLE
/*----------------------------Mode Esclave------------------------------------*/
static eTwiStatus eTwiSlvStatus = TWI_STATUS_LCALL;

QUEUE_STATIC_DECLARE (xTwiSlvTxQueue, TWI_SLAVE_TXBUFSIZE);
QUEUE_STATIC_DECLARE (xTwiSlvRxQueue, TWI_SLAVE_RXBUFSIZE);

/* private functions ======================================================== */

// ----------------------------------------------------------------------------
void
prvvSlvInit (void) {

  vQueueFlush (&xTwiSlvTxQueue);
  vQueueFlush (&xTwiSlvRxQueue);
}

// ----------------------------------------------------------------------------
// Ecrit le prochain octet dans le registre de transmission si la pile n'est
// pas vide et valide le bit ACK.
// Invalide ACK si la pile est vide
// Mode slave seulement
static void
prvvSlvSendNextByte (void) {

  if (xQueueIsEmpty (&xTwiSlvTxQueue)) {

    TWDR = 0xFF;
    prvvSetNack (); // Dernier octet émis !
  } else {

    TWDR = ucQueuePull (&xTwiSlvTxQueue);
    prvvSetAck ();
  }
}

// --------------------------------------------------------------------------
static void
prvvSlvSetErrorAndStop (eTwiStatus eError) {

  eTwiLastStatus = eError;
  eTwiCurrentState = TWI_STATE_READY;
  prvvStop ();
}

/* internal public functions ================================================ */

#  else
      /* TWI_SLAVE_ENABLE not defined */
// -----------------------------------------------------------------------------
#    define prvvSlvInit()

// -----------------------------------------------------------------------------
#  endif
       /* TWI_SLAVE_ENABLE defined */

/*---------------------------Partie Commune-----------------------------------*/

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Wrapper vers la fonction d'erreur appropriée au mode maître ou esclave
static inline void
prvvSetErrorAndStop (eTwiStatus eError) {
#  if    defined(TWI_MASTER_ENABLE) && !defined(TWI_SLAVE_ENABLE)
  prvvMstSetErrorAndStop (eError);
#  elif !defined(TWI_MASTER_ENABLE) &&  defined(TWI_SLAVE_ENABLE)
  prvvSlvSetErrorAndStop (eError);
#  else
      /* TWI_SLAVE_ENABLE and TWI_MASTER_ENABLE defined */
  if (eTwiCurrentState & (TWI_STATE_SLAVE_TX | TWI_STATE_SLAVE_RX)) {

    prvvSlvSetErrorAndStop (eError);
  } else {

    prvvMstSetErrorAndStop (eError);
  }
#  endif
       /* TWI_SLAVE_ENABLE and TWI_MASTER_ENABLE defined */
}

// -----------------------------------------------------------------------------
// Routine d'interruption
ISR (TWI_vect, ISR_BLOCK) {
  uint8_t ucStatus = TW_STATUS; /* Lecture des bits d'états */

  switch (ucStatus) {

#  ifdef TWI_MASTER_ENABLE
/*-----------------------------Mode Maître------------------------------------*/
      /* ==================================================================== */
      /* Master General */
      /* ==================================================================== */

      /* ->> 0x10: Envoi une condition de START répétée <<------------------- */
    case TW_REP_START:
      /* ->> 0x08: Envoi la condition de START <<---------------------------- */
    case TW_START:
      if ((eTwiCurrentState & (TWI_STATE_MASTER_SLA_NACK |
                               TWI_STATE_MASTER_ARB_LOST)) == 0) {

        prvvMstNextFrame ();
      }

      /* Envoi de l'adresse destination avec la bit RW */
      eTwiLastStatus = TWI_SUCCESS;
      prvvMstSendByte (xTwiMstDeviceAddr);
      break;

      /* ->> 0x48: L'adresse esclave n'a pas été reconnue <<----------------- */
    case TW_MR_SLA_NACK:
      /* ->> 0x20: L'adresse esclave n'a pas été reconnue <<----------------- */
    case TW_MT_SLA_NACK:
    // Le circuit adressé n'a pas répondu ...
      if (iTwiMstAddrNackCount > 0) {

        /**
         * @todo TW_MR_SLA_NACK ou TW_MT_SLA_NACK : rechercher l'origine de ce
         *       problème en mode esclave.
         */
      // ReStart -> nouvelle tentative !
        iTwiMstAddrNackCount--;
        eTwiCurrentState |= TWI_STATE_MASTER_SLA_NACK;
        prvvStart ();
      } else {

      // Envoi d'un stop et retour ready
        prvvMstSetErrorAndStop (TWI_ERROR_ADDR_NACK);
      }
      break;

      /* ==================================================================== */
      /* Master Transmitter status codes */
      /* ==================================================================== */

      /* ->> 0x18: Adresse reconnue par l'esclave <<------------------------- */
    case TW_MT_SLA_ACK:
      eTwiCurrentState &= ~(TWI_STATE_MASTER_SLA_NACK |
                            TWI_STATE_MASTER_ARB_LOST);
    // Pas de break, c'est normal !

      /* ->> 0x28: Donnée acceptée par l'esclace <<-------------------------- */
    case TW_MT_DATA_ACK:
      if (xQueueIsEmpty (&xTwiMstTxQueue)) {

      // Succès ! La pile est vide, tous les octets ont été transmis
        prvvMstRestartOrStop ();
      } else {

      // Envoi octet suivant
        prvvMstSendByte (ucQueuePull (&xTwiMstTxQueue));
        xTwiMstDeviceCounter++;
      }
      break;

      /* ->> 0x30: Data not acknowledged <<---------------------------------- */
    case TW_MT_DATA_NACK:
    //
      if (xQueueIsEmpty (&xTwiMstTxQueue)) {

      // Succès ! Tous les octets ont pu être envoyés...
        prvvMstRestartOrStop ();
      } else {

      // Erreur ! Tous les octets n'ont pas pu être envoyés...
        prvvMstSetErrorAndStop (TWI_ERROR_DATA_NACK);
      }
      break;

      /* ->> 0x38: Bus arbitration lost <<----------------------------------- */
    case TW_MT_ARB_LOST:
      prvvMstReleaseBus (); // libère le bus
      eTwiLastStatus = TWI_STATUS_ARB_LOST;
      eTwiCurrentState |= TWI_STATE_MASTER_ARB_LOST;
      prvvStart (); // Envoi un START dès que le bus se libère
      break;

      /* ==================================================================== */
      /* Master Receiver status codes */
      /* ==================================================================== */
      /* ->> 0x50: Donnée reçue de l'esclave <<------------------------------ */
    case TW_MR_DATA_ACK:
      vQueuePush (&xTwiMstRxQueue, TWDR); // stocke octet reçu
      xTwiMstDeviceCounter++;
      prvvMstAskNextByte ();
      break;

      /* ->> 0x40: Adresse reconnue par l'esclave <<------------------------- */
    case TW_MR_SLA_ACK:
      eTwiCurrentState &=
         ~(TWI_STATE_MASTER_SLA_NACK | TWI_STATE_MASTER_ARB_LOST);
      prvvMstAskNextByte ();
      break;

      /* ->> 0x58: Donnée reçue, NACK émis <<-------------------------------- */
    case TW_MR_DATA_NACK:
      vQueuePush (&xTwiMstRxQueue, TWDR); // stocke le dernier octet
      xTwiMstDeviceCounter++;
      prvvMstRestartOrStop ();
      break;

    // -------------------------------------------------------------------------
#  endif
       /* TWI_MASTER_ENABLE defined */

#  ifdef TWI_SLAVE_ENABLE
/*----------------------------Mode Esclave------------------------------------*/
      /* ==================================================================== */
      /* Slave Receiver status codes */
      /* ==================================================================== */

      /* ->> 0x70: GCA+W has been received, ACK has been returned <<--------- */
    case TW_SR_GCALL_ACK:
      // pas de break, c'est normal !

      /* ->> 0x78: GCA+W has been received, ACK has been returned <<--------- */
    case TW_SR_ARB_LOST_GCALL_ACK:
      /*
       * Réception d'un appel général
       */
      eTwiSlvStatus = TWI_STATUS_GCALL;
      // pas de break, c'est normal ! continue en passant en réception

      /* ->> 0x60: own SLA+W has been received, ACK has been returned <<----- */
    case TW_SR_SLA_ACK:
      // pas de break, c'est normal !

      /* ->> 0x68: own SLA+W has been received, ACK has been returned <<----- */
    case TW_SR_ARB_LOST_SLA_ACK:
      /*
       * Réception d'un appel local
       * Nous sommes adressé comme esclave en écriture, des données vont
       * arriver du maître
       */
      eTwiCurrentState = TWI_STATE_SLAVE_RX;
      vQueueFlush (&xTwiSlvRxQueue);
      TWI_LED_TOGGLE (LED_LED1);
      prvvSetAck ();
      break;

      /* ->> 0x90: data byte has been received, ACK has been returned <<----- */
    case TW_SR_GCALL_DATA_ACK:
      // pas de break, c'est normal !

      /* ->> 0x80: data byte has been received, ACK has been returned <<----- */
    case TW_SR_DATA_ACK:
      // Réception des données suite à un adressage local ou global
      if (!xQueueIsFull (&xTwiSlvRxQueue)) {

        vQueuePush (&xTwiSlvRxQueue, TWDR); /* stocke octet reçu */

        if (xQueueFree (&xTwiSlvRxQueue) <= 1) {

          // Il ne reste plus qu'un octet de libre dans la pile de réception
          prvvSetNack ();
        }
        else {

          prvvSetAck ();
        }
      }
      else {

        prvvSetNack ();
      }
      TWI_LED_TOGGLE (LED_LED2);
      break;

      /* ->> 0x98: data byte has been received, NACK has been returned <<---- */
    case TW_SR_GCALL_DATA_NACK:
      // pas de break, c'est normal !

      /* ->> 0x88: data byte has been received, NACK has been returned <<---- */
    case TW_SR_DATA_NACK:
      if (!xQueueIsFull (&xTwiSlvRxQueue)) {
        vQueuePush (&xTwiSlvRxQueue, TWDR); /* stocke dernier octet */
      }
      /* Réception terminée, appel du gestionnaire */
      eTwiLastStatus = eTwiSlaveRxCB (&xTwiSlvRxQueue, eTwiSlvStatus);
      prvvSetAck ();
      TWI_LED_TOGGLE (LED_LED3);
      break;

      /* ->> 0xA0: STOP or REPEATED START has been received while addressed
       * as slave <<----------------------------------------------------------*/
    case TW_SR_STOP:
      /* Réception terminée, appel du gestionnaire */
      prvvSetAck ();
      eTwiLastStatus = eTwiSlaveRxCB (&xTwiSlvRxQueue, eTwiSlvStatus);
      eTwiSlvStatus = TWI_STATUS_LCALL;
      eTwiCurrentState = TWI_STATE_READY;
      TWI_LED_TOGGLE (LED_LED4);
      break;

      /* ==================================================================== */
      /* Slave Transmitter */
      /* ==================================================================== */

      /* ->> 0xA8: own SLA+R has been received, ACK has been returned <<----- */
    case TW_ST_SLA_ACK:
      // pas de break, c'est normal !

      /* ->> 0xB0: GCA+R has been received, ACK has been returned <<--------- */
    case TW_ST_ARB_LOST_SLA_ACK:
      /*
       * Réception d'un appel local
       * Nous sommes adressé comme esclave en lecture, nous devons envoyer des
       * données au maître.
       */
      eTwiCurrentState = TWI_STATE_SLAVE_TX;

      /* Demande des données à envoyer au gestionnaire utilisateur */
      while (xQueueIsEmpty (&xTwiSlvTxQueue)) {

        eTwiLastStatus = eTwiSlaveTxCB (&xTwiSlvTxQueue, TWI_STATUS_LCALL);
        if (eTwiLastStatus != TWI_SUCCESS)
          break;
      }
      prvvSlvSendNextByte ();
      TWI_LED_TOGGLE (LED_LED6);
      break;

      /* ->> 0xB8: data byte has been transmitted, ACK has been received <<-- */
    case TW_ST_DATA_ACK:
      while (xQueueIsEmpty (&xTwiSlvTxQueue)) {

        eTwiLastStatus =
           eTwiSlaveTxCB (&xTwiSlvTxQueue, TWI_STATUS_TXBUFFER_EMPTY);
        if (eTwiLastStatus != TWI_SUCCESS)
          break;
      }
      prvvSlvSendNextByte ();
      TWI_LED_TOGGLE (LED_LED7);
      break;

      /* ->> 0xC0: data byte has been transmitted, NACK has been received <<- */
    case TW_ST_DATA_NACK:
      /* ->> 0xC8: <<--------------------------------------------------       */
      // pas de break, c'est normal !

    case TW_ST_LAST_DATA:
    // Tout a été transmis
      eTwiCurrentState = TWI_STATE_READY;
      prvvSetAck ();
      TWI_LED_TOGGLE (LED_LED8);
      break;
    // -------------------------------------------------------------------------
#  endif  /* TWI_SLAVE_ENABLE defined */

      /* ==================================================================== */
      /*                            Divers                                    */
      /* ==================================================================== */
      /* ->> 0xF8: No relevant state information <<-------------------------- */
    case TW_NO_INFO:
      // Ne fait rien
      break;

      /* ->> 0x00: Bus error due to illegal start or stop condition <<------- */
    case TW_BUS_ERROR:
      /* Reset de l'interface matérielle et libération du bus */
      prvvSetErrorAndStop (TWI_ERROR_ILLEGAL_START_STOP);
      break;

      /* ->> Mode non pris en charge <<-------------------------------------- */
    default:
      prvvSetErrorAndStop (TWI_ERROR_ILLEGAL_CODE);
      break;
  }
}

/* internal public functions ================================================ */

// ------------------------------------------------------------------------------
void
vTwiInit (void) {

  TWCR = 0;
  prvvMstInit ();
  prvvSlvInit ();

  eTwiCurrentState = TWI_STATE_READY;
  eTwiLastStatus = TWI_SUCCESS;
  TWBR = 0;
  TWAR = 0;
  TWCR = _BV (TWINT) | _BV (TWEN) | _BV (TWIE) | _BV (TWEA);
  vTwiEnablePullup ();
  sei ();
}

// -----------------------------------------------------------------------------
bool
bTwiIsBusy (void) {

  return eTwiCurrentState != TWI_STATE_READY;
}

#endif /* AVRIO_TWI_ENABLE defined */
/* ========================================================================== */
