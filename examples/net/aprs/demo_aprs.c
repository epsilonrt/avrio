/**
 * @file
 * Exemple d'utilisation des modules Ax25 et Afsk
 *
 * Envoie une trame Ax25/APRS de façon périodique et affiche les trames reçues
 * sur la liaison série.
 */
#include <avrio/ax25.h>
#include <avrio/afsk.h>
#include <avrio/tc.h>
#include <avrio/task.h>
#include <avrio/mutex.h>

/* constants ================================================================ */
// Les constantes ci_dessous peuvent être modifiées au besoin par l'utilisateur
#define BAUDRATE  115200
#define PORT      "tty0"

// Période d'envoi des trames en ms
#define TX_PERIOD_MS  5000
// Station destination de la trame envoyée
#define APRS_DST "tlm100"
// Station source de la trame envoyée
#define APRS_SRC "nocall"
// Information APRS envoyé
#define APRS_MSG ">Test %05u SolarPi APRS http://www.btssn.net"

/* private variables ======================================================== */
static xAx25 ax25;
static xAx25Node path[] = AX25_PATH(AX25_CALL(APRS_DST, 0), AX25_CALL(APRS_SRC, 0));
static xTaskHandle xScheduler;
static xMutex xMutexTx = MUTEX_LOCK;

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
// Gestion des trames reçues
static void
vMsgCallBack (struct xAx25Frame *msg) {

  vAx25Print (stdout, msg);
}

// -----------------------------------------------------------------------------
// Cadence l'envoi de trame en dévérouillant le mutex toutes les TX_PERIOD_MS
// millisecondes
static void
vScheduler (xTaskHandle xScheduler) {

  vMutexUnlock (&xMutexTx);
  vTaskStart (xScheduler);
}

/* internal public functions ================================================ */
int
main(void) {

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port; // le port série est la sortie d'erreur
  stdin = serial_port; // le port série est l'entrée standard
  vAfskInit (AFSK_MODE_NOBLOCK);
  vAx25Init (&ax25, &xAfskPort, &xAfskPort, vMsgCallBack);
  sei();

  xScheduler = xTaskCreate (xTaskConvertMs (TX_PERIOD_MS), vScheduler);
  vTaskStart (xScheduler);

  for (;;) {

    vAx25Poll (&ax25);

    if (xMutexTryLock(&xMutexTx) == 0) {
      static uint16_t usCount;
      char msg[80];
      // Numérotation de la trame afin de suivre la perte éventuelle à la
      // réception
      snprintf (msg, sizeof(msg), APRS_MSG, usCount++);

      vAx25SendVia (&ax25, path, countof(path), msg, strlen(msg));
    }
  }
  return 0;
}
