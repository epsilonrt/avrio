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
 * @file xbee1.c
 * @brief XBee S1 module interface functions
 * Copyright © 2006-2008 Tymm Twillman <tymm@booyaka.com>
 *
 * Revision History ------------------------------------------------------------
 *    20130228 - Initial version by epsilonRT
 */
#include <string.h>
#include "xbee_private.h"

/* internal public functions ================================================ */
/*==============================================================================
 * 
 * TODO: Partie non testée
 * 
 *============================================================================*/
//#warning This file was not debugged, it should be used with great caution !!

/* -----------------------------------------------------------------------------
 * Send a data packet to another module using its 64-bit unique ID
 */
int
iXBeeSend64 (xXBee *xbee,
                  const void *data,
                  uint8_t len,
                  const uint8_t addr[8],
                  uint8_t opt) {
  xXBeeTxReq64Pkt *pkt;
  int ret;
  uint8_t frame_id;

  pkt = (xXBeeTxReq64Pkt *) pvXBeeAllocPkt (xbee, XBEE_XMIT, len + 15);
  if (pkt == NULL) {

    INC_TX_ERROR (xbee);
    return -ENOMEM;
  }

  XBEE_HDR_INIT (pkt->hdr, len + 11);

  pkt->type = XBEE_PKT_TYPE_TX64;
  memcpy (pkt->dest, addr, 8);
  pkt->opt = opt;
  frame_id = ucXBeeNextFrameId (xbee);
  pkt->frame_id = frame_id;
  memcpy (pkt->data, data, len);
  pkt->data[len] = ucXBeeCrc ( (xXBeePkt *) pkt);

  ret = iXBeeOut (xbee, (xXBeePkt *) pkt, len + sizeof (xXBeeTxReq64Pkt) + 1);

  if (ret >= 0) {

    return frame_id;
  }

  INC_TX_ERROR (xbee);

  vXBeeFreePkt (xbee, (xXBeePkt *) pkt);

  return ret;
}

/* -----------------------------------------------------------------------------
 * Send a data packet to another module using its 16-bit ID
 */
int
iXBeeSend16 (xXBee *xbee,
                  const void *data,
                  uint8_t len,
                  const uint8_t addr[2],
                  uint8_t opt) {
  xXBeeTxReq16Pkt *pkt;
  uint8_t frame_id;
  int ret;


  pkt = (xXBeeTxReq16Pkt *) pvXBeeAllocPkt (xbee, XBEE_XMIT, len + 9);
  if (pkt == NULL) {

    INC_TX_ERROR (xbee);
    return -ENOMEM;
  }

  XBEE_HDR_INIT (pkt->hdr, len + 5);

  pkt->type = XBEE_PKT_TYPE_TX16;
  memcpy (pkt->dest, addr, 2);
  pkt->opt = opt;
  frame_id = ucXBeeNextFrameId (xbee);
  pkt->frame_id = frame_id;
  memcpy (pkt->data, (uint8_t *) data, len);
  pkt->data[len] = ucXBeeCrc ( (xXBeePkt *) pkt);

  ret = iXBeeOut (xbee, (xXBeePkt *) pkt, len + sizeof (xXBeeTxReq16Pkt) + 1);

  if (ret >= 0) {

    return frame_id;
  }

  INC_TX_ERROR(xbee);
  vXBeeFreePkt (xbee, (xXBeePkt *) pkt);
  return ret;
}
/* ========================================================================== */
