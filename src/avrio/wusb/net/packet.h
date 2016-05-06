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
 * @file packet.h
 * @brief Modélisation d'un paquet WirelessUSB CYWUSB6935
 *
 * Revision History ------------------------------------------------------------
 *    20111113 - Initial version by epsilonRT
 */
#ifndef _AVRIO_WUSB_PACKET_H_
#define _AVRIO_WUSB_PACKET_H_

#include <avrio/defs.h>

__BEGIN_C_DECLS
/* ========================================================================== */
/**
 * @addtogroup wusb_module
 * @{
 *
 *  @defgroup wusb_packet Paquet réseau WirelessUSB Nto1
 *  Couche Liaison - OSI2 Data Link Layer
 *  @{
 */

/* == <API> == public application interface  ========class xWusbPacket======== */
/* constants ================================================================ */

/**
 * @def WPKT_BIND_CHECKSUM_SEED
 * @brief Valeur initiale dans le calcul du checksum d'un paquet BIND 
 */
# define WPKT_BIND_CHECKSUM_SEED    0x00

/**
 * @def WPKT_BIND_CRC_SEED
 * @brief Valeur initiale dans le calcul du CRC d'un paquet BIND 
 */
# define WPKT_BIND_CRC_SEED         0x0000

/* types ==================================================================== */
/**
 * @enum eWPktFlag
 * @brief Index des drapeaux de l'entête d'un paquet 
 */ 
typedef enum  {

  WPKT_SINGLE = 0,
  WPKT_ACK = 1,
  WPKT_SYNC = 2,
  WPKT_TOGGLE = 3,
  WPKT_VALID = 2,
  WPKT_RESP = 3,
  WPKT_BCAST = 2
} eWPktFlag;

/**
 * @enum eWPktType
 * @brief Identifiant du type de paquet stocké dans l'entête d'un paquet   
 */ 
typedef enum {

  WPKT_BIND_REQUEST_PACKET = 0x00,
  WPKT_BIND_RESPONSE_PACKET = 0x01,
  WPKT_PING_PACKET = 0x02,
  WPKT_ACK_PACKET = 0x03,
  WPKT_DATA_PACKET = 0x04,
  NO_PACKET = 0x0F
} eWPktType;

/* structures =============================================================== */
/**
 * @struct xWusbPacket
 * @brief Paquet réseau WirelessUSB  
 */ 
typedef struct xWusbPacket {

  uint8_t xSize;    /**< Taille du paquet en octets */
  uint8_t xLen;     /**< Nombre d'octets dans le paquet */
  uint8_t * pData;  /**< Pointeur sur les octets */
} xWusbPacket;
 
#if defined(__DOXYGEN__)
/* constants ================================================================ */
/**
 * @def WPKT_PING_PACKET_SIZE
 * @brief Taille d'un paquet de PING
 */
# define WPKT_PING_PACKET_SIZE

/**
 * @def WPKT_BINDREQ_PACKET_SIZE
 * @brief Taille d'un paquet de réponse BIND
 */
# define WPKT_BINDREQ_PACKET_SIZE

/**
 * @def WPKT_DID_OFFSET
 * @brief Offset du DeviceId dans le paquet 
 */
# define WPKT_DID_OFFSET

/**
 * @def WPKT_MID_OFFSET
 * @brief Offset du MID dans un paquet BIND_REQUEST
 */
# define WPKT_MID_OFFSET

/* macros =================================================================== */
/**
 * @def WPKT_DECLARE(xVarName, xBufferSize)
 * @brief Déclare une variable de type xWusbPacket
 *
 * @param xVarName Nom de la variable
 * @param xBufferSize Nombre d'octets pouvant être stockés dans le paquet
 */
# define WPKT_DECLARE(xVarName, xBufferSize)

/**
 * @def WPKT_STATIC_DECLARE(xVarName, xBufferSize)
 * @brief Déclare une variable static de type xWusbPacket
 *
 * @param xVarName Nom de la variable
 * @param xBufferSize Nombre d'octets pouvant être stockés dans le paquet
 */
# define WPKT_STATIC_DECLARE(xVarName, xBufferSize)

/**
 * @brief Initialise les champs de la structure du paquet
 */ 
inline void vWPktInit(xWusbPacket * pxPkt, uint8_t * pucBuffer, uint8_t ucBufferSize);

/**
 * @brief Initialise le paquet en l'effacant et en ajoutant l'entête fourni.
 */ 
void vWPktPrepare(xWusbPacket * pxPkt, uint8_t ucHeader);

/**
 * @brief Efface un paquet 
 */ 
inline void vWPktClear(xWusbPacket * pxPkt);

/**
 * @brief Renvoie la valeur d'un octet du paquet  
 */ 
uint8_t ucWPktByte(xWusbPacket * pxPkt, uint8_t Offset);

/**
 * @brief Modifie la valeur d'un octet du paquet  
 */ 
void vWPktSetByte(xWusbPacket * pxPkt, uint8_t Offset, uint8_t ucValue);

/**
 * @brief Ajoute un octet au paquet 
 */ 
void vWPktAddByte(xWusbPacket * pxPkt, uint8_t ucValue);

/**
 * @brief Ajoute une suite d'octets au paquet  
 */ 
void vWPktAddBytes(xWusbPacket * pxPkt, const uint8_t * pucBytes, uint8_t ucLength);

/**
 * @brief Ajoute une suite d'octets au paquet  
 */ 
void vWPktAddBytes_P(xWusbPacket * pxPkt, const uint8_t * pucBytes, uint8_t ucLength);

/**
 * @brief Ajoute une chaîne de caractères sans le zéro terminal au paquet  
 */ 
void vWPktAddStr(xWusbPacket * pxPkt, const char* pcStr);

/**
 * @brief Ajoute une chaîne de caractères sans le zéro terminal au paquet  
 */ 
void vWPktAddStr_P(xWusbPacket * pxPkt, const char* pcStr);

/**
 * @brief Renvoie la valeur d'un mot de 16 bits (little endian) du paquet
 */ 
uint16_t usWPktWord(xWusbPacket * pxPkt, uint8_t Offset);

/**
 * @brief Ajoute un mot de 16 bits (little endian) au paquet 
 */ 
void vWPktAddWord(xWusbPacket * pxPkt, uint16_t usValue);

/**
 * @brief Renvoie la valeur d'un mot de 16 bits (little endian) du paquet
 */ 
uint32_t ulWPktDword(xWusbPacket * pxPkt, uint8_t Offset);

/**
 * @brief Renvoie la valeur de l'en-tête du paquet 
 */ 
inline uint8_t ucWPktHeader(xWusbPacket * pxPkt);

/**
 * @brief Modifie l'en-tête préexistant du paquet  
 */ 
inline void vWPktSetHeader(xWusbPacket * pxPkt, uint8_t ucValue);

/**
 * @brief Renvoie le type du paquet 
 */ 
uint8_t ucWPktType(xWusbPacket * pxPkt);

/**
 * @brief Modifie le type de paquet
 */ 
void vWPktSetType(xWusbPacket * pxPkt, uint8_t ucValue);

/**
 * @brief Renvoie l'état d'un drapeau du paquet 
 */ 
inline bool xWPktFlag(xWusbPacket * pxPkt, uint8_t ucIndex);

/**
 * @brief Modifie l'état d'un bit de flag de l'entête d'un paquet 
 */ 
inline void vWPktSetFlag(xWusbPacket * pxPkt, uint8_t ucIndex, bool xValue);

/**
 * @brief Modifie l'état des bits de flag de l'entête d'un paquet 
 */ 
bool xWPktSetAllFlags(xWusbPacket * pxPkt, uint8_t ucFlags);

/**
 * @brief Bascule l'état d'un drapeau du paquet 
 */ 
inline void vWPktToggleFlag(xWusbPacket * pxPkt, uint8_t ucIndex);

/**
 * @brief Ajoute le CRC et le Checksum au paquet
 * Si le paquet est de type BIND ou PING Broadcast, le Crc et le Checksum 
 * ajoutés sont calculés avec des valeurs initiales WPKT_BIND_CRC_SEED et 
 *  WPKT_BIND_CHECKSUM_SEED. 
 */ 
void vWPktAddTrailer (xWusbPacket * pxPkt, uint8_t CrcSeed, uint8_t ChecksumSeed);

/**
 * @brief Renvoie un pointeur sur la zone mémoire où sont stockés les octets du paquet  
 */ 
inline uint8_t * pucWPktRaw(xWusbPacket * pxPkt);

/**
 * @brief Renvoie un pointeur sur la zone mémoire où sont stockés les octets du payload
 * @return le pointeur ou 0 si le paquet n'est pas de type WPKT_DATA_PACKET    
 */ 
uint8_t * pucWPktDataPayload(xWusbPacket * pxPkt);

/**
 * @brief Renvoie le nombre d'octets du payload  
 * @return la longueur ou 0 si le paquet n'est pas de type WPKT_DATA_PACKET    
 */ 
uint8_t xWPktDataPayloadLength(xWusbPacket * pxPkt);

/**
 * @brief Indique si le paquet est plein  
 */ 
inline bool xWPktIsFull(xWusbPacket * pxPkt);

/**
 * @brief Indique si le paquet est vide  
 */ 
inline bool xWPktIsEmpty(xWusbPacket * pxPkt);

/**
 * @brief Renvoie le nombre d'octets stockés dans le paquet  
 */ 
inline uint8_t xWPktLength(xWusbPacket * pxPkt);

/**
 * @brief Renvoie la taille de la zone mémoire de stockage 
 */ 
inline uint8_t xWPktSize(xWusbPacket * pxPkt);

/**
 * @brief Vérifie un paquet et le corrige éventuellement
 * @param pxPkt paquet à vérifier (DATA)
 * @param Valid paquet contenant les octets de validation du paquet à vérifier (VALID)
 * @param CrcSeed valeur initiale du Crc, ignoré si le paquet est un BIND ou un PING Broadcast
 * @param ChecksumSeed Valeur initiale du Checksum, ignoré si le paquet est un BIND ou un PING Broadcast
 * @return true si le paquet est correct ou s'il a pu être réparé    
 */ 
bool xWPktCheckAndRepair(xWusbPacket * pxPkt, xWusbPacket * Valid, uint8_t CrcSeed, uint8_t ChecksumSeed);

/**
 * @brief Calcule le Crc 16 bits CCITT (X^16 + X^12 + X^5 + 1) d'une suite d'octets
 */ 
uint16_t usWPktCrc (const uint8_t * pData, uint8_t ucLength, uint16_t Seed);

/**
 * @brief Calcule le checksum d'une suite d'octets
 */ 
uint8_t ucWPktChecksum (const uint8_t * pData, uint8_t ucLength, uint8_t Seed);

/**
 * @brief Renvoie l'état du bit de rang \c Bit de l'octet \c Byte
 */ 
bool xWPktBit(uint8_t Byte, uint8_t Bit);

/**
 * @brief Modifie l'état du bit de rang \c Bit de l'octet pointé par \c Byte
 */ 
void vWPktSetBit(uint8_t * Byte, uint8_t Bit, bool xValue);
/* == <API-END> == public application interface  ====class xWusbPacket======== */
/**
 *  @}
 * @}
 */

#else  /* __DOXYGEN__ defined */
# include "wusb-config.h"
# include <avr/pgmspace.h>

/* constants ================================================================ */

/* Packet sizes */
# define WPKT_HEADER_SIZE           1 /* Type + ucFlags */
# define WPKT_MID_SIZE              4
# define WPKT_CRC_SIZE              2
# define WPKT_CS_SIZE               1
# define WPKT_TRAILER_SIZE          (WPKT_CRC_SIZE + WPKT_CS_SIZE)
# define WPKT_BROADCAST_PACKET_SIZE (WPKT_HEADER_SIZE + WPKT_TRAILER_SIZE)
# define WPKT_PING_PACKET_SIZE      (WPKT_BROADCAST_PACKET_SIZE)
# define WPKT_BINDREQ_PACKET_SIZE     (WPKT_BROADCAST_PACKET_SIZE + WPKT_MID_SIZE)

/* Offsets */
# define WPKT_DID_OFFSET             WPKT_HEADER_SIZE
# define WPKT_MID_OFFSET             WPKT_HEADER_SIZE  /* Pour BIND_REQUEST */

/* Tailles dépendantes de la taille du device id */
# define WPKT_DID_SIZE_MAX          2
# define WPKT_NULL_DATA_PACKET_SIZE_MAX (WPKT_BROADCAST_PACKET_SIZE + WPKT_DID_SIZE_MAX)
# define WPKT_WPKT_ACK_PACKET_SIZE_MAX        WPKT_NULL_DATA_PACKET_SIZE_MAX
# define WPKT_BIND_RESPONSE_SIZE_MAX    (WPKT_NULL_DATA_PACKET_SIZE_MAX + WPKT_MID_SIZE + 2)

/* macros =================================================================== */
# define WPKT_PACKET_SIZE_MAX(PayloadSize) ( \
                                      WPKT_NULL_DATA_PACKET_SIZE_MAX + \
                                      PayloadSize)


// -----------------------------------------------------------------------------
# define WPKT_DECLARE(__name,__size)                 \
  uint8_t __name ## Buffer[__size];                  \
  xWusbPacket __name = { .pData  = __name ## Buffer, \
                         .xSize  = __size,           \
                         .xLen   = 0 }

// -----------------------------------------------------------------------------
# define WPKT_STATIC_DECLARE(__name,__size)                 \
  static uint8_t __name ## Buffer[__size];                  \
  static xWusbPacket __name = { .pData  = __name ## Buffer, \
                                .xSize  = __size }

  /* Checked = Ok */ uint8_t   ucWPktByte(xWusbPacket *, uint8_t);
  /* Checked = Ok */ void      vWPktSetByte(xWusbPacket *, uint8_t, uint8_t);
  /* Checked = Ok */ void      vWPktAddByte(xWusbPacket * pxPkt, uint8_t ucValue);
  /* Checked = Ok */ void      vWPktAddBytes(xWusbPacket * pxPkt, const uint8_t * pucBytes, uint8_t ucLength);
  /* Checked = Ok */ void      vWPktAddStr(xWusbPacket * pxPkt, const char* pcStr);
  /* Checked = Ok */ void      vWPktAddBytes_P(xWusbPacket * pxPkt, const uint8_t * pucBytes, uint8_t ucLength);
  /* Checked = Ok */ void      vWPktAddStr_P(xWusbPacket * pxPkt, const char* pcStr);
  /* Checked = Ok */ uint8_t * pucWPktCurrentByte (xWusbPacket * pxPkt);

  /* Checked = Ok */ uint16_t  usWPktWord(xWusbPacket * pxPkt, uint8_t Offset);
  /* Checked = Ok */ void      vWPktAddWord(xWusbPacket * pxPkt, uint16_t usValue);

  /* Checked = Ok */ uint32_t  ulWPktDword(xWusbPacket * pxPkt, uint8_t Offset);

  /* Checked = Ok */ bool      xWPktBit(uint8_t Byte, uint8_t Bit);
  /* Checked = Ok */ void      vWPktSetBit(uint8_t * Byte, uint8_t Bit, bool xValue);

  /* Checked = Ok */ uint8_t   ucWPktType(xWusbPacket *);
  /* Checked = Ok */ void      vWPktSetType(xWusbPacket *, uint8_t);
  void      xWPktSetAllFlags(xWusbPacket *, uint8_t);

  uint8_t * pucWPktDataPayload(xWusbPacket * pxPkt);
  uint8_t   xWPktDataPayloadLength(xWusbPacket * pxPkt);

  /* Checked = Ok */ uint16_t  usWPktCrc (const uint8_t * pData, uint8_t ucLength, uint16_t Seed);
  /* Checked = Ok */ uint8_t   ucWPktChecksum (const uint8_t * pData, uint8_t ucLength, uint8_t Seed);
  /* Checked = Ok */ void      vWPktAddTrailer (xWusbPacket * pxPkt, uint8_t CrcSeed, uint8_t ChecksumSeed);
  /* Checked = Ok */ bool      xWPktCheckAndRepair(xWusbPacket * pxPkt, xWusbPacket * Valid, uint8_t CrcSeed, uint8_t ChecksumSeed);
  
  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (void 
  vWPktInit(xWusbPacket * pxPkt, uint8_t * pucBuffer, uint8_t ucBufferSize)) {
  
    pxPkt->pData = pucBuffer;
    pxPkt->xSize = ucBufferSize;
    pxPkt->xLen  = 0;  
  }
  
  // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (uint8_t * 
  pucWPktRaw(xWusbPacket * pxPkt)) {
  
    return pxPkt->pData;
  }

  // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (uint8_t 
    xWPktLength(xWusbPacket * pxPkt)) {

    return pxPkt->xLen;
  }

  // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (uint8_t 
    xWPktSize(xWusbPacket * pxPkt)) {

    return pxPkt->xSize;
  }

   // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (void 
    vWPktClear(xWusbPacket * pxPkt)) {

    pxPkt->xLen = 0;
  }

  // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (void 
    vWPktPrepare(xWusbPacket * pxPkt, uint8_t ucHeader)) {

    vWPktClear (pxPkt);
    vWPktAddByte (pxPkt, ucHeader); 
  }

  // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (bool 
    xWPktIsFull(xWusbPacket * pxPkt)) {

    return (pxPkt->xLen == pxPkt->xSize);
  }

  // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (bool 
    xWPktIsEmpty(xWusbPacket * pxPkt)) {

    return (pxPkt->xLen == 0);
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (void 
    vWPktSetHeader(xWusbPacket * pxPkt, uint8_t ucValue)) {

    vWPktSetByte (pxPkt, 0, ucValue); 
  }

  // ---------------------------------------------------------------------------
  __STATIC_ALWAYS_INLINE (uint8_t 
  ucWPktHeader(xWusbPacket * pxPkt)) {

    return ucWPktByte(pxPkt, 0);
  }

  // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (void 
    vWPktSetFlag(xWusbPacket * pxPkt, uint8_t ucIndex, bool xValue)) {
    
    vWPktSetBit (pxPkt->pData, ucIndex, xValue);
  } 

  // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (void 
    vWPktToggleFlag(xWusbPacket * pxPkt, uint8_t ucIndex)) {

    *pxPkt->pData ^= _BV(ucIndex);
  } 

  // ---------------------------------------------------------------------------
  /* Checked = Ok */ __STATIC_ALWAYS_INLINE (bool 
    xWPktFlag(xWusbPacket * pxPkt, uint8_t ucIndex)) {

    return (*pxPkt->pData & _BV(ucIndex)) != 0;
  } 

# ifdef DEBUG_ONUART
  void vWPktDbgDump (xWusbPacket * pxPkt);
# else
#  define vWPktDbgDump(p)
# endif  /* DEBUG_ONUART defined */

# ifdef WUSB_WPKT_TEST
  int iWPktTest(void);
# else   /* WUSB_WPKT_TEST defined */
#  define iWPktTest() (0)
# endif  /* WUSB_WPKT_TEST defined */

#endif  /* __DOXYGEN__ defined */
/* ========================================================================== */
__END_C_DECLS
#endif  /* _AVRIO_WUSB_PACKET_H_ */
