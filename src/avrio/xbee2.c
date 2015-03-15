/**
 * @file xbee2.c
 * @brief XBee S2 module interface functions
 * @author Copyright © 2006-2008 Tymm Twillman <tymm@booyaka.com>
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20130228 - Initial version by epsilonRT
 */
#if AVRIO_XBEE_SERIES == 2
#include <string.h>
#include "xbee_private.h"

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * Send a data packet to another module using its 64-bit unique ID
 */
int iXBeeZbSend (xXBee *xbee,
                 const void *data,
                 uint8_t len,
                 const uint8_t addr64[8],
                 const uint8_t addr16[2], uint8_t opt, uint8_t radius) {
  int ret;
  uint8_t frame_id;
  xXBeeZbTxReqPkt *pkt;
  const uint16_t pkt_size = len + sizeof (xXBeeZbTxReqPkt) + 1;

  pkt = (xXBeeZbTxReqPkt *) pvXBeeAllocPkt (xbee, XBEE_XMIT, pkt_size);
  if (pkt == NULL) {

    INC_TX_ERROR (xbee);
    return -ENOMEM;
  }

  XBEE_HDR_INIT (pkt->hdr, pkt_size - 4);

  pkt->type = XBEE_PKT_TYPE_ZB_TX_REQ;
  memcpy (pkt->dest64, addr64, 8);
  memcpy (pkt->dest16, addr16, 2);
  pkt->opt = opt;
  pkt->radius = radius;
  frame_id = ucXBeeNextFrameId (xbee);
  pkt->frame_id = frame_id;
  memcpy (pkt->data, data, len);
  pkt->data[len] = ucXBeeCrc ( (xXBeePkt *) pkt);

  ret = iXBeeOut (xbee, (xXBeePkt *) pkt, pkt_size);

  if (ret == 0) {

    return frame_id;
  }

  INC_TX_ERROR (xbee);
  vXBeeFreePkt (xbee, (xXBeePkt *) pkt);
  return ret;
}

/* -----------------------------------------------------------------------------
 * Send a zigbee data packet to the coordinator (Series 2)
 */
int 
iXBeeZbSendToCoordinator (xXBee *xbee, const void *data, uint8_t len) {
  
  return iXBeeZbSend (xbee, data, len, 
                      pucXBeeAddr64Coordinator(), 
                      pucXBeeAddr16Unknown(), 0, 0);
}
#endif  /* AVRIO_XBEE_SERIES == 2 */
/* ========================================================================== */
