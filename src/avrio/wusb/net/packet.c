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
 * @file packet.c
 * @brief Implémentation d'un paquet WirelessUSB CYWUSB6935
 *
 * Revision History ------------------------------------------------------------
 *    20111113 - Initial version by epsilonRT
 */
#include "avrio-config.h"

#ifdef AVRIO_WUSB_ENABLE
/* ========================================================================== */
# include <avrio/wusb/net/packet.h>

/* internal private functions =============================================== */
/* --------------------------------------------------------------------------
 * 
 * Function: ValidateChecksum
 * 
 * Description: Fixes a packet if possible and validates a checksum.
 * Also detects bit inversions.
 * 
 * Returns: true if the packet is valid; otherwise, false
 * 
 * ---- Corrupt the data --------------------------------------
 * Use this code to test this function works properly.
 * Test with printing of RX packets on so you can see the action.
 * Uncomment out one test at a time
 * Uncomment the 7 lines to force a known good packet.
 * pxPkt->pData.dataptr[0] = 0x45;
 * pxPkt->pData.dataptr[1] = 0x01;
 * pxPkt->pData.dataptr[2] = 0x44;
 * Valid->pData.dataptr[0] &= 0xFF;
 * Valid->pData.dataptr[1] &= 0xFF;
 * Valid->pData.dataptr[2] &= 0xFF;
 * Nto1Dcb.Rx.Packet.ucLength = 3;

 * Test 1 - Simlate Bit Inversion
 * Result: Packet lost due bit inversion
 * pxPkt->pData.dataptr[1] |= 0x10;

 * Test 2 - Checksum: Data is correct but the bit is invalid
 * Result: Packet is recovered
 * Valid->pData.dataptr[0] &= ~0x01;

 * Test 3 - Checksum - Data bit is incorrectly set and invalid bit is cleared
 * Result: Packet is recovered
 * pxPkt->pData.dataptr[1] |= 0x04;
 * Valid->pData.dataptr[1] &= ~0x04;

 * Test 4 - Checksum - Data bit is cleared and invalid bit is cleared
 * Result: Packet is recovered
 * pxPkt->pData.dataptr[1] &= ~0x01;
 * Valid->pData.dataptr[1] &= ~0x01;
 */
static bool
prvxCheckChecksumAndRepair (xWusbPacket * pxPkt, xWusbPacket * Valid, uint8_t ChecksumSeed)
{
  bool Result = true;
  uint8_t Col;
  uint8_t Row;
  uint8_t CorrectablePacket;
  uint8_t GoodPacket;
  uint8_t Checksum = ChecksumSeed;

  CorrectablePacket = 0xFF;
  GoodPacket = 0xFF;

/* Mentally we line up all the bytes into a column
 * Count up the bit errors and figure out if it is fixable.
 * CorrectablePacket will keep track of the number of invalid bits
 * for each column.  If there is more than one invalid bit in the
 * column then the corresponding column bit in CorrectablePacket
 * will be cleared.
 */
  for (Row = 0; Row < pxPkt->xLen; ++Row) {
    uint8_t validByte;

    validByte = ucWPktByte (Valid, Row);
    CorrectablePacket &= GoodPacket | validByte;
    GoodPacket &= validByte;
    Checksum ^= ucWPktByte (pxPkt, Row); /* Valeur initiale de Checksum = Seed */
  }
  // ----------------------------------------------------------------------

  // Checksum == 0. If not we have one or more invalid bits or a bit
  // inversion.  All will be handled below.
  // 
  // The calculated checksum of the received packet XORD with the
  // checksum byte transmitted in the packet will result in a mask of bits
  // that need to be corrected or inverted to be correct.  IOW the CS of a
  // correct packet XORED with the CS byte will yield 0x00.  The CS of a
  // packet with errors XORED with the CS byte will reveal the errors.

  // ----------------------------------------------------------------------
  // Fix via Checksum
  // ----------------------------------------------------------------------
  if (CorrectablePacket != 0xFF) {
  
    /* Le paquet ne peut pas être corrigé */
    Result = false;
  }
  else {
  
    for (Col = 0x01; Col; Col <<= 1) {
 
    /* Pour chaque colonne */
      for (Row = 0; Row < pxPkt->xLen; ++Row) {
 
      /* Pour chaque ligne */
        if (!(ucWPktByte (Valid, Row) & Col)) {
        
          /* Correction du bit */
          pxPkt->pData[Row] ^= Checksum & Col;
        }
      }

      /* Si la colonne a tous les bits valide alors vérifier le checksum */
      if ((GoodPacket & Col) && (Checksum & Col)) { 
        
        /* checksum incorect */
        Result = false;
      }
    }
  }

  return Result;
}

/* -------
 * 
 */
static bool
prvxCheckCrc (xWusbPacket * pxPkt, uint8_t ucSeed) {
  uint16_t usPktCrc;
  uint8_t ucTrailerIndex;
  
  ucTrailerIndex = pxPkt->xLen - WPKT_TRAILER_SIZE; 

  usPktCrc = usWPktWord (pxPkt, ucTrailerIndex);

  if (usPktCrc == usWPktCrc (pxPkt->pData, ucTrailerIndex, ucSeed))
    return true;

  return false;
}

/* internal public functions ================================================ */
// -----------------------------------------------------------------------------
uint8_t *
pucWPktCurrentByte (xWusbPacket * pxPkt) {

  return &pxPkt->pData[pxPkt->xLen];
}

// -----------------------------------------------------------------------------
void 
vWPktAddByte(xWusbPacket * pxPkt, uint8_t ucValue) {

  vWPktSetByte (pxPkt, pxPkt->xLen++, ucValue);
}

// -----------------------------------------------------------------------------
void 
vWPktAddWord(xWusbPacket * pxPkt, uint16_t usValue) {

  vWPktAddByte (pxPkt, (uint8_t) usValue);         // LSB
  vWPktAddByte (pxPkt, (uint8_t) (usValue >> 8));  // MSB
}

// -----------------------------------------------------------------------------
void 
vWPktAddBytes(xWusbPacket * pxPkt, const uint8_t * pucBytes, uint8_t ucLength) {
  uint8_t * pucDst;
  
  pucDst = pucWPktCurrentByte(pxPkt);
  while (ucLength--) {
  
    *pucDst++ = *pucBytes++;
    pxPkt->xLen++;
  }
}

// -----------------------------------------------------------------------------
void 
vWPktAddStr(xWusbPacket * pxPkt, const char* pcStr) {
  uint8_t * pucDst;
  
  pucDst = pucWPktCurrentByte(pxPkt);
  while (*pcStr) {
  
    *pucDst++ = *(uint8_t *)pcStr++;
    pxPkt->xLen++;
  }
}

// -----------------------------------------------------------------------------
void 
vWPktAddBytes_P(xWusbPacket * pxPkt, const uint8_t * pucBytes, uint8_t ucLength) {
  uint8_t * pucDst;
  
  pucDst = pucWPktCurrentByte(pxPkt);
  while (ucLength--) {
  
    *pucDst++ = pgm_read_byte(pucBytes++);
    pxPkt->xLen++;
  }
}

// -----------------------------------------------------------------------------
void 
vWPktAddStr_P(xWusbPacket * pxPkt, const char* pcStr) {
  uint8_t * pucDst;
  char c;
  
  pucDst = pucWPktCurrentByte(pxPkt);
  
  c = pgm_read_byte(pcStr++);
  
  while (c) {
  
    *pucDst++ = c;
    pxPkt->xLen++;
    c = pgm_read_byte(pcStr++);
  }
}

/*
 * Ajoute le CRC et le Checksum au paquet
 * Si le paquet est de type BIND ou PING Broadcast, le Crc et le Checksum 
 * ajoutés sont calculés avec des valeurs initiales WPKT_BIND_CRC_SEED et 
 *  WPKT_BIND_CHECKSUM_SEED. 
 */ 
void
vWPktAddTrailer (xWusbPacket * pxPkt, uint8_t CrcSeed, uint8_t ChecksumSeed) {

  /* Les paquets de BIND et PING Broadcast n'utilisent pas de seed */
  if ( (ucWPktType(pxPkt) == WPKT_BIND_REQUEST_PACKET) ||
       (ucWPktType(pxPkt) == WPKT_BIND_RESPONSE_PACKET) ||
       ((ucWPktType(pxPkt) == WPKT_PING_PACKET) && ((xWPktFlag(pxPkt, WPKT_BCAST) == true))) )
  {
    CrcSeed = WPKT_BIND_CRC_SEED;
    ChecksumSeed = WPKT_BIND_CHECKSUM_SEED;
  }
  vWPktAddWord (pxPkt, usWPktCrc (pxPkt->pData, pxPkt->xLen, CrcSeed));
  vWPktAddByte (pxPkt, ucWPktChecksum (pxPkt->pData, pxPkt->xLen, ChecksumSeed));
}

/*
 * Lit un mot de 16 bits (little endian) du paquet
 */ 
uint16_t 
usWPktWord(xWusbPacket * pxPkt, uint8_t Index) {
  uint16_t usValue;
  
  usValue  = ucWPktByte (pxPkt, Index++);
  usValue += ucWPktByte (pxPkt, Index) << 8;
  return usValue;
}

/*
 * Lit un mot de 32 bits (little endian) du paquet
 */ 
uint32_t 
ulWPktDword(xWusbPacket * pxPkt, uint8_t Index) {
  uint32_t ulValue;
  
  ulValue = usWPktWord (pxPkt, Index);
  ulValue += (uint32_t)(usWPktWord (pxPkt, Index + 2)) << 16;
  return ulValue;
}

/*
 * Vérifie un paquet et le corrige éventuellement
 * @param pxPkt paquet à vérifier (DATA)
 * @param Valid paquet contenant les octets de validation du paquet à vérifier (VALID)
 * @param CrcSeed valeur initiale du Crc, ignoré si le paquet est un BIND ou un PING Broadcast
 * @param ChecksumSeed Valeur initiale du Checksum, ignoré si le paquet est un BIND ou un PING Broadcast
 * @return true si le paquet est correct ou s'il a pu être réparé    
 */ 
bool 
xWPktCheckAndRepair( xWusbPacket * pxPkt, xWusbPacket * Valid, uint8_t CrcSeed, 
                        uint8_t ChecksumSeed) {
  uint8_t ValidByte = 0;
  uint8_t Mask = 0x80;
  uint8_t BitCount = 0;

  /*
   * Nous devons d'abord nous assurer que tous les bits du type de paquet
   * sont valides.
   */
  if (ucWPktHeader (Valid) >= 0xF0){
  
    /*
     * On récupère les bits de validation de l'octet de checksum
     */
    ValidByte = ucWPktByte (Valid, pxPkt->xLen - 1);

    /*
     * Puis on vérifie que cet octet n'a pas plus de 2 bits à zéro, ce qui
     * correspond à 6 bits valides 
     */
    while (Mask) {
    
      if (!(Mask & ValidByte))
        BitCount++;
      Mask >>= 1;
    }

    if (BitCount <= 2) {
    
      /* Les paquets de BIND et PING Broadcast n'utilisent pas de seed */
      if ( (ucWPktType(pxPkt) == WPKT_BIND_REQUEST_PACKET) ||
           (ucWPktType(pxPkt) == WPKT_BIND_RESPONSE_PACKET) ||
           ((ucWPktType(pxPkt) == WPKT_PING_PACKET) && ((xWPktFlag(pxPkt, WPKT_BCAST) == true))) )
      {
        CrcSeed = WPKT_BIND_CRC_SEED;
        ChecksumSeed = WPKT_BIND_CHECKSUM_SEED;
      }

      /* Vérifie le checksum, puis si correct, le crc */
      if (prvxCheckChecksumAndRepair (pxPkt, Valid, ChecksumSeed) == true) {
      
        if (prvxCheckCrc (pxPkt, CrcSeed) == true)
          return true;
      }
    }
  }
  return false;
}

/*
 * Renvoie un pointeur sur la zone mémoire où sont stockés les octets du payload
 * @return le pointeur ou 0 si le paquet n'est pas de type WPKT_DATA_PACKET    
 */ 
uint8_t * pucWPktDataPayload(xWusbPacket * pxPkt) {

  if (ucWPktType (pxPkt) ==  WPKT_DATA_PACKET) {
    uint8_t * p;
    
    p = &pxPkt->pData[WPKT_HEADER_SIZE + 1];
    if (xWPktFlag (pxPkt, WPKT_SINGLE) == false)
      p++; /* device id sur 2 octets */
    return p;  
  }
  return 0;
}

/*
 * Renvoie le nombre d'octets du payload  
 * @return la longueur ou 0 si le paquet n'est pas de type WPKT_DATA_PACKET    
 */ 
uint8_t xWPktDataPayloadLength(xWusbPacket * pxPkt) {

  if (ucWPktType (pxPkt) ==  WPKT_DATA_PACKET) {

    uint8_t s = pxPkt->xLen - 5;
    if (xWPktFlag (pxPkt, WPKT_SINGLE) == false)
      --s;
    return s;
  }  
  return 0;
}

/*
 * Calcule le Crc 16 bits CCITT (X^16 + X^12 + X^5 + 1) d'une suite d'octets
 */ 
uint16_t
usWPktCrc (const uint8_t * pData, uint8_t ucLength, uint16_t usSeed) {

  for (; ucLength > 0; ucLength--, pData++) {
  
    usSeed = (uint8_t) (usSeed >> 8) | (usSeed << 8);
    usSeed ^= *pData;
    usSeed ^= (uint8_t) (usSeed & 0xff) >> 4;
    usSeed ^= (usSeed << 8) << 4;
    usSeed ^= ((usSeed & 0xff) << 4) << 1;
  }

  return usSeed;
}

/*
 * Calcule le checksum d'une suite d'octets
 */ 
uint8_t
ucWPktChecksum (const uint8_t * pData, uint8_t ucLength, uint8_t ucSeed) {

  for (; ucLength > 0; ucLength--, pData++) {
  
    ucSeed ^= *pData;
  }

  return ucSeed;
}

/*
 * Renvoie l'état du bit de rang \c Bit de l'octet \c Byte
 */ 
bool 
xWPktBit(uint8_t Byte, uint8_t ucBit) {

  return (Byte & _BV(ucBit)) != 0;
}

/*
 * Modifie l'état du bit de rang \c Bit de l'octet pointé par \c Byte
 */ 
void 
vWPktSetBit(uint8_t * pucByte, uint8_t ucBit, bool xValue) {

    if (xWPktBit(*pucByte, ucBit) != xValue)
      *pucByte ^= _BV(ucBit);
}

// ---------------------------------------------------------------------------
void 
vWPktSetByte(xWusbPacket * pxPkt, uint8_t ucIndex, uint8_t ucValue) {

  pxPkt->pData[ucIndex] = ucValue;
}

// ---------------------------------------------------------------------------
uint8_t 
ucWPktByte(xWusbPacket * pxPkt, uint8_t ucIndex) {

  return pxPkt->pData[ucIndex];
}

// ---------------------------------------------------------------------------
void 
  xWPktSetAllFlags(xWusbPacket * pxPkt, uint8_t ucFlags) {

  *pxPkt->pData = (*pxPkt->pData & 0xF0) | (ucFlags & 0x0F);
}

// ---------------------------------------------------------------------------
void 
  vWPktSetType(xWusbPacket * pxPkt, uint8_t ucValue) {

  vWPktSetByte (pxPkt, 0, (ucWPktHeader(pxPkt) & 0x0F) | ((ucValue & 0x0F)<<4)); 
}

// ---------------------------------------------------------------------------
uint8_t 
  ucWPktType(xWusbPacket * pxPkt) {

  return ucWPktHeader(pxPkt) >> 4;
}

/* internal public functions ================================================ */

#ifdef DEBUG_ONUART
# include <avrio/term.h>
# include <avrio/serial.h>

void 
vWPktDbgDump (xWusbPacket * pxPkt) {

  vTermPutHexBytes (pxPkt->pData, pxPkt->xLen, &xSerialPort);
  fputc (0x0D, &xSerialPort);
}
#endif

#ifdef WUSB_WPKT_TEST
/*
 * Tests unitaires
 * -----------------------------------------------------------------------------
 */
#include <string.h>

#define WUSB_WPKT_TEST_PAYLOADSIZE  3
#define WUSB_WPKT_TEST_RAWSIZE  WPKT_PACKET_SIZE_MAX(WUSB_WPKT_TEST_PAYLOADSIZE)
#define WPKT_ASSERT(__test) \
  if (!(__test)) return errno; \
  errno--
#define pTstPkt (&TstPkt)
#define pTstValid (&TstValid)

static int errno = -1;
WPKT_STATIC_DECLARE (TstPkt, WUSB_WPKT_TEST_RAWSIZE);
WPKT_STATIC_DECLARE (TstValid, WUSB_WPKT_TEST_RAWSIZE);

  
// -----------------------------------------------------------------------------
static inline int
prviCheckAndRepairTest (void) {

  /*
   *   Crc(0x450144) = 0xCD2C
   *   Checksum(0x450144CD2C) = 0xE1
   */
  vWPktPrepare (pTstPkt, 0x45);
  vWPktAddByte (pTstPkt, 0x01);
  vWPktAddByte (pTstPkt, 0x44);
  vWPktAddTrailer (pTstPkt, 0, 0);

  vWPktPrepare (pTstValid, 0xFF);
  vWPktAddByte (pTstValid, 0xFF);
  vWPktAddByte (pTstValid, 0xFF);
  vWPktAddByte (pTstValid, 0xFF);
  vWPktAddByte (pTstValid, 0xFF);
  vWPktAddByte (pTstValid, 0xFF);

/* Test 1 - Simlate Bit Inversion
 * Result: Packet lost due bit inversion
 * Pkt->pData.dataptr[1] |= 0x10;
 */
 pucWPktRaw(pTstPkt)[0] |= 0x10;
 WPKT_ASSERT(xWPktCheckAndRepair(pTstPkt, pTstValid, 0, 0) == false);
 vWPktSetByte(pTstPkt, 0,  0x45);


/* Test 2 - Checksum: Data is correct but the bit is invalid
 * Result: Packet is recovered
 * Valid->pData.dataptr[0] &= ~0x01;
 */
 pucWPktRaw(pTstValid)[0] &= ~0x01;
 WPKT_ASSERT (xWPktCheckAndRepair(pTstPkt, pTstValid, 0, 0) == true);
 vWPktSetByte(pTstValid, 0,  0xFF);

/* Test 3 - Checksum - Data bit is incorrectly set and invalid bit is cleared
 * Result: Packet is recovered
 * Pkt->pData.dataptr[1] |= 0x04;
 * Valid->pData.dataptr[1] &= ~0x04;
 */
 pucWPktRaw(pTstPkt)[1] |= 0x04;
 pucWPktRaw(pTstValid)[1] &= ~0x04;
 WPKT_ASSERT (xWPktCheckAndRepair(pTstPkt, pTstValid, 0, 0) == true);
 vWPktSetByte(pTstPkt, 1, 0x01);
 vWPktSetByte(pTstValid, 1, 0xFF);

/* Test 4 - Checksum - Data bit is cleared and invalid bit is cleared
 * Result: Packet is recovered
 * Pkt->pData.dataptr[1] &= ~0x01;
 * Valid->pData.dataptr[1] &= ~0x01;
 */
 pucWPktRaw(pTstPkt)[1] &= ~0x01;
 pucWPktRaw(pTstValid)[1] &= ~0x01;
 WPKT_ASSERT(xWPktCheckAndRepair(pTstPkt, pTstValid, 0, 0) == true);
 vWPktSetByte(pTstPkt, 1, 0x01);
 vWPktSetByte(pTstValid, 1, 0xFF);

 return 0;
}

// -----------------------------------------------------------------------------
static inline int
prviHeaderTest (void) {
  uint8_t Index;

  vWPktPrepare (pTstPkt, 0);
  vWPktSetType (pTstPkt, 0xFF);
  WPKT_ASSERT (ucWPktType(pTstPkt) == 0x0F);

  vWPktPrepare (pTstPkt, 0xFF);
  vWPktSetType (pTstPkt, 0x00);
  WPKT_ASSERT (ucWPktType(pTstPkt) == 0x00);

  vWPktPrepare (pTstPkt, 0x00);
  for (Index = 0; Index < 4; Index++)
  {
    vWPktSetFlag (pTstPkt, Index, true);
    WPKT_ASSERT (xWPktFlag(pTstPkt, Index) == true);

    vWPktSetFlag (pTstPkt, Index, false);
    WPKT_ASSERT (xWPktFlag(pTstPkt, Index) == false);

    vWPktToggleFlag (pTstPkt, Index);
    WPKT_ASSERT (xWPktFlag(pTstPkt, Index) == true);
  }

  return 0;
}

// -----------------------------------------------------------------------------
#define TEST_LSB 0x5678
#define TEST_MSB 0x1234
#define TEST_DWORD (((uint32_t)TEST_MSB << 16) + TEST_LSB)
#define TEST_TEXT "Test"
#define TEST_TEXT_SIZE 4

static const uint8_t pucText_P[] PROGMEM = TEST_TEXT;

static inline int
prviBasicTest (void) {
  uint8_t ucByte;
  const uint8_t pucText[] = TEST_TEXT;
  
  vWPktClear (pTstPkt);
  WPKT_ASSERT (xWPktIsEmpty(pTstPkt) == true);

  vWPktPrepare (pTstPkt, 0x55);
  WPKT_ASSERT (xWPktSize(pTstPkt) == WUSB_WPKT_TEST_RAWSIZE);
  WPKT_ASSERT (xWPktLength(pTstPkt) == 1);
  WPKT_ASSERT (pucWPktRaw(pTstPkt)[0] == 0x55);
  WPKT_ASSERT (pucWPktCurrentByte(pTstPkt) == &pucWPktRaw(pTstPkt)[1]);
  WPKT_ASSERT (xWPktIsEmpty(pTstPkt) == false);
  WPKT_ASSERT (xWPktIsFull(pTstPkt) == false);
  
  ucByte = 0x01;
  while (!xWPktIsFull(pTstPkt)) {
  
    vWPktAddByte (pTstPkt, ucByte++);
  }
  for (ucByte = 1; ucByte < (xWPktSize(pTstPkt)); ucByte++) {
  
    WPKT_ASSERT (ucWPktByte(pTstPkt, ucByte) == ucByte);
  }
 
  vWPktClear (pTstPkt);
  vWPktAddWord (pTstPkt, TEST_LSB);
  WPKT_ASSERT (xWPktLength(pTstPkt) == 2);
  WPKT_ASSERT (usWPktWord(pTstPkt, 0) == TEST_LSB);

  vWPktAddWord (pTstPkt, TEST_MSB);
  WPKT_ASSERT (xWPktLength(pTstPkt) == 4);
  WPKT_ASSERT (ulWPktDword(pTstPkt, 0) == TEST_DWORD);
 
  vWPktClear (pTstPkt);
  vWPktAddBytes (pTstPkt, pucText, TEST_TEXT_SIZE);
  WPKT_ASSERT (xWPktLength(pTstPkt) == TEST_TEXT_SIZE);
  WPKT_ASSERT (memcmp (pucWPktRaw(pTstPkt), pucText, TEST_TEXT_SIZE) == 0);

  vWPktClear (pTstPkt);
  vWPktAddBytes_P (pTstPkt, pucText_P, TEST_TEXT_SIZE);
  WPKT_ASSERT (xWPktLength(pTstPkt) == TEST_TEXT_SIZE);
  WPKT_ASSERT (memcmp_P (pucWPktRaw(pTstPkt), pucText_P, TEST_TEXT_SIZE) == 0);

  vWPktClear (pTstPkt);
  vWPktAddStr (pTstPkt, (const char *)pucText);
  WPKT_ASSERT (xWPktLength(pTstPkt) == TEST_TEXT_SIZE);
  WPKT_ASSERT (memcmp (pucWPktRaw(pTstPkt), pucText, strlen((const char *)pucText)) == 0);

  vWPktClear (pTstPkt);
  vWPktAddStr_P (pTstPkt, (const char *)pucText_P);
  WPKT_ASSERT (xWPktLength(pTstPkt) == TEST_TEXT_SIZE);
  WPKT_ASSERT (memcmp_P (pucWPktRaw(pTstPkt), pucText_P, strlen_P((const char *)pucText_P)) == 0);
    
  return 0;
}

/* ========================================================================== */
int
iWPktTest (void) {

  WPKT_ASSERT (prviBasicTest () == 0);
  WPKT_ASSERT (prviHeaderTest () == 0);
  WPKT_ASSERT (prviCheckAndRepairTest () == 0);

  return 0;
}
#endif  /* WUSB_WPKT_TEST defined */

#endif /* AVRIO_WUSB_ENABLE defined */
/* ========================================================================== */
