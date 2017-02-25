/**
 * @file
 * @brief Demo réception RFM69 (module rf69)
 * Ce programme attend la réception de messages, les affichent sur la liaison
 * série et retourne un accusé réception si demandé. \n
 * Il est possible de modifier la puissance d'emission et  l'activation du mode 
 * promiscuité. \n
 * Voilà un exemple d'affichage :
 * @code
    ** Demo Gateway RFM69 **
    ----------- Menu -----------
    [space]: start/stop Rx
          p: toggle promiscous
          w: change power level

    Frf: 868000 kHz - Power Level: 0 dBm - Promiscuous: 1
    Own address: [10]
    Receiving data on network 1...

    R[10]<[1] < 'Hello 0' > Ack [RSSI = -28]
    R[10]<[1] < 'Hello 1' > Ack [RSSI = -28]
    ....
    wPower level [-18,13]? 3
    Frf: 868000 kHz - Power Level: 3 dBm - Promiscuous: 1
    Own address: [10]
    Receiving data on network 1...
    R[10]<[1] < 'Hello 10' > Ack [RSSI = -28]
 * @endcode
 *
 * -----------------------------<<WARNING>>-------------------------------------
 * Un module RFM69 est alimenté en 3.3V et n'est donc pas compatible
 * avec ARDUINO sans adaptation de niveau... ATTENTION DANGER !!!
 * -----------------------------------------------------------------------------
 */
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/rf69.h>
#include <avrio/tc.h>
#include <avrio/assert.h>
#include <avr/pgmspace.h>
#include <string.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT     "tty0"

#define DIO0_IRQ  INT0  // Interruption utilisée pour DIO0 (IO2 sur Arduino)
#define MYNODE_ID 10    // Adresse de notre module
#define NET_ID    1     // Numéro de notre réseau

/* main ===================================================================== */
int
main (void) {
  int ret, c;
  int src, level, net;
  bool bPrintStat = true;
  bool bRxOn = true;
  bool bPromiscuous = true;
  
  /*
   * Attention, le LED L de l'Arduino UNO est connectée sur PB5 c'est à dire
   * SCK du SPI, donc comme utilise le bus SPI, LED1 est externe et connectée
   * sur PD7 (IO7) (voir board/avrio-board-led.h)
   */
  vLedInit(); // LED1 s'allume à la réception d'un message

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  // FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  FILE * serial_port = xFileOpen (PORT, O_RDWR, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port; // le port série est la sortie d'erreur
  stdin = serial_port; // le port série est l'entrée standard

  // Affiche le menu
  printf_P (PSTR ("\n\n** Demo Gateway RFM69 **\n"
                  "----------- Menu -----------\n"
                  "[space]: start/stop Rx\n"
                  "      p: toggle promiscous\n"
                  "      w: change power level\n"));

  // Création de l'objet xRf69
  // Les 2 premiers paramètres ne sont pas utilisés sur AVR
  xRf69 * rf = xRf69New (0, 0, DIO0_IRQ);
  assert (rf); // vérifie que le pointeur n'est pas nul

  // Ouverture de la connexion vers le module RFM69
  ret = iRf69Open (rf, eRf69Band868Mhz, MYNODE_ID, NET_ID);
  assert (ret == 0);

  // Lecture de notre identifiant (pour vérification et affichage)
  src = iRf69NodeId (rf);
  assert (src == MYNODE_ID);

  // Lecture de notre  réseau (pour vérification et affichage)
  net = iRf69NetworkId (rf);
  assert (net == NET_ID);

  for (;;) {

    if (iFileDataAvailable (stdin)) { // Caractères reçus sur liaison série ?

      c = getchar();
      // Commandes utilisateur
      switch (c) {
        case ' ': // Arrêt / Marche transmission
          bRxOn = ! bRxOn;
          printf_P (PSTR ("\nRx %s\n"), bRxOn ? "On" : "Off");
          break;

        case 'p': // Basculement mode promiscuité
          bPromiscuous = ! bPromiscuous;
          ret = iRf69SetPromiscuous (rf, bPromiscuous);
          printf_P (PSTR ("\nPromiscuous %s\n"), bPromiscuous ? "On" : "Off");
          break;

        case 'w': // Modification de la puissance d'émission
          do { // demande de saisie
            printf_P (PSTR ("\nPower level [-18,13]? "), RF69_BROADCAST_ADDR);
            scanf ("%d", &ret);
          }
          while ( (ret < -18) || (ret > 13));// tant que la puissance n'est pas correcte
          level = ret + 18; // puissance relative 0 à 31
          ret = iRf69SetPowerLevel (rf, level); // modification puissance
          assert (ret == 0); // vérification absence d'erreur
          bPrintStat = true; // demande affichage des caractèristiques de la liaison
          break;
        default: // tous les autres caractères sont ignorés
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
                frf / 1000, level, bPromiscuous, src , net);
      bPrintStat = false; // on désactive l'affichage
    }

    // Réception des paquets
    if (bRxOn) { // Réception activée
      ret = iRf69ReceiveDone (rf); // a-t-on reçu ?
      assert (ret >= 0);

      if (ret == true) { // Oui
      
        vLedSet (LED_LED1);
        // Lecture niveau de réception
        int rssi = iRf69Rssi (rf, false);
        assert (rssi != INT_MAX);

        // Affichage message
        printf_P (PSTR ("R[%d]<[%d] "), iRf69TargetId (rf), iRf69SenderId (rf));
        if (iRf69DataLength (rf)) {

          printf_P (PSTR ("'%s'"), sRf69Data (rf));
        }

        ret = iRf69AckRequested (rf); // Ack demandé ?
        assert (ret >= 0);

        if (ret == true) { // Oui

          // Envoi Ack (message vide, uniquement Ack)
          ret = iRf69SendAck (rf, 0, 0);
          assert (ret == true);
          printf_P (PSTR (" > Ack")); //  > Ack [RSSI = -28]
        }
        printf_P (PSTR (" [RSSI = %d]\n"), rssi);
        vLedClear (LED_LED1);
      }

    }
  }
  return 0;
}

/* ========================================================================== */
