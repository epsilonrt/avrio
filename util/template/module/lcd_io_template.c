#include "avrio-config.h"

#ifdef AVRIO_LCD_ENABLE
#include "avrio-board-lcd.h"

#if LCD_IO == LCD_IO_TEMPLATE
/* macros =================================================================== */
/* constants ================================================================ */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* private functions ======================================================== */
/* public variables ========================================================= */
/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void 
vLcdIoInit (void) {

}

// -----------------------------------------------------------------------------
// Nécessaire si contrôleur LCD avec mode 4 bits (qu'il soit utilisé en 4 ou en 8)
void
vLcdIoWriteNibble (uint8_t ucHighNibble, uint8_t ucAddress) {

}

// -----------------------------------------------------------------------------
void 
vLcdIoWrite (uint8_t ucData, uint8_t ucAddress) {

}

// -----------------------------------------------------------------------------
#ifdef LCD_IO_READ_ENABLE
uint8_t 
ucLcdRead (uint8_t ucAddress) {

}
#endif /* LCD_IO_READ_ENABLE */

/* ========================================================================== */
#endif  /* LCD_IO == LCD_IO_TEMPLATE */
#endif /* AVRIO_LCD_ENABLE defined */
