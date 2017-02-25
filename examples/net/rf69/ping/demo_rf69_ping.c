/**
 * @file
 * Demo émission-réception RFM69 (module rf69)
 * Ce programme : \n
 * - Transmet des messages 'Hello' à un noeud du réseau (gateway) et attend
 *   un accusé réception (Ack) et affiche le résultat sur la liaison série.
 * - Affiche les messages reçus (autre que Ack) sur la liaison série.
 * .
 * Il est possible de modifier la puissance d'emission, l'adresse du noeud
 * destinataire, l'activation du mode promiscuité. \n
 * Voilà un exemple d'affichage :
 * @code
    ** RFM69 Ping Demo **
    ----------- Menu -----------
    [space]: start/stop Tx
          p: toggle promiscous
          t: change target node
          w: change power level

    Frf: 868000 kHz - Power Level: 0 dBm - Promiscuous: 1
    Own address: [1]
    Transmitting data on network 1 to node [10]...

    T[1]>[10] 'Hello 0' > Ack [RSSI = -28]
    T[1]>[10] 'Hello 1' > Ack [RSSI = -28]
    ....
    tTarget ID (255 for broadcast)? 255
    Frf: 868000 kHz - Power Level: 0 dBm - Promiscuous: 1
    Own address: [1]
    Transmitting data on network 1 to node [255]...

    T[1]>[255] 'Hello 16' >
 * @endcode
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

#define DIO0_IRQ  INT0  // Interruption utilisée pour DIO0 (IO2 sur Arduino)
#define MYNODE_ID 1     // Adresse de notre module
#define TARGET_ID 10    // Adresse du module destinataire de nos messages
#define NET_ID    1     // Numéro de notre réseau

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
  printf_P (PSTR ("\n\n** RFM69 Ping Demo **\n"
                  "----------- Menu -----------\n"
                  "[space]: start/stop Tx\n"
                  "      p: toggle promiscous\n"
                  "      t: change target node\n"
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

  target = TARGET_ID; // L'adresse de la cible est stockée et pourra être modifiée

  for (;;) {

    if (iFileDataAvailable (stdin)) { // Caractères reçus sur liaison série ?

      c = getchar();
      // Commandes utilisateur
      switch (c) {
        case ' ': // Arrêt / Marche transmission
          bTxOn = ! bTxOn;
          printf_P (PSTR ("\nTx %s\n"), bTxOn ? "On" : "Off");
          break;

        case 'p': // Basculement mode promiscuité
          bPromiscuous = ! bPromiscuous;
          ret = iRf69SetPromiscuous (rf, bPromiscuous);
          printf_P (PSTR ("\nPromiscuous %s\n"), bPromiscuous ? "On" : "Off");
          break;

        case 't': // Modification de l'adresse de la cible
          do { // demande de saisie
            printf_P (PSTR ("\nTarget ID (%d for broadcast)? "), RF69_BROADCAST_ADDR);
            scanf ("%d", &ret);
          }
          while ( (ret < 1) || (ret > 255)); // tant que l'adresse n'est pas correcte
          target = ret; // mémorisation adresse cible
          bPrintStat = true; // demande affichage des caractèristiques de la liaison
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
                      "Transmitting data on network %d to node [%d]...\n"),
                frf / 1000, level, bPromiscuous, src, net, target);
      bPrintStat = false; // on désactive l'affichage
    }

    if (bTxOn) {
      // Transmission activée, on envoie...

      // Peut-on émettre ?
      ret = iRf69CanSend (rf);
      assert (ret >= 0);

      if (ret == true) { // oui
        static int count; // compteur de message
        char msg[20]; // buffer de caractères pour cpnstruire le message

        // Construction du message 'Hello XXX' avec XXX = count
        snprintf_P (msg, sizeof (msg), PSTR ("Hello %d"), count++);
        printf_P (PSTR ("\nT[%d]>[%d] '%s' > "), src, target, msg);

        // Envoi du message, on demande un Ack uniquement si la cible n'est pas le broadcast
        ret = iRf69Send (rf, target, msg, strlen (msg), target != RF69_BROADCAST_ADDR);
        assert (ret == true); // vérification trame envoyée

        if (target != RF69_BROADCAST_ADDR) { // si pas broadcast
          int rssi;

          // On attend l'ack de la cible
          ret = iRf69WaitAckReceived (rf, target, 1000);
          if (ret == true) {
            // on récupère le niveau de réception
            rssi = iRf69Rssi (rf, false);
          }
          assert (rssi != INT_MAX);
          // On affiche le résultat
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
        delay_ms (1000); // attente nécessaire pour éviter un débordement réseau
      }
    }

    // Réception des paquets hors ACK

    ret = iRf69ReceiveDone (rf); // a-t-on reçu ?
    assert (ret >= 0);
    
    if (ret == true) { // Oui

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
    }
  }
  return 0;
}

/* ========================================================================== */
