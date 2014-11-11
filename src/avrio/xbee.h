/**
 * @file xbee.h
 * @brief Maxstream XBee module Interface Header
 * @author Copyright © 2006-2008 Tymm Twillman <tymm@booyaka.com>
 *
 * NOTE: This doesn't touch hardware; it's up to developers to link in functions
 *  that handle hardware communication.
 *
 *  DEVELOPERS: Pieces you need to implement (see prototypes, below):
 *    pvXBeeAllocPkt   (can just return static data)
 *    vXBeeFreePkt    (can do nothing if not dynamic)
 *
 *    iXBeeOut
 *    iXBeeRecvPktCB
 *
 *   What you need to call from wherever you read data from UART, etc:
 *    vXBeeIn
 *
 *  Incoming data from UART, etc. should be passed to vXBeeIn; it will
 *   be built into well-formed packets and passed to iXBeeRecvPktCB
 *   for further processing.
 *
 *  Outgoing data will be passed to iXBeeOut to be passed off to
 *   the XBee hardware.
 *
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20141111 - Initial version by epsilonRT
 *
 */
#ifndef _AVRIO_XBEE_H_
#define _AVRIO_XBEE_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include <errno.h>
#include <stdio.h>

/**
 * @addtogroup net_group
 * @{
 *
 *  @defgroup xbee_module XBee
 *  Ce module permet d'utiliser des modules XBee
 *
 *  @author Copyright © 2006-2008 Tymm Twillman <tymm@booyaka.com>
 *  @copyright GNU Lesser General Public License version 3
 *             <http://www.gnu.org/licenses/lgpl.html>
 *  @{
 */

/*----------------------------------------------------------------------------
               Definitions for commands the XBee recognizes
 ----------------------------------------------------------------------------*/
/* constants ================================================================ */

/* 
 * Basic communication parameters/values
 */
#define XBEE_CMD_CHANNEL                "CH"
#define XBEE_CMD_PAN_ID                 "ID"
#define XBEE_CMD_DEST_ADDR64_HI         "DH"
#define XBEE_CMD_DEST_ADDR64_LO         "DL"
#define XBEE_CMD_SRC_ADDR16             "MY"
#define XBEE_CMD_SER_HI                 "SH"
#define XBEE_CMD_SER_LO                 "SL"
#define XBEE_CMD_RAND_DLY_SLOTS         "RN"
#define XBEE_CMD_MAC_MODE               "MM"
#define XBEE_CMD_COORD_ENA              "CE"
#define XBEE_CMD_SCAN                   "SC"
#define XBEE_CMD_SCAN_DURATION          "SD"
#define XBEE_CMD_ASSOC_END              "A1"
#define XBEE_CMD_ASSOC_COORD            "A2"
#define XBEE_CMD_ASSOC_STATUS           "AI"
#define XBEE_CMD_RSSI                   "DB"

/* 
 * Transceiver Control 
 */
#define XBEE_CMD_PWR_LEVEL              "PL"
#define XBEE_CMD_CCA_THRESH             "CA"

/* 
 * Sleep Parameters
 */
#define XBEE_CMD_SLEEP_MODE             "SM"
#define XBEE_CMD_SLEEP_TIMEOUT          "ST"
#define XBEE_CMD_SLEEP_PERIOD           "SP"
#define XBEE_CMD_SLEEP_PERIOD_DISASSOC  "DP"

/* 
 * Interface parameters
 */
#define XBEE_CMD_DATA_RATE              "BD"
#define XBEE_CMD_PACKETIZATION_TIMEOUT  "RO"
#define XBEE_CMD_DIO7_CONFIG            "D7"
#define XBEE_CMD_DIO6_CONFIG            "D6"
#define XBEE_CMD_DIO5_CONFIG            "D5"
#define XBEE_CMD_DIO4_CONFIG            "D4"
#define XBEE_CMD_DIO3_CONFIG            "D3"
#define XBEE_CMD_DIO2_CONFIG            "D2"
#define XBEE_CMD_DIO1_CONFIG            "D1"
#define XBEE_CMD_DIO0_CONFIG            "D0"
#define XBEE_CMD_PWM0_CONFIG            "PO"
#define XBEE_CMD_API_ENA                "AP"
#define XBEE_CMD_PULLUP_ENA             "PR"

/* 
 * Version Info
 */
#define XBEE_CMD_VERS_FIRMWARE          "VR"
#define XBEE_CMD_VERS_HARDWARE          "HV"
#define XBEE_CMD_VERS_FIRM_VERBOSE      "VL"

/*
 * Received Signal Strength
 */
#define XBEE_CMD_RSSI_PWM_TIMER         "RP"
#define XBEE_CMD_RSS                    "DB"

/* 
 * Error counters
 */
#define XBEE_CMD_CCA_FAILS              "EC"
#define XBEE_CMD_ACK_FAILS              "EA"

/* 
 * AT Command Params
 */
#define XBEE_CMD_AT_MODE_TIMEOUT        "CT"
#define XBEE_CMD_AT_GUARD_TIME          "GT"
#define XBEE_CMD_AT_CMD_CHAR            "CC"
#define XBEE_CMD_AT_EXIT                "CN"

/* 
 * XBEE specific routing
 */
#define XBEE_CMD_NODE_FIND_DEST         "DN"
#define XBEE_CMD_NODE_DISCOVER          "ND"
#define XBEE_CMD_NODE_ID                "NI"
#define XBEE_CMD_ACTIVE_SCAN            "AS"
#define XBEE_CMD_FORCE_DISASSOC         "DA"
#define XBEE_CMD_ENERGY_SCAN            "ED"
#define XBEE_CMD_FORCE_POLL             "FP"

/* 
 * IO Line Passing / Sensor Interfacing
 */
#define XBEE_CMD_SAMPLE_RATE            "IR"
#define XBEE_CMD_SAMPLES_BEFORE_TX      "IT"

/* 
 * Misc
 */
#define XBEE_CMD_WRITE_PARAMS           "WR"
#define XBEE_CMD_RESET_SOFT             "FR"
#define XBEE_CMD_APPLY_CHANGES          "AC"
#define XBEE_CMD_RESTORE_DEFAULTS       "RE"

/* structures =============================================================== */

/** 
 * @brief Packet header
 * Packets are wrapped with a start & length
 */
typedef struct xXBeePktHdr {
  uint8_t         start; /* 0x7e */
  uint16_t        len;
} __attribute__ ( (__packed__)) xXBeePktHdr;


/** 
 * @brief Packet
 * Packets can be broken up into headers, a packet type, a number of data
 *  bytes and a crc (at the end of the data)
 */
typedef struct xXBeePkt {
  xXBeePktHdr  hdr;
  uint8_t         type;
  uint8_t         data[0];
  /* uint8_t         crc; */
} __attribute__ ( (__packed__)) xXBeePkt;


/** 
 * @brief Context for tracking current state of communication with an XBee module
 */
typedef struct {
  struct {
    uint8_t bytes_left;
    uint8_t bytes_rcvd;
    xXBeePkt *packet;
    uint8_t hdr_data[sizeof (xXBeePktHdr)];
  } in;
  struct {
    uint8_t frame_id;
  } out;
  FILE * io_stream;
  void *user_context; // yours to pass data around with
} __attribute__ ( (__packed__)) xXBee;

/** 
 * @brief 
 * 
 * This is used for keeping track of your data as things get passed around
 *  through the xbee interface
 */
#define pvXBeeUserContext(xbee) ((xbee).user_context)


/* internal public functions ================================================ */
/** 
 * @brief Initialize the XBee interface
 * @param io_stream stream to read and write bytes to the XBEE module. It must
 *        be a stream opened for reading and writing, read must be opened
 *        in non-blocking mode.
 */
void vXBeeInit (xXBee *xbee, FILE * io_stream);

/** 
 * @brief Handle an incoming packet
 * 
 * the packet will be fully formed and verified
 * for proper construction before being passed off to this function.  This
 * function should dig into the packet & process based on its contents.
 */
int iXBeeRecvPktCB (xXBee *xbee, xXBeePkt *pkt, uint8_t len);

/** 
 * @brief Send a packet with a 64-bit destination address (Series 1)
 */
int iXBeeSend64 (xXBee *xbee,
                 const void *data,
                 uint8_t len,
                 uint8_t opt,
                 const uint8_t addr[8]);

/** 
 * @brief Send a packet with a 16-bit destination address (Series 1)
 */
int iXBeeSend16 (xXBee *xbee,
                 const void *data,
                 uint8_t len,
                 uint8_t opt,
                 const uint8_t addr[2]);

/** 
 * @brief Send a command to the xbee modem
 */
int iXBeeSendAt (xXBee *xbee,
                      const char cmd[],
                      uint8_t param_len,
                      const uint8_t *params);

/** 
 * @brief Send a command to a remote xbee modem (Series 2 & Newer Series 1 only)
 */
int iXBeeSendRemoteAt (xXBee *xbee,
                             const char cmd[],
                             uint8_t param_len,
                             uint8_t apply,
                             const uint8_t params[],
                             const uint8_t addr64[8],
                             const uint8_t addr16[2]);



/** 
 * @brief Poll the inputstream to read incoming bytes
 * 
 * You must call this function as often as possible 
 * *** YOU NEED TO CALL THIS ***
 */
int iXBeePoll (xXBee *xbee);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /* #ifndef _AVRIO_XBEE_H_ ... */
