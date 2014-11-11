/**
 * @file xbee.c
 * @brief XBee Zigbee module interface functions
 * @author Copyright © 2006-2008 Tymm Twillman <tymm@booyaka.com>
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20130228 - Initial version by epsilonRT
 */
#include <string.h>
#include <stdlib.h>
#include "xbee_protocol.h"

# ifdef CONFIG_XBEE_REENTRANT_TX
#  error CONFIG_XBEE_REENTRANT_TX requires XBEE_ALLOC to be set!
# endif

/* macros =================================================================== */

/* In case we need to serialize access for transmission;
 *  reception is made to always come from one XBee module so
 *  shouldn't need to serialize that.
 */
#ifndef CONFIG_XBEE_REENTRANT_TX
# define LOCK_FRAME_ID(xbee)      do {} while(0)
# define UNLOCK_FRAME_ID(xbee)    do {} while(0)
#endif


/* Error counters can be added later if desired */
#define INC_RX_CRC_ERROR(xbee) do {} while(0)
#define INC_RX_ERROR(xbee)     do {} while(0)
#define INC_RX_DROPPED(xbee) do {} while(0)
#define INC_TX_ERROR(xbee)     do {} while(0)
#define INC_TX_DROPPED(xbee) do {} while(0)

/* constants ================================================================ */
#ifndef ENOMEM
# define ENOMEM 12
#endif

/* private functions ======================================================== */

/* -----------------------------------------------------------------------------
 * Generate & return next 8-bit frame ID
 */
static inline uint8_t
ucNextFrameId (xXBee *xbee) {
  uint8_t frame_id;


  LOCK_FRAME_ID (xbee);
  if (++xbee->out.frame_id == 0) {
    ++xbee->out.frame_id;
  }
  frame_id = xbee->out.frame_id;
  UNLOCK_FRAME_ID (xbee);

  return frame_id;
}

/* -----------------------------------------------------------------------------
 * Generate CRC for an XBee packet
 */
static uint8_t
ucXBeeCrc (const xXBeePkt *pkt) {
  uint8_t *pkt_data = ( (uint8_t *) pkt) + sizeof (xXBeePktHdr);
  uint16_t i;
  uint8_t crc = 0;


  for (i = 0; i < ntohs ( ( (xXBeePktHdr *) pkt)->len); i++) {
    crc += * (pkt_data++);
  }

  return ~crc;
}

/* internal public functions ================================================ */

/* -----------------------------------------------------------------------------
 * Initialize this package
 */
void vXBeeInit (xXBee *xbee, FILE * io_stream) {

  memset (xbee, 0, sizeof (xXBee));
  xbee->io_stream = io_stream;
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

  frame_id = ucNextFrameId (xbee);

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

  frame_id = ucNextFrameId (xbee);
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

  if (ret >= 0) {

    return frame_id;
  }

  vXBeeFreePkt (xbee, (xXBeePkt *) pkt);

  INC_TX_ERROR();

  return ret;
}


/* -----------------------------------------------------------------------------
 * Send a data packet to another module using its 64-bit unique ID
 */
int 
iXBeeSend64 (xXBee *xbee, const void *data, uint8_t len, uint8_t opt, const uint8_t addr[8]) {
  xXBeeA64TxPkt *pkt;
  int ret;
  uint8_t frame_id;


  pkt = (xXBeeA64TxPkt *) pvXBeeAllocPkt (xbee, XBEE_XMIT, len + 15);
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

  ret = iXBeeOut (xbee, (xXBeePkt *) pkt, len + sizeof (xXBeeA64TxPkt) + 1);

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
  xXBeeA16TxPkt *pkt;
  uint8_t frame_id;
  int ret;


  pkt = (xXBeeA16TxPkt *) pvXBeeAllocPkt (xbee, XBEE_XMIT, len + 9);
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

  ret = iXBeeOut (xbee, (xXBeePkt *) pkt, len + sizeof (xXBeeA16TxPkt) + 1);

  if (ret >= 0) {
    
    return frame_id;
  }

  INC_TX_ERROR();
  vXBeeFreePkt (xbee, (xXBeePkt *) pkt);
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
  size_t ulAllLen = 0;

  while (ulAllLen < len) {

    ulAllLen += fwrite (pkt, len, 1, xbee->io_stream);
    if (ferror (xbee->io_stream)) {
      
      return -1;
    }
  }
  return 0;
}

/* -----------------------------------------------------------------------------
 * Poll the inputstream to read incoming bytes
 */
int 
iXBeePoll (xXBee *xbee) {
  size_t ulReadLen;
  char buffer[16];
  
  for(;;) {
    
    ulReadLen = fread (buffer, sizeof(buffer), 1, xbee->io_stream);
    
    if (ulReadLen) {
      
      vXBeeIn (xbee, buffer, ulReadLen);
    }
    if (feof(xbee->io_stream)) {
      
      return 0;
    }
    if (ferror (xbee->io_stream)) {
      
      return -1;
    }
  }
}

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
