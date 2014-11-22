/**
 * @file xbee.h
 * @brief Maxstream XBee module Interface Header
 * @author Copyright © 2006-2008 Tymm Twillman <tymm@booyaka.com>
 * @author Copyright © 2014 epsilonRT. All rights reserved.
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
 *  @author Copyright © 2014 epsilonRT. All rights reserved.
 *  @copyright GNU Lesser General Public License version 3
 *             <http://www.gnu.org/licenses/lgpl.html>
 *  @{
 */

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


/* macros =================================================================== */

/**
 * @brief
 *
 * This is used for keeping track of your data as things get passed around
 *  through the xbee interface
 */
#define pvXBeeUserContext(xbee) ((xbee).user_context)


/* types ==================================================================== */
#ifndef __DOXYGEN__
struct xXBeePkt;
struct xXBee;
#endif

/**
 * @brief Receive callback function
 */
typedef int (*iXBeeRxCB) (struct xXBee *xbee, struct xXBeePkt *pkt, uint8_t len);

/**
 * @brief Receive callback type
 */
typedef enum {
  XBEE_CB_AT_LOCAL      = 0,
  XBEE_CB_AT_REMOTE     = 1,
  XBEE_CB_DATA          = 2,
  XBEE_CB_IO            = 3,
  XBEE_CB_TX_STATUS     = 4,
  XBEE_CB_MODEM_STATUS  = 5,
  XBEE_CB_SENSOR        = 6, /* S2 Series only */
  XBEE_CB_UNKNOWN       = -1
} eXBeeCbType;

/**
 * Types of packets from/to xbee modules; these are used in the "type" field
 * of each packet structure
 */
typedef enum {
  /* S1 & S2 Series */
  XBEE_PKT_TYPE_ATCMD             = 0x08, 
  XBEE_PKT_TYPE_QATCMD            = 0x09, /* wait til an immed param or apply cmd */
  XBEE_PKT_TYPE_REMOTE_ATCMD      = 0x17,
  XBEE_PKT_TYPE_ATCMD_RESP        = 0x88,
  XBEE_PKT_TYPE_MODEM_STATUS      = 0x8a,
  XBEE_PKT_TYPE_REMOTE_ATCMD_RESP = 0x97,

  /* S1 Series */
  XBEE_PKT_TYPE_TX64              = 0x00,
  XBEE_PKT_TYPE_TX16              = 0x01,
  XBEE_PKT_TYPE_RX64              = 0x80,
  XBEE_PKT_TYPE_RX16              = 0x81,
  XBEE_PKT_TYPE_RX64_IO           = 0x82,
  XBEE_PKT_TYPE_RX16_IO           = 0x83,
  XBEE_PKT_TYPE_TX_STATUS         = 0x89,

  /* S2 Series */
  XBEE_PKT_TYPE_ZB_TX_REQ         = 0x10,
  XBEE_PKT_TYPE_ZB_CMD_FRAME      = 0x11, /* Not yet impl */
  XBEE_PKT_TYPE_ZB_TX_STATUS      = 0x8b,
  XBEE_PKT_TYPE_ZB_RX             = 0x90,
  XBEE_PKT_TYPE_ZB_RX_IO          = 0x92,
  XBEE_PKT_TYPE_ZB_RX_SENSOR      = 0x94,
  XBEE_PKT_TYPE_ZB_NODE_IDENT     = 0x95, /* Not yet impl */
} eXBeePktType;

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
 * @brief Generic Packet
 * Packets can be broken up into headers, a packet type, a number of data
 *  bytes and a crc (at the end of the data)
 */
typedef struct xXBeePkt {
  xXBeePktHdr  hdr;
  uint8_t         type;
  uint8_t         data[0];
} __attribute__ ( (__packed__)) xXBeePkt;

/**
 * @brief Context for tracking current state of communication with an XBee module
 */
#if defined(__DOXYGEN__)
typedef struct xXBee xXBee;
#else
typedef struct xXBee {
  struct {
    uint8_t bytes_left;
    uint8_t bytes_rcvd;
    xXBeePkt *packet;
    uint8_t hdr_data[sizeof (xXBeePktHdr)];
    iXBeeRxCB user_cb[7];
  } __attribute__ ( (__packed__)) in;
  struct {
    uint8_t frame_id;
  } __attribute__ ( (__packed__)) out;
  FILE * io_stream;
  void *user_context; // yours to pass data around with
} __attribute__ ( (__packed__)) xXBee;
#endif


/* internal public functions ================================================ */
/**
 * @brief Initialize the XBee interface
 * @param io_stream stream to read and write bytes to the XBEE module. It must
 *        be a stream opened for reading and writing, read must be opened
 *        in non-blocking mode.
 */
void vXBeeInit (xXBee *xbee, FILE * io_stream);

/**
 * @brief Set receive callbacks
 */
void vXBeeSetCB (xXBee *xbee, eXBeeCbType cb_type, iXBeeRxCB cb);

/**
 * @brief Poll the inputstream to read incoming bytes
 *
 * You must call this function as often as possible
 * *** YOU NEED TO CALL THIS ***
 */
int iXBeePoll (xXBee *xbee);

/**
 * @brief Send a command to the xbee modem
 */
int iXBeeSendAt (xXBee *xbee,
                 const char cmd[],
                 const uint8_t *params,
                 uint8_t param_len);

/**
 * @brief Send a command to a remote xbee modem (Series 2 & Newer Series 1 only)
 */
int iXBeeSendRemoteAt (xXBee *xbee,
                       const char cmd[],
                       const uint8_t params[],
                       uint8_t param_len,
                       const uint8_t addr64[8],
                       const uint8_t addr16[2],
                       uint8_t apply);


/**
 * @brief Send a zigbee data packet to a remote xbee modem (Series 2)
 * @return frame id or -1 on error
 */
int iXBeeZbSend (xXBee *xbee,
                 const void *data,
                 uint8_t len,
                 const uint8_t addr64[8],
                 const uint8_t addr16[2], 
                 uint8_t opt, 
                 uint8_t radius);

/**
 * @brief Send a zigbee data packet to the coordinator (Series 2)
 * @return frame id or -1 on error
 */
int iXBeeZbSendToCoordinator (xXBee *xbee, const void *data, uint8_t len);

/**
 * @brief Send a packet with a 64-bit destination address (Series 1)
 * @return frame id or -1 on error
 */
int iXBeeSend64 (xXBee *xbee,
                  const void *data,
                  uint8_t len,
                  const uint8_t addr[8],
                  uint8_t opt);

/**
 * @brief Send a packet with a 16-bit destination address (Series 1)
 * @return frame id or -1 on error
 */
int iXBeeSend16 (xXBee *xbee,
                  const void *data,
                  uint8_t len,
                  const uint8_t addr[2],
                  uint8_t opt);

/**
 * @brief Return a buffer for an xbee packet
 *
 * at least <len> bytes need to be allocated
 * Direction since we may want to have different allocations mechanisms/etc
 * for xmit vs recv.
 */
void *pvXBeeAllocPkt (xXBee *xbee, uint8_t direction, uint8_t len);

/**
 * @brief Free up an allocated packet
 */
void vXBeeFreePkt (xXBee *xbee, xXBeePkt *pkt);

/**
 * @brief Lecture du type de paquet
 */
uint8_t ucXBeePktType (xXBeePkt *pkt);

/**
 * @brief Vérifie l'égalité de 2 adresses réseau de len octets
 */
bool bXBeePktAddressIsEqual (const uint8_t *a1, const uint8_t *a2, uint8_t len);

/**
 * @brief Adresse 16-bits inconnue (0xFFFE)
 */
const uint8_t * pucXBeeAddr16Unknown (void);

/**
 * @brief Adresse 64-bits inconnue (0xFFFFFFFFFFFFFFFF)
 */
const uint8_t * pucXBeeAddr64Unknown (void);

/**
 * @brief Adresse 64-bits du cordinateur Zigbee (0x0000000000000000)
 */
const uint8_t * pucXBeeAddr64Coordinator (void);

/**
 * @brief Adresse 64-bits de broadcast (0x000000000000FFFF)
 */
const uint8_t * pucXBeeAddr64Broadcast (void);

/**
 * @brief Lecture de la partie haute de l'adresse 64-bits source du paquet
 * @return pointeur sur l'adresse (Big Endian) ou 0 si erreur
 */
uint8_t * pucXBeePktAddrSrc64 (xXBeePkt *pkt);

/**
 * @brief Lecture du pointeur sur l'adresse réseau 16-bits source du paquet (Big endian)
 * @return pointeur sur l'adresse (Big Endian) ou 0 si erreur
 */
uint8_t * pucXBeePktAddrSrc16 (xXBeePkt *pkt);

/**
 * @brief Lecture du pointeur sur l'adresse réseau 16-bits destination du paquet (Big endian)
 * @return pointeur sur l'adresse (Big Endian) ou 0 si erreur
 */
uint8_t * iXBeePktDst16 (xXBeePkt *pkt);

/**
 * @brief Lecture du pointeur sur les données du paquet
 * La suite d'octets n'est pas terminée par un octet nul (mais par le CRC du 
 * paquet qui pourra être écrasé par un octet nul)
 */
uint8_t * pucXBeePktData (xXBeePkt *pkt);

/**
 * @brief Lecture du nombre d'octet de données (CRC exclu)
 */
int iXBeePktDataLen (xXBeePkt *pkt);

/**
 * @brief Lecture de l'identifiant de paquet
 */
int iXBeePktFrameId (xXBeePkt *pkt);

/**
 * @brief Lecture du status de paquet
 */
int iXBeePktStatus (xXBeePkt *pkt);

/**
 * @brief Lecture du status de découverte de paquet
 */
int iXBeePktDiscovery (xXBeePkt *pkt);

/**
 * @brief Lecture du nombre de tentatives de paquet
 */
int iXBeePktRetry (xXBeePkt *pkt);

/*==============================================================================
 * 
 * TODO: Partie non finalisée par manque de temps
 * 
 *============================================================================*/
/*
int getAddress16 (unsigned char addr[2]);
int getAddress64 (unsigned char addr[8]);
int getApiID (void);
int getDigital (int array[9], unsigned int index = 0);
int getAnalog (int array[6], unsigned int index = 0);
int getRSSI (void);
int getTotalSamples (void);
int getRawPacket (unsigned char *data, int maxlen);
int getNextDataByte (void);
*/
const char * pcXBeePktCommand (xXBeePkt *pkt);
uint8_t * pucXBeePktParam (xXBeePkt *pkt);
int iXBeePktParamLen (xXBeePkt *pkt);
int iXBeePktOptions (xXBeePkt *pkt);
int iXBeePktRadius (xXBeePkt *pkt);
int iXBeePktApply (xXBeePkt *pkt);

/**
 *   @}
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif /*  _AVRIO_XBEE_H_ defined */
