/*
 * Démonstration utilisation capteur HIH6130
 * Effectue des mesures et les affichent sur le terminal série, les valeurs
 * sont affichées de façon tabulaire afin de pouvoir être traitée par un 
 * tableur, voilà un exemple d'affichage:
 *  HIH6130 Demo
 *  T(oC),H(%)
 *  21.2,53.5
 *  ...
 * Le logiciel serialchart https://code.google.com/archive/p/serialchart peut
 * être utilisé pour afficher les mesures sous forme de graphe. Le fichier
 * de configuration de serialchart (hih6130.scc) doit être modifié afin qu'il
 * corresponde à la liaison série connecté au PC (port=COM1).
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/hih6130.h>
#include <avrio/serial.h>
#include <avrio/assert.h>
#include <stdio.h>

/* constants ================================================================ */
#define TEST_BAUDRATE 38400
#define TEST_SETUP    (SERIAL_DEFAULT + SERIAL_RW + SERIAL_RTSCTS)

/* main ===================================================================== */
int
main (void) {
  eTwiStatus eTwiError;
  eHih6130Error eError;
  xHih6130Data xData;
  
  vLedInit ();
  // Init. liaison série pour l'affichage
  vSerialInit (TEST_BAUDRATE / 100, TEST_SETUP);
  stdout = &xSerialPort;
  stderr = &xSerialPort;
  sei();
  
  printf ("\nHIH6130 Demo\n");
  // Init. bus I²C en master 100 kHz et vérification succès
  vTwiInit ();
  eTwiError = eTwiSetSpeed (100);
  assert (eTwiError == TWI_SUCCESS);

  // Init. capteur et vérification succès
  eError = eHih6130Init (0);
  assert (eError == HIH6130_SUCCESS);
  
  // Affichage entête
  printf ("T(oC),H(%%)\n");
  
  for (;;) {

    
    // Test 1 - Mesure 
    // Le temps d'allumage LED_LED1 correspond au temps de mesure
    vLedSet (LED_LED1);
    // Démarrage mesure et vérification succès
    eError = eHih6130Start();
    assert (eError == HIH6130_SUCCESS);
    
    do {
    
      // Lecture
      eError = eHih6130Read (&xData);
      // Vérif absence d'erreur
      assert (eError >= HIH6130_SUCCESS);
    } while (eError == HIH6130_BUSY);
    vLedClear (LED_LED1);
    
    // Affichage mesures
    printf ("%.1f,%.1f\n", xData.iTemp / 10.0, xData.iHum / 10.0);
    delay_ms (100);
  }
  return 0;
}

/* ========================================================================== */
