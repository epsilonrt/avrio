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
 * @file can.h
 * @brief Bus CAN
 *
 * @author Portions Copyright © 2005-2009 proconX Pty Ltd. All rights reserved.
 * @author Portions Copyright © 2004 by Ole Reinhardt <ole.reinhardt@kernelconcepts.de>, Kernelconcepts http://www.kernelconcepts.de

 * @todo Documentation à traduire.
 */
#ifndef _AVRIO_CAN_H_
#  define _AVRIO_CAN_H_

#  include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
  /* ======================================================================== */
/**
 * @addtogroup net_group
 * @{
 *
 *  @defgroup can_module Bus CAN
 *
 *  Ce module permet de gérer la communication sur le bus CAN (Control Area
 *  Network).\n
 *  La macro AVRIO_CAN_ENABLE doit être définie dans avrio-config.h et dans ce
 *  cas, les options peuvent être définies dans le fichier
 *  avrio-board-can.h se trouvant dans le même répertoire.\n
 * @htmlonly
 *              <center><h2>&lt;ATTENTION&gt;</h2>
 *      <p><b>Module en développement, non fonctionnel !</b></p></center>
 * @endhtmlonly
 *
 * @author Portions Copyright © 2005-2009 proconX Pty Ltd. All rights reserved.
 * @author Portions Copyright © 2004 by Ole Reinhardt <ole.reinhardt@kernelconcepts.de>, Kernelconcepts http://www.kernelconcepts.de
 *  @{
 */
  /* constants ============================================================== */
/**
 * @enum eCanSpecification
 * @brief Version de bus CAN
 */
   typedef enum {

  CAN_2A = 0x01,  /**< Version A du protocole */
  CAN_2B = 0x02   /**< Version B du protocole */
} eCanSpecification;

/**
 * @enum eCanSpeed
 * @brief Vitesse bus CAN Baud
 */
typedef enum {
  CAN_SPEED_10K = 0,      /**< 10 kbit/s, max. cable length 5000 m */
  CAN_SPEED_20K = 1,      /**< 20 kbit/s, max. cable length 2500 m */
  CAN_SPEED_50K = 2,      /**< 50 kbit/s, max. cable length 1000 m */
  CAN_SPEED_100K = 3,     /**< 100 kbit/s, max. cable length 600 m */
  CAN_SPEED_125K = 4,     /**< 125 kbit/s, max. cable length 500 m */
  CAN_SPEED_250K = 5,     /**< 250 kbit/s, max. cable length 250 m */
  CAN_SPEED_500K = 6,     /**< 500 kbit/s, max. cable length 100 m */
  CAN_SPEED_800K = 7,     /**< 800 kbit/s, max. cable length 50 m  */
  CAN_SPEED_1M = 8,       /**< 1 Mbit/s, max. cable length 25 m    */
  CAN_SPEED_CUSTOM = 255  /**< Vitesse personnalisée */
} eCanSpeed;

/**
 * @enum eCanError
 * @brief Codes d'erreur
 */
typedef enum {

  CAN_SUCCESS = 0,          /**< Successful operation */
  CAN_TXBUF_FULL = -1,      /**< All TX message objects busy */
  CAN_RXBUF_EMPTY = -2,     /**< All RX message objects busy */
  CAN_ILLEGAL_MOB = -3,     /**< Message object index out of range */
  CAN_INVALID_SPEED = -4,   /**< Invalid baud rate parameter */
  CAN_PASSIVE = -5,         /**< Bus is in passive state */
  CAN_BUS_OFF = -6,         /**< Bus is bus-off */
  CAN_TIMEOUT = -7          /**< Temps d'attente dépassé */
} eCanError;

/* structures =============================================================== */
/**
 * @union xCanArbField
 * @brief Champs d'arbitrage
 *
 * Compatible CAN 2.0 A et B
 */
typedef union xCanArbField {

  uint32_t xValue;
  struct {
    uint32_t xIde:1;        /**< IDentifier Extension */
    uint32_t xRtr:1;        /**< Remote Transmission Request */
    uint32_t xUnused:1;
    uint32_t xId:29;        /**< Identifier */
  };
} xCanArbField;

/**
 * @union xCanCtrlField
 * @brief Champs de contrôle
 */
typedef union xCanCtrlField {

  uint8_t xValue;
  struct {

    uint8_t xUnused:2;
    uint8_t xR1:1;       /**< Reserved Bit 1 */
    uint8_t xR0:1;       /**< Reserved Bit 0 */
    uint8_t xDlc:4;      /**< Data Length Code */
  };
} xCanCtrlField;

/**
 * @struct xCanFrame
 * @brief Trame CAN
 */
typedef struct xCanFrame {

  xCanArbField xArb;      /**< Champs d'arbitrage */
  xCanCtrlField xCtrl;    /**< Champs de contrôle */
  uint8_t ucData[8];      /**< Octets de données */
} xCanFrame;

/* internal public functions ================================================ */
/**
 * @brief Initialize CAN interface.
 * @return 0 for successful initialisation or -1 in case init failed
 */
eCanError eCanInit (eCanSpecification eType);

/**
 * @brief Sets the CAN baud rate
 * @param dev Pointer to the device structure
 * @param baudrate Baud rate (One of the defined baud rates. See AtCan.h)
 * @return 0 for success
 */
eCanError eCanSetSpeed (eCanSpeed eBaudrate);

/**
 * @brief  Sets the acceptance code and mask
 * @param am 4 byte char array with the acceptance mask
 */
void vCanSetFilter (uint8_t * pucAcceptanceCode, uint8_t * pucAcceptanceMask);

/**
 * @brief Write a frame from to output buffer
 *
 * This function writes a frame to the output buffer. If the output buffer
 * is full the function will block until frames are send.
 *
 * @param frame Pointer to the receive frame
 */
eCanError eCanSendFrame (const xCanFrame * pxFrame);

/**
 * @brief Checks if there's still space in output buffer
 * @param dev Pointer to the device structure
 * @return true if space is available
 */
bool xCanTxFree (void);

/**
 * @brief Transmission non bloquante d'une trame
 */
eCanError eCanTrySendFrame (const xCanFrame * pxFrame);

/**
 * @brief Reads a frame from input buffer
 *
 * This function reads a frame from the input buffer. If the input buffer
 * is empty the function will block unitl new frames are received,
 * or the timeout is reached.
 *
 * @param frame Pointer to the receive frame
 * @return -1 if timeout, 0 otherwise
 */
eCanError eCanReceiveFrame (xCanFrame * pxFrame);

/**
 * @brief Checks if data is available in input buffer
 * @param dev Pointer to the device structure
 * @return Number of frames available
 */
uint8_t ucCanRxAvail (void);

/**
 * @brief Réception non bloquante d'une trame
 */
eCanError eCanTryReceiveFrame (xCanFrame * pxFrame);

/**
 * @brief Sets the CAN receive timeout
 *
 * @param dev Pointer to the device structure
 * @param timeout Timeout in milliseconds, WAIT_INFINITE = no time-out

 * \warning Timeout values are given in milliseconds and are limited to
 *          the granularity of the system timer. To disable timeout,
 *          set the parameter to WAIT_INFINITE.
 */
void vCanSetRxTimeout (uint16_t usTimeout);

#  if defined(__DOXYGEN__)
/* 
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

  /**
   *   @}
   * @}
   */
#  else
/* 
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

#  endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* _AVRIO_CAN_H_ */
