/**
 * @file xbee_protocol.h
 * Maxstream XBee module protocol Header
 * @author Copyright © 2006-2008 Tymm Twillman <tymm@booyaka.com>
 */
#ifndef _AVRIO_XBEE_PROTOCOL_H_
#define _AVRIO_XBEE_PROTOCOL_H_
#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include "xbee.h"

#if !defined(ntohs) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
# define ntohs(n) ((((short)(n)) & 0xff00) >> 8 | (((short)(n)) & 0xff) << 8)
# define htons(n) ntohs(n)
#elif !defined(ntohs)
# define ntohs(n) ((short)(n))
# define htons(n) ntohs(n)
#endif

#if !defined(ntohl) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
# define ntohl(x) ((((x)&0xff000000)>>24) \
                  |(((x)&0x00ff0000)>>8)  \
                  |(((x)&0x0000ff00)<<8)  \
                  |(((x)&0x000000ff)<<24))
# define htonl(n) ntohl(n)
#elif !defined(ntohl)
# define ntohl(n) ((long)(n))
# define htonl(n) ntohs(n)
#endif

/* p2p  CE=0 (end devices) A1=0 (no end dev assoc) same ID/CH
 * coordinator: CE=1, A2=n (coordinator assoc)
 *   SP= sleep perd ST= time betw sleep (should be same on
 *   coord/noncoord)
 * assoc - coord'd only; comm between modules thru coord'r
 *  PAN's - need coordinator.  A1 allows totally dynamic assoc
 */
 
 
/* --- General XBee Definitions --- */
 
/* "Start of packet" byte; always sent as the first
 *  byte of each packet
 */ 
#define XBEE_PKT_START 0x7e


/* Maximum packet size; datasheet basically says 100 payload bytes max */
#define XBEE_MAX_DATA_LEN        128


/* --- Bits in packets --- */

/* Communication status bits */

#define XBEE_STATUS_HW_RESET      0x01
#define XBEE_STATUS_WD_RESET      0x02
#define XBEE_STATUS_ASSOC         0x04
#define XBEE_STATUS_DISASSOC      0x08
#define XBEE_STATUS_SYNC_LOST     0x10
#define XBEE_STATUS_COORD_REALIGN 0x20
#define XBEE_STATUS_COORD_RESET   0x40

/* Command status bits */

#define XBEE_CMDSTATUS_OK  0
#define XBEE_CMDSTATUS_ERR 1

/* Transmit options */

#define XBEE_TX_FLAG_NO_ACK 0x01
#define XBEE_TX_FLAG_SEND_BCAST_PAN_ID 0x04

/* Transmit status bits */

#define XBEE_TXSTATUS_SUCCESS  0x00
#define XBEE_TXSTATUS_NO_ACK   0x01
#define XBEE_TXSTATUS_CCA_FAIL 0x02
#define XBEE_TXSTATUS_PURGES   0x03

/* Received options */

#define XBEE_RX_FLAG_ADDR_BCAST 0x02
#define XBEE_RX_FLAG_PAN_BCAST  0x04


/* --- Definitions & macros for library use --- */

/* For tracking memory allocations  */
#define XBEE_RECV                0x00
#define XBEE_XMIT                0x01

/* Initialize an XBee header */
#define XBEE_HDR_INIT(hdr, data_len) \
         ((hdr).start = 0x7e, (hdr).len = htons(data_len)) 

/* To get the length of the data portion of a received packet */

#define XBEE_RECV_A64_DATA_LEN(pkt) (ntohs(pkt->hdr.len) - 11)
#define XBEE_RECV_A16_DATA_LEN(pkt) (ntohs(pkt->hdr.len) - 5)
#define XBEE_CMD_RESP_PARAM_LEN(pkt) (ntohs(pkt->hdr.len) - 5)

#ifdef XBEE_ALLOC
# define XBEE_ALLOC_PKT(dir, data_len) \
   (xXBeePkt *)XBEE_ALLOC_BUF((dir), (data_len) + sizeof(xXBeePktHdr) + 1)
#endif

/* Types of packets from/to xbee modules; these are used
 *  in the "type" field of each packet structure
 */
 
typedef enum {
    XBEE_PKT_TYPE_TX64              = 0x00,
    XBEE_PKT_TYPE_TX16              = 0x01,
    XBEE_PKT_TYPE_ATCMD             = 0x08,
    XBEE_PKT_TYPE_QATCMD            = 0x09, /* wait til an immed param or apply cmd */
    XBEE_PKT_TYPE_ZB_TX_REQ         = 0x10,
    XBEE_PKT_TYPE_ZB_CMD_FRAME      = 0x11, /* Not yet impl */
    XBEE_PKT_TYPE_REMOTE_ATCMD      = 0x17,
    XBEE_PKT_TYPE_RX64              = 0x80,
    XBEE_PKT_TYPE_RX16              = 0x81,
    XBEE_PKT_TYPE_RX64_IO           = 0x82,
    XBEE_PKT_TYPE_RX16_IO           = 0x83,
    XBEE_PKT_TYPE_ATCMD_RESP        = 0x88,
    XBEE_PKT_TYPE_TX_STATUS         = 0x89,
    XBEE_PKT_TYPE_MODEM_STATUS      = 0x8a,
    XBEE_PKT_TYPE_ZB_TX_STATUS      = 0x8b, /* Not yet impl */
    XBEE_PKT_TYPE_ADV_MODEM_STATUS  = 0x8c, /* Not yet impl */
    XBEE_PKT_TYPE_ZB_RX             = 0x90,
    XBEE_PKT_TYPE_ZB_RX_IO          = 0x92, /* Not yet impl */
    XBEE_PKT_TYPE_RX_SENSOR         = 0x94, /* Not yet impl */
    XBEE_PKT_TYPE_NODE_IDENT        = 0x95, /* Not yet impl */
    XBEE_PKT_TYPE_REMOTE_ATCMD_RESP = 0x97,
} eXBeePktType;


/* --- Packet layouts --- */

typedef struct xXBeeAtCmdPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t frame_id;
    uint8_t command[2];
    uint8_t param[0];
} __attribute__ ((__packed__)) xXBeeAtCmdPkt;


typedef struct xXBeeRemoteAtCmdPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t frame_id;
    uint8_t dest64[8];
    uint8_t dest16[2];
    uint8_t apply;
    uint8_t command[2];
    uint8_t param[0];
} __attribute__ ((__packed__)) xXBeeRemoteAtCmdPkt;


typedef struct xXBeeA64TxPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t frame_id;
    uint8_t dest[8];
    uint8_t opt;
    uint8_t data[0];
} __attribute__ ((__packed__)) xXBeeA64TxPkt;


typedef struct xXBeeA16TxPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t frame_id;
    uint8_t dest[2];
    uint8_t opt;
    uint8_t data[0];
} __attribute__ ((__packed__)) xXBeeA16TxPkt;


typedef struct xXBeeTxStatusPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t frame_id;
    uint8_t status;
} __attribute__ ((__packed__)) xXBeeTxStatusPkt;


typedef struct xXBeeModemStatusPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t status;
} __attribute__ ((__packed__)) xXBeeModemStatusPkt;


typedef struct xXBeeCmdRespPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t frame_id;
    uint8_t command[2];
    uint8_t status;
    uint8_t param[0];
} __attribute__ ((__packed__)) xXBeeCmdRespPkt;


typedef struct xXBeeRemoteCmdRespPkt {
   xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t frame_id;
    uint8_t src64[8];
    uint8_t src16[2];
    uint8_t command[2];
    uint8_t status;
    uint8_t param[0];
} __attribute__ ((__packed__)) xXBeeRemoteCmdRespPkt;


typedef struct xXBeeA64RxPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t src[8];
    uint8_t rssi; /* signal strength */
    uint8_t opt;
    uint8_t data[0];
} __attribute__ ((__packed__)) xXBeeA64RxPkt;


typedef struct xXBeeA16RxPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t src[2];
    uint8_t rssi;
    uint8_t opt;
    uint8_t data[0];
} __attribute__ ((__packed__)) xXBeeA16RxPkt;


typedef struct xXBeeIoA64RxPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t src[8];
    uint8_t rssi; /* signal strength */
    uint8_t opt;
    uint8_t num_samples;
    uint16_t ch_ind; /* bits 14-9: a5-a0 bits 8-0: d8-d0 active */
    uint16_t data[0]; /* First sample digital if any digital chan active
                          rest are 16-bit analog rdgs                  */
} __attribute__ ((__packed__)) xXBeeIoA64RxPkt;


typedef struct xXBeeIoA16RxPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t src[2];
    uint8_t rssi;
    uint8_t opt;
    uint8_t num_samples;
    uint16_t ch_ind; /* bits 14-9: a5-a0 bits 8-0: d8-d0 active */
    uint16_t data[0]; /* First sample digital if any digital chan active
                          rest are 16-bit analog rdgs                  */
} __attribute__ ((__packed__)) xXBeeIoA16RxPkt;


typedef struct xXBeeZbTxReqPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t frame_id;
    uint8_t dest64[8];
    uint8_t dest16[2];
    uint8_t radius;
    uint8_t opt;      /* Multicast = bit 3 */
    uint8_t data[0];  /* Up to 72 bytes/pkt */
} __attribute__ ((__packed__)) xXBeeZbTxReqPkt;


typedef struct xXBeeZbRxPkt {
    xXBeePktHdr  hdr;
    uint8_t type;
    uint8_t src64[8];
    uint8_t src16[2];
    uint8_t opt;
    uint8_t data[0];  /* Up to 72 bytes/pkt */
} __attribute__ ((__packed__)) xXBeeZbRxPkt;


/* 
 * Receive data
 * 
 * calling iXBeeRecvPktCB on each packet when it's done assembling; this should 
 * be called with raw data from UART, etc.
 * as it comes in.  *** YOU NEED TO CALL THIS ***
 */
void vXBeeIn (xXBee *xbee, const void *data, uint8_t len);

/*----------------------------------------------------------------------------
                MUST be provided externally to this package
 ----------------------------------------------------------------------------*/

/* 
 * Queue a packet for transmission
 * 
 * needs to queue packet to be sent to XBEE module; e.g. copy the packet to a 
 * UART buffer.
 *  On error, -1 should be returned and the packet should NOT be freed.
 *  On success, 0 should be returned; if XBEE_ALLOC is set, this function or
 *   someone downstream is responsible for freeing it -- the packet has been
 *   handed off.  This is to minimize copying of data.
 */
int iXBeeOut (xXBee *xbee, xXBeePkt *pkt, uint8_t len);

/*----------------------------------------------------------------------------
  Must be provided externally only if using dynamic memory (which allows more
                     than one packet to be queued at a time)
 ----------------------------------------------------------------------------*/

/* 
 * Return a buffer for an xbee packet
 * 
 * at least <len> bytes need to be allocated
 * Direction since we may want to have different allocations mechanisms/etc
 * for xmit vs recv.
 */
void *pvXBeeAllocPkt (xXBee *xbee, uint8_t direction, uint8_t len);

/* 
 * Free up an allocated packet
 */
void vXBeeFreePkt (xXBee *xbee, xXBeePkt *pkt);

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* #ifndef _AVRIO_XBEE_PROTOCOL_H_ ... */

