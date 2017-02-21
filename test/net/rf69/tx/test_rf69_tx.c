/**
 * @file
 * @brief Test de transmission avec un module RFM69
 *
 * -----------------------------<<WARNING>>-------------------------------------
 * Un module RFM69 est alimenté en 3.3V et n'est donc pas compatible
 * avec ARDUINO sans adaptation de niveau... on utilisera plutôt une carte xplbee !
 * -----------------------------------------------------------------------------
 */
#include <avrio/delay.h>
#include <avrio/rf69.h>
#include <avrio/tc.h>
#include <avrio/assert.h>
#include <avr/pgmspace.h>
#include <string.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT     "tty0"

#define DIO0_IRQ  INT0
#define NODE_ID   1
#define NET_ID    1

/* macros =================================================================== */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* public variables ========================================================= */
/* private functions ======================================================== */
/* internal public functions ================================================ */

/* main ===================================================================== */
int
main (void) {
  int t = 1, ret;

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port; // le port série est la sortie d'erreur
  stdin = serial_port; // le port série est l'entrée standard
  sei();

  printf_P (PSTR ("\n\n*** RFM69 TX test ***\n\n"));

  printf_P (PSTR ("Test %d - xRf69New(0,0,%d)\n"), t++, DIO0_IRQ);
  xRf69 * rf = xRf69New (0, 0, DIO0_IRQ);
  assert (rf);
  printf_P (PSTR ("Success\n\n"));

  printf_P (PSTR ("Test %d - iRf69Open (rf,eRf69Band868Mhz,%d,%d)\n"), t++,
            NODE_ID, NET_ID);
  ret = iRf69Open (rf, eRf69Band868Mhz, NODE_ID, NET_ID);
  assert (ret == 0);
  printf_P (PSTR ("Success\n\n"));

  // NodeId
  printf_P (PSTR ("Test %d - iRf69NodeId()\n"), t++);
  ret = iRf69NodeId (rf);
  printf_P (PSTR ("Node ID: 0x%02X\n"), ret);
  assert (ret == NODE_ID);
  printf_P (PSTR ("Success\n\n"));

  // NetId
  printf_P (PSTR ("Test %d - iRf69NetworkId()\n"), t++);
  ret = iRf69NetworkId (rf);
  printf_P (PSTR ("Network ID: 0x%02X\n"), ret);
  assert (ret == NET_ID);
  printf_P (PSTR ("Success\n\n"));

  // temperature
  printf_P (PSTR ("Test %d - iRf69Temperature()\n"), t++);
  ret = iRf69Temperature (rf, 0);
  printf_P (PSTR ("Temperature: %d oC\n"), ret);
  assert (ret != INT_MAX);
  printf_P (PSTR ("Success\n\n"));

  // frequency
  printf_P (PSTR ("Test %d - lRf69Frequency()\n"), t++);
  double f = (double) lRf69Frequency (rf);
  printf_P (PSTR ("Frequency: %.0f Hz (%.3f MHz)\n"), f, f / 1E6);
  assert (ret >= 0);
  printf_P (PSTR ("Success\n\n"));

  // powerlevel/highpower
  printf_P (PSTR ("Test %d - iRf69PowerLevel() & bRf69isHighPower()\n"), t++);
  int level = iRf69PowerLevel (rf);
  printf_P (PSTR ("Power Level: %d (raw)\n"), level);
  assert (level >= 0);
  ret = bRf69isHighPower (rf);
  printf_P (PSTR ("High Power mode: %d\n"), ret);
  assert (ret >= 0);
  if (ret) {
    level = level / 2 + 5;
  }
  else {
    level -= 18;
  }
  printf_P (PSTR ("Power Level: %d dBm\n"), level);
  printf_P (PSTR ("Success\n\n"));

  // rssi
  printf_P (PSTR ("Test %d - iRf69Rssi()\n"), t++);
  ret = iRf69Rssi (rf, false);
  printf_P (PSTR ("RSSI: %i dBm\n"), ret);
  assert (ret != INT_MAX);
  printf_P (PSTR ("Success\n\n"));

  // bRf69isPromiscuous
  printf_P (PSTR ("Test %d - Promiscuous\n"), t++);
  ret = bRf69isPromiscuous (rf);
  printf_P (PSTR ("Promiscuous: %d\n"), ret);
  assert (ret == true);
  ret = iRf69SetPromiscuous (rf, false);
  assert (ret == 0);
  ret = bRf69isPromiscuous (rf);
  assert (ret == false);
  ret = iRf69SetPromiscuous (rf, true);
  assert (ret == 0);
  ret = bRf69isPromiscuous (rf);
  assert (ret == true);
  printf_P (PSTR ("Success\n\n"));

  // iRf69SetEncryptKey
  printf_P (PSTR ("Test %d - AES Encrypt\n"), t++);
  ret = iRf69isEncrypted (rf);
  printf_P (PSTR ("AES Encrypt: %d\n"), ret);
  assert (ret == false);
  ret = iRf69SetEncryptKey (rf, "ABCDABCDABCDABCD");
  assert (ret == 0);
  ret = iRf69isEncrypted (rf);
  assert (ret == true);
  ret = iRf69SetEncryptKey (rf, 0);
  assert (ret == 0);
  ret = iRf69isEncrypted (rf);
  assert (ret == false);
  printf_P (PSTR ("Success\n\n"));

  // iRf69SetEncryptKey
  printf_P (PSTR ("Test %d - iRf69RcCalibration()\n"), t++);
  ret = iRf69RcCalibration (rf);
  assert (ret == 0);
  printf_P (PSTR ("Success\n\n"));

  // iRf69Sleep
  printf_P (PSTR ("Test %d - iRf69Sleep()\n"), t++);
  ret = iRf69Sleep (rf);
  assert (ret == 0);
  printf_P (PSTR ("Success\n\n"));

  for (;;) {

    // iRf69CanSend / iRf69Send / iRf69SendWithRetry
    // Ajout iRf69Poll()
    ret = iRf69CanSend (rf);
    assert (ret >= 0);
    if (ret) {
      static int count;
      char msg[20];

      snprintf_P (msg, sizeof (msg), PSTR("Coucou %d"), count++ % 10);
      ret = iRf69Send (rf, RF69_BROADCAST_ADDR, msg, strlen (msg), false);
      assert (ret == 0);
      printf_P (PSTR("T>'%s' - Rssi %d dBm\n"), msg, iRf69Rssi(rf,false));
      delay_ms (5000);
    }

    ret = iRf69ReceiveDone (rf);
    assert (ret >= 0);
    if (ret) {
      
      if (iRf69DataLength (rf)) {
        printf_P (PSTR("R>%s\n"), sRf69Data (rf));
      }
      
      ret = iRf69AckRequested (rf);
      assert (ret >= 0);
      if (ret) {
        ret = iRf69SendAck (rf, 0, 0);
        assert (ret == 0);
      }
    }
  }
  return 0;
}

/* ========================================================================== */
