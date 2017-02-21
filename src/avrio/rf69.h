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
#include <limits.h>

/**
 *  @defgroup sysio_rfm69 Module RFM69
 *  @{
 */
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
 * 
 * Classe opaque pour l'utilisateur
 */
struct xRf69;

/* types ==================================================================== */
/**
 * @typedef xRf69
 * @brief Classe permettant de manipuler une liaison RFM69
 * 
 * Classe opaque pour l'utilisateur
 */
typedef struct xRf69 xRf69;

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
 * @brief Vérifie si on peut émettre
 * Si oui, passe en mode StandBy
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return true, false, valeur négative si erreur
 */
int iRf69CanSend (xRf69 * rf);

/**
 * @brief Transmission d'un paquet
 *
 * @param rf pointeur sur l'objet xRf69
 * @param toAddress destinataire ou RF69_BROADCAST_ADDR
 * @param tx_buffer données à transmettre
 * @param tx_len nombre d'octets des données
 * @param bRequestACK demande d'ACK
 * @return 0, valeur négative si erreur
 */
int iRf69Send (xRf69 * rf, uint8_t toAddress, const void* tx_buffer, uint8_t tx_len, bool bRequestACK);

/**
 * @brief Vérifie la réception d'un ACK
 * 
 * Vérifie la réception d'un ACK en réponse à l'envoi d'un paquet avec
 * demande d'ACK. \n
 * Devrait être scruté immédiatement après l'envoi d'un paquet avec
 * demande d'ACK. 
 * 
 * @param rf pointeur sur l'objet xRf69
 * @param fromNodeId Adresse du noeud dont on attend la réponse
 * @return true, false, valeur négative si erreur
 */
int iRf69AckReceived (xRf69 * rf, uint8_t fromNodeId);

/**
 * @brief Envoi d'un paquet avec répétition
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
 * @brief Vérifie si un paquet a été reçu
 * 
 * Si non, active la réception, si oui passe en mode StandBy.
 *
 * @param rf pointeur sur l'objet xRf69
 * @return true, false, valeur négative si erreur
 */
int iRf69ReceiveDone (xRf69 * rf);

/**
 * @brief Nombre d'octets du dernier message reçu
 * 
 * Les données sont disponibles jusqu'au prochain appel à \c iRf69ReceiveDone()
 * ou jusqu'à l'envoi d'un paquet.
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return Nombre d'octets de données utiles (sans en-tête)
 */
int iRf69DataLength(const xRf69 * rf);

/**
 * @brief Données utiles (sans en-tête) du dernier message reçu
 * 
 * Les données sont terminées par un octet nul.
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return pointeur sur les données
 */
const char * sRf69Data(const xRf69 * rf);

/**
 * @brief Indique si le dernier paquet reçu nécessite un ACK (paquet non-broadcast)
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return true, false, valeur négative si erreur
 */
int iRf69AckRequested (const xRf69 * rf);

/**
 * @brief Envoi d'un paquet ACK suite à demande
 * 
 * Devrait être appelé immédiatement après réception d'un paquet avec demande d'ACK.
 * 
 * @param rf pointeur sur l'objet xRf69
 * @param tx_buffer données à transmettre dans la réponse (NULL si inutile)
 * @param tx_len nombre d'octets de données (0 si inutile)
 * @return 0, valeur négative si erreur 
 */
int iRf69SendAck (xRf69 * rf, const void * tx_buffer, uint8_t tx_len);

/**
 * @brief Modification de notre identifiant
 * 
 * @param rf pointeur sur l'objet xRf69
 * @param ucNodeId nouvel identifiant
 * @return 0, valeur négative si erreur
 */
int iRf69SetNodeId (xRf69 * rf, uint8_t ucNodeId);

/**
 * @brief Lecture de notre identifiant
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return identifiant, valeur négative si erreur
 */
int iRf69NodeId (const xRf69 * rf);

/**
 * @brief Modification de notre réseau
 * 
 * @param rf pointeur sur l'objet xRf69
 * @param ucNetId identifiant du nouveau réseau
 * @return 0, valeur négative si erreur
 */
int iRf69SetNetworkId (const xRf69 * rf, uint8_t ucNetId);

/**
 * @brief Lecture de notre réseau
 * @param rf pointeur sur l'objet xRf69
 * @return identifiant de notre réseau, valeur négative si erreur
 */
int iRf69NetworkId (const xRf69 * rf);

/**
 * @brief Lecture de la fréquence d'émission/réception
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return La fréquence en Hz, valeur négative si erreur
 */
long lRf69Frequency (const xRf69 * rf);

/**
 * @brief Modification de la fréquence d'émission/réception
 * 
 * @param rf pointeur sur l'objet xRf69
 * @param ulFreq
 * @return 0, valeur négative si erreur
 */
int iRf69SetFrequency (xRf69 * rf, uint32_t ulFreq);

/**
 * @brief Modification du mode haute puissance d'émission
 * 
 * Disponible uniquement sur RFM69HW. Dans ce mode, la puissance d'émission
 * varie entre 5 et 20 dBm, au lieu de -18 à 13 dBm.
 * 
 * @param rf pointeur sur l'objet xRf69
 * @param bOn nouvel état
 * @return 0, valeur négative si erreur
 */
int iRf69SetHighPower (xRf69 * rf, bool bOn);

/**
 * @brief Indique si le mode  haute puissance d'émission est activé
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return true, false, valeur négative si erreur
 */
bool bRf69isHighPower (const xRf69 * rf);

/**
 * @brief Modification de la puissance d'émission
 * 
 * reduce/increase transmit power level
 * set *transmit/TX* output power: 0=min, 31=max
 * this results in a "weaker" transmitted signal, and directly results in a lower rf->rssi at the receiver
 * the power configurations are explained in the SX1231H datasheet (Table 10 on p21; RegPaLevel p66): http://www.semtech.com/images/datasheet/sx1231h.pdf
 * valid ucLevel parameter values are 0-31 and result in a directly proportional effect on the output/transmission power
 * this function implements 2 modes as follows:
 *       - for RFM69W the range is from 0-31 [-18dBm to 13dBm] (PA0 only on RFIO pin)
 *       - for RFM69HW the range is from 0-31 [5dBm to 20dBm]  (PA1 & PA2 on PA_BOOST pin & high Power PA settings - see section 3.3.7 in datasheet, p22)
 * @param rf pointeur sur l'objet xRf69
 * @param ucLevel nouvelle puissance relative (0 à 31 dBm)
 * @return 0, valeur négative si erreur
 */
int iRf69SetPowerLevel (xRf69 * rf, int level);

/**
 * @brief Lecture de la puissance d'émission
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return puissance relative (0 à 31 dBm)
 */
int iRf69PowerLevel (const xRf69 * rf);

/**
 * @brief Lecture de la température du capteur 8 bits embarqué
 * 
 * @param rf pointeur sur l'objet xRf69
 * @param iCalFactor correction de zéro (suite à étalonnage)
 * @return La température en °C, INT_MAX si erreur
 */
int iRf69Temperature (xRf69 * rf, int iCalFactor);

/**
 * @brief Lecture du niveau de réception
 *
 * Cette fonction retourne le niveau de réception en dBm
 *
 * @param rf pointeur sur l'objet xRf69
 * @param bForceTrigger force le module à effectuer la mesure, si false, c'est
 * le niveau de réception correspondant à la dernière réception qui est lu.
 * @return Le niveau de réception en dBm, INT_MAX si erreur
 */
int iRf69Rssi (const xRf69 * rf, bool bForceTrigger);

/**
 * @brief Modification de la clé d'encryptage
 * To enable encryption: radio.vRf69SetEncryptKey("ABCDEFGHIJKLMNOP");
 * To disable encryption: radio.vRf69SetEncryptKey(null) or radio.vRf69SetEncryptKey(0)
 *
 * @param rf pointeur sur l'objet xRf69
 * @param sKey clé de cryptage doit avoir 16 caractères de long
 * @return 0, valeur négative si erreur
 */
int iRf69SetEncryptKey (xRf69 * rf, const char * sKey);

/**
 * @brief Indique si la transmission est cryptée
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return true, false, valeur négative si erreur
 */
int iRf69isEncrypted (const xRf69 * rf);

/**
 * @brief Modification du mode promiscuité
 *
 * @param rf pointeur sur l'objet xRf69
 * @param bOn false réception des seuls paquets correspondant à notre adresse ou
 * au broadcast, true réception de tous les paquets.
 * @return 0, valeur négative si erreur
 */
int iRf69SetPromiscuous (xRf69 * rf, bool bOn);

/**
 * @brief Lecture du mode promiscuité
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return true, false, valeur négative si erreur
 */
bool bRf69isPromiscuous (const xRf69 * rf);

/**
 * @brief Passage en mode sommeil
 * 
 * Afin d'économiser de l'énergie, un appel à iRf69ReceiveDone() réveille le
 * module.
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return 0, valeur négative si erreur
 */
int iRf69Sleep (xRf69 * rf);

/**
 * @brief Calibrage de l'oscilateur RC interne
 * 
 * Cet oscillateur est calivré automatiquement au démarrage de l'alimentation,
 * Cette fonction est utile pour une utilisation sur une plage de température 
 * plus large ou si le module est démarré sur une longue période (pour corriger
 * le glissement).
 * 
 * @param rf pointeur sur l'objet xRf69
 * @return 0, valeur négative si erreur
 */
int iRf69RcCalibration (const xRf69 * rf);

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
