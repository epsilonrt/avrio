/*
 * Tests unitaires du module USI en esclave (partie maître)
 *
 * Ce test est conçu pour être utilisé avec un celui se trouvant dans le dossier
 * test/net/twi/slave/usi:
 * - Test 1
 *   Le maître du bus I2C envoie un octet à l'esclave et vérifie que l'esclave
 *   renvoie un ACK. Il affiche le résulat du test sur le terminal série.
 *   L'esclave mémorise l'octet et bascule l'état de la LED
 * - Test 2
 *   Le maître du bus I2C lit un octet dans l'esclave et vérifie que l'esclave
 *   renvoie un ACK. Il affiche le résulat du test sur le terminal série.
 *   L'esclave renvoie le dernier octet reçu et bascule l'état de la LED
 * 
 * Les 2 tests peuvent être enchainés, soit au coup par coup (appui sur la barre
 * espace côté maître) soit très rapidement de façon à stresser l'esclave.
 *
 */
#include <avrio/twi.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/term.h>
#include <avr/pgmspace.h>
#include <stdio.h>

/* constants ================================================================ */
// Adresse de notre circuit sur le bus I2C (alignée à gauche)
#define SLAVE_ADDR 0x20
// Vitesse de transmission de la liaison série
#define TEST_BAUDRATE 115200
// Drapeaux gérant le fonctionnement du test
enum {
  WR_FLAG = 0x01, // Test écriture
  RD_FLAG = 0x02, // Test lecture
  ONESHOT_FLAG = 0x04 // Test au coup par coup
};
/* private variables ======================================================== */
static uint8_t ucWrite;
static uint8_t ucRead;
static uint8_t ucFlag = WR_FLAG | ONESHOT_FLAG;
static uint8_t ucSlaveAddr = SLAVE_ADDR;

// Messages de résulats affichés
static const char cMsg0[] PROGMEM =   "%c(%02d):TWI_SUCCESS\n";
static const char cMsg1[] PROGMEM =   "%c(%02d):TWI_ERROR_DATA_NACK\n";
static const char cMsg2[] PROGMEM =   "%c(%02d):TWI_ERROR_ADDR_NACK\n";
static const char cMsg3[] PROGMEM =   "%c(%02d):TWI_ERROR_ILLEGAL_START_STOP\n";
static const char cMsg4[] PROGMEM =   "%c(%02d):TWI_ERROR_INVALID_SPEED\n";
static const char cMsg5[] PROGMEM =   "%c(%02d):TWI_ERROR_INVALID_LENGTH\n";
static const char cMsg6[] PROGMEM =   "%c(%02d):TWI_ERROR_BUFFER_FULL\n";
static const char cMsg7[] PROGMEM =   "%c(%02d):TWI_ERROR_TIMEOUT\n";
static const char cMsg8[] PROGMEM =   "%c(%02d):TWI_ERROR_ILLEGAL_CODE\n";
PGM_P pcMsg[] = {
  cMsg0,
  cMsg1,
  cMsg2,
  cMsg3,
  cMsg4,
  cMsg5,
  cMsg6,
  cMsg7,
  cMsg8
};

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

// -----------------------------------------------------------------------------
void 
vPrintMode (void) {


  puts_P (PSTR("RETURN to setup"));
  if (ucFlag & ONESHOT_FLAG) {
  
    puts_P (PSTR("SPACE = ONESHOT\n"));
  }
}

// -----------------------------------------------------------------------------
void 
vPrintFlags (void) {

  putchar ((ucFlag & WR_FLAG) ? 'W' : '.');
  putchar ((ucFlag & RD_FLAG) ? 'R' : '.');
  putchar ((ucFlag & ONESHOT_FLAG) ? 'T' : '.');
  printf_P(PSTR("\n@=0x%02X\n"), ucSlaveAddr);
  vPrintMode ();
}

// -----------------------------------------------------------------------------
void 
vPrintMenu (void) {

  printf_P(PSTR("Menu\n"
                " R = Toggle READ\n"
                " W = Toggle WRITE\n"
                " T = Toggle ONESHOT\n"
                " @ = Change slave addr.\n"
                " RETURN = Ok\n"
                " Current: "));

  vPrintFlags();
}


// -----------------------------------------------------------------------------
// Dialogue permettant de modifier les tests en fonction
void 
vUpdateFlag (void) {
  char cKey;
  
  vPrintMenu();
  printf_P(PSTR("Choice ? "));
  
  for(;;) {
  
    cKey = getchar();
    
    switch (cKey) {
    
      case 'W':
      case 'w':
        ucFlag ^= WR_FLAG;
        putchar ((ucFlag & WR_FLAG) ? 'W' : '.');
        break;
      case 'R':
      case 'r':
        ucFlag ^= RD_FLAG;
        putchar ((ucFlag & RD_FLAG) ? 'R' : '.');
        break;
      case 'T':
      case 't':
        ucFlag ^= ONESHOT_FLAG;
        putchar ((ucFlag & ONESHOT_FLAG) ? 'T' : '.');
        break;
      case '@':
        printf_P(PSTR("\nOld slave add. = 0x%02X\n"
                      "New slave addr. ? 0x"), ucSlaveAddr);
        ucSlaveAddr = (uint8_t) iTermGetHex (stdin, 2);
        break;
      case '\r':
      case '\n':
        vPrintFlags();
        return;
      default:
        putchar ('?');
        break;
    }
  }
}
/* main ===================================================================== */
int
main (void) {
  eTwiStatus eError;
  int iIndex;

  vLedInit ();
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  stdin  = &xSerialPort;
 
  vTwiInit ();
  eError = eTwiSetSpeed (400);
  vAssert (eError== TWI_SUCCESS);
  printf_P (PSTR("\n\nTest unitaire TWI-USI Esclave\n"));
  vPrintMenu();
  delay_ms (150);

  for (;;) {

    for (ucWrite = 4; ucWrite <= 16; ucWrite += 2) {
    
      if (ucFlag & ONESHOT_FLAG) {
        char c;
        
        while (usSerialHit() == 0)
          ;
        
        c = getchar();
        if ((c == '\n')||(c == '\r')) {

          vUpdateFlag();
        }
      }
      else {
      
        if (usSerialHit() != 0) {
          
          (void) getchar(); // flush last char
          vUpdateFlag();
        }
      }

      if (ucFlag & WR_FLAG) {
      
        eError = eTwiWrite (ucSlaveAddr, ucWrite);
        iIndex = -eError;
        printf_P (pcMsg[iIndex],'W',ucWrite);
      }
      
      if (ucFlag & RD_FLAG) {
      
        eError = eTwiRead (ucSlaveAddr, &ucRead);
        iIndex = -eError;
        printf_P (pcMsg[iIndex],'R',ucRead);
      }
      
      if (ucFlag & ONESHOT_FLAG) {
      
        vLedToggle (LED_LED1);
        delay_ms (150);
      }
    }
  }
  return 0;
}

/* ========================================================================== */
