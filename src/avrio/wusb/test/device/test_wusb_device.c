/**
 * Copyright © 2011-2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 *
 * @file test_net_wusb_device.c
 * @brief Test unitaire Couche Device WirelessUSB
 *
 * Revision History ------------------------------------------------------------
 *    20111003 - Initial version by epsilonRT
 */
#include <avrio/wusb/net/device/device.h>
#include <avrio/delay.h>
#include <avrio/led.h>
#include <avrio/button.h>
#include <avrio/semaphore.h>
#include <avrio/term.h>
#include <avrio/serial.h>
#include <stdlib.h>
#include <ctype.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
#define MAX_PACKET_SIZE 40
#define BAUDRATE 38400
#define TEST_PNCODE 1

/* internal public functions ================================================ */
void vDeviceInit (void);

/* Fonctions de test */
void vDeviceTestRx (void);
void vDeviceTestTxHello (void);
void vDeviceTestTxText (void);
void vDeviceTestRssi (void);

/* Fonctions de modification des paramètres */
void vDeviceConfigChannel (void);
void vDeviceConfigPnCode (void);
void vDeviceConfigPaBias (void);
void vDeviceConfigThreshold (void);
void vDeviceConfigSyncount (void);

/* Fonctions d'affichage des paramètres */
void vDevicePrintConfig (void);
void vDevicePrintMid (void);
void vDevicePrintRegisters (void);

void vDevicePrintChannel (void);
void vDevicePrintPnCode (void);
void vDevicePrintPaBias (void);
void vDevicePrintThreshold (void);
void vDevicePrintSyncount (void);

/* Fonctions annexes */
void vInit (void);
void vMenuLoop (void);
void vAssert (bool xTest);
void vActLedToggle (void);
int  iGetInt (uint8_t ucSize); /* 4 chiffres max. */
void vStrToPnCode (uint8_t * pucPnCode, const char * pcStr);
void vNewLine (void);
void vPrintHexByte (uint8_t ucByte);
void vPrintHexBytes (uint8_t * ucBytes, int8_t iLen); /* en ordre inverse */
void vPrintStr_P (const char * pcStr);

/* main ===================================================================== */
int
main (void) {
  vInit ();

  for (;;) {

    vMenuLoop ();
  }
}

/* private variables ======================================================== */
static volatile uint8_t ucRxLen;
static volatile uint8_t ucRxPacket[MAX_PACKET_SIZE];
static volatile uint8_t ucRxValid[MAX_PACKET_SIZE];
static volatile uint16_t usOverRunCount;
static volatile uint16_t usUnvalidCount;

static size_t xTxLen;
static uint8_t ucBuffer[MAX_PACKET_SIZE];
static xSem xRxPacketReceived;
static uint8_t ucPnCode[8];

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
static void
prvvDeviceTestTx (void) {

  printf_P (PSTR ("\nTransmission -> %s\n"), ucBuffer);
  vWDevTransmit (ucBuffer, xTxLen);
}

// -----------------------------------------------------------------------------
static const char pcHelloWorld[] PROGMEM = "Hello World !";
void
vDeviceTestTxHello (void) {

  xTxLen = strlen_P (pcHelloWorld) + 1;
  memcpy_P (ucBuffer, pcHelloWorld, xTxLen);
  printf_P (PSTR ("Test de transmission %s\n"), ucBuffer);
  prvvDeviceTestTx ();
}

// -----------------------------------------------------------------------------
void
vDeviceTestTxText (void) {

  printf_P (PSTR ("Test de transmission d'un texte\nEnter un texte (max %d) ? "), 
              MAX_PACKET_SIZE - 1);

  xTxLen = iTermGetStr ((char *) ucBuffer, MAX_PACKET_SIZE - 1, stdin) + 1;
  if (xTxLen) {
  
    prvvDeviceTestTx ();
  }
}

// -----------------------------------------------------------------------------
void
vDeviceTestRx (void) {
  uint8_t i, c;

  for (;;) {
  
    c = 0;
    vPrintStr_P (PSTR(  "\nTest Reception\n"
                    "Appuyer sur le bouton 1 pour interrompre\n"
                    "Attente trame"));
    
    vWDevEnable (bRX_ENABLE);
    while (xSemTryWait (&xRxPacketReceived) != 0) {
    
      vActLedToggle ();
      if (xButGet (BUTTON_BUTTON1)) {
      
        break;
      }
      
      if (c++ < 160) {
      
        if ((c % 4) == 0)
          putchar ('.'); // Un point tous les 4 octets reçus
      } 
      else {
        
        c = 0;
        vNewLine (); // Nouvelle ligne de 40 caractères
      }
      delay_ms (250);
    }
    
    /* Trame reçue */
    if (ucRxLen) {
    
      puts_P (PSTR ("\a\n>>>> Trame recue ! <<<<"));
      
      if (ucRxPacket[ucRxLen - 1] == 0) {
      
        /* La trame reçue est une chaine de caractères */
        printf_P (PSTR ("\nTexte=[%s]\n\n"), ucRxPacket);
      }
      
      vPrintStr_P (PSTR("Trame\n"));
      /* Affichage des caractères imprimables */
      for (i = 0; i < ucRxLen; i++) {
      
        if (isprint (ucRxPacket[i])) {
        
          printf_P (PSTR (" %c "), ucRxPacket[i]);
        }
        else {
        
          vPrintStr_P (PSTR("   "));
       }
      }
      vNewLine ();

      /* Affichage des octets de données en hexa */
      for (i = 0; i < ucRxLen; i++) {
      
        vPrintHexByte (ucRxPacket[i]);
      }
      vNewLine ();

      /* Affichage des octets de validation en hexa */
      for (i = 0; i < ucRxLen; i++) {
      
       vPrintHexByte ( ucRxValid[i]);
      }
      vNewLine ();
      printf_P (PSTR (  "\nCaracteres    : %d\n"
                        "-Invalides    : %d\n"
                        "-Recouvrements: %d\n\n"), 
                        ucRxLen, usUnvalidCount, usOverRunCount);

      ucRxLen = 0;
      usUnvalidCount = 0;
      usOverRunCount = 0;
    } 
    else {

      vPrintStr_P (PSTR("\nRéception interrompue !\n"));
      break;
    }
  }
}

// -----------------------------------------------------------------------------
// Gestionnaire réception (sous interruption !)
void
vWDevRxCB (uint8_t * RxInfo) {

  if (RxInfo[WDEV_RXSTAT] & bFULL_A) {
  
    if (RxInfo[WDEV_RXSTAT] & bRX_VALID_A) {
    
      /* Octet valide ...*/
      if (ucRxLen == MAX_PACKET_SIZE - 1) {
      
        /* Dépassement de la taille du buffer de réception */
        ucRxLen = 0;
        vWDevDisable ();
        putchar ('M');
        vSemPost (&xRxPacketReceived);
      } 
      else {
      
        /* Mémorisation octet */
        ucRxPacket[ucRxLen] = RxInfo[WDEV_RXDATA];
        ucRxValid[ucRxLen] = RxInfo[WDEV_RXVALID];
        ++ucRxLen;
      }
    } 
    else {
    
      /* Octet invalide ...*/
      ++usUnvalidCount;
    }

  }

  if (RxInfo[WDEV_RXSTAT] & bOVER_A) {
  
    /* Recouvrement d'octet */
    ++usOverRunCount;
  }

  if (RxInfo[WDEV_RXSTAT] & bEOF_A) {
  
    if (ucRxLen > 0) {
      
      /* Fin de trame détectée */
      vWDevDisable ();
      vSemPost (&xRxPacketReceived);
    }
  }
}

// -----------------------------------------------------------------------------
#define RSSI_MEAN_TERMS 32
void
vDeviceTestRssi (void) {
  int8_t siRssi;
  uint8_t ucChan, ucPrevChan;

  ucPrevChan = ucWDevGetChannel ();
  printf_P (PSTR (
    "\nTest de qualite des canaux\n"
    "\tMoy. sur %d echantillons\n"
    "\tLeg:\t- = Occupe --- * = Mauvais\n"
    "   0   1   2   3   4   5   6   7   8   9"), RSSI_MEAN_TERMS);
  
   
  for (ucChan = 0; ucChan <= 77; ucChan++) {
    
    if (ucChan % 10 == 0) {
    
      printf_P (PSTR ("\n%02d "), ucChan);
    }
    vWDevSetChannel (ucChan);
#if RSSI_MEAN_TERMS > 0
    {
      int iSumRssi = 0;
      uint8_t ucCount;
      
      for (ucCount = 0; ucCount < RSSI_MEAN_TERMS; ucCount++) {
      
        iSumRssi += iWDevRssi ();
      }
      siRssi = iSumRssi / RSSI_MEAN_TERMS;
    }
#else
    siRssi = iWDevRssi ();
#endif


    printf_P (PSTR ("%02d%c "), siRssi, 
      (siRssi <= 10 ? ' ' : (siRssi < 28 ? '-' : '*')));
  }

  printf_P (PSTR ("\n\n"));

  vWDevSetChannel (ucPrevChan);  /* restaure le canal d'origine */
}

// -----------------------------------------------------------------------------
void
vDeviceConfigChannel (void) {

  vPrintStr_P (PSTR("\n---Reglage Canal\n"));
  vDevicePrintChannel ();
  vPrintStr_P (PSTR("Canal ? "));
  vWDevSetChannel (iGetInt (3));
  vNewLine ();
}

// -----------------------------------------------------------------------------
void
vDeviceConfigPaBias (void) {

  vPrintStr_P (PSTR("\n---Reglage Puissance\n"));
  vDevicePrintPaBias ();
  vPrintStr_P (PSTR("PA Bias ? "));
  vWDevSetPaBias (iGetInt (2));
  vNewLine ();
}

// -----------------------------------------------------------------------------
void
vDeviceConfigThreshold (void) {

  vPrintStr_P (PSTR("\n---Reglage Seuils\n"));
  vDevicePrintThreshold ();
  vPrintStr_P (PSTR("Seuil 0L [0-127] ? "));
  vWDevSetThresholdLow (iGetInt (4));
  vNewLine ();
  vPrintStr_P (PSTR("Seuil 1L [0-127] ? "));
  vWDevSetThresholdHigh (iGetInt (4));
  vNewLine ();
}

// -----------------------------------------------------------------------------
void
vDeviceConfigSyncount (void) {

  vPrintStr_P (PSTR("\n---Reglage Synthe Lock Count\n"));
  vDevicePrintSyncount ();
  vPrintStr_P (PSTR("Count [0-255] ? "));
  vWDevSetSynthLockCount (iGetInt (4));
  vNewLine ();
}

// -----------------------------------------------------------------------------
void
vDeviceConfigPnCode (void) {
  char ucPnText[17];
  int iLen;

  vPrintStr_P (PSTR("\nReglage PN Code\n"));
  vDevicePrintPnCode ();
  vPrintStr_P (PSTR("Pn Code ? 0x"));
  iLen = iTermGetStr (ucPnText, 17, stdin);
  
  if (iLen == 16) {
  
    vStrToPnCode (ucPnCode, ucPnText);
    vWDevSetPnCode (ucPnCode);
  }
  else {
  
  }
  vNewLine ();
  vDevicePrintPnCode ();
}

// -----------------------------------------------------------------------------
void
vDevicePrintRegisters (void) {
  uint8_t i;

  vPrintStr_P (PSTR("Dump Registers"));
  vPrintStr_P (PSTR("\n---- "));
  
  for (i = 0; i < 16; ++i) {
  
    vPrintHexByte (i);
  }

  for (i = 0; i < 0x39; ++i) {
  
    if ((i % 16) == 0) {
    
      printf_P (PSTR ("\n0x%02X "), i);
    }
    vPrintHexByte (ucWIfcRead (i));
  }
  vNewLine ();
}

// -----------------------------------------------------------------------------
void
vDevicePrintChannel (void) {

  printf_P (PSTR ("Canal = %02d\n"), ucWDevGetChannel ());
}

// -----------------------------------------------------------------------------
void
vDevicePrintPaBias (void) {

  printf_P (PSTR ("PA Bias = %01d\n"), ucWDevGetPaBias ());
}

// -----------------------------------------------------------------------------
void
vDevicePrintThreshold (void) {

  printf_P (  PSTR ("Seuil Niveau Bas = %d\nSeuil Niveau Haut = %d\n"), 
              ucWDevGetThresholdLow (), 
              ucWDevGetThresholdHigh ());
}

// -----------------------------------------------------------------------------
void
vDevicePrintSyncount (void) {

  printf_P (PSTR ("Synthe Count = %d\n"), ucWDevGetSynthLockCount ());
}

// -----------------------------------------------------------------------------
void
vDevicePrintConfig (void) {

  vPrintStr_P (PSTR("\n---Configuration\n"));
  vDevicePrintMid ();
  vDevicePrintChannel ();
  vDevicePrintPaBias ();
  vDevicePrintPnCode ();
  vDevicePrintThreshold ();
}

// -----------------------------------------------------------------------------
void
vDevicePrintMid (void) {
  uint8_t ucMid[4];

  memcpy (ucMid, pucWDevMid (), 4);

  vPrintStr_P (PSTR("Mid: 0x"));
  vPrintHexBytes (ucMid, 4);
  vNewLine ();
}

// -----------------------------------------------------------------------------
void
vDevicePrintPnCode (void) {
  memcpy (ucPnCode, pucWDevGetPnCode (), 8);

  vPrintStr_P (PSTR("PN Code = 0x"));
  vPrintHexBytes (ucPnCode, 8);
  vNewLine ();
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
 *      4- Pn Code
 *      5- Seuils
 *      6- Rssi
 *      7- Registres
 *      8- Precedent
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
static const char pcTexte[] PROGMEM = "Texte";

static const char pcAfficher[] PROGMEM = "Afficher";
static const char pcCanal[] PROGMEM = "Canal";
static const char pcPuissance[] PROGMEM = "Puissance";
static const char pcPnCode[] PROGMEM = "Pn Code";
static const char pcSeuils[] PROGMEM = "Seuils";
static const char pcRssi[] PROGMEM = "Rssi";
static const char pcRegistres[] PROGMEM = "Registres";

/* 
 * Menu Configuration
 */
static const xMenuItem pxSetupItems[] PROGMEM = {
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcAfficher,
    .xAction.pvFunc = vDevicePrintConfig  },
    
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcCanal,
    .xAction.pvFunc = vDeviceConfigChannel  },
    
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcPuissance,
    .xAction.pvFunc = vDeviceConfigPaBias  },
    
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcPnCode,
    .xAction.pvFunc = vDeviceConfigPnCode  },
    
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcSeuils,
    .xAction.pvFunc = vDeviceConfigThreshold  },
    
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcRssi,
    .xAction.pvFunc = vDeviceTestRssi  },

  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcRegistres,
    .xAction.pvFunc = vDevicePrintRegisters  },
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
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcHello,
    .xAction.pvFunc = vDeviceTestTxHello },
    
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcTexte,
    .xAction.pvFunc = vDeviceTestTxText  },
    
  { .xSubMenu.eFlag = MENU_FLAG_MENU,
    .xSubMenu.pcName = pcConfiguration,
    .xSubMenu.pxMenu = &xSetupMenu }
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
  { .xSubMenu.pcName = pcTransmission,
    .xSubMenu.eFlag = MENU_FLAG_MENU,
    .xSubMenu.pxMenu = &xTransmitMenu },
    
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcReception,
    .xAction.pvFunc = vDeviceTestRx },
    
  { .xSubMenu.eFlag = MENU_FLAG_MENU,
    .xSubMenu.pcName = pcConfiguration,
    .xSubMenu.pxMenu = &xSetupMenu  },
    
  { .xAction.eFlag = MENU_FLAG_ACTION,
    .xAction.pcName = pcReset,
    .xAction.pvFunc = vInit }
};
static xMenu xMainMenu = {
  .pcName = pcTitre,
  .pxItem = pxMainItems,
  .iSize = MENU_SIZEOF (pxMainItems),
  .pxUp = NULL,
  .eFlag = MENU_FLAG_PGM_ALL
};

/* 
 * Le terminal
 */
static xTerm xMyTerm = {
  .pxInputStream = &xSerialPort,
  .pxOuputStream = &xSerialPort,
  .pxMenu = &xMainMenu
};

/* internal public functions ================================================ */
/*---- Pseudo noise code tables ------*/
static const uint8_t ucPnCodeTable16kps[] PROGMEM = {
// LSB                                       MSB
  0xD3, 0x64, 0x44, 0x7A, 0x2D, 0xA8, 0xF7, 0x83, // PN Code ID 0  - ucIndex 0
  0xC9, 0x7A, 0x48, 0x71, 0xAA, 0x4E, 0x2C, 0x3F, // PN Code ID 1  - ucIndex 1
  0x82, 0xC7, 0x90, 0x36, 0x21, 0x9E, 0xFF, 0x17, // PN Code ID 2  - ucIndex 2
//0xAD, 0xB6, 0x30, 0x3A, 0x9A, 0xB5, 0x46, 0xA6, // PN Code ID 3
  0xEB, 0x42, 0x7F, 0xEE, 0x5B, 0x9A, 0x5D, 0xBC, // PN Code ID 4  - ucIndex 3
  0xE7, 0x74, 0x77, 0x7A, 0xF8, 0xDD, 0xF5, 0x24, // PN Code ID 5  - ucIndex 4
  0x95, 0xAD, 0x84, 0xDC, 0x94, 0x7C, 0x70, 0x3D, // PN Code ID 6  - ucIndex 5
  0xD4, 0x11, 0x7B, 0x52, 0x37, 0xF0, 0x6A, 0x1E, // PN Code ID 7  - ucIndex 6
  0xAF, 0xBF, 0x33, 0xFC, 0xAA, 0x2B, 0xF5, 0x62, // PN Code ID 8  - ucIndex 7
  0x97, 0x5D, 0xD9, 0x0F, 0xD9, 0x32, 0x56, 0x40, // PN Code ID 9  - ucIndex 8
  0xCA, 0x20, 0xFF, 0xA7, 0xA9, 0xD0, 0x4A, 0x8E, // PN Code ID 10 - ucIndex 9
//0xBD, 0xA6, 0x78, 0x24, 0xAB, 0x31, 0xB3, 0x38, // PN Code ID 11
  0xBE, 0xB5, 0x3D, 0xB8, 0xBF, 0x9D, 0x97, 0x4C, // PN Code ID 12 - ucIndex 10
  0xBD, 0x6D, 0xCA, 0x9F, 0x30, 0x24, 0x5D, 0x0C, // PN Code ID 13 - ucIndex 11
//0x8A, 0x4D, 0xF4, 0xC8, 0xB5, 0x7B, 0x29, 0x29, // PN Code ID 14
  0xAD, 0x95, 0x78, 0xF1, 0xDE, 0x33, 0x14, 0x50, // PN Code ID 15 - ucIndex 12
//0xA7, 0xA6, 0xB3, 0x96, 0x10, 0xAA, 0xA6, 0xD6, // PN Code ID 16
//0x94, 0xA6, 0x66, 0xE3, 0x12, 0x0D, 0xCE, 0xF6, // PN Code ID 17
  0xC9, 0x10, 0xF2, 0xF0, 0xF9, 0xFA, 0x3C, 0x0C, // PN Code ID 18 - ucIndex 13
  0xB3, 0x6F, 0x4E, 0xBF, 0xDB, 0x06, 0xDA, 0xF4, // PN Code ID 19 - ucIndex 14
//0xE6, 0xCF, 0x39, 0x1A, 0x14, 0x4E, 0x9C, 0x93, // PN Code ID 20
//0xD7, 0x9E, 0xB9, 0x86, 0x93, 0x06, 0x2C, 0xC9, // PN Code ID 21
  0xF0, 0xE8, 0x5E, 0x59, 0xAE, 0xD1, 0x08, 0x9E, // PN Code ID 22 - ucIndex 15
  0x8E, 0x2B, 0x8E, 0x7C, 0xBB, 0x8F, 0x90, 0xC0, // PN Code ID 23 - ucIndex 16
  0xE7, 0x49, 0xF8, 0x08, 0x80, 0x26, 0x69, 0x80, // PN Code ID 24 - ucIndex 17
  0xC1, 0x40, 0x80, 0xd0, 0x54, 0x49, 0x2d, 0x7d, // PN Code ID 25 - ucIndex 18
  0xB4, 0x36, 0x5A, 0x80, 0x1B, 0xE6, 0xF2, 0xB6, // PN Code ID 26 - ucIndex 19
  0xF6, 0x05, 0x67, 0xDA, 0x1C, 0x9C, 0xAE, 0x42, // PN Code ID 27 - ucIndex 20
  0x80, 0xB5, 0x8D, 0x14, 0xE0, 0xF7, 0x75, 0x9B, // PN Code ID 28 - ucIndex 21
  0x88, 0x5A, 0x30, 0xB7, 0xB9, 0x98, 0x54, 0xBF, // PN Code ID 29 - ucIndex 22
  0x88, 0xC9, 0xD4, 0x23, 0x97, 0xFC, 0xD1, 0x35, // PN Code ID 30 - ucIndex 23
  0x93, 0x40, 0xBD, 0x5F, 0x26, 0x31, 0xD6, 0xE1, // PN Code ID 31 - ucIndex 24
  0x8C, 0xAF, 0xAE, 0x97, 0x99, 0x08, 0x68, 0xDC, // PN Code ID 32 - ucIndex 25
  0xBA, 0x06, 0x32, 0x0E, 0x16, 0x01, 0x0E, 0xC3, // PN Code ID 33 - ucIndex 26
  0xAC, 0x8F, 0x3E, 0xAB, 0xFA, 0x01, 0x83, 0xE0, // PN Code ID 34 - ucIndex 27
  0x84, 0x7D, 0x9C, 0x46, 0xB8, 0x9C, 0xD5, 0x5C, // PN Code ID 35 - ucIndex 28
  0xB7, 0x4F, 0xA5, 0x9D, 0x5C, 0xFE, 0xC6, 0xF1, // PN Code ID 36 - ucIndex 29
  0xB0, 0xF1, 0x28, 0x0E, 0xDD, 0xB3, 0xB5, 0x58, // PN Code ID 37 - ucIndex 30
//0x9C, 0x32, 0xC5, 0xA4, 0xBA, 0x95, 0xD6, 0x9A, // PN Code ID 38
  0xA1, 0xF4, 0x45, 0x96, 0x56, 0x3B, 0x30, 0x5F, // PN Code ID 39 - ucIndex 31
  0xF8, 0xFE, 0xBD, 0xEF, 0x8A, 0x6E, 0xBC, 0x03, // PN Code ID 40 - ucIndex 32
  0xD6, 0x06, 0xBF, 0x2D, 0x3B, 0x13, 0x17, 0x88, // PN Code ID 41 - ucIndex 33
  0xA9, 0x88, 0x1C, 0xA1, 0x21, 0x30, 0x94, 0xF1, // PN Code ID 42 - ucIndex 34
  0xB4, 0xE3, 0x2F, 0x82, 0xBC, 0x8E, 0xD2, 0xD0, // PN Code ID 43 - ucIndex 35
  0xD0, 0x66, 0xA5, 0x83, 0x9B, 0x47, 0xFA, 0x8C, // PN Code ID 44 - ucIndex 36
  0xB8, 0x0F, 0x31, 0xC8, 0x26, 0x9F, 0xBD, 0x07, // PN Code ID 45 - ucIndex 37
//0x86, 0xAE, 0x89, 0x5E, 0xB1, 0x54, 0xA1, 0xD7, // PN Code ID 46
  0x9D, 0x61, 0x71, 0xB4, 0x89, 0x95, 0x03, 0xEF, // PN Code ID 47 - ucIndex 38
  0xD1, 0xCC, 0x4F, 0x86, 0xD5, 0x97, 0xBA, 0x40, // PN Code ID 48 - ucIndex 39
};

// -----------------------------------------------------------------------------
void
vInit (void) {
  
  /* Init terminal */
  vSerialInit (BAUDRATE / 100, SERIAL_DEFAULT + SERIAL_RW);
  xTransmitMenu.pxUp = &xMainMenu;
  xSetupMenu.pxUp = &xMainMenu;
  stdout = &xSerialPort;
  stdin  = &xSerialPort;

  /* Init variables globales */
  vSemInit (&xRxPacketReceived, 0);
  xTxLen = 0;

  /* Init module WirelessUSB */
  vWDevInit (WDEV_RATE_16KBPS);
  vWDevSetPnCode_P (&ucPnCodeTable16kps[TEST_PNCODE*8]);
  
  sei(); /* Le module Device fonctionne sous interruption... */
}

// -----------------------------------------------------------------------------
void 
vMenuLoop (void) {
  int eErr;

  eErr = eMenuPoll (&xMyTerm);
}

// ------------------------------------------------------------------------------
// Lecture d'un entier
int
iGetInt (uint8_t ucSize) {

  return iTermGetDec (stdin, 4);
}

// ------------------------------------------------------------------------------
void
vAssert (bool test) {

  if (test == false) {

     for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (500);
    }
  }
}

// ------------------------------------------------------------------------------
void 
vActLedToggle (void) {

  vLedToggle (LED_LED1);
}

// ------------------------------------------------------------------------------
void 
vNewLine (void) {

  putchar ('\n');  
}

// ------------------------------------------------------------------------------
void
vPrintHexByte (uint8_t ucByte) {

  printf_P (PSTR ("%02X "), ucByte);
}

// -----------------------------------------------------------------------------
void
vPrintHexBytes (uint8_t * ucBytes, int8_t iLen) {
  int8_t iIndex;

  /* Affichage en ordre inverse (Little Endian) */
  for (iIndex = iLen - 1; iIndex >= 0; iIndex--) {
  
    vPrintHexByte (ucBytes[iIndex]);
  }
}

// -----------------------------------------------------------------------------
void 
vPrintStr_P (const char * pcStr) {

  fputs_P (pcStr, stdout);
}

// -----------------------------------------------------------------------------
static uint8_t
prvucAsciiToInt (char c) {
  c = toupper (c);

  c -= 0x30;
  if (c > 9)
    c -= 7;

  return (uint8_t) c;
}

// -----------------------------------------------------------------------------
void
vStrToPnCode (uint8_t * pucPnCode, const char * pcStr) {
  uint8_t ucByte;
  int8_t iIndex;

  for (iIndex = 14; iIndex >= 0; iIndex -= 2) {
  
    ucByte = prvucAsciiToInt (pcStr[iIndex]);
    ucByte <<= 4;
    ucByte += prvucAsciiToInt (pcStr[iIndex + 1]);
    *pucPnCode++ = ucByte;
  }
}

#if 0
// -----------------------------------------------------------------------------
// tableau entier little endian en str
static char *
leuatoa (char *str, uint8_t * array, size_t len) {
  ssize_t i;

  for (i = len - 1; i >= 0; i--) {
  
    sprintf (&str[(len - 1 - i) * 2], "%02X", array[i]);
  }
  return str;
}
#endif

/* ========================================================================== */
