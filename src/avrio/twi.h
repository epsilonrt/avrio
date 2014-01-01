/**
 * @file twi.h
 * @brief Bus I2C
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#ifndef _AVRIO_TWI_H_
#define _AVRIO_TWI_H_

#include <avrio/defs.h>
#include <avrio/queue.h>
#include <avrio/twi_addr.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 *  @addtogroup twi_group
 *  @{
 *  @defgroup twi_module Bus I2C avec coupleur TWI
 *
 *  Ce module utilise le coupleur I2C interne du microcontrôleur et travaille
 *  sous interruption.\n
 *  La macro AVRIO_TWI_ENABLE doit être définie (dans le fichier avrio-config.h 
 *  ou à la compilation), et dans ce cas, le fichier avrio-board-twi.h permet
 *  de configurer le module et de l'adapater à la carte utilisée.\n
 *  Le module dispose de deux modes :
 *  - Le mode maître
 *  - Le mode esclave
 *  .
 *  Les deux modes peuvent être validés ou non dans avrio-board-twi.h à l'aide
 *  des macros TWI_MASTER_ENABLE et TWI_SLAVE_ENABLE. Un mode au moins, doit
 *  être validé, et il est possible de les valider tous les deux dans le cas
 *  d'un environnement multi-maîtres (les confilts sont gérés).\n
 *  L'implémentation sous interruption est conçue pour être utilisée dans 
 *  les routines d'interruption (les portions de code en attente d'interruption
 *  sont protégées du dead-lock).
 *
 *  @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 *  @copyright GNU Lesser General Public License version 3
 *             <http://www.gnu.org/licenses/lgpl.html>
 *  @bug <b>Absence de réponse de l'esclave TW_MR_SLA_NACK ou TW_MT_SLA_NACK</b> : \n
 *  TWI_MASTER_ADDR_NACK_RETRY permet en mode maître, de renvoyer un certain
 *  nombre de demandes à l'esclave qui n'a pas répondu la première fois (cela
 *  ne semble pas arriver avec un PCF8583, mais arrive avec un esclave réalisé
 *  avec ce module: 1 retry nécessaire).\n S'agit'il d'un bug ? ou d'un problème
 *  logique lié à la relation entre la fréquence CPU de l'esclave et la fréquence
 *  du bus imposée par le maître ? \n
 *  A investiguer avec un esclave cadencé à la même fréquence CPU 
 *  (par exemple entre 2 cartes MAVRIX).
 *  @todo Timeout en mode maître ?
 *  @{
 *    @defgroup twiboard_module Configuration du module I2C
 *
 *    Eléments permettant de personnaliser le configuration du module dans 
 *    avrio-board-twi.h communs aux deux modes : maître et esclave.  
 *    @{
 *      @defgroup twimasterboard_module Mode maître 
 *
 *      Eléments de configuration spécifiques au mode maître dans
 *      avrio-board-twi.h
 *  
 *      @defgroup twislaveboard_module Mode esclave
 *
 *      Eléments de configuration spécifiques au mode esclave dans
 *      avrio-board-twi.h
 *    @}
 */
  /* constants ============================================================== */
/**
 * @enum eTwiStatus
 * @brief Codes d'erreur et d'état
 *
 * Les codes d'erreur sont négatifs, les codes d'état sont positifs ou nuls.
 */
   typedef enum {

  TWI_SUCCESS = 0,          /**< Succès */

  /* Codes d'état */
  TWI_STATUS_TX = 1,                /**< Le module est en cours de transmission en mode maître */
  TWI_STATUS_RX = 2,                /**< Le module est en cours de réception en mode maître */
  TWI_STATUS_BUSY = 3,              /**< Module occupé (not ready) */
  TWI_STATUS_LCALL = 5,             /**< Adressé comme esclave par appel local */
  TWI_STATUS_GCALL = 4,             /**< Adressé comme esclave par appel général (lecture uniquement) */
  TWI_STATUS_TXBUFFER_EMPTY = 6,    /**< Le buffer de transmission esclave est vide, le maître demande d'autres données */
  TWI_STATUS_ARB_LOST = 7,          /**< Le contrôle du bus a été perdu suite à un arbitrage */

  /* Codes Erreur */
  TWI_ERROR_DATA_NACK = -1,             /**< L'esclave n'a pas accepté toutes les données envoyées  */
  TWI_ERROR_ADDR_NACK = -2,             /**< L'esclave n'a pas répondu */
  TWI_ERROR_ILLEGAL_START_STOP = -3,    /**< Condition de start ou de stop illégale sur le bus */
  TWI_ERROR_INVALID_SPEED = -4,         /**< Vitesse invalide */
  TWI_ERROR_INVALID_LENGTH = -5,        /**< Longueur de trame invalide */
  TWI_ERROR_BUFFER_FULL = -6,           /**< Le buffer est plein */
  TWI_ERROR_TIMEOUT = -7,               /**< Délai d'attente dépassé */
  TWI_ERROR_ILLEGAL_CODE = -8,          /**< Code illégal */
  TWI_ERROR_INVALID_DEVICE = -9,        /**< Circuit I2C incorrect */

  TWIMEM_ERROR_READ = -10               /**< Erreur de lecture dans une mémoire I2C */
} eTwiStatus;

/* types ==================================================================== */
/**
 * @typedef xTwiDeviceAddr
 * @brief Adresse d'un circuit sur le bus I2C 
 */
typedef uint8_t xTwiDeviceAddr;
#define TWI_DEVICE_ADDR_SIZEOF 1

/**
 * @typedef xTwiLength
 * @brief Longueur d'une trame 
 */
typedef uint8_t xTwiLength;
#define TWI_LENGTH_SIZEOF 1

/**
 * @struct xTwiFrame
 * @brief Trame I2C 
 */
typedef struct xTwiFrame {
  uint8_t * pxData;     /**< Pointeur sur les octets de la trame */
  xTwiLength xLen;      /**< Nombre d'octets de la trame */
  xTwiDeviceAddr xAddr; /**< Adresse I2C du circuit I2C */
} xTwiFrame;
  
/* internal public functions ================================================ */
/**
 * @brief Initialise le module
 *
 * Ni la vitesse de transmission (eTwiSetSpeed()), ni l'adresse esclave locale 
 * (vTwiSetDeviceAddress()) ne sont configurées.
 */
void vTwiInit (void);

/*-----------------------------Mode Maître------------------------------------*/
/**
 *  @defgroup twimaster_module Bus I2C en mode maître
 *
 *  Ce module permet de transmettre ou reçevoir des informations sur le bus
 *  I2C en tant que maître.\n
 *  La macro TWI_MASTER_ENABLE dans avrio-board-twi.h pour valider ce module.\n
 *  Les conflits de bus sont gérés (arbitrage).\n\n
 *  \b Dépendances:\n
 *  - Module \ref queue_module
 *  .
 *  @{
 */

/* types ==================================================================== */
/**
 * @typedef xTwiId
 * @brief Identifiant d'une trame
 */
typedef int8_t xTwiId;
#define TWI_ID_SIZEOF 1

/* internal public functions ================================================ */

/**
 * @brief Emission d'un message en mode maître
 *
 * Cette fonction est bloquante, et ne retourne qu'après avoir transmis la trame
 *  ou en cas d'erreur.\n
 * Un reset du timer chien de garde est effectué dans la boucle d'attente.
 *
 * @param pxFrame Pointeur sur la trame où sont stockés les octets à envoyer.
 * @return TWI_SUCCESS si la trame a pu être transmise, le code erreur sinon.
 */
eTwiStatus eTwiSend (xTwiFrame * pxFrame);

/**
 * @brief Envoi d'un octet en mode maître
 *
 * Cette fonction est bloquante, et ne retourne qu'après avoir transmis l'octet
 *  ou en cas d'erreur.\n
 * Un reset du timer chien de garde est effectué dans la boucle d'attente. \n
 * Cette fonction ne devrait pas être utilisé pour envoyer une suite d'octets
 * car elle déclenche un START et un STOP après chaque envoi 
 * (risque de conflit). Pour envoyer une suite d'octets, il est préférable 
 * d'utiliser eTwiSend().
 *
 * @param xDeviceAddr Adresse I2C du circuit
 * @param ucByte l'octet à envoyer.
 * @return TWI_SUCCESS si la trame a pu être transmise, le code erreur sinon.
 */
eTwiStatus eTwiWrite (xTwiDeviceAddr xDeviceAddr, uint8_t ucByte);

/**
 * @brief Réception d'un message en mode maître
 *
 * Cette fonction est bloquante, et ne retourne qu'après avoir reçue une trame
 *  ou en cas d'erreur.\n
 * Un reset du timer chien de garde est effectué dans la boucle d'attente.
 *
 * @param pxFrame Pointeur sur la trame où seront stockés les octets reçus
 * @return TWI_SUCCESS si une trame a été reçue, un code erreur sinon.
 */
eTwiStatus eTwiReceive (xTwiFrame * pxFrame);

/**
 * @brief Réception d'un octet en mode maître
 *
 * Cette fonction est bloquante, et ne retourne qu'après avoir transmis l'octet
 *  ou en cas d'erreur.\n
 * Un reset du timer chien de garde est effectué dans la boucle d'attente. \n
 * Cette fonction ne devrait pas être utilisé pour recevoir une suite d'octets
 * car elle déclenche un START et un STOP après chaque réception 
 * (risque de conflit). Pour recevoir une suite d'octets, il est préférable 
 * d'utiliser eTwiReceive().
 *
 * @param xDeviceAddr Adresse I2C du circuit
 * @param pucByte Pointeur sur l'octet reçu.
 * @return TWI_SUCCESS si la trame a pu être transmise, le code erreur sinon.
 */
eTwiStatus eTwiRead (xTwiDeviceAddr xDeviceAddr, uint8_t * pucByte);

/**
 * @brief Ecriture dans une mémoire I2C (Adresse mémoire 8 bits)
 *
 * @param ucFisrtByte Adresse dans la mémoire ou octet de commande à
 * envoyer avant l'écriture des octets contenu dans la trame.
 * @param pxFrame Pointeur sur la trame
 * @return TWI_SUCCESS si une trame a été reçue, un code erreur sinon.
 */
eTwiStatus eTwiMem8Write (uint8_t ucFisrtByte, xTwiFrame * pxFrame);

/**
 * @brief Lecture dans une mémoire I2C (Adresse mémoire 8 bits)
 *
 * @param ucFisrtByte Adresse dans la mémoire ou octet de commande à
 * envoyer avant la lecture des octets.
 * @param pxFrame Pointeur sur la trame
 * @return TWI_SUCCESS si une trame a été reçue, un code erreur sinon.
 */
eTwiStatus eTwiMem8Read  (uint8_t ucFisrtByte, xTwiFrame * pxFrame);

/**
 * @brief Ecriture dans une mémoire I2C (Adresse mémoire 16 bits)
 *
 * @param usMemAddr Adresse dans la mémoire
 * @param pxFrame Pointeur sur la trame
 * @return TWI_SUCCESS si une trame a été reçue, un code erreur sinon.
 */
eTwiStatus eTwiMem16Write (uint16_t usMemAddr, xTwiFrame * pxFrame);

/**
 * @brief Lecture dans une mémoire I2C (Adresse mémoire 16 bits)
 *
 * @param usMemAddr Adresse dans la mémoire
 * @param pxFrame Pointeur sur la trame
 * @return TWI_SUCCESS si une trame a été reçue, un code erreur sinon.
 */
eTwiStatus eTwiMem16Read  (uint16_t usMemAddr, xTwiFrame * pxFrame);

/*=============================================================================
 |                    Sous interruption uniquement                            |
 =============================================================================*/

/**
 * @brief Emission non bloquante d'un message en mode maître
 *
 * Cette fonction est non bloquante et l'envoi se fait sous interruption, cela
 * signifie que la fonction retourne alors que toutes données ne sont pas
 * forcément transmises.\n
 * Elle est prévue pour être utilisée dans un contexte multitâche.\n
 * Si cette fonction renvoie l'identifiant d'une trame, il est nécessaire
 * d'appeler réguliérement eTwiGetStatus() tant que celle-ci ne renvoie pas
 * TWI_SUCCESS ou un code erreur.\n
 * En dehors d'un contexte multitâche, il est préférable d'utiliser la fonction
 * eTwiSend().
 *
 * @param pxFrame Pointeur sur la trame où sont stockés les octets à envoyer.
 * @return Identifiant de la trame (nombre strictement positif) ou 0 si la 
 *  demande n'a pu être mémorisée, dans ce cas, un nouvel appel est nécessaire
 *  (tant que la fonction retourne 0).
 */
xTwiId xTwiTrySend (const xTwiFrame * pxFrame);

/**
 * @brief Réception non bloquante d'un message en mode maître
 *
 * Cette fonction est non bloquante et la réception se fait sous interruption,
 * cela signifie que la fonction retourne alors que toutes données n'ont pas
 * forcément été reçues.\n
 * Elle est prévue pour être utilisée dans un contexte multitâche.\n
 * Si cette fonction renvoie l'identifiant d'une trame, il est nécessaire
 * d'appeler réguliérement eTwiGetStatus() tant que celle-ci ne renvoie pas
 * TWI_SUCCESS. Il est alors possible de récupérer la trame reçue grâce à
 * eTwiGetRxFrame().\n
 * En dehors d'un contexte multitâche, il est préférable d'utiliser la fonction
 * eTwiSend().
 *
 * @param pxFrame Pointeur sur la trame où sont stockés les octets à envoyer.
 * @return Identifiant de la trame (nombre strictement positif) ou 0 si la 
 *  demande n'a pu être mémorisée, dans ce cas, un nouvel appel est nécessaire
 *  (tant que la fonction retourne 0).
 */
xTwiId xTwiTryReceive (xTwiFrame * pxFrame);

/**
 * @brief Lecture de l'état du module maître
 *
 * @param xFrameId L'identifiant de la trame (renvoyé par xTwiTrySend(),
 *  xTwiTryReceive() ou xTwiTrySendReceive()).
 * @param pxLength Pointeur sur une variable qui contiendra le nombre d'octets
 *  transmis ou reçus (0 si TWI_STATUS_BUSY). Si l'utilisateur ne souhaite 
 *  pas utiliser cette valeur, il suffit de passer un pointeur NULL.
 * @return
 * - TWI_SUCCESS si la trame a été totalement transmise ou reçue.
 * - TWI_STATUS_BUSY si le module est occupé.
 * - Un code négatif en cas d'erreur.
 * .
 */
eTwiStatus eTwiGetStatus (xTwiId xFrameId, xTwiLength * pxLength);

/**
 * @brief Lecture d'une trame reçue
 *
 * @param pxFrame Pointeur sur la trame où sont stockés les octets à envoyer.
 * @return
 * - TWI_SUCCESS si la trame a été totalement lue.
 * - Un code négatif en cas d'erreur
 * .
 */
eTwiStatus eTwiGetRxFrame (xTwiFrame * pxFrame);

/**
 * @}
 */

/*----------------------------Mode Esclave------------------------------------*/
/**
 *  @defgroup twislave_module Bus I2C en mode esclave
 *
 *  Ce module permet de transmettre ou reçevoir des informations sur le bus
 *  I2C en tant qu'esclave.\n
 *  La macro TWI_SLAVE_ENABLE dans avrio-board-twi.h pour valider ce module.\n
 *  Les conflits de bus sont gérés (arbitrage).
 *  @{
 */

/* constants ================================================================ */
/**
 * 
 * @def TWI_GENCALL
 * @brief Permet de valider la réponse à un appel général en mode esclave
 *
 * Utilisé pour un appel à la fonction vTwiSetDeviceAddress()
 */
#define TWI_GENCALL 0x01

/* internal public functions ================================================ */
/**
 * @brief Gestionnaire de réception esclave
 *
 * Cette fonction est appelée par la routine d'interruption lorsqu'une trame
 * a été reçue en mode esclave.\n
 * Cette fonction doit être implémentée par l'utilisateur si le mode esclave
 * a été validé dans avrio-config-twi.h .\n
 *
 * @param pxRxPayload Pointeur sur la pile des octets reçus.
 * @param eStatus
 *  - TWI_SUCCESS
 *  - TWI_STATUS_GCALL
 *  - Un code négatif en cas d'erreur
 *  .
 * @return TWI_SUCCESS ou un code d'erreur
 * @warning Cette fonction est exécutée sous interruption. Il faut donc qu'elle 
 * soit très rapide de façon à éviter que le maître du bus ne nous interroge une 
 * nouvelle fois pendant son exécution (SLA_NACK). 
 */
eTwiStatus eTwiSlaveRxCB (xQueue * pxRxPayload, eTwiStatus eStatus);

/**
 * @brief Gestionnaire de transmission esclave
 *
 * Cette fonction est appelée par la routine d'interruption lorsque des données
 * doivent être transmises au maître.\n
 * Cette fonction doit être implémentée par l'utilisateur si le mode esclave
 * a été validé dans avrio-config-twi.h .\n
 *
 * @param pxTxPayload Pointeur sur la pile des octets à transmettre.
 * @return TWI_SUCCESS ou un code d'erreur
 * @warning Cette fonction est exécutée sous interruption. Il faut donc qu'elle 
 * soit très rapide de façon à éviter que le maître du bus ne nous interroge une 
 * nouvelle fois pendant son exécution (SLA_NACK). 
 */
eTwiStatus eTwiSlaveTxCB (xQueue * pxTxPayload, eTwiStatus eStatus);

#if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
/* macros =================================================================== */
/**
 * @def TWIFRAME_DECLARE(xVarName, xBufferSize)
 * @brief Déclare une variable de type trame I2C
 *
 * @param xVarName Nom de la variable
 * @param xBufferSize Nombre d'octets pouvant être stockés dans la trame
 */
#define TWIFRAME_DECLARE(xVarName, xBufferSize)

/**
 * @def TWIFRAME_STATIC_DECLARE(xVarName, xBufferSize)
 * @brief Déclare une variable static de type trame I2C
 *
 * @param xVarName Nom de la variable
 * @param xBufferSize Nombre d'octets pouvant être stockés dans la trame
 */
#define TWIFRAME_STATIC_DECLARE(xVarName, xBufferSize)

/* inline public functions ================================================== */
/**
 * @brief Modifie l'adresse de notre circuit en mode esclave
 *
 * @param xMyDeviceAddr Les 7 bits de poids fort correspondent à l'adresse
 *  esclave du circuit qui doivent être différents de 0. Le bit de poids le plus
 *  faible valide la réponse à l'adresse d'appel général 0x00 (\ref TWI_GENCALL).
 *  Mettre xMyDeviceAddr à 0 revient à invalider sa réponse en tant qu'esclave.
 */
void vTwiSetDeviceAddress (xTwiDeviceAddr xMyDeviceAddr);

/*--------------------------Fin Mode Esclave----------------------------------*/
/**
 * @}
 * @addtogroup twimaster_module
 * @{
 */
/*-----------------------------Mode Maître------------------------------------*/
/**
 * @brief Modifie la vitesse d'horloge du bus en mode maître.
 *
 * Les calculs devraient être effectués par le préprocesseur si usSpeed
 * est constant.
 *
 * @param usSpeed vitesse de transmission en kb/s
 * @return TWI_SUCCESS ou TWI_ERROR_INVALID_SPEED en cas de vitesse invalide.
 */
eTwiStatus eTwiSetSpeed (uint16_t usSpeed);

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
#include "avrio-config.h"

#ifdef AVRIO_TWI_ENABLE
#include "avrio-board-twi.h"

/* macros =================================================================== */
// ----------------------------------------------------------------------------
#define TWIFRAME_DECLARE(__name,__size)               \
    uint8_t __name ##Buffer[__size];                 \
    xTwiFrame __name = {  .pxData = __name ##Buffer, \
                          .xLen   = 0,                \
                          .xAddr  = 0 }
// ----------------------------------------------------------------------------
#define TWIFRAME_STATIC_DECLARE(__name,__size)               \
    static uint8_t __name ##Buffer[__size];                 \
    static xTwiFrame __name = { .pxData = __name ##Buffer }

/* inline public functions ================================================== */
#include <avr/io.h>
  /*----------------------------Mode Maître-----------------------------------*/

  // ----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (eTwiStatus eTwiSetSpeed (uint16_t usSpeed)) {
  uint16_t usRatio;

#ifdef TWPS0
  TWSR &= ~(_BV (TWPS0) | _BV (TWPS1));
#endif

  usRatio = ((AVRIO_CPU_FREQ / 1000UL) / usSpeed);
  if ((usRatio < 16) || (usRatio > (255 * 2 + 16))) {

    return TWI_ERROR_INVALID_SPEED;
  }
  TWBR = (uint8_t) (usRatio - 16) / 2;
  return TWI_SUCCESS;
}

  /*---------------------------Mode Esclave-----------------------------------*/

  // ----------------------------------------------------------------------------
__STATIC_ALWAYS_INLINE (void
                        vTwiSetDeviceAddress (xTwiDeviceAddr xMyDeviceAddr)) {

  TWAR = xMyDeviceAddr;
  TWCR = _BV (TWINT) | _BV (TWEN) | _BV (TWIE) | _BV (TWEA);
}
#endif /* AVRIO_TWI_ENABLE defined */

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_TWI_H_ */
