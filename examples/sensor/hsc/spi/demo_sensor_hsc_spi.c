/**
 * @file
 * Démonstration utilisation capteur pression HSC (SPI)
 * 
 * Effectue des mesures et les affichent sur le terminal série, les valeurs
 * sont affichées de façon tabulaire afin de pouvoir être traitée par un 
 * tableur, voilà un exemple d'affichage:
 *  HSC SPI Demo
 *  P(hPa),T(oC)
 *  1010.25,23.2
 *  ...
 * Le logiciel serialchart https://code.google.com/archive/p/serialchart peut
 * être utilisé pour afficher les mesures sous forme de graphe. Le fichier
 * de configuration de serialchart (hsc.scc) doit être modifié afin qu'il
 * corresponde à la liaison série connecté au PC (port=COM1).
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/tc.h>
#include <avrio/spi.h>
#include <avrio/hsc.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"

// Division de la fréquence d'horloge SPI
#define SPI_DIV SPI_DIV32 // Fsclk 800 KHz max.
// Capteur 1.6 Bar
#define P_MAX 1600.0f
// Capteur 15 PSI
//#define P_MAX (PSI_TO_PA(15)/100.0f)

/* internal public functions ================================================ */
void vSensorSelect (bool bEnable);

/* main ===================================================================== */
int
main (void) {
  int iError;
  xHscSensor xSensor;
  xHscRaw xRaw;
  xHscValue xValue;

  vLedInit();
  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en sortie 
  FILE * serial_port = xFileOpen (PORT, O_WRONLY, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port;
  sei();
  
  printf ("\nHSC SPI Demo\nP(hPa),T(oC)\n");
  
  // Init. bus SPI
  vSpiSetSsAsOutput();      // Master Only.
  vSpiMasterInit (SPI_DIV); // Fsclk 800 KHz max.
  
  // Init. capteur
  iHscInitSpiSensor (&xSensor,  0, P_MAX, 0, vSensorSelect);

  for (;;) {

    // Lecture valeurs brutes pression et température
    iError = iHscGetRaw (&xSensor, &xRaw);
    if (iError == 0) {

      // Conversion valeures brutes en grandeurs physiques et affichage 
      vHscRawToValue (&xSensor, &xRaw, &xValue);
    // Affiche les valeurs de pression et température
      printf ("%.2f,%.2f\n", xValue.dPress, xValue.dTemp);
    }
    else {
      
      printf ("Sensor Error: %d\n", iError);
    }
    // Temporise 100ms et bascule la led1
    delay_ms (100);
    vLedToggle (LED_LED1);
  }

  return 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
// Fonction pour la sélection du capteur (broche /SS à 0)
void vSensorSelect (bool bEnable) {

  if (bEnable)
    vSpiSetSs();
  else
    vSpiClearSs();
}

/* ========================================================================== */
