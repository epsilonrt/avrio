/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
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
 * @file test_xbee.c
 * @brief Test noeud XBee
 * - Affiche le contenu des paquets de données reçus
 * - Transmet périodiquement un paquet de données de test
 * .
 */
#define __ASSERT_USE_STDERR
#include <avrio/assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <avr/pgmspace.h>
#include <avrio/xbee.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/lcd.h>
#include <avrio/button.h>

/* constants ================================================================ */
#define DEFAULT_BAUDRATE  38400
#define TX_INTERVAL_DELAY 5000
#define POLL_DELAY        10

/* private variables ======================================================== */
static struct xXBee * xbee;
static char cMyNi[21];
volatile int frame_id = 0;

/* private functions ======================================================== */
int iDataCB (struct xXBee *xbee, struct xXBeePkt *pkt, uint8_t len);
int iTxStatusCB (struct xXBee *xbee, struct xXBeePkt *pkt, uint8_t len);
int iLocalAtCB (struct xXBee *xbee, struct xXBeePkt *pkt, uint8_t len);
void vLedAssert (int i);

/* internal public functions ================================================ */
int
main (void) {
  int ret;
  xSerialIos xIosSet = {
    .baud = DEFAULT_BAUDRATE, .dbits = SERIAL_DATABIT_8, .parity = SERIAL_PARITY_NONE,
    .sbits = SERIAL_STOPBIT_ONE, .flow = SERIAL_FLOW_RTSCTS, .flag = 0
  };

  /*
   * Init LED, utilisée pour signaler la réception d'un paquet ou pour signaler
   * une erreur LCD
   */
  vLedInit();
  vLedSet (LED_LED1);

  /*
   * Init LCD, c'est la sortie standard et d'erreur
   * Dans le cas de la carte XNODE, l'afficheur est connecté par I2C, le bus
   * I2C est donc initialisé à la vitesse par défaut (100 kHz)
   */
  vLedAssert (iLcdInit() == 0);
  ucLcdBacklightSet (32);
  stdout = &xLcd;
  stderr = &xLcd;

  printf ("**  XBee Test **\n");

  /*
   * Init bouton poussoir, un appui permet de déclencher une mesure
   */
  vButInit();

  /*
   * Init liaison série vers module XBee
   * Mode lecture/écriture, non bloquant, avec contrôle de flux matériel
   */
  if ( (xbee = xXBeeOpen (NULL, &xIosSet, XBEE_SERIES_S2)) == NULL) {

    fprintf (stderr, "Open failed !");
    return -1;
  }

  /*
   * Mise en place des gestionnaires de réception
   */
  vXBeeSetCB (xbee, XBEE_CB_DATA, iDataCB);
  vXBeeSetCB (xbee, XBEE_CB_TX_STATUS, iTxStatusCB);
  vXBeeSetCB (xbee, XBEE_CB_AT_LOCAL, iLocalAtCB);
  sei(); // La réception UART utilise les interruptions....

  ret = iXBeeSendAt (xbee, XBEE_CMD_NODE_ID, NULL, 0);
  assert (ret >= 0);

  for (;;) {

    // Scrute la réception des paquets
    ret = iXBeePoll (xbee, POLL_DELAY);
    assert (ret == 0);
    if (xButGet (BUTTON_BUTTON1)) {
      char message[33];
      static int iCount = 1;

      vLedSet (LED_LED1);
      snprintf (message, 32, "%s #%d", cMyNi, iCount++);

      frame_id = iXBeeZbSendToCoordinator (xbee, message, strlen (message));
      assert (frame_id >= 0);
      vLcdClear();
      printf ("Tx%d>%s\n", frame_id, message);
      while (xButGet (BUTTON_BUTTON1))
        ; // Attente relachement bouton pour éviter des envois multiples
    }
  }

  ret = iXBeeClose (xbee);
  return 0;
}

// -----------------------------------------------------------------------------
// Gestionnaire de réponse à une commande AT locale
int
iLocalAtCB (struct xXBee *xbee, struct xXBeePkt *pkt, uint8_t len) {

  char * cCmd = pcXBeePktCommand (pkt);

  if (iXBeePktStatus (pkt) == XBEE_PKT_STATUS_OK) {

    if (strncmp (cCmd, XBEE_CMD_NODE_ID, 2) == 0) {
      // Ni String
      int ret = iXBeePktParamGetStr (cMyNi, pkt, 21);
      assert (ret >= 0);
      printf ("Ni:%s\n", cMyNi);
      vLedClear (LED_LED1);
    }
  }
  else {
    // Erreur
    fprintf (stderr, "%2s failed 0x%02X\n",
             cCmd, iXBeePktStatus (pkt));
  }
  vXBeeFreePkt (xbee, pkt);
  return 0;
}

// -----------------------------------------------------------------------------
// Gestionnaire de réception des paquets de données
int
iDataCB (struct xXBee *xbee, struct xXBeePkt *pkt, uint8_t len) {
  int size;

  size = iXBeePktDataLen (pkt);
  if (size > 0) {
    volatile char * p;
    uint8_t * src64 = pucXBeePktAddrSrc64 (pkt);

    vLcdClear();
    printf("...");

    // Affiche les 4 octets LSB de l'adresse 64-bits de la source
    for (int i = 4; i < 8; i++) {

      printf ("%02X", src64[i]);
    }

    putchar ('>');
    // Indique si le paquet est en broadcast
    if (iXBeePktIsBroadcast (pkt)) {
      putchar ('*');
    }

    // puis le contenu du paquet
    p = (char *) pucXBeePktData (pkt);
    p[size] = 0; // Ajout d'un caractère de fin de chaine
    printf ("\n%s\n", p);
  }

  vXBeeFreePkt (xbee, pkt);
  return 0;
}

// -----------------------------------------------------------------------------
// Gestionnaire de status de fin de transmission
int
iTxStatusCB (struct xXBee *xbee, struct xXBeePkt *pkt, uint8_t len) {

  if (iXBeePktFrameId (pkt) == frame_id) {
    int status = iXBeePktStatus (pkt);

    if (status) {

      printf ("Tx%d Err. %d\n", frame_id, status);
    }
    else {

      printf ("Tx%d Ok\n", frame_id);
      vLedClear (LED_LED1);
    }
    frame_id = 0;
  }
  vXBeeFreePkt (xbee, pkt);
  return 0;
}

/* -----------------------------------------------------------------------------
 * Vérifie que la condition passée est vraie, sinon fait clignoter rapidement
 * la led 1 à l'infinie
 */
void
vLedAssert (int i) {

  if (!i) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (5);
      vLedClear (LED_LED1);
      delay_ms (25);
    }
  }
}
/* ========================================================================== */
