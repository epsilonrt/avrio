/**
 * @file test_twi_master.c
 * @ingroup test_group
 * @brief Tests unitaires du module \ref twi_module "Bus I2C".
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
   ========================================================================== */
#include <avrio/led.h>
#include <avrio/twi.h>
#include <avrio/button.h>
#include <avrio/delay.h>
#include <string.h>

/* ========================================================================== */
/**
 * @addtogroup test_group
 * @{
 */

/* constants ================================================================ */
#define LED_ALIVE LED_LED1
// Caractéristiques de la trame envoyée
#define FRAME_LEN  31
#define FRAME_FIRST_VALUE 0x41

// Caractéristiques du circuit esclave
#define SLAVE_ADDR 0xA0 // émule un PCF8583
#define RAM_BASE 0x10 // Adresse du premier octet de RAM
#define RAM_SIZE 0xF0 // Nombre d'octet de RAM
#define RAM_END (RAM_BASE + RAM_SIZE - 1)

/* 
 * Validation des tests à effectuer
 */
//#define TEST_SLA_ADDR_NACK_ERROR
//#define TEST_SINGLE_WAIT

/* private variables ======================================================== */
TWIFRAME_STATIC_DECLARE (xTxFrame, FRAME_LEN + 1);
TWIFRAME_STATIC_DECLARE (xRxFrame, FRAME_LEN);

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

// ----------------------------------------------------------------------------
// Envoi un octet au circuit 
static void
prvvTestBase (void) {

  vAssert (eTwiWrite (SLAVE_ADDR, 0x41) == TWI_SUCCESS);
}

// ----------------------------------------------------------------------------
// Remplit le buffer de transmission avec des valeurs croissantes en partant
// de FRAME_FIRST_VALUE
static void
prvvPrepareTxPayload (void) {
  uint8_t ucValue = FRAME_FIRST_VALUE;
  uint8_t ucCount;

  xTxFrame.pxData[0] = RAM_BASE;
  for (ucCount = 1; ucCount <= FRAME_LEN; ucCount++) {

    xTxFrame.pxData[ucCount] = ucValue++;
  }
}

// ----------------------------------------------------------------------------
// Envoi d'une trame de FRAME_LEN octets au circuit
// Vérifie que tous les octets ont été envoyés
static void
prvvTestWriteRAM (void) {

  prvvPrepareTxPayload ();
  xTxFrame.xLen = FRAME_LEN + 1;
  vAssert (eTwiSend (&xTxFrame) == TWI_SUCCESS);
  vAssert (xTxFrame.xLen == FRAME_LEN + 1);
}

// ----------------------------------------------------------------------------
// Lecture d'une trame de FRAME_LEN octets dans le circuit circuit
// L'adresse RAM est envoyée avec eTwiSend() puis la lecture est effectuée
// avec eTwiReceive(): perte de contrôle du bus entre les deux opérations.
// Vérifie que la trame reçue est conforme à la trame envoyée par 
// prvvTestWriteRAM()
static void
prvvTestReadRAM (void) {

  xTxFrame.pxData[0] = RAM_BASE;
  xTxFrame.xLen = 1;
  xRxFrame.xLen = FRAME_LEN;
  

  // Envoi de l'adresse RAM de début de lecture
  vAssert (eTwiSend (&xTxFrame) == TWI_SUCCESS);
  // Lecture
  vAssert (eTwiReceive (&xRxFrame) == TWI_SUCCESS);
  // Vérification
  vAssert (xRxFrame.xLen == FRAME_LEN);
  vAssert (memcmp (&xTxFrame.pxData[1], xRxFrame.pxData, FRAME_LEN) == 0);
}

#ifdef TEST_SLA_ADDR_NACK_ERROR
// ----------------------------------------------------------------------------
// Envoi un octet à une adresse incorrecte
// xDeviceAddr ne doit correspondre à aucun circuit présent sur le bus
static void
prvvTestSlaveAddrNackError (void) {

  vAssert (eTwiWrite (SLAVE_ADDR + 4, 0x41) == TWI_ERROR_ADDR_NACK);
}
#else
#  define prvvTestSlaveAddrNackError()
#endif

#ifdef TEST_SINGLE_WAIT
  // ----------------------------------------------------------------------------
static void
prvvWaitButton (void) {

  while (xButGet (BUTTON_BUTTON1) == 0) ;
}
#else
#  define prvvWaitButton()
#endif

/* main ===================================================================== */
int
main (void) {
  eTwiStatus eError;

  vTwiInit ();
  vLedInit ();
  vLedSet (LED_ALIVE);

  eError = eTwiSetSpeed (100);
  xTxFrame.xAddr = SLAVE_ADDR;
  xRxFrame.xAddr = SLAVE_ADDR;
  delay_ms (200); // permet à l'esclave de démarrer

  for (;;) {

    prvvWaitButton ();

    // Test 1: écriture d'un octet
    prvvTestBase ();

    // Test 2: Ecriture en RAM du PCF8583
    prvvTestWriteRAM ();

    // Test 3: Lecture en RAM du PCF8583
    prvvTestReadRAM ();

    // Test 4: Erreur Address NACK Esclave
    prvvTestSlaveAddrNackError ();

    vLedToggle (LED_ALIVE);
  }
  return 0;
}

/** @} */
/* ========================================================================== */
