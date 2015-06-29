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
