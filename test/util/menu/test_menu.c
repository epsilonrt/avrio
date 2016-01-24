#include <avrio/term.h>
#include <avrio/serial.h>

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
prvvSetupPrint (void) {

  puts_P (PSTR ("Print setup..."));
}

// ------------------------------------------------------------------------------
static void
prvvSetupChannel (void) {

  puts_P (PSTR ("Setup channel..."));
}

// ------------------------------------------------------------------------------
static void
prvvSetupPower (void) {

  puts_P (PSTR ("Setup power..."));
}

// ------------------------------------------------------------------------------
static void
prvvSendHello (void) {

  puts_P (PSTR ("Send Hello..."));
}

// ------------------------------------------------------------------------------
static void
prvvSendText (void) {

  puts_P (PSTR ("Send Text..."));
}

// ------------------------------------------------------------------------------
static void
prvvReceive (void) {

  puts_P (PSTR ("Receiving data..."));
}

// ------------------------------------------------------------------------------
static void
prvvReset (void) {

  puts_P (PSTR ("Reset the device..."));
}

/* private variables ======================================================== */
/* 
 * Test Menu ------
 * 1+ Transmission
 *      1- Hello
 *      2- Texte
 *      3+ Configuration
 *      4- Precedent
 * 2- Reception
 * 3+ Configuration
 *      1- Afficher
 *      2- Canal
 *      3- Puissance
 *      4- Precedent
 * 4- Reset
 */

/* 
 * Les messages stockés en mémoire programme
 */
static const char pcTitre[] PROGMEM = "Test Menu";

static const char pcTransmission[] PROGMEM = "Transmission";
static const char pcReception[] PROGMEM = "Reception";
static const char pcConfiguration[] PROGMEM = "Configuration";
static const char pcReset[] PROGMEM = "Reset";

static const char pcHello[] PROGMEM = "Hello";
static const char pcNamee[] PROGMEM = "Texte";

static const char pcAfficher[] PROGMEM = "Afficher";
static const char pcCanal[] PROGMEM = "Canal";
static const char pcPuissance[] PROGMEM = "Puissance";

/* 
 * Menu Configuration
 */
static const xMenuItem pxSetupItems[] PROGMEM = {
  {.xAction.eFlag = MENU_FLAG_ACTION,.xAction.pcName =
   pcAfficher,.xAction.pvFunc = prvvSetupPrint},
  {.xAction.eFlag = MENU_FLAG_ACTION,.xAction.pcName =
   pcCanal,.xAction.pvFunc = prvvSetupChannel},
  {.xAction.eFlag = MENU_FLAG_ACTION,.xAction.pcName =
   pcPuissance,.xAction.pvFunc = prvvSetupPower}
};
static xMenu xSetupMenu = {
  .pcName = pcConfiguration,
  .pxItem = pxSetupItems,
  .iSize = MENU_SIZEOF (pxSetupItems),
  .eFlag = MENU_FLAG_PGM_ALL
};

/* 
 * Menu Transmission
 */
static const xMenuItem pxTransmitItems[] PROGMEM = {
  {.xAction.eFlag = MENU_FLAG_ACTION,.xAction.pcName =
   pcHello,.xAction.pvFunc = prvvSendHello},
  {.xAction.eFlag = MENU_FLAG_ACTION,.xAction.pcName =
   pcNamee,.xAction.pvFunc = prvvSendText},
  {.xSubMenu.eFlag = MENU_FLAG_MENU,.xSubMenu.pcName =
   pcConfiguration,.xSubMenu.pxMenu = &xSetupMenu}
};
static xMenu xTransmitMenu = {
  .pcName = pcTransmission,
  .pxItem = pxTransmitItems,
  .iSize = MENU_SIZEOF (pxTransmitItems),
  .eFlag = MENU_FLAG_PGM_ALL
};

/* 
 * Menu Principal
 */
const xMenuItem pxMainItems[] PROGMEM = {
  {.xSubMenu.pcName = pcTransmission,.xSubMenu.eFlag =
   MENU_FLAG_MENU,.xSubMenu.pxMenu = &xTransmitMenu},
  {.xAction.eFlag = MENU_FLAG_ACTION,.xAction.pcName =
   pcReception,.xAction.pvFunc = prvvReceive},
  {.xSubMenu.eFlag = MENU_FLAG_MENU,.xSubMenu.pcName =
   pcConfiguration,.xSubMenu.pxMenu = &xSetupMenu},
  {.xAction.eFlag = MENU_FLAG_ACTION,.xAction.pcName =
   pcReset,.xAction.pvFunc = prvvReset}
};
static xMenu xMainMenu = {
  .pcName = pcTitre,
  .pxItem = pxMainItems,
  .iSize = MENU_SIZEOF (pxMainItems),
  .pxUp = NULL,
  .eFlag = MENU_FLAG_PGM_ALL
};

static xTerm xMyTerm = {
  .pxInputStream = &xSerialPort,
  .pxOuputStream = &xSerialPort,
  .pxMenu = &xMainMenu
};

/* main ===================================================================== */
int
main (void) {
  int eErr;

  vSerialInit (96, SERIAL_DEFAULT + SERIAL_RW);
  xTransmitMenu.pxUp = &xMainMenu;
  xSetupMenu.pxUp = &xMainMenu;
  stdout = &xSerialPort;

  for (;;) {

    eErr = eMenuPoll (&xMyTerm);
  }
  return 0;
}

/**
 *@}
  =========================================================================== */
