#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/keyb.h>
#include <avrio/lcd.h>
#include <avrio/term.h>
#include <avrio/twi.h>
#include <avrio/serial.h>
#include <avr/pgmspace.h>
#include <stdio.h>

/* constants ================================================================ */
// Vitesse de transmission de la liaison série
#define TEST_BAUDRATE 115200

// Drapeaux gérant le fonctionnement du test
enum {
  ONESHOT_FLAG    = 0x01, // Test au coup par coup
  INIT_FLAG       = 0x02, // Test d'initialisation HMI
  LCD_FLAG        = 0x04, // Test LCD de base
  BARGRAPH_FLAG   = 0x08, // Test LCD affichage bargraphe 
  CONTRAST_FLAG   = 0x10, // Test LCD réglage contraste
  BACKLIGHT_FLAG  = 0x20, // Test LCD réglage rétroéclairage
};

#define TIMEOUT_MS 5000
#define LOOP_DELAY_MS 10
#define COUNT_MAX (TIMEOUT_MS/LOOP_DELAY_MS)

/* private variables ======================================================== */
static uint8_t ucContrast  = 16;
static uint8_t ucBackLight = 32;
static uint8_t ucFlag = LCD_FLAG | ONESHOT_FLAG;

/* internal public functions ================================================ */
void vTestInit (void);
void vTestLcd (void);
void vTestBargraph (void);
void vTestContrast (void);
void vTestBacklight (void);
void vPrintMode (void);
void vPrintFlags (void);
void vPrintButtons (void);
void vPrintMenu (void);
void vUpdateFlag (void);
void vAssert (bool test);

/* main ===================================================================== */
int
main (void) {
  
  vLedInit();
  // Leds dispo pour Assert()
  
  vSerialInit (TEST_BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  stderr = &xSerialPort;
  stdin = &xSerialPort;
  stdout = &xLcd;
  // Lcd dispo pour test

  vButInit();
  vKeybInit();
  
  fprintf_P (stderr, PSTR("\n\nTest unitaire Twi HMI\n"));
  vTestInit();
  vPrintMenu();
  
  for (;;) {

    // Affiche les signaux de boutons reçus
    vPrintButtons();
    
    if (ucFlag & ONESHOT_FLAG) {
      char c;
      
      while (usSerialHit() == 0)
        vPrintButtons();
      
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

     
    if (ucFlag & INIT_FLAG) {
    
      vTestInit();
    }
    if (ucFlag & LCD_FLAG) {
    
      vTestLcd();
    }
    if (ucFlag & BARGRAPH_FLAG) {
    
      vTestBargraph();
    }
    if (ucFlag & CONTRAST_FLAG) {
    
      vTestContrast();
    }
    if (ucFlag & BACKLIGHT_FLAG) {
    
      vTestBacklight();
    }
    
    vLcdClear();
    puts_P (PSTR("The end..."));
    
    if (ucFlag & ONESHOT_FLAG) {
    
      vLedToggle (LED_LED1);
      delay_ms (150);
    }

  }
  return 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (25);
      vLedClear (LED_LED1);
      delay_ms (100);
    }
  }
}

// -----------------------------------------------------------------------------
void 
vPrintMode (void) {


  fputs_P (PSTR("RETURN to setup\n"), stderr);
  if (ucFlag & ONESHOT_FLAG) {
  
    fputs_P (PSTR("SPACE = ONESHOT\n\n"), stderr);
  }
}

// -----------------------------------------------------------------------------
void 
vPrintButtons (void) {

  if (usKeybHit() > 0) {
    fprintf_P (stderr, PSTR("\nButtons: "));
  
    while (usKeybHit() > 0) {
    
      fprintf_P (stderr, PSTR("0x%02X "), cKeybGetChar());
    }
  }
}

// -----------------------------------------------------------------------------
void 
vPrintFlags (void) {

  fputc ((ucFlag & INIT_FLAG)       ? 'I' : '.', stderr);
  fputc ((ucFlag & LCD_FLAG)        ? 'L' : '.', stderr);
  fputc ((ucFlag & BARGRAPH_FLAG)   ? 'G' : '.', stderr);
  fputc ((ucFlag & CONTRAST_FLAG)   ? 'C' : '.', stderr);
  fputc ((ucFlag & BACKLIGHT_FLAG)  ? 'B' : '.', stderr);
  fputc ((ucFlag & ONESHOT_FLAG)    ? 'T' : '.', stderr);
  fprintf_P(stderr, PSTR("\nC=%d\nB=%d\n"), ucContrast, ucBackLight);
  vPrintMode ();
}

// -----------------------------------------------------------------------------
void 
vPrintMenu (void) {

  fputs_P(PSTR("Menu\n"
                " I = Init. Test\n"
                " L = LCD Test\n"
                " G = Bargraph Test\n"
                " C = Contrast Test\n"
                " B = Backlight Test\n"
                " T = Toggle ONESHOT\n"
                " c = Contrast change\n"
                " b = Backlight change\n"
                " RETURN = Ok\n"
                " Current: "), stderr);

  vPrintFlags();
}


// -----------------------------------------------------------------------------
// Dialogue permettant de modifier les tests en fonction
void 
vUpdateFlag (void) {
  char cKey;
  
  vPrintMenu();
  fputs_P(PSTR("Choice ? "), stderr);
  
  for(;;) {
  
    cKey = getchar();
    
    switch (cKey) {
    
      case 'I':
      case 'i':
        ucFlag ^= INIT_FLAG;
        fputc ((ucFlag & INIT_FLAG) ? 'I' : '.', stderr);
        break;
      
      case 'L':
      case 'l':
        ucFlag ^= LCD_FLAG;
        fputc ((ucFlag & LCD_FLAG) ? 'L' : '.', stderr);
        break;
      
      case 'G':
      case 'g':
        ucFlag ^= BARGRAPH_FLAG;
        fputc ((ucFlag & BARGRAPH_FLAG) ? 'G' : '.', stderr);
        break;
      
      case 'C':
        ucFlag ^= CONTRAST_FLAG;
        fputc ((ucFlag & CONTRAST_FLAG) ? 'C' : '.', stderr);
        break;
      
      case 'B':
        ucFlag ^= BACKLIGHT_FLAG;
        fputc ((ucFlag & BACKLIGHT_FLAG) ? 'B' : '.', stderr);
        break;
      
      case 'T':
      case 't':
        ucFlag ^= ONESHOT_FLAG;
        fputc ((ucFlag & ONESHOT_FLAG) ? 'T' : '.', stderr);
        break;
        
      case 'c':
        fprintf_P (stderr, PSTR("\nOld contrast = %d\n"
                                "New ? "), ucContrast);
        ucContrast = ucLcdContrastSet ((uint8_t) iTermGetDec (stdin, 2));
        fprintf_P (stderr, PSTR("\nSet = %d\n"), ucContrast);
        break;
        
      case 'b':
        fprintf_P (stderr, PSTR("\nOld backlight = %d\n"
                                "New ? "), ucContrast);
        ucBackLight = ucLcdBacklightSet ((uint8_t) iTermGetDec (stdin, 2));
        fprintf_P (stderr, PSTR("\nSet = %d\n"), ucBackLight);
        break;

      case '\r':
      case '\n':
        vPrintFlags();
        return;
      default:
        fputc ('?', stderr);
        break;
    }
   }
}

// -----------------------------------------------------------------------------
void 
vTestInit (void) {
  int iError;

  fprintf_P (stderr, PSTR("  Init. HMI "));
  iError = iLcdInit();
  if (iError == 0) {
  
    fprintf_P (stderr, PSTR("Ok\n"));
    //             0123456789012345
    puts_P (PSTR("- Test Hmi 1.0 -"));
  }
  else {
  
    fprintf_P (stderr, PSTR("Erreur N° %d\n"), iError);
  }
}

/* -----------------------------------------------------------------------------
 | ------------------------------   Test de base ----------------------------- |
 * ---------------------------------------------------------------------------*/
#define LETTRE_DEBUT  0x41 // Premier caractère affiché

void
vTestLcd (void) {
  char c = LETTRE_DEBUT; 
  xLcdCoord xX;

  vLcdClear ();
  fputs_P (PSTR("Hello World !"),stdout);
  
  do {
    
    /* Positionne le curseur un caractère après "Hello World !" */
    vLcdGotoXY (14, 0);
    putchar (c); // Affiche la lettre en cours */
    xX = (c - LETTRE_DEBUT) % xLcdWidth (); // calcul de la postion où mettre '>'
    if (xX) {
    
      // Si pas début de ligne, se positionne sur '>' précédent
      vLcdGotoXY (xX - 1, 1);
    }
    else {
    
      // Si début de ligne, se positionne en fin de ligne
      vLcdGotoXY (xLcdWidth () - 1, 1);
    }
    putchar(' '); // efface le '>' précédent      
    
    vLcdGotoXY (xX, 1); // Se repositionne pour ...
    putchar ('>'); // Afficher le nouveau '>'
    
    vLedToggle (LED_LED1);
    vPrintButtons();
    delay_ms (200);
  }
  while (c++ < (LETTRE_DEBUT + 26 - 1));
}

/* -----------------------------------------------------------------------------
 | -------------------------   Test de contraste ----------------------------- |
 * ---------------------------------------------------------------------------*/
void
vTestContrast (void) {
  uint16_t usCount;
  
  vLcdClear ();
  printf_P(PSTR("Test Constraste\nC=%02d"), ucContrast);
  
  for (usCount = 0; usCount < COUNT_MAX; usCount++) {
  
    if (xButGet(BUTTON_BUTTON1)) {
    
      if (++ucContrast >= 32) {
        
        ucContrast = 0;
      }
      vLcdGotoXY (0, 1);
      printf_P(PSTR("C=%02d"), ucContrast);
      ucLcdContrastSet (ucContrast);
      delay_ms (50);
      usCount = 0;
    }
    vPrintButtons();
    delay_ms (10);  
  }
}

/* -----------------------------------------------------------------------------
 | -------------------------   Test de backlight ----------------------------- |
 * ---------------------------------------------------------------------------*/
void
vTestBacklight (void) {
  uint16_t usCount;
  
  vLcdClear ();
  //             0123456789012345
  printf_P(PSTR("Test Backlight\nL=%02d"), ucBackLight);
  
  for (usCount = 0; usCount < COUNT_MAX; usCount++) {
  
    if (xButGet(BUTTON_BUTTON1)) {
    
      if (++ucBackLight >= 64) {
        
        ucBackLight = 0;
      }
      vLcdGotoXY (0, 1);
      printf_P(PSTR("L=%02d"), ucBackLight);
      ucLcdBacklightSet (ucBackLight);
      delay_ms (50);
      usCount = 0;
    }
    vPrintButtons();
    delay_ms (10);  
  }
}


#if defined(LCD_BARGRAPH_ENABLE)
/* -----------------------------------------------------------------------------
 | ---------------------------   Test du bargraph ---------------------------- |
 * ---------------------------------------------------------------------------*/

/* Exemple de motifs de bloc pour un afficheur à caractères 8x5 pixels passé
 * à la fonction vBargraphInit() :
 * - Chaque ligne correspond à un motif, un motif fait 8 pixels de haut sur
 *   5 pixels de large. 
 * - Chaque octet de la ligne correspond donc à une ligne du motif, comme le 
 *   motif fait 8 pixels de haut, il y a 8 octets par motif.
 * - Chaque bit de l'octet correspond à un pixel, la largeur du motif étant de
 *   5, les 3 bits de poids forts ne sont pas utilisés et toujours à zéro.
 * Les images correspondant à ces motifs sont fournies à titre d'exemple dans
 * le répertoire ./images
 */
static const PROGMEM uint8_t pucBarPatterns[] = {
  0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F, /* 0. ./images/bloc0.png */
  0x1F, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1F, /* 1. ./images/bloc1.png */
  0x1F, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x1F, /* 2. ./images/bloc2.png */
  0x1F, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1F, /* 3. ./images/bloc3.png */
  0x1F, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1F, 0x1F, /* 4. ./images/bloc4.png */
  0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F  /* 5. ./images/bloc5.png */
};

// -----------------------------------------------------------------------------
void 
vTestBargraph (void) {
  uint8_t ucValue, ucLen;

  for (ucLen = xLcdWidth(); ucLen >= 2; ucLen -= 2) {
  
    vLcdClear ();
    
    // Positionne le curseur afin de centrer le bargraph
    vLcdGotoXY ((xLcdWidth() - ucLen) >> 1, 1);
    vLcdBargraphInit (pucBarPatterns);

    // Affiche Hello et les caractères blocs
    vLcdGotoXY (0, 0);
    fputs_P (PSTR("Hello ! "),stdout);
    for (ucValue = 0; ucValue < 6; ucValue++)
      putchar (ucValue);
    
    for (ucValue = 0; ucValue < 255; ucValue++) {

      /* Valeur variant entre 0 et 255 sur une largeur de ucLen caractères */
      vLcdBargraph (ucValue, 255, ucLen);
      delay_ms (1); // Pour avoir le temps de voir la progression...
      vPrintButtons();
    }
    
    vLedToggle (LED_LED1);
    delay_ms (500); // Pour voir le bargraph complet...
  }
}
#endif

/* ========================================================================== */
