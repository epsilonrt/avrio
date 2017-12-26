#include <avrio/led.h>
#include <avrio/delay.h>
#include <stdlib.h>
#include <ad/AD7124.h>      /* AD7124 definitions */
#include <ad/AD7124_regs.h> /* We want to use the ad7124_regs array defined in ad7124_regs.h/.c */

/* constants ================================================================ */
/* macros =================================================================== */
/* structures =============================================================== */
/* types ==================================================================== */
/* private variables ======================================================== */
/* public variables ========================================================= */
/* private functions ======================================================== */
/* internal public functions ================================================ */
void vExit (int code);
void vAssert (int cond);

/* main ===================================================================== */
int
main (void) {
  struct ad7124_device my_ad7124;                    /* A new driver instance */
  struct ad7124_device *ad7124_handler = &my_ad7124; /* A driver handle to pass around */
  enum ad7124_registers regNr;                       /* Variable to iterate through registers */
  long timeout = 10000;                               /* Number of tries before a function times out */
  long ret = 0;                                      /* Return value */
  long sample;                                       /* Stores raw value read from the ADC */

  vLedInit();

  /* Initialize AD7124 device. */
  ret = AD7124_Setup (ad7124_handler, AD7124_SLAVE_ID, (ad7124_st_reg *) &ad7124_regs);
  if (ret < 0) {
    vExit (EXIT_FAILURE);
  }
  delay_ms (100);

  /* Read all registers */
  for (regNr = AD7124_Status; (regNr < AD7124_REG_NO) && ! (ret < 0); regNr++) {
    ret = AD7124_ReadRegister (ad7124_handler, &ad7124_regs[regNr]);
    if (ret < 0) {
      vExit (EXIT_FAILURE);
    }
  }

  ad7124_regs[AD7124_ADC_Control].value |= AD7124_ADC_CTRL_REG_REF_EN;
  ret = AD7124_WriteRegister (ad7124_handler, ad7124_regs[AD7124_ADC_Control]);
  if (ret < 0) {
    vExit (EXIT_FAILURE);
  }


  for (;;) {

    /* Read data from the ADC */
    ret = AD7124_WaitForConvReady (ad7124_handler, timeout);
    if (ret < 0) {
      vExit (EXIT_FAILURE);
    }

    ret = AD7124_ReadData (ad7124_handler, &sample);
    if (ret < 0) {
      vExit (EXIT_FAILURE);
    }

    vLedToggle (LED_LED2);
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vExit (int code) {


  while (code == EXIT_FAILURE) {

    vLedSet (LED_LED1);
    delay_ms (10);
    vLedClear (LED_LED1);
    delay_ms (50);
  }
}

// -----------------------------------------------------------------------------
void
vAssert (int cond) {

  if (cond == 0) {

    vExit (EXIT_FAILURE);
  }
}

/* ========================================================================== */
