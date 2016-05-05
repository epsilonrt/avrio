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
 * @file tnc.h
 * @brief Terminal Node Controller
 *
 * Les messages ont le format suivant:
 * <SOH><STX>[Data Hexa ASCII]<ETX>[CRC CCITT Hexa ASCII]<EOT>
 */

#ifndef _AVRIO_TNC_H_
#define _AVRIO_TNC_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include <stdio.h>

/* constants ================================================================ */
#define TNC_SOH 1
#define TNC_STX 2
#define TNC_ETX 3
#define TNC_EOT 4
#define TNC_TXT 5
#define TNC_CRC 6

/**
 * List of error codes returned by the functions.
 */
typedef enum {

  TNC_SUCCESS                  =  0,
  TNC_CRC_ERROR                = -1,
  TNC_NOT_ENOUGH_MEMORY        = -2,
  TNC_OBJECT_NOT_FOUND         = -3,
  TNC_ILLEGAL_MSG              = -4,
  TNC_FILE_NOT_FOUND           = -5,
  TNC_IO_ERROR                 = -6

} eTncError;

//##############################################################################
//#                                                                            #
//#                              xTnc Class                                    #
//#                                                                            #
//##############################################################################

struct xTnc;

/**
 * Create and initialize a new xTnc object
 *
 * This object should be deleted with vTncDelete()
 * @return pointer on the object, NULL on error
 */
void vTncInit (struct xTnc *p, FILE *fin, FILE *fout);

/**
 * Clear all fields of the object
 */
void vTncClear (struct xTnc *p);

/**
 * Check if there are any TNC messages to be processed.
 * This function read available characters from the medium and search for
 * any TNC messages.
 * If a message is found, this function return true and the message can be read
 * with iTncRead().
 * This function may be blocking if there are no available chars and the FILE
 * used in @a tnc to access the medium is configured in blocking mode.
 *
 * @param tnc TNC object to operate on.
 * @return true if , negative value on error
 */
int iTncPoll(struct xTnc *tnc);

/**
 * Send an TNC message
 *
 * @param tnc TNC object to operate on.
 * @param frame message to send
 * @return TNC_SUCCESS, negative value on error
 */
ssize_t iTncWrite (struct xTnc *tnc, const void *buf, size_t count);

/**
 * Retruns the last error code.
 */
int iTncError (struct xTnc *tnc);

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */
#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */
#include "avrio-config.h"

#ifdef AVRIO_TNC_ENABLE
#include "avrio-board-tnc.h"
/**
 * Tnc Class
 */
typedef struct xTnc {

  FILE * fin;      ///< stream used to access the physical input
  FILE * fout;     ///< stream used to access the physical output
  uint8_t rxbuf[TNC_RXBUFSIZE];    ///< buffer for payload
  size_t  len;        ///< buffer length
  uint16_t crc_rx;  ///< computed rx CRC
  uint16_t crc_tx;  ///< received from tx CRC
  int state;
  uint8_t cnt;
  uint8_t msb;
} xTnc;

#endif /* AVRIO_TNC_ENABLE defined */

#endif /* __DOXYGEN__ not defined */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */

#endif /* _AVRIO_TNC_H_ */
