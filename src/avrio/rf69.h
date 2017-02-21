/**
 * @file
 * @brief
 *
 * Copyright © 2017 epsilonRT, All rights reserved.
 * This software is governed by the CeCILL license <http://www.cecill.info>
 */
#ifndef _AVRIO_RF69_H_
#define _AVRIO_RF69_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ========================================================================== */
#include <stdarg.h>

/**
 *  @defgroup sysio_rfm69 Module RFM69
 *  @{
 */
/* macros =================================================================== */
/* constants ================================================================ */
/**
 * @brief Adresse d'appel général (broadcast)
 */
#define RF69_BROADCAST_ADDR 255

/**
 * @enum eRf69Band
 * @brief Bande de fréquence
 */
typedef enum {
  eRf69Band315Mhz = 31, /**< non trivial values to avoid misconfiguration */
  eRf69Band433Mhz = 43,
  eRf69Band868Mhz = 86,
  eRf69Band915Mhz = 91,
  eRf69BandUnknown = -1
} eRf69Band;

/**
 * @enum eRf69Mode
 * @brief Mode de fonctionnement
 */
typedef enum {
  eRf69ModeSleep   =      0, /**< XTAL OFF */
  eRf69ModeStandby =      1, /**< XTAL ON */
  eRf69ModeSynth   =      2, /**< PLL ON */
  eRf69ModeRx      =      3, /**< RX MODE */
  eRf69ModeTx      =      4  /**< TX MODE */
} eRf69Mode;

/* structures =============================================================== */
/**
 * @class xRf69
 * @brief Classe permettant de manipuler une liaison RFM69
 * Classe opaque pour l'utilisateur
 */
struct xRf69;

/* types ==================================================================== */
typedef struct xRf69 xRf69;

/* private variables ======================================================== */
/* private functions ======================================================== */

/* internal public functions ================================================ */

/**
 * @brief Création d'un objet Rfm69
 *
 * @param iBus
 * @param iDevice
 * @param iIrqPin
 * @return pointeur sur l'objet, NULL si erreur
 */
xRf69 * xRf69New (int iBus, int iDevice, int iIrqPin);

/**
 * @brief Destruction d'une liaison LiveEz
 * @param rf pointeur sur l'objet xRf69
 */
void vRf69Delete (xRf69 * rf);

/**
 * @brief
 * @param rf pointeur sur l'objet xRf69
 * @param eBand
 * @param ucId
 * @param ucNetId
 * @return 0, valeur négative si erreur
 */
int iRf69Open (xRf69 * rf, eRf69Band eBand, uint8_t ucId, uint8_t ucNetId);

/**
 * @brief Close the input-output layer.
 * @param rf pointeur sur l'objet xRf69
 * @return 0, valeur négative si erreur
 */
int iRf69Close (xRf69 * rf);


/**
 * @brief
 *
 * @param rf pointeur sur l'objet xRf69
 * @return
 */
int iRf69CanSend (xRf69 * rf);

/**
 * @brief
 *
 * @param rf pointeur sur l'objet xRf69
 * @param toAddress
 * @param tx_buffer
 * @param tx_len
 * @param bRequestACK
 * @return 0, valeur négative si erreur
 */
int iRf69Send (xRf69 * rf, uint8_t toAddress, const void* tx_buffer, uint8_t tx_len, bool bRequestACK);

/**
 * @brief
 * should be polled immediately after sending a packet with ACK request
 * @param rf pointeur sur l'objet xRf69
 * @param fromNodeId
 * @return
 */
int iRf69AckReceived (xRf69 * rf, uint8_t fromNodeId);

/**
 * @brief 
 * 
 * to increase the chance of getting a packet across, call this function instead of send
 * and it handles all the ACK requesting/retrying for you :)
 * The only twist is that you have to manually listen to ACK requests on the other side and send back the ACKs
 * The reason for the semi-automaton is that the lib is interrupt driven and
 * requires user action to read the received data and decide what to do with it
 * replies usually take only 5..8ms at 50kbps@915MHz
 * 
 * @param rf pointeur sur l'objet xRf69
 * @param toAddress
 * @param tx_buffer
 * @param tx_len
 * @param retries
 * @param retryWaitTime
 * @return 
 */
int iRf69SendWithRetry (xRf69 * rf, uint8_t toAddress,
                        const void * tx_buffer, uint8_t tx_len,
                        uint8_t retries, int retryWaitTime);
/**
 * @brief checks if a packet was received and/or puts transceiver in receive mode
 *
 * @param rf pointeur sur l'objet xRf69
 * @return
 */
int iRf69ReceiveDone (xRf69 * rf);

/**
 * @brief check whether an ACK was requested in the last received packet (non-broadcasted packet)
 * @param rf pointeur sur l'objet xRf69
 * @return
 */
int iRf69AckRequested (xRf69 * rf);

/**
 * @brief 
 * should be called immediately after reception in case sender wants ACK
 * @param rf
 * @param tx_buffer
 * @param tx_len
 * @return 
 */
int iRf69SendAck (xRf69 * rf, const void * tx_buffer, uint8_t tx_len);

/**
 * @brief
 * @param rf pointeur sur l'objet xRf69
 * @param ucNodeId
 * @return 0, valeur négative si erreur
 */
int iRf69SetNodeId (xRf69 * rf, uint8_t ucNodeId);

int iRf69DataLength(xRf69 * rf);

const char * sRf69Data(xRf69 * rf);

/**
 * @brief 
 * @param rf
 * @return 
 */
int iRf69NodeId (xRf69 * rf);

/**
 * @brief
 * @param rf pointeur sur l'objet xRf69
 * @param ucNetId
 * @return 0, valeur négative si erreur
 */
int iRf69SetNetworkId (xRf69 * rf, uint8_t ucNetId);

/**
 * @brief 
 * @param rf
 * @return 
 */
int iRf69NetworkId (xRf69 * rf);

/**
 * @brief
 * @param rf pointeur sur l'objet xRf69
 * @return La fréquence en Hz, valeur négative si erreur
 */
long lRf69Frequency (xRf69 * rf);

/**
 * @brief
 * @param rf pointeur sur l'objet xRf69
 * @param ulFreq
 * @return 0, valeur négative si erreur
 */
int iRf69SetFrequency (xRf69 * rf, uint32_t ulFreq);

/**
 * @brief has to be called after iRf69Open() for RFM69HW
 * @param rf pointeur sur l'objet xRf69
 * @param bOn
 * @return 0, valeur négative si erreur
 */
int iRf69SetHighPower (xRf69 * rf, bool bOn);

/**
 * @brief
 * @param rf pointeur sur l'objet xRf69
 * @return
 */
bool bRf69isHighPower (xRf69 * rf);

/**
 * @brief reduce/increase transmit power level
 * @param rf pointeur sur l'objet xRf69
 * @param ucLevel
 * @return 0, valeur négative si erreur
 */
int iRf69SetPowerLevel (xRf69 * rf, uint8_t ucLevel);

/**
 * @brief
 * @param rf pointeur sur l'objet xRf69
 * @return
 */
int iRf69PowerLevel (xRf69 * rf);

/**
 * @brief get CMOS temperature (8bit)
 * @param rf pointeur sur l'objet xRf69
 * @param ucCalFactor
 * @return La température en °C, valeur négative si erreur
 */
int iRf69Temperature (xRf69 * rf, uint8_t ucCalFactor);

/**
 * @brief Lecture du niveau de réception
 *
 * Cette fonction retourne le niveau de réception en -dBm, par exemple, si
 * cette fonction retourne 90, cela correspond à un niveau de réception de
 * -90dBm. Donc plus la valeur retournée est forte, plus le signal est faible...
 *
 * @param rf pointeur sur l'objet xRf69
 * @param bForceTrigger force le module à effectuer la mesure, si false, c'est
 * le niveau de réception correspondant à la dernière réception qui est lu.
 * @return Le niveau de réception en -dBm, valeur négative si erreur
 */
int iRf69Rssi (xRf69 * rf, bool bForceTrigger);

/**
 * @brief Modification de la clé d'encryptage
 * To enable encryption: radio.vRf69SetEncryptKey("ABCDEFGHIJKLMNOP");
 * To disable encryption: radio.vRf69SetEncryptKey(null) or radio.vRf69SetEncryptKey(0)
 *
 * @param rf pointeur sur l'objet xRf69
 * @param sKey KEY HAS TO BE 16 bytes !!!
 * @return 0, valeur négative si erreur
 */
int iRf69SetEncryptKey (xRf69 * rf, const char * sKey);

/**
 * @brief disable filtering to capture all frames on network
 *
 * @param rf pointeur sur l'objet xRf69
 * @param bOn false = enable node/broadcast filtering to capture only frames
 * sent to this/broadcast address
 * @return 0, valeur négative si erreur
 */
int iRf69SetPromiscuous (xRf69 * rf, bool bOn);

/**
 * @brief
 * @param rf pointeur sur l'objet xRf69
 * @return
 */
bool bRf69isPromiscuous (const xRf69 * rf);

/**
 * @brief put transceiver in sleep mode to save battery
 * to wake or resume receiving just call iRf69ReceiveDone()
 * @param rf pointeur sur l'objet xRf69
 * @return 0, valeur négative si erreur
 */
int iRf69Sleep (xRf69 * rf);

/**
 * @brief calibrate the internal RC oscillator
 * for use in wide temperature variations - see datasheet section [4.3.5. RC Timer Accuracy]
 * @param rf pointeur sur l'objet xRf69
 * @return 0, valeur négative si erreur
 */
int iRf69RcCalibration (xRf69 * rf);

// -----------------------------------------------------------------------------
#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/**
 *   @}
 * @}
 */
#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
#endif /* __DOXYGEN__ not defined */

#if 0
// -----------------------------------------------------------------------------


/* internal public functions ================================================ */


/**
 * @brief  Receive a message from the network
 *
 * The call can be blocking, Rf69FuncPoll will be used before so you do not block.
 *
 * @param rf rf layer
 * @param buffer a buffer in which bytes are stored
 * @param count number of bytes requested
 * @param source if not NULL, returns the source address.
 * @return number of bytes read, a negative value if error
 */
int iRf69Recv (xRf69 * rf, void * buffer, int count, Rf69Addr * source);

/**
 * @brief Send a message to the network
 *
 * @param rf rf layer
 * @param buffer buffer where the bytes were stored
 * @param count number of bytes to send
 * @param target if target is NULL or if her broadcast flag is set, the
 * broadcast address of the network is used.
 * @return number of bytes sent or frame identifier (greater than or equal to
 * one), a negative value if error occurs
 */
int iRf69Send (xRf69 * rf, const void * buffer, int count, const Rf69Addr * target);

/**
 * @brief device-specific input/output operations
 *
  Rf69FuncNone = 0,
  Rf69FuncPoll,
  Rf69FuncGetIface,
  Rf69FuncGetBcastAddr,
  Rf69FuncGetNetInfo,
  Rf69FuncGetLocalAddrList,
  Rf69FuncNetAddrToString,
  Rf69FuncNetAddrFromString,
  Rf69FuncError = -1
 * @param rf rf layer
 * @param c device-dependent request code
 * @param ap iterating arguments from Rf69Ctl()
 * @return 0, -1 if error occurs
 */
int iRf69IoCtl (xRf69 * rf, int c, va_list ap);

// -----------------------------------------------------------------------------
#endif

/* ========================================================================== */
__END_C_DECLS
#endif /*_AVRIO_RF69_H_ defined */
