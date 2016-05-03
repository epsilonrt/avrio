/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file mmc.c
 * @brief Module d'accès aux cartes MMC/SD - Implémentation
 * Copyright © 2006 Jesper Hansen <jesper@redegg.net>
 *
 * Revision History ------------------------------------------------------------
 *    20120323 - Initial version from Michael Spiceland version
 *               (http://tinkerish.com)
 */
#include "avrio-config.h"

#ifdef AVRIO_MMC_ENABLE
/* ========================================================================== */
#  include <stdio.h>
#  include <avr/io.h>
#  include "avrio-board-mmc.h"
#  include "delay.h"
#  include "spi.h"
#  include "mmc.h"

/* macros =================================================================== */
#ifdef MMC_CD
# define MMC_CD_INIT() do { MMC_CD_PORT |= _BV(MMC_CD); MMC_CD_DIR &= ~_BV(MMC_CD); } while (0)
# define MMC_CD_IS_SET() (((MMC_CD_PIN & _BV(MMC_CD)) ^ (MMC_CD_LEVEL << MMC_CD)) == 0)
#else
# define MMC_CD_INIT()
# define MMC_CD_IS_SET() (true)
#endif

#ifdef MMC_WP
# define MMC_WP_INIT() do { MMC_WP_PORT |= _BV(MMC_WP); MMC_WP_DIR &= ~_BV(MMC_WP); } while (0)
# define MMC_WP_IS_SET() (((MMC_WP_PIN & _BV(MMC_WP)) ^ (MMC_WP_LEVEL << MMC_WP)) == 0)
#else
# define MMC_WP_INIT()
# define MMC_WP_IS_SET() (false)
#endif

#ifdef MMC_EN
# if MMC_EN_LEVEL == 0
#  define MMC_ENABLE()  do { MMC_EN_PORT &= ~_BV(MMC_EN); } while (0);
#  define MMC_DISABLE()  do { MMC_EN_PORT |= _BV(MMC_EN); } while (0);
#  define MMC_IS_ENABLE()  ((MMC_EN_PORT | _BV(MMC_EN) == 0)
# else
#  define MMC_ENABLE()  do { MMC_EN_PORT |= _BV(MMC_EN); } while (0);
#  define MMC_DISABLE()  do { MMC_EN_PORT &= ~_BV(MMC_EN); } while (0);
#  define MMC_IS_ENABLE()  ((MMC_EN_PORT | _BV(MMC_EN) != 0)
# endif
#  define MMC_EN_INIT() do { MMC_DISABLE(); MMC_EN_DIR |= _BV(MMC_EN); } while (0)
#else
# define MMC_EN_INIT()
# define MMC_ENABLE()
# define MMC_DISABLE()
# define MMC_IS_ENABLE()  (true)
#endif

/* structures =============================================================== */
/*
  Helper structure.
  This simplify conversion between bytes and words.
*/
struct u16bytes
{
  uint8_t low; //< byte member
  uint8_t high; //< byte member
};

/*
  Helper union.
  This simplify conversion between bytes and words.
*/
union u16convert
{
  uint16_t value; //< for word access
  struct u16bytes bytes; //< for byte access
};

/*
  Helper structure.
  This simplify conversion between bytes and longs.
*/
struct u32bytes
{
  uint8_t byte1; //< byte member
  uint8_t byte2; //< byte member
  uint8_t byte3; //< byte member
  uint8_t byte4; //< byte member
};

/*
  Helper structure.
  This simplify conversion between words and longs.
*/
struct u32words
{
  uint16_t low; //< word member
  uint16_t high; //< word member
};

/*
  Helper union.
  This simplify conversion between bytes, words and longs.
*/
union u32convert
{
  uint32_t value; //< for long access
  struct u32words words; //< for word access
  struct u32bytes bytes; //< for byte access
};

typedef enum {
  MMC_OPEN      = 0x01,
  MMC_READONLY  = 0x02,
  MMC_DETECTED  = 0x04
} eMmcFlags;

/* private variables ======================================================== */
static uint8_t ucMmcFlags;

/* private functions ======================================================== */

/* -----------------------------------------------------------------------------
  Send a command to the MMC/SD card.
  \param ucCmd  Command to send
  \param usParam1  Command parameter 1
  \param usParam2  Command parameter 2
*/
static void
prvvSendCommand (uint8_t ucCmd, uint16_t usParam1, uint16_t usParam2) {
  register union u16convert r;

  MMC_CS_PORT &= ~_BV(MMC_CS); // enable CS

  ucSpiMasterWriteRead (0xFF); // dummy byte

  ucSpiMasterWriteRead (ucCmd | 0x40);

  r.value = usParam1;
  ucSpiMasterWriteRead (r.bytes.high); // high byte of param x
  ucSpiMasterWriteRead (r.bytes.low);  // low byte of param x

  r.value = usParam2;
  ucSpiMasterWriteRead (r.bytes.high); // high byte of param y
  ucSpiMasterWriteRead (r.bytes.low);  // low byte of param y

  ucSpiMasterWriteRead (0x95); // correct CRC for first command in SPI
                               // after that CRC is ignored, so no problem with
                               // always sending 0x95
  ucSpiMasterWriteRead (0xFF); // ignore return byte
}

/* -----------------------------------------------------------------------------
  Get Token.
  Wait for and return a non-ff token from the MMC/SD card
  \return The received token or 0xFF if timeout
*/
static uint8_t
prvucGetToken (void) {
  uint16_t i = 0xFFFF;
  uint8_t b = 0xFF;

  while ((b == 0xFF) && (--i)) {

    b = ucSpiMasterWriteRead (0xFF);
  }

  return b;
}

/* -----------------------------------------------------------------------------
  Get Datatoken.
  Wait for and return a data token from the MMC/SD card
  \return The received token or 0xFF if timeout
*/
static uint8_t
prvucGetDataToken (void) {
  uint16_t i = 0xFFFF;
  uint8_t b = 0xFF;

  while ((b != 0xFE) && (--i)) {

    b = ucSpiMasterWriteRead (0xFF);
  }
  return b;
}

/* -----------------------------------------------------------------------------
  Finish Clocking and Release card.
  Send 10 clocks to the MMC/SD card
  and release the CS line
*/
static void
prvvClockAndRelease(void) {
  uint8_t i;

  // SD cards require at least 8 final clocks
  for(i = 0; i < 10; i++) {

    ucSpiMasterWriteRead (0xFF);
  }

  MMC_CS_PORT |= _BV(MMC_CS); // release CS
}

/* -----------------------------------------------------------------------------
  MMC get response
  Repeatedly reads the MMC until we get the response we want or timeout
  this function taken from the PIC CCS example
*/
static int
prviGetResponse (uint8_t response) {
  // 16 bit repeat, it may be possible to shrink this to 8 bit but there is not
  // much point
  uint32_t count = 0xFFFF;

  while ((ucSpiMasterWriteRead (0xFF) != response) && (--count > 0))
    ;

  if (count == 0)
    return 1; // error: loop was exited due to timeout
  else
    return 0; // success: loop was exited before timeout
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vMmcInit(void) {

  // setup I/O ports
  MMC_CS_PORT |= _BV(MMC_CS); // Initial level is high
  MMC_CS_DIR  |= _BV(MMC_CS); // Direction is output
  MMC_EN_INIT();
  MMC_CD_INIT();
  MMC_WP_INIT();
}

// -----------------------------------------------------------------------------
void
vMmcClose (void) {

  ucMmcFlags &= ~MMC_OPEN;
  MMC_DISABLE();
}


// -----------------------------------------------------------------------------
bool
xMmcIsDetected (void) {

  return MMC_CD_IS_SET();
}

// -----------------------------------------------------------------------------
bool
xMmcIsOpen (void) {

  return (ucMmcFlags & MMC_OPEN) != 0;
}

// -----------------------------------------------------------------------------
bool
xMmcIsReadOnly (void) {

  return (ucMmcFlags & MMC_READONLY) != 0;
}

// -----------------------------------------------------------------------------
int
iMmcOpen(void) {
  int i;

  if (xMmcIsOpen()) {

    return MMC_SUCCESS;
  }
  ucMmcFlags &= ~MMC_OPEN;

  if ( !xMmcIsDetected() ) {

    // Retourne une erreur si la carte n'est pas détectée
    MMC_DISABLE();
    return MMC_ERROR;
  }
  else {

    MMC_ENABLE();
    delay_us (100);
  }

  //
  // send 80 clocks while card power stabilizes
  //
  for(i = 0; i < 10; i++) {

    ucSpiMasterWriteRead (0xFF);
  }

  //
  // send CMD0 - reset card
  //
  prvvSendCommand (0, 0, 0);
  if (prvucGetToken() != 1) {

    // if no valid response code
    prvvClockAndRelease();
    return MMC_ERROR; // card cannot be detected
  }

  //
  // send CMD1 until we get a 0 back, indicating card is done initializing
  //
  i = 0xFFFF; // max timeout
  while ((ucSpiMasterWriteRead (0xFF) != 0) && (--i)) {

    // wait for it
    prvvSendCommand (1, 0, 0); // send CMD1 - activate card init
  }
  prvvClockAndRelease(); // clean up

  if (i == 0) {

    // if we timed out above
    return MMC_ERROR; // return failure code
  }

  if (MMC_WP_IS_SET()) {

    ucMmcFlags |= MMC_READONLY;
  }
  else {

    ucMmcFlags &= ~MMC_READONLY;
  }
  ucMmcFlags |= MMC_OPEN;
  return MMC_SUCCESS;
}

// -----------------------------------------------------------------------------
int
iMmcWriteSector (uint32_t ulSector, uint8_t *pucBuffer, uint32_t ulSectorCount) {
  uint16_t i;

  if ( !MMC_CD_IS_SET() || MMC_WP_IS_SET() ) {

    // Retourne une erreur si la carte n'est pas détectée ou si la protection
    // en écriture est activée.
    return MMC_ERROR;
  }

  while (ulSectorCount--) {

    // send read command and logical sector address
    prvvSendCommand (24, (ulSector>>7) & 0xFFFF, (ulSector<<9) & 0xFFFF);

    // need wait for resonse here
    if ((prviGetResponse (0x00)) == 1) {

      // Error
      prvvClockAndRelease(); // cleanup
      return MMC_ERROR;
    }

    // need to send token here
    ucSpiMasterWriteRead (0xFE); // send data token

    for (i=0; i < MMC_SECTOR_SIZE; i++) {

      // write sector data
      ucSpiMasterWriteRead(*pucBuffer++);
    }

    ucSpiMasterWriteRead (0xFF); // ignore dummy checksum
    ucSpiMasterWriteRead (0xFF); // ignore dummy checksum

    // do we check the status here ?
    if ((ucSpiMasterWriteRead (0xFF) & 0x0F) != 0x05) {

      return MMC_ERROR;
    }

    i = 0xFFFF; // max timeout
    while (!ucSpiMasterWriteRead (0xFF) && (--i))
      ; // wait until we are not busy

    prvvClockAndRelease(); // cleanup
  }

  return MMC_SUCCESS; // return success
}

// -----------------------------------------------------------------------------
int
iMmcReadSector (uint32_t ulSector, uint8_t *pucBuffer, uint32_t ulSectorCount) {
  uint16_t i;

  if ( !MMC_CD_IS_SET() ) {

    // Retourne une erreur si la carte n'est pas détectée
    return MMC_ERROR;
  }

  while (ulSectorCount--) {

    // send read command and logical sector address
    prvvSendCommand (17, (ulSector >> 7) & 0xFFFF, (ulSector << 9) & 0xFFFF);

    if (prvucGetDataToken() != 0xFE)  {

      // if no valid token
      prvvClockAndRelease(); // cleanup and
      //for (i=0; i < MMC_SECTOR_SIZE; i++)
      // we don't want a false impression that everything was fine.
      //  *pucBuffer++ = 0x00;
      return MMC_ERROR; // return error code
    }

    for (i=0; i < MMC_SECTOR_SIZE; i++) {

      // read ulSector data
      *pucBuffer++ = ucSpiMasterWriteRead (0xFF);
    }

    ucSpiMasterWriteRead (0xFF); // ignore dummy checksum
    ucSpiMasterWriteRead (0xFF); // ignore dummy checksum

    prvvClockAndRelease(); // cleanup
  }

  return MMC_SUCCESS; // return success
}

#endif /* AVRIO_MMC_ENABLE defined */
/* ========================================================================== */

