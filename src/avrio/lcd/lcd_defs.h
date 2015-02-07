/**
 * @file lcd_defs.h
 * @brief Définitions afficheur LCD
 * @author epsilonRT. Copyright © 2012 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120501 - Initial version by epsilonRT
 */
#ifndef _AVRIO_LCD_DEFS_H_
#define _AVRIO_LCD_DEFS_H_

#include <avrio/defs.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
#include <avrio/twi_addr.h>

/**
 * @addtogroup lcd_module
 * @{
 */

/* constants ================================================================ */
/**
 * @enum eLcdCursor
 * @brief Constantes utilisées pour spécifier le type de curseur à vLcdEnableCursor()
 */
 typedef enum {

  LCD_CURSOR_NONE = 0,          /**< Pas de curseur affiché (défaut)  */
  LCD_CURSOR_UNDERSCORE = 0x02, /**< Curseur souligné */
  LCD_CURSOR_BLINK = 0x03       /**< Curseur clignotant */
} eLcdCursor;

/* types ==================================================================== */
/**
 * @brief Coordonnée d'un point ou d'un caractère
 */
typedef uint8_t xLcdCoord;

/* structures =============================================================== */
/**
 * @struct xLcdPos
 * @brief Coordonnées d'un point ou d'un caractère
 */
typedef struct xLcdPos {
  
  xLcdCoord xX;
  xLcdCoord xY;
} xLcdPos;
/**
 * @}
 */

/* init. constants ========================================================== */

/**
 * @addtogroup lcd_io
 * @{
 *
 *  @defgroup lcd_io_defs Configuration
 *
 *  Ces constantes permettent de configurer l'interface vers le LCD 
 *  dans le fichier avrio-board-lcd.h
 *  @{
 */
#define LCD_IO_4BITS        7    /**< Bit indiquant qu'une interface est 4 bits */
#define LCD_IO_PIO_8        (0x01 + (0<<LCD_IO_4BITS)) /**< Interface Port IO à 8 bits */
#define LCD_IO_PIO_4        (0x01 + (1<<LCD_IO_4BITS)) /**< Interface Port IO à 4 bits */
#define LCD_IO_TWI           0x02 /**< Interface IĠC */
#define LCD_TWI_PCF8574     (0x03 + (1<<LCD_IO_4BITS)) /**< Interface IĠC à PCF8574 (4 bits) */
/**
 *  @}
 * @}
 */
 
/**
 * @addtogroup lcd_ctrl
 * @{
 *
 *  @defgroup lcd_ctrl_defs Configuration
 *
 *  Ces constantes permettent de configurer le contrôleur graphique dans le 
 *  fichier avrio-board-lcd.h
 *  @{
 */
#define LCD_CTRL_HD44780  0x01  /**< Contrôleur HD44780 ou compatible */
#define LCD_CTRL_PCF2119  0x02  /**< Contrôleur PCF2119 ou compatible */
#define LCD_CTRL_ST7032   0x03  /**< Contrôleur ST7032 ou compatible */
#define LCD_CTRL_THMI     0x04  /**< Contrôleur THMI ou compatible */

/**
 *   @defgroup lcd_ctrl_hd44780_defs Contrôleur HD44780
 *
 *   Ces constantes permettent de configurer un contrôleur graphique HD44780 
 *   dans le fichier avrio-board-lcd.h
 *   @{
 */
/* HD44780 - Bits de contrôle du registre d'instruction */
#define HD44780_ENTM  0x04  /**< Commande Entry Mode Set */
#define HD44780_S     0x01  /**< S=0 Le curseur se décale - S=1 c'est tout l'afficheur */
#define HD44780_ID    0x02  /**< I/D=0 curseur vers la gauche - I/D=1 vers la droite */

#define HD44780_DCTL  0x08  /**< Commande Display on/off Control */
#define HD44780_B     0x01  /**< B=1 Curseur clignotant */
#define HD44780_C     0x02  /**< C=1 Curseur affiché */
#define HD44780_D     0x04  /**< D=1 Afficheur allumé */

#define HD44780_SHFT  0x10  /**< Commande Display or Cursor Shift */
#define HD44780_RL    0x04  /**< R=1 vers la droite */
#define HD44780_SC    0x08  /**< S=1 afficheur */

#define HD44780_FSET  0x20  /**< Commande Function Set */
#define HD44780_F     0x04  /**< F=0 car. 5x8 - F=1 5x10 */
#define HD44780_N     0x08  /**< N=0 1 ligne - N=1 2 lignes */
#define HD44780_DL    0x10  /**< DL=0 4 bits - DL=1 8 bits */
/**
 *  @}
 */

/**
 *   @defgroup lcd_ctrl_pcf2119_defs Contrôleur PCF2119
 *
 *   Ces constantes permettent de configurer un contrôleur graphique PCF2119 
 *   dans le fichier avrio-board-lcd.h
 *   @{
 */

/* PCF2119 - Bits de contrôle du registre d'instruction */
#define PCF2119_ENTM  0x04  /**< Commande Entry Mode Set */
#define PCF2119_S     0x01  /**< S=0 Le curseur se décale - S=1 c'est tout l'afficheur */
#define PCF2119_ID    0x02  /**< I/D=0 curseur vers la gauche - I/D=1 vers la droite */

#define PCF2119_DCTL  0x08  /**< Commande Display on/off Control */
#define PCF2119_B     0x01  /**< B=1 Curseur clignotant */
#define PCF2119_C     0x02  /**< C=1 Curseur affiché */
#define PCF2119_D     0x04  /**< D=1 Afficheur allumé */

#define PCF2119_SHFT  0x10  /**< Commande Display or Cursor Shift */
#define PCF2119_RL    0x04  /**< R=1 vers la droite */
#define PCF2119_SC    0x08  /**< S=1 afficheur */

#define PCF2119_FSET  0x20  /**< Commande Function Set */
#define PCF2119_H     0x01  /**< H=0 Mode HD44780 - H=1 Mode étendu */
#define PCF2119_SL    0x02  /**< Mode Single Line Low current actif si SL=1 */
#define PCF2119_M     0x04  /**< M=0 1 ligne - M=1 2 lignes */
#define PCF2119_DL    0x10  /**< DL=0 4 bits - DL=1 8 bits */
/**
 *  @}
 */

/**
 *   @defgroup lcd_ctrl_st7032_defs Contrôleur ST7032
 *
 *   Ces constantes permettent de configurer un contrôleur graphique ST7032 
 *   dans le fichier avrio-board-lcd.h
 *   @{
 */

/* ST7032 - Bits de contrôle du registre d'instruction */
#define ST7032_ENTM  0x04  /**< Commande Entry Mode Set */
#define ST7032_S     0x01  /**< S=0 Le curseur se décale - S=1 c'est tout l'afficheur */
#define ST7032_ID    0x02  /**< I/D=0 curseur vers la gauche - I/D=1 vers la droite */

#define ST7032_DCTL  0x08  /**< Commande Display on/off Control */
#define ST7032_B     0x01  /**< B=1 Curseur clignotant */
#define ST7032_C     0x02  /**< C=1 Curseur affiché */
#define ST7032_D     0x04  /**< D=1 Afficheur allumé */

#define ST7032_SHFT  0x10  /**< Commande Display or Cursor Shift */
#define ST7032_RL    0x04  /**< R=1 vers la droite */
#define ST7032_SC    0x08  /**< S=1 afficheur */

#define ST7032_FSET  0x20  /**< Commande Function Set */
#define ST7032_IS    0x01  /**< IS=0 Mode HD44780 - IS=1 Mode étendu */
#define ST7032_DH    0x04  /**< Mode Fonte Double (5x16) actif si DH=1 */
#define ST7032_N     0x08  /**< N=0 1 ligne - N=1 2 lignes */
#define ST7032_DL    0x10  /**< DL=0 4 bits - DL=1 8 bits */
/**
 *  @}
 */

/**
 *   @defgroup lcd_ctrl_thmi_defs Contrôleur THMI
 *
 *   Ces constantes permettent de configurer un contrôleur graphique THMI 
 *   dans le fichier avrio-board-lcd.h
 *   @{
 */

/* THMI - Bits de contrôle du registre d'instruction */
#define THMI_ENTM  0x04  /**< Commande Entry Mode Set */
#define THMI_S     0x01  /**< S=0 Le curseur se décale - S=1 c'est tout l'afficheur */
#define THMI_ID    0x02  /**< I/D=0 curseur vers la gauche - I/D=1 vers la droite */

#define THMI_DCTL  0x08  /**< Commande Display on/off Control */
#define THMI_B     0x01  /**< B=1 Curseur clignotant */
#define THMI_C     0x02  /**< C=1 Curseur affiché */
#define THMI_D     0x04  /**< D=1 Afficheur allumé */

#define THMI_SHFT  0x10  /**< Commande Display or Cursor Shift */
#define THMI_RL    0x04  /**< R=1 vers la droite */
#define THMI_SC    0x08  /**< S=1 afficheur */

#define THMI_FSET  0x20  /**< Commande Function Set */
#define THMI_IS    0x01  /**< IS=0 Mode HD44780 - IS=1 Mode étendu */
#define THMI_F     0x04  /**< F=0 car. 5x8 - F=1 5x10 */
#define THMI_N     0x08  /**< N=0 1 ligne - N=1 2 lignes */
#define THMI_DL    0x10  /**< DL=0 4 bits - DL=1 8 bits */

/* Commandes en mode étendu */
#define THMI_SRST  0x01  /**< Reset Logiciel */
#define THMI_FVER  0x02  /**< Lecture Version du Firmware */
#define THMI_FNC3  0x04  /**< Fonction dispo 3 */
#define THMI_FNC2  0x08  /**< Fonction dispo 2 */
#define THMI_FNC1  0x10  /**< Fonction dispo 1 */
#define THMI_BON   0x20  /**< 0: Alim Lcd Off - 1: Alim Lcd On (avec THMI_CSET uniquement) */
#define THMI_CSET  0x40  /**< Commande Contrast Set */
#define THMI_BSET  0x80  /**< Commande Backlight Set */

/**
 *  @}
 */
 
/**
 * @}
 */

/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_LCD_DEFS_H_ not defined */
