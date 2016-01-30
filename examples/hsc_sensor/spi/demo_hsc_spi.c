/*
 * Exemple capteur de pression HSC MAND015PASA5 (SPI - 5V - 15PSI)
 * Affiche les mesures sur la laison série
 */
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/spi.h>
#include <avrio/hsc.h>

/* constants ================================================================ */
#define BAUDRATE 38400
// Division de la fréquence d'horloge SPI
#define SPI_DIV SPI_DIV32 // Fsclk 800 KHz max.
// Capteur 1.6 Bar
#define P_MAX 1600.0f
// Capteur 15 PSI
//#define P_MAX (PSI_TO_PA(15)/100.0f)

/* internal public functions ================================================ */
void vSensorSelect (bool bEnable);
void vAssert (bool bTest);

/* main ===================================================================== */
int
main (void) {
  int iError;
  xHscSensor xSensor;
  xHscRaw xRaw;
  xHscValue xValue;

  vLedInit();
  // Init. liaison série
  vSerialInit (BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  stderr = &xSerialPort;
  sei();
  printf ("\nTest unitaire HSC SPI\n");
  
  // Init. bus SPI
  vSpiSetSsAsOutput();      // Master Only.
  vSpiMasterInit (SPI_DIV); // Fsclk 800 KHz max.
  
  // Init. capteur
  iHscInitSpiSensor (&xSensor,  0, P_MAX, 0, vSensorSelect);

  for (;;) {

    // Lecture valeurs brutes pression et température
    iError = iHscGetRaw (&xSensor, &xRaw);
    if (iError) {

      printf ("Sensor Error: %d\n", iError);
    }
    else {
      // Conversion valeures brutes en grandeurs physiques et affichage 
      vHscRawToValue (&xSensor, &xRaw, &xValue);
      printf ("Press %.02f Temp %.02f\n", xValue.dPress, xValue.dTemp);
    }
    // Mesure toutes les 0.5 s, on bascule la LED1
    vLedToggle (LED_LED1);
    delay_ms (500);
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
