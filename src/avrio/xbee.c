/**
 * @file xbee.c
 * @brief XBee module interface functions
 * @author Copyright © 2006-2008 Tymm Twillman <tymm@booyaka.com>
 * @author Copyright © 2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20130228 - Initial version by epsilonRT
 */
#include <string.h>
#include <stdlib.h>
#include <util/atomic.h>
#include "xbee_private.h"

/* private functions ======================================================== */

/* -----------------------------------------------------------------------------
 * Generate & return next 8-bit frame ID
 */
uint8_t
ucXBeeNextFrameId (xXBee *xbee) {
  uint8_t frame_id;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {

    if (++xbee->out.frame_id == 0) {
      ++xbee->out.frame_id;
    }
    frame_id = xbee->out.frame_id;
  }
  return frame_id;
}

/* -----------------------------------------------------------------------------
 * Generate CRC for an XBee packet
 */
uint8_t
ucXBeeCrc (const xXBeePkt *pkt) {
  uint8_t *pkt_data = ( (uint8_t *) pkt) + sizeof (xXBeePktHdr);
  uint16_t i;
  uint8_t crc = 0;


  for (i = 0; i < ntohs ( ( (xXBeePktHdr *) pkt)->len); i++) {
    crc += * (pkt_data++);
  }

  return ~crc;
}

// -----------------------------------------------------------------------------
uint8_t * 
pucXBeePktAddress64 (xXBeePkt *pkt) {

  switch (pkt->type) {
#if AVRIO_XBEE_SERIES == 1
    case XBEE_PKT_TYPE_RX64:
      return ((xXBeeRx64Pkt *) pkt)->src;
    case XBEE_PKT_TYPE_RX64_IO:
      return ((xXBeeRxIo64Pkt *) pkt)->src;
#elif AVRIO_XBEE_SERIES == 2
    case XBEE_PKT_TYPE_ZB_RX:
      return ((xXBeeZbRxPkt *) pkt)->src64;
    case XBEE_PKT_TYPE_ZB_RX_IO:
      return ((xXBeeZbRxIoPkt *) pkt)->src64;
    case XBEE_PKT_TYPE_ZB_RX_SENSOR:
      return ((xXBeeZbRxSensorPkt *) pkt)->src64;
#endif
    case XBEE_PKT_TYPE_REMOTE_ATCMD_RESP:
      return ((xXBeeRemoteAtCmdRespPkt *) pkt)->src64;
    default:
      break;
  }
  return 0;
}

// -----------------------------------------------------------------------------
uint32_t
ulXBeePktAddress32 (xXBeePkt *pkt, uint8_t ucOffset) {
  static uint32_t s;
  uint8_t *p = pucXBeePktAddress64 (pkt);

  if (p) {

    memcpy (&s, p + ucOffset, 4);
    vSwapBytes ( (uint8_t *) &s, 4);
  }
  else {

    s = XBEE_SH_UNKNOWN;
  }
  return s;
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
bool 
bXBeePktAddressIsEqual (const uint8_t *a1, const uint8_t *a2, uint8_t len) {
  
}

// -----------------------------------------------------------------------------
uint32_t
ulXBeePktAddressSH (xXBeePkt *pkt) {

  return ulXBeePktAddress32 (pkt, 0);
}

// -----------------------------------------------------------------------------
uint32_t ulXBeePktAddressSL (xXBeePkt *pkt) {

  return ulXBeePktAddress32 (pkt, 4);
}

// -----------------------------------------------------------------------------
uint16_t usXBeePktAddress16 (xXBeePkt *pkt) {
  uint16_t * p = 0;
  uint16_t addr16;

  switch (pkt->type) {
#if AVRIO_XBEE_SERIES == 1
    case XBEE_PKT_TYPE_RX16:
      p = (uint16_t *) ((xXBeeRx16Pkt *) pkt)->src;
      break;
    case XBEE_PKT_TYPE_RX16_IO:
      p = (uint16_t *) ((xXBeeRxIo16Pkt *) pkt)->src;
      break;
#elif AVRIO_XBEE_SERIES == 2
    case XBEE_PKT_TYPE_ZB_RX:
      p = (uint16_t *) ((xXBeeZbRxPkt *) pkt)->src16;
      break;
    case XBEE_PKT_TYPE_ZB_RX_IO:
      p = (uint16_t *) ((xXBeeZbRxIoPkt *) pkt)->src16;
      break;
    case XBEE_PKT_TYPE_ZB_RX_SENSOR:
      p = (uint16_t *) ((xXBeeZbRxSensorPkt *) pkt)->src16;
      break;
    case XBEE_PKT_TYPE_ZB_TX_STATUS:
      p = (uint16_t *) ((xXBeeZbTxStatusPkt *) pkt)->dst16;
      break;
#endif
    case XBEE_PKT_TYPE_REMOTE_ATCMD_RESP:
      p = (uint16_t *) ((xXBeeRemoteAtCmdRespPkt *) pkt)->src16;
    default:
      break;
  }

  if (p) {

    addr16 = ntohs (*p);
  }
  else {

    addr16 = XBEE_A16_UNKNOWN;
  }
  return addr16;
}

// -----------------------------------------------------------------------------
uint8_t *
pucXBeePktData (xXBeePkt *pkt) {

  switch (pkt->type) {
#if AVRIO_XBEE_SERIES == 1
    case XBEE_PKT_TYPE_RX64:
      return ((xXBeeRx64Pkt *) pkt)->data;
    case XBEE_PKT_TYPE_RX64_IO:
      return ((xXBeeRxIo64Pkt *) pkt)->data;
    case XBEE_PKT_TYPE_RX16:
      return ((xXBeeRx16Pkt *) pkt)->data;
    case XBEE_PKT_TYPE_RX16_IO:
      return ((xXBeeRxIo16Pkt *) pkt)->data;
#elif AVRIO_XBEE_SERIES == 2
    case XBEE_PKT_TYPE_ZB_RX:
      return ((xXBeeZbRxPkt *) pkt)->data;
    case XBEE_PKT_TYPE_ZB_RX_IO:
      return (uint8_t *)((xXBeeZbRxIoPkt *) pkt)->data;
#endif
    default:
      break;
  }
  return 0;
}

// -----------------------------------------------------------------------------
int
iXBeePktDataLen (xXBeePkt *pkt) {
  uint8_t pkt_size = 0;

  switch (pkt->type) {
#if AVRIO_XBEE_SERIES == 1
    case XBEE_PKT_TYPE_RX64:
      pkt_size = sizeof (xXBeeRx64Pkt);
      break;
    case XBEE_PKT_TYPE_RX64_IO:
      pkt_size = sizeof (xXBeeRxIo64Pkt);
      break;
    case XBEE_PKT_TYPE_RX16:
      pkt_size = sizeof (xXBeeRx16Pkt);
      break;
    case XBEE_PKT_TYPE_RX16_IO:
      pkt_size = sizeof (xXBeeRxIo16Pkt);
      break;
#elif AVRIO_XBEE_SERIES == 2
    case XBEE_PKT_TYPE_ZB_RX:
      pkt_size = sizeof (xXBeeZbRxPkt);
      break;
    case XBEE_PKT_TYPE_ZB_RX_IO:
      pkt_size = sizeof (xXBeeZbRxIoPkt);
      break;
#endif
    default:
      break;
  }

  if (pkt_size) {

    return ntohs (pkt->hdr.len) - (pkt_size + 1) + 4;
  }
  return -1;
}

// -----------------------------------------------------------------------------
uint8_t 
ucXBeePktType (xXBeePkt *pkt) {
  
  return pkt->type;
}

// -----------------------------------------------------------------------------
const char * 
pcXBeePktCommand (xXBeePkt *pkt) {

  // TODO
  return 0;
}

// -----------------------------------------------------------------------------
uint8_t * 
pucXBeePktParam (xXBeePkt *pkt) {
  
  // TODO
  return 0;
}

// -----------------------------------------------------------------------------
int 
iXBeePktParamLen (xXBeePkt *pkt) {
  
  // TODO
  return -1;
}

// -----------------------------------------------------------------------------
int 
iXBeePktFrameId (xXBeePkt *pkt) {
  
  // TODO
  return -1;
}

// -----------------------------------------------------------------------------
int 
iXBeePktStatus (xXBeePkt *pkt) {
  
  // TODO
  return -1;
}

// -----------------------------------------------------------------------------
int 
iXBeePktRadius (xXBeePkt *pkt) {
  
  // TODO
  return -1;
}

// -----------------------------------------------------------------------------
int 
iXBeePktApply (xXBeePkt *pkt) {
  
  // TODO
  return -1;
}

// -----------------------------------------------------------------------------
int
iXBeePktOptions (xXBeePkt *pkt) {

  // TODO
  if (pkt->type == XBEE_PKT_TYPE_ZB_RX) {

    return ( (xXBeeZbRxPkt *) pkt)->opt;
  }
  return -1;
}

/* -----------------------------------------------------------------------------
 * Initialize this package
 */
void vXBeeInit (xXBee *xbee, FILE * io_stream) {

  memset (xbee, 0, sizeof (xXBee));
  xbee->io_stream = io_stream;
}

/* -----------------------------------------------------------------------------
 *
 */
void
vXBeeSetCB (xXBee *xbee, eXBeeCbType cb_type, iXBeeRxCB cb) {

  if ( (cb_type >= XBEE_CB_AT_LOCAL) && (cb_type <= XBEE_CB_SENSOR)) {

    xbee->in.user_cb[cb_type] = cb;
  }
}

/* -----------------------------------------------------------------------------
 * Send a command to an XBee module
 */
int
iXBeeSendAt (xXBee *xbee,
             const char cmd[],
             uint8_t param_len,
             const uint8_t params[]) {
  xXBeeAtCmdPkt *pkt;
  uint8_t frame_id;
  int ret;


  pkt = (xXBeeAtCmdPkt *) pvXBeeAllocPkt (xbee, XBEE_XMIT, param_len + 8);
  if (pkt == NULL) {

    INC_TX_ERROR();
    return -ENOMEM;
  }

  XBEE_HDR_INIT (pkt->hdr, param_len + 4);

  pkt->type = XBEE_PKT_TYPE_ATCMD;

  frame_id = ucXBeeNextFrameId (xbee);

  pkt->frame_id = frame_id;

  pkt->command[0] = cmd[0];
  pkt->command[1] = cmd[1];

  memcpy (pkt->param, params, param_len);
  pkt->param[param_len] = ucXBeeCrc ( (xXBeePkt *) pkt);

  ret = iXBeeOut (xbee, (xXBeePkt *) pkt,
                  sizeof (xXBeeAtCmdPkt) + param_len + 1);

  if (ret >= 0) {

    return frame_id;
  }

  vXBeeFreePkt (xbee, (xXBeePkt *) pkt);

  INC_TX_ERROR();

  return ret;
}


/* -----------------------------------------------------------------------------
 * Send a command to a remote XBee module
 */
int
iXBeeSendRemoteAt (xXBee *xbee,
                   const char cmd[],
                   uint8_t param_len,
                   uint8_t apply,
                   const uint8_t params[],
                   const uint8_t addr64[8],
                   const uint8_t addr16[2]) {
  xXBeeRemoteAtCmdPkt *pkt;
  uint8_t frame_id;
  int ret;


  pkt = (xXBeeRemoteAtCmdPkt *) pvXBeeAllocPkt (xbee, XBEE_XMIT, param_len + 19);
  if (pkt == NULL) {

    INC_TX_ERROR();
    return -ENOMEM;
  }

  XBEE_HDR_INIT (pkt->hdr, param_len + 15);

  pkt->type = XBEE_PKT_TYPE_REMOTE_ATCMD;

  frame_id = ucXBeeNextFrameId (xbee);
  pkt->frame_id = frame_id;

  memcpy (pkt->dest64, addr64, 8);
  memcpy (pkt->dest16, addr16, 2);

  pkt->apply = apply ? 2 : 0;

  pkt->command[0] = cmd[0];
  pkt->command[1] = cmd[1];

  memcpy (pkt->param, params, param_len);
  pkt->param[param_len] = ucXBeeCrc ( (xXBeePkt *) pkt);

  ret = iXBeeOut (xbee, (xXBeePkt *) pkt,
                  sizeof (xXBeeRemoteAtCmdPkt) + param_len + 1);

  if (ret == 0) {

    return frame_id;
  }

  vXBeeFreePkt (xbee, (xXBeePkt *) pkt);

  INC_TX_ERROR();

  return ret;
}

/* -----------------------------------------------------------------------------
 * Queue a packet for transmission
 *
 * needs to queue packet to be sent to XBEE module; e.g. copy the packet to a
 * UART buffer.
 *  On error, -1 should be returned and the packet should NOT be freed.
 *  On success, 0 should be returned; if XBEE_ALLOC is set, this function or
 *   someone downstream is responsible for freeing it -- the packet has been
 *   handed off.  This is to minimize copying of data.
 */
int
iXBeeOut (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {
  char * data = (char *) pkt;

  for (uint8_t i = 0; i < len; i++) {

    if (fputc (data[i], xbee->io_stream) == EOF) {

      return -1;
    }
  }
  return 0;
}

/* -----------------------------------------------------------------------------
 * Accept data from an XBee module & build into valid XBEE
 *  packets
 */
void
vXBeeIn (xXBee *xbee, const void *buf, uint8_t len) {
  uint8_t *data = (uint8_t *) buf;

  while (len) {
    switch (xbee->in.bytes_rcvd) {

      case 0:
        while (*data != XBEE_PKT_START) {

          if (!--len) {

            return;
          }
          data++;
        }

        xbee->in.hdr_data[xbee->in.bytes_rcvd++] = *data++;
        if (!--len) {

          return;
        }

        /* Fall thru */

      case 1:
        xbee->in.hdr_data[xbee->in.bytes_rcvd++] = *data++;
        if (!--len) {

          return;
        }

        /* Fall thru */

      case 2:
        xbee->in.hdr_data[xbee->in.bytes_rcvd++] = *data++;

        /* Got enough to get packet length */

        xbee->in.bytes_left = ntohs ( ( (xXBeePktHdr *) xbee->in.hdr_data)->len);

        if (xbee->in.bytes_left > XBEE_MAX_DATA_LEN
            || ( (xbee->in.packet
                  = pvXBeeAllocPkt (xbee, XBEE_RECV, xbee->in.bytes_left + 4)) == NULL)
           ) {

          xbee->in.bytes_left = 0;
          INC_RX_ERROR (xbee);
          continue;
        }

        xbee->in.bytes_left++; /* Extra for crc (alloc_pkt already accounts for it) */

        memcpy (& (xbee->in.packet->hdr), & (xbee->in.hdr_data),
                sizeof (xbee->in.hdr_data));

        if (!--len) {

          return;
        }

        /* Fall thru */

      default
          :
        while (xbee->in.bytes_left--) {

          ( (uint8_t *) xbee->in.packet) [xbee->in.bytes_rcvd++] = *data++;
          if (!--len && xbee->in.bytes_left) {

            return;
          }
        }
    }

    if (ucXBeeCrc (xbee->in.packet)
        != ( (uint8_t *) xbee->in.packet) [xbee->in.bytes_rcvd - 1]) {

      xbee->in.bytes_rcvd = 0;
      INC_RX_CRC_ERROR (xbee);
      continue;
    }

    if (iXBeeRecvPktCB (xbee, xbee->in.packet, xbee->in.bytes_rcvd)) {

      vXBeeFreePkt (xbee, xbee->in.packet);
      INC_RX_DROPPED (xbee);
    }

    xbee->in.bytes_rcvd = 0;
  }
}

/* -----------------------------------------------------------------------------
 * Poll the inputstream to read incoming bytes
 */
int
iXBeePoll (xXBee *xbee) {
  int c;

  c = fgetc (xbee->io_stream);
  if (c != EOF) {

    vXBeeIn (xbee, &c, 1);
  }
  else {

    if (ferror (xbee->io_stream)) {

      return -1;
    }
  }
  return 0;
}

/* -----------------------------------------------------------------------------
 * Handle an incoming packet
 *
 * the packet will be fully formed and verified
 * for proper construction before being passed off to this function.  This
 * function should dig into the packet & process based on its contents.
 */
int
iXBeeRecvPktCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len) {
  eXBeeCbType eCb = XBEE_CB_UNKNOWN;

  switch (pkt->type) {

    case XBEE_PKT_TYPE_ATCMD_RESP:
      eCb = XBEE_CB_AT_LOCAL;
      break;

    case XBEE_PKT_TYPE_REMOTE_ATCMD_RESP:
      eCb = XBEE_CB_AT_REMOTE;
      break;

    case XBEE_PKT_TYPE_MODEM_STATUS:
      eCb = XBEE_CB_MODEM_STATUS;
      break;

#if AVRIO_XBEE_SERIES == 1
      //--------------------------------------------------------------------------
    case XBEE_PKT_TYPE_TX_STATUS:
      eCb = XBEE_CB_TX_STATUS;
      break;

    case XBEE_PKT_TYPE_RX16:
    case XBEE_PKT_TYPE_RX64:
      eCb = XBEE_CB_DATA;
      break;

    case XBEE_PKT_TYPE_RX16_IO:
    case XBEE_PKT_TYPE_RX64_IO:
      eCb = XBEE_CB_IO;
      break;
      //--------------------------------------------------------------------------

#elif AVRIO_XBEE_SERIES == 2
      //--------------------------------------------------------------------------
    case XBEE_PKT_TYPE_ZB_TX_STATUS:
      eCb = XBEE_CB_TX_STATUS;
      break;

    case XBEE_PKT_TYPE_ZB_RX:
      eCb = XBEE_CB_DATA;
      break;

    case XBEE_PKT_TYPE_ZB_RX_IO:
      eCb = XBEE_CB_IO;
      break;

    case XBEE_PKT_TYPE_ZB_RX_SENSOR:
      eCb = XBEE_CB_SENSOR;
      break;
      //--------------------------------------------------------------------------
#endif

    default
        :
      break;
  }

  if (eCb != XBEE_CB_UNKNOWN) {

    if (xbee->in.user_cb[eCb]) {

      return xbee->in.user_cb[eCb] (xbee, pkt, len);
    }
  }
  return -1; /* pkt was dropped */
}

/*----------------------------------------------------------------------------
       Must be provided externally only if using dynamic memory
      (which allows more than one packet to be queued at a time)
 ----------------------------------------------------------------------------*/

/* -----------------------------------------------------------------------------
 * Return a buffer for an xbee packet
 *
 * at least <len> bytes need to be allocated
 * Direction since we may want to have different allocations mechanisms/etc
 * for xmit vs recv.
 */
void *
pvXBeeAllocPkt (xXBee *xbee, uint8_t direction, uint8_t len) {

  return malloc (len);
}

/* -----------------------------------------------------------------------------
 * Free up an allocated packet
 */
void
vXBeeFreePkt (xXBee *xbee, xXBeePkt *pkt) {

  free (pkt);
}

/* ========================================================================== */
