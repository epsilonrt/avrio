/**
 * @file
 * @brief Test de transmission avec un module RFM69
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
#define MYNODE_ID 1
#define TARGET_ID 10
#define NET_ID    1

/* main ===================================================================== */
int
main (void) {
  int ret, c;
  int src, target, level, net;
  bool bPrintStat = true;
  bool bTxOn = true;
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
  printf_P (PSTR ("\n\n** Test Ping RFM69 **\n"
                  "----------- Menu -----------\n"
                  "[space]: start/stop Tx\n"
                  "      p: toggle promiscous\n"
                  "      t: change target node\n"
                  "      w: change power level\n"));

  xRf69 * rf = xRf69New (0, 0, DIO0_IRQ);
  assert (rf);

  ret = iRf69Open (rf, eRf69Band868Mhz, MYNODE_ID, NET_ID);
  assert (ret == 0);

  src = iRf69NodeId (rf);
  assert (src >= 0);

  net = iRf69NetworkId (rf);
  assert (net >= 0);

  target = TARGET_ID;

  for (;;) {

    if (iFileDataAvailable (stdin)) {

      c = getchar();
      // Commandes utilisateur
      switch (c) {
        case ' ':
          bTxOn = ! bTxOn;
          printf_P (PSTR ("\nTx %s\n"), bTxOn ? "On" : "Off");
          break;

        case 'p':
          bPromiscuous = ! bPromiscuous;
          ret = iRf69SetPromiscuous (rf, bPromiscuous);
          printf_P (PSTR ("\nPromiscuous %s\n"), bPromiscuous ? "On" : "Off");
          break;

        case 't':
          ret = -1;
          do {
            printf_P (PSTR ("\nTarget ID (%d for broadcast)? "), RF69_BROADCAST_ADDR);
            scanf ("%d", &ret);
          }
          while ( (ret < 1) || (ret > 255));
          target = ret;
          bPrintStat = true;
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
                      "Transmitting data on network %d to node [%d]...\n"),
                frf / 1000, level, src, bPromiscuous, net, target);
      bPrintStat = false;
    }

    if (bTxOn) {
      // Transmission activée, on envoit...
      ret = iRf69CanSend (rf);
      assert (ret >= 0);
      if (ret) {
        static int count;
        char msg[20];
        snprintf_P (msg, sizeof (msg), PSTR ("Hello %d"), count++);
        printf_P (PSTR ("\nT[%d]>[%d] '%s' > "), src, target, msg);

        ret = iRf69Send (rf, target, msg, strlen (msg), target != RF69_BROADCAST_ADDR);
        assert (ret == true);

        if (target != RF69_BROADCAST_ADDR) {
          int rssi;
          
          ret = iRf69WaitAckReceived (rf, target, 1000);
          rssi = iRf69Rssi (rf, false);
          assert (rssi != INT_MAX);
          switch (ret) {
            case false:
              printf_P (PSTR ("Timeout"));
              break;
            case true:
              printf_P (PSTR ("Ack"));
              break;
            default:
              printf_P (PSTR ("Error %d"), ret);
              break;
          }
          printf_P (PSTR (" [RSSI = %d]"), rssi);
        }
        delay_ms (1000);
      }
    }

    // Réception des paquets hors ACK
    ret = iRf69ReceiveDone (rf);
    assert (ret >= 0);
    if (ret) {

      if (iRf69DataLength (rf)) {

        printf_P (PSTR ("R>%s\n"), sRf69Data (rf));
      }

      ret = iRf69AckRequested (rf);
      assert (ret >= 0);
      if (ret) {

        ret = iRf69SendAck (rf, 0, 0);
        assert (ret == true);
      }
    }
  }
  return 0;
}

/* ========================================================================== */
