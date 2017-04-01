/**
 * @file
 * @brief Test de réception avec un module RFM69
 *
 * -----------------------------<<WARNING>>-------------------------------------
 * Un module RFM69 est alimenté en 3.3V et n'est donc pas compatible
 * avec ARDUINO sans adaptation de niveau... ATTENTION DANGER !!!
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
#define MYNODE_ID 10
#define NET_ID    1

/* main ===================================================================== */
int
main (void) {
  int ret, c;
  int src, level, net;
  bool bPrintStat = true;
  bool bRxOn = true;
  bool bPromiscuous = true;

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  // FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  FILE * serial_port = xFileOpen (PORT, O_RDWR, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port; // le port série est la sortie d'erreur
  stdin = serial_port; // le port série est l'entrée standard

  // Affiche le menu
  printf_P (PSTR ("\n\n** Test Gateway RFM69 **\n"
                  "----------- Menu -----------\n"
                  "[space]: start/stop Rx\n"
                  "      p: toggle promiscous\n"
                  "      w: change power level\n"));

  xRf69 * rf = xRf69New (0, 0, DIO0_IRQ);
  assert (rf);

  ret = iRf69Open (rf, eRf69Band868Mhz, MYNODE_ID, NET_ID);
  assert (ret == 0);

  src = iRf69NodeId (rf);
  assert (src >= 0);

  net = iRf69NetworkId (rf);
  assert (net >= 0);

  for (;;) {

    if (iFileDataAvailable (stdin)) {

      c = getchar();
      // Commandes utilisateur
      switch (c) {
        case ' ':
          bRxOn = ! bRxOn;
          printf_P (PSTR ("\nTx %s\n"), bRxOn ? "On" : "Off");
          break;

        case 'p':
          bPromiscuous = ! bPromiscuous;
          ret = iRf69SetPromiscuous (rf, bPromiscuous);
          printf_P (PSTR ("\nPromiscuous %s\n"), bPromiscuous ? "On" : "Off");
          break;

        case 'w':
          ret = -1;
          do {
            printf_P (PSTR ("\nPower level [-18,13]? "), RF69_BROADCAST_ADDR);
            scanf ("%d", &ret);
          }
          while ( (ret < -18) || (ret > 13));
          level = ret + 18;
          ret = iRf69SetPowerLevel (rf, level);
          assert (ret == 0);
          bPrintStat = true;
          break;
        default:
          break;
      }
    }

    if (bPrintStat) {
      // Affiche les infos sur la liaison
      long frf = lRf69Frequency (rf);
      assert (frf >= 0);

      level = iRf69PowerLevel (rf);
      assert (level >= 0);
      ret = bRf69isHighPower (rf);
      assert (ret >= 0);
      if (ret) {

        level = level / 2 + 5;
      }
      else {
        level -= 18;
      }

      bPromiscuous = bRf69isPromiscuous (rf);

      printf_P (PSTR ("\nFrf: %lu kHz - Power Level: %d dBm - Promiscuous: %d\n"
                      "Own address: [%d]\n"
                      "Receiving data on network %d...\n"),
                frf / 1000, level, bPromiscuous, src, net);
      bPrintStat = false;
    }

    if (bRxOn) {
      // Réception des paquets

      ret = iRf69ReceiveDone (rf);
      assert (ret >= 0);

      if (ret) {
        int rssi = iRf69Rssi (rf, false);
        assert (rssi != INT_MAX);

        printf_P (PSTR ("R[%d]<[%d] "), iRf69TargetId (rf), iRf69SenderId (rf));
        if (iRf69DataLength (rf)) {

          printf_P (PSTR ("'%s'"), sRf69Data (rf));
        }

        ret = iRf69AckRequested (rf);
        assert (ret >= 0);

        if (ret) {

          ret = iRf69SendAck (rf, 0, 0);
          assert (ret == true);
          printf_P (PSTR (" > Ack")); //  > Ack [RSSI = -28]
        }
        printf_P (PSTR (" [RSSI = %d]\n"), rssi);
      }
    }
  }
  return 0;
}

/* ========================================================================== */
