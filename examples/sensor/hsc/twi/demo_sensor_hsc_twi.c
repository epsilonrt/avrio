/**
 * @file
 * Démonstration utilisation capteur pression HSC (TWI)
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
#include <avrio/twi.h>
#include <avrio/hsc.h>
#include <avrio/assert.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT "tty0"

void vLedAssert (int i);

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
  
  printf ("\nTest unitaire HSC I2C\n");
  
  vTwiInit ();
  iError = eTwiSetSpeed (100);
  assert (iError == 0);
  iError = iHscInitTwiSensor (&xSensor,  0, 1600, HSC_DEFAULT_TWIADDR);
  assert (iError == 0);

  for (;;) {

    iError = iHscGetRaw (&xSensor, &xRaw);
    if (iError) {

      printf ("Sensor Error: %d\n", iError);
    }
    else {

      vHscRawToValue (&xSensor, &xRaw, &xValue);
      printf ("Press %.02f Temp %.02f\n", xValue.dPress, xValue.dTemp);
    }
    vLedToggle (LED_LED1);
    delay_ms (500);
  }

  return 0;
}

/* ========================================================================== */
