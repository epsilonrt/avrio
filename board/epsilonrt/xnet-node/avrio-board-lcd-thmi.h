/*
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use, 
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>. 
 * 
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 */
#ifndef _AVRIO_BOARD_LCD_THMI_H_
#define _AVRIO_BOARD_LCD_THMI_H_

#include <avrio/lcd/lcd_defs.h>
#include <avr/io.h>

/* Interface mat�rielle ===================================================== */
/* Choix de l'interface reliant l'afficheur au microcontr�leur LCD_IO :
 * Valeurs possibles:
 * LCD_IO_PIO_4       -> Interface sur port IO avec bus afficheur 4 bits
 * LCD_IO_PIO_8       -> Interface sur port IO avec bus afficheur 8 bits
 * LCD_IO_TWI         -> Interface I�C native (Le contr�leur int�gr� est I�C)
 * LCD_TWI_PCF8574 -> Afficheur en 4 bits reli� � un PCF8574 (I�C)
 */
#define LCD_IO LCD_IO_TWI

/* Valide la compilation et l'utilisation de la lecture dans l'afficheur (BF) */
//#define LCD_IO_READ_ENABLE

/* Broche de RESET (effectu� � l'init si LCD_IO_RST_BIT d�fini */
//#define LCD_IO_RST_BIT    6
/* Port utilis� par la broche de RESET */
#define LCD_IO_RST_DDR    DDRD
#define LCD_IO_RST_PORT   PORTD
/* Niveau d'activation du RESET
 * Actif � l'�tat haut = 1
 * Actif � l'�tat bas  = 0
 */
#define LCD_IO_RST_LEVEL  1
/* Dur�e d'impulsion de RESET en ms */
#define LCD_IO_RST_PW     1
/* Temps d'attente apr�s de RESET */
#define LCD_IO_RST_DELAY  2

/* Interface LCD_IO_PIO ============ */

/* Valide l'inversion du bus de donn�es pour faciliter le routage du PCB :
 * DB7->DB0...DB0->DB7 en 8 bits
 * DB7->DB4...DB4->DB7 en 4 bits
 */
//#define LCD_PIO_DATA_REVERSE

/* Largeur de l'impulsion de validation sur E en �s */
#define LCD_PIO_E_PW       1

/* - Num�ro des bits de port utilis�s en PIO - */
#define LCD_PIO_RS_BIT   6
#define LCD_PIO_E_BIT    7
//#define LCD_PIO_RW_BIT   6
//#define LCD_PIO_RD_BIT   7

/* - Ports utilis�s en PIO - */
#define LCD_PIO_DATA_PORT  PORTA
#define LCD_PIO_DATA_DDR   DDRA

#define LCD_PIO_RS_PORT    PORTC
#define LCD_PIO_RS_DDR     DDRC

#define LCD_PIO_E_PORT     PORTC
#define LCD_PIO_E_DDR      DDRC

//#define LCD_PIO_RW_PORT    PORTD
//#define LCD_PIO_RW_DDR     DDRD

//#define LCD_PIO_RD_PORT    PORTD
//#define LCD_PIO_RD_DDR     DDRD

/* -- Interface LCD_IO_PIO_4 ======= */
/* - Num�ro du fil du port reli� � DB4 en PIO4 - */
#define LCD_PIO_DATA_DB4 4

/* Interface LCD_IO_TWI ============ */
/* Vitesse du bus I2c en KHz, si d�finie, le module LCD initialise le module
 * TWI ma�tre avec cette vitesse, sinon, il est du ressort de l'utilisateur
 * d'initialiser le module TWI ma�tre AVANT d'initialiser le module LCD.
 */
#define LCD_TWI_MASTER_INIT 100

/* Adresse I2c du circuit contr�lant l'afficheur */
#define LCD_TWI_DEVICE_ADDR THMI_BASE

/* -- Interface LCD_TWI_PCF8574 = */
/* Valide l'inversion du bus de donn�es pour faciliter le routage du PCB :
 * DB7->DB0...DB0->DB7 en 8 bits
 * DB7->DB4...DB4->DB7 en 4 bits
 */
//#define LCD_PCF8574_DATA_REVERSE

/* - Num�ro des bits de port utilis�s avec PCF8574 - */
#define LCD_PCF8574_RS_BIT   0
#define LCD_PCF8574_E_BIT    1
#define LCD_PCF8574_DATA_DB4 2
//#define LCD_PCF8574_RW_BIT   6

/* Contr�leur Afficheur ===================================================== */
/* Choix du contr�leur LCD_CTRL:
 * Valeurs possibles:
 * - LCD_CTRL_HD44780
 * - LCD_CTRL_PCF2119
 * - LCD_CTRL_ST7032
 * - LCD_CTRL_THMI
 */
#define LCD_CTRL LCD_CTRL_THMI

/* Validation de la prise en charge de l'affichage d'ic�nes */
#define LCD_CTRL_ICON_ENABLE

/* Valide la prise en charge du r�tro-�clairage (avrio-board-lcdbacklight.h) */
#define LCD_CTRL_BACKLIGHT_ENABLE

/* Valide la prise en charge du contraste */
//#define LCD_CTRL_CONTRAST_ENABLE

/* Contr�leur HD44780 =============== */
/* Configuration Function Set: HD44780_FSET
 * + HD44780_DL -> DL=0 si interface 4 bits / DL=1 si interface 8 bits
 * + HD44780_N  -> N=0 1 ligne              / N=1 2 lignes
 * + HD44780_F  -> F=0 Caract�re 5x8        / F=1 Caract�re 5x10 (uniquement en N=0)
 */
#define LCD_CTRL_HD44780_INIT_FSET (HD44780_FSET + HD44780_N)

/* Configuration Entry Mode Set: HD44780_ENTM
 * + HD44780_ID -> I/D=0 curseur vers la gauche / I/D=1 vers la droite
 * + HD44780_S  -> S  =0 Le curseur se d�cale   / S  =1 c'est tout l'afficheur
 */
#define LCD_CTRL_HD44780_INIT_ENTM (HD44780_ENTM + HD44780_ID)

/* Configuration Display On/Off Control: HD44780_DCTL
 * + HD44780_D -> D=1 Afficheur allum�   / D=0 �teint
 * + HD44780_C -> C=1 Curseur affich�    / C=0 pas de curseur
 * + HD44780_B -> B=1 Curseur clignotant / B=0 Curseur _
 */
#define LCD_CTRL_HD44780_INIT_DCTL (HD44780_DCTL + HD44780_D)

/* Contr�leur PCF2119 =============== */
/* Table de caract�res de l'afficheur
 * - A: ASCII
 * - D: ASCII tout majuscule
 * - F,R,S: non ASCII, table partielle de 0x20 � 0x7A d�cal�e de 0x80
 */
#define LCD_CTRL_PCF2119_CHAR_SET 'R'

/* Valeur minimale dans le registre de r�glage de la tension VA qui permet
 * de r�gler le contraste � l'aide du g�n�rateur de tension interne
 * 0 d�valide le g�n�rateur interne
 */
#define LCD_CTRL_PCF2119_VA_MIN 23

/* Configuration Function Set: PCF2119_FSET
 * + PCF2119_DL -> DL=0 si interface 4 bits / DL=1 si interface 8 bits (I�C)
 * + PCF2119_M  -> M=0 1 ligne              / M=1 2 lignes
 * + PCF2119_SL -> Mode Single Line Low current actif si SL=1 (�conome en �nergie)
 */
#define LCD_CTRL_PCF2119_INIT_FSET (PCF2119_FSET + PCF2119_DL + PCF2119_M)

/* Configuration Display On/Off Control: PCF2119_DCTL
 * + PCF2119_D -> D=1 Afficheur allum�   / D=0 �teint
 * + PCF2119_C -> C=1 Curseur affich�    / C=0 pas de curseur
 * + PCF2119_B -> B=1 Curseur clignotant / B=0 Curseur _
 */
#define LCD_CTRL_PCF2119_INIT_DCTL (PCF2119_DCTL + PCF2119_D)

/* Configuration Entry Mode Set: PCF2119_ENTM
 * + PCF2119_ID -> I/D=0 curseur vers la gauche / I/D=1 vers la droite
 * + PCF2119_S  -> S  =0 Le curseur se d�cale   / S  =1 c'est tout l'afficheur
 */
#define LCD_CTRL_PCF2119_INIT_ENTM (PCF2119_ENTM + PCF2119_ID)

/* Contr�leur ST7032 =============== */

/* Valeur de la tension d'alimentation de l'afficheur en millivolts
 * Permet de r�gler la g�n�ration interne de la tension LCD (V0)
 * 0 d�valide la g�n�ration interne de la tension LCD
 */
#define LCD_CTRL_ST7032_VDD 3300

/* Configuration Function Set: ST7032_FSET
 * + ST7032_DL -> DL=0 si interface 4 bits / DL=1 si interface 8 bits (I�C)
 * + ST7032_N  -> N=0 1 ligne              / N=1 2 lignes
 * + ST7032_DH -> Mode Fonte Double (5x16) actif si DH=1
 */
#define LCD_CTRL_ST7032_INIT_FSET (ST7032_FSET + ST7032_DL + ST7032_N)

/* Configuration Display On/Off Control: ST7032_DCTL
 * + ST7032_D -> D=1 Afficheur allum�   / D=0 �teint
 * + ST7032_C -> C=1 Curseur affich�    / C=0 pas de curseur
 * + ST7032_B -> B=1 Curseur clignotant / B=0 Curseur _
 */
#define LCD_CTRL_ST7032_INIT_DCTL (ST7032_DCTL + ST7032_D)

/* Configuration Entry Mode Set: ST7032_ENTM
 * + ST7032_ID -> I/D=0 curseur vers la gauche / I/D=1 vers la droite
 * + ST7032_S  -> S  =0 Le curseur se d�cale   / S  =1 c'est tout l'afficheur
 */
#define LCD_CTRL_ST7032_INIT_ENTM (ST7032_ENTM + ST7032_ID)

/* Contr�leur THMI =============== */
/* Configuration Function Set: THMI_FSET
 * + THMI_DL -> DL=0 si interface 4 bits / DL=1 si interface 8 bits
 * + THMI_N  -> N=0 1 ligne              / N=1 2 lignes
 * + THMI_F  -> F=0 Caract�re 5x8        / F=1 Caract�re 5x10 (uniquement en N=0)
 */
#define LCD_CTRL_THMI_INIT_FSET (THMI_FSET + THMI_N)

/* Configuration Entry Mode Set: THMI_ENTM
 * + THMI_ID -> I/D=0 curseur vers la gauche / I/D=1 vers la droite
 * + THMI_S  -> S  =0 Le curseur se d�cale   / S  =1 c'est tout l'afficheur
 */
#define LCD_CTRL_THMI_INIT_ENTM (THMI_ENTM + THMI_ID)

/* Configuration Display On/Off Control: THMI_DCTL
 * + THMI_D -> D=1 Afficheur allum�   / D=0 �teint
 * + THMI_C -> C=1 Curseur affich�    / C=0 pas de curseur
 * + THMI_B -> B=1 Curseur clignotant / B=0 Curseur _
 */
#define LCD_CTRL_THMI_INIT_DCTL (THMI_DCTL + THMI_D)

/* Broche reli�e au signal HIRQ (� mettre en commentaire si non utilis�) */
#define LCD_CTRL_THMI_NRDY 2
/* Registre PORT de la broche reli�e au signal HIRQ */
#define LCD_CTRL_THMI_NRDY_PORT PORTD
/* Registre DDR de la broche reli�e au signal HIRQ */
#define LCD_CTRL_THMI_NRDY_DDR  DDRD
/* Registre PIN de la broche reli�e au signal HIRQ */
#define LCD_CTRL_THMI_NRDY_PIN  PIND

/* Afficheur ================================================================ */
/* Nombre de lignes */
#define LCD_HEIGHT 2
/* Nombre de caract�res par ligne */
#define LCD_WIDTH  16
/* Largeur d'un caract�re en pixels */
#define LCD_PATTERN_WIDTH 5
/* Hauteur d'un caract�re en pixels */
#define LCD_PATTERN_HEIGHT 8

/* Contraste par d�faut */
#define LCD_DEFAULT_CONTRAST 16

/* Valide la compilation des fonctions de bargraph */
//#define LCD_BARGRAPH_ENABLE

/* ========================================================================== */
#endif  /* _AVRIO_BOARD_LCD_THMI_H_ not defined */
