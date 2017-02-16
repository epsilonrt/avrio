/**
 * Demo compteur d'événement (counter)
 *
 * Le module counter permet de compter des événements sur une durée donnée
 * (fenêtre temporelle). Il permet d'en déterminer la fréquence.
 * Il utilise le module Task pour générer la fenêtre de mesure.
 * Le dispositif de comptage est
 *
 * Ce programme affiche la fréquence du signal en entrée de comptage du TIMER1
 * T1 (IO5 sur Arduino UNO). La fréquence max. est de 655350 Hz (655.35 kHz).
 *
 * Frequency Meter
 * Window=100 ms
 * Count,Freq
 * 975,9750.0
 * ....
 *
 * Il est prévu pour une carte Arduino UNO, le fichier avrio-board-adc.h dans
 * le répertoire board devra être adapté pour une autre carte.
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tc.h>
#include <avrio/counter.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"

/* private functions ======================================================== */
/*
 * Le module compteur utilise 4 opérations de base pour effectuer son comptage,
 * ces opérations sont :
 * - Une fonction chargée d'initialiser le compteur: */
void vTimerInit (void);
/* - Une fonction chargée de lire le compteur */
uint16_t usTimerRead (void);
/* - Une fonction chargée de remettre à zéro le compteur */
void vTimerClear (void);
/* - Une fonction chargée de démarrer ou arrêter le compteur */
void vTimerEnable (bool En);

/* main ===================================================================== */
int
main (void) {
  xCounter fm; // Objet counter (structure en C...)

  /*
   * Ces 4 opérations sont définies par l'utilisateur et passées au module counter
   * grâce à la structure xCounterOps
   */
  xCounterOps ops = {
    .init = vTimerInit,
    .clear = vTimerClear,
    .enable = vTimerEnable,
    .read = usTimerRead
  };

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard

  /* Initialise la structure xCounter :
   * - le comptage est arrêté en mode eCounterSingle
   * - le compteur est initialisé et remis à zéro
   */
  vCounterInit (&fm, &ops);
  // Réglage de la fenêtre temporelle à 100 ms
  vCounterSetWindow (&fm, 100);

  printf ("Frequency Meter\nWindow=%u ms\nCount,Freq\n", fm.usWindow);

  for (;;) {

    // A l'initialisation, bCounterIsComplete() retourne true
    if (bCounterIsComplete (&fm)) { // fin de la fenêtre temporelle

      double dFreq = dCounterFreq (&fm);
      uint16_t usCount = usCounterCount (&fm);

      printf ("%u,%.1f\n", usCount, dFreq);
      vCounterStart (&fm);
    }
  }
  return 0;
}

/* -----------------------------------------------------------------------------
 * Fonction chargée d'initialiser le compteur
 * TIMER1 en mode normal, non démarré
 */
void
vTimerInit (void) {

  TCCR1A = 0;
  TCCR1B = 0;
}

/* -----------------------------------------------------------------------------
 * Fonction chargée de remettre à zéro le compteur
 */
void
vTimerClear (void) {

  TCNT1 = 0;
}

/* -----------------------------------------------------------------------------
 * Fonction chargée de démarrer ou arrêter le compteur
 */
void
vTimerEnable (bool En) {

  if (En) {

    TCCR1B = 0x07; // Horloge T1 sur front montant
  }
  else {

    TCCR1B = 0; // Stop
  }
}

/* -----------------------------------------------------------------------------
 * Fonction chargée de lire le compteur
 */
uint16_t
usTimerRead (void) {

  return TCNT1;
}

/* ========================================================================== */
