/** @file
 * @brief Démonstration capteur TSL230
 * 
 * Effectue des mesures et les affichent sur le terminal série, les valeurs
 * sont affichées de façon tabulaire afin de pouvoir être traitée par un 
 * tableur, voilà un exemple d'affichage:
 *  Tsl230 Demo
 *  Range:20 W/m2
 *  fo(Hz),Ee(W/m2)
 *  97.520,2.469
 *  ...
 * 
 * Le logiciel serialchart https://code.google.com/archive/p/serialchart peut
 * être utilisé pour afficher les mesures sous forme de graphe. Le fichier
 * de configuration de serialchart (tsl230.scc) doit être modifié afin qu'il
 * corresponde à la liaison série connecté au PC (port=COM1) et à la valeur
 * max d'irradiance (paramètre max de la section [_default_].
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tc.h>
#include <avrio/tsl230.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"

/* private variables ======================================================== */
static double dFreq, dIrradiance;

/* main ===================================================================== */
int
main (void) {
  uint16_t usRange;

  vLedInit ();
  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie 
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard
  // Init. du module TSL230
  vTsl230Init();
  sei(); // Valide les interruptions (utilisées par les modules tsl230 et serial)

  // Lecture plage de mesure intiale
  usRange = usTsl230Range();
  // Effectue un autorange si décommenté
  usRange = usTsl230AutoRange();

  // affiche la plage de mesure et l'entête
  printf ("\nTsl230 Demo\nRange:%u W/m2\nfo(kHz),Ee(W/m2)\n", usRange);

  for (;;) {

    /*
     * Déclenchement mesure et lecture fréquence
     * >> Mettre false à la place de true pour désactiver l'autorange...
     */
    dFreq = dTsl230ReadFreq (true);
    // Convertit la fréquence en irradiance µW/cm²
    dIrradiance = dTsl230FreqToIrradiance (dFreq);
    // Affiche les valeurs fréquence en kHz, irradiance en W/m²
    printf ("%.3f,%.3f\n", dFreq / 1000., dIrradiance / 100.);
    // Temporise 100ms et bascule la led1
    delay_ms (100);
    vLedToggle (LED_LED1);
  }
  return 0;
}
/* ========================================================================== */
