#include <avr/pgmspace.h>
#include <avrio/lcd.h>
#include <avrio/delay.h>
#include <avrio/led.h>

/* constants ================================================================ */
#define TEST_BASE
//#define TEST_CONTRAST
#define TEST_BARGRAPH

#ifdef TEST_CONTRAST
#include <avrio/button.h>
/* private variables ======================================================== */
static uint8_t ucContrast = 16;
#endif

/* internal public functions ================================================ */
void vTestBase (void);
void vTestBargraph (void);
void vTestContrast (void);
void vAssert (bool bTestIsTrue);

/* main ===================================================================== */
int
main (void) {

  vLedInit();
  delay_ms(150);
  
  vAssert (iLcdInit() == 0);
  ucLcdBacklightSet (32);
  ucLcdContrastSet (24);
  stdout = &xLcd;
#ifdef TEST_CONTRAST
  vButInit();
  ucLcdContrastSet (ucContrast);
#endif
  
  for (;;) {

#if !defined(LCD_BARGRAPH_ENABLE) || defined(TEST_BASE)
    vTestBase();
#endif
#ifdef TEST_CONTRAST
    vTestContrast();
#endif
#if defined(LCD_BARGRAPH_ENABLE) && defined(TEST_BARGRAPH)
    vTestBargraph();
#endif
  }
  return 0;
}

/* internal public functions ================================================ */
/* -----------------------------------------------------------------------------
 | ------------------------------   Test de base ----------------------------- |
 * ---------------------------------------------------------------------------*/
#define LETTRE_DEBUT  0x41 // Premier caractère affiché

void
vTestBase (void) {
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
    delay_ms (100);
  }
  while (c++ < (LETTRE_DEBUT + 26 - 1));
}

#if defined(LCD_BARGRAPH_ENABLE) && defined(TEST_BARGRAPH)
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
      // delay_ms (1); // Pour avoir le temps de voir la progression...
    }
    
    vLedToggle (LED_LED1);
    delay_ms (500); // Pour voir le bargraph complet...
  }
}
#endif

// -----------------------------------------------------------------------------
void
vAssert (bool bTestIsTrue) {

  if (bTestIsTrue != true) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

/* -----------------------------------------------------------------------------
 | -------------------------   Test de contraste ----------------------------- |
 * ---------------------------------------------------------------------------*/
#ifdef TEST_CONTRAST
#define TIMEOUT_MS 5000
#define LOOP_DELAY_MS 10
#define COUNT_MAX (TIMEOUT_MS/LOOP_DELAY_MS)
void
vTestContrast (void) {
  uint16_t usCount;
  
  vLcdClear ();
  printf_P(PSTR("Test Constraste\nC=%02d"), ucContrast);
  
  // Positionne le curseur afin de centrer le bargraph
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
    delay_ms (10);  
  }
}
#endif    

/* ========================================================================== */
