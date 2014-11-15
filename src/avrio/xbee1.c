/**
 * @file xbee1.c
 * @brief XBee S1 module interface functions
 * @author Copyright © 2006-2008 Tymm Twillman <tymm@booyaka.com>
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20130228 - Initial version by epsilonRT
 */
#if AVRIO_XBEE_SERIES == 1
#include <string.h>
//#include <stdlib.h>
#include "xbee_private.h"


/* internal public functions ================================================ */


/* -----------------------------------------------------------------------------
 * Send a data packet to another module using its 64-bit unique ID
 */
int
iXBeeSend64 (xXBee *xbee, const void *data, uint8_t len, uint8_t opt, const uint8_t addr[8]) {
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
  frame_id = ucNextFrameId (xbee);
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
iXBeeSend16 (xXBee *xbee, const void *data, uint8_t len, uint8_t opt, const uint8_t addr[2]) {
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
  frame_id = ucNextFrameId (xbee);
  pkt->frame_id = frame_id;
  memcpy (pkt->data, (uint8_t *) data, len);
  pkt->data[len] = ucXBeeCrc ( (xXBeePkt *) pkt);

  ret = iXBeeOut (xbee, (xXBeePkt *) pkt, len + sizeof (xXBeeTxReq16Pkt) + 1);

  if (ret >= 0) {

    return frame_id;
  }

  INC_TX_ERROR();
  vXBeeFreePkt (xbee, (xXBeePkt *) pkt);
  return ret;
}
#endif  /* AVRIO_XBEE_SERIES == 1 */
/* ========================================================================== */
