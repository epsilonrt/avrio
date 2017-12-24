#include <avrio/led.h>
#include <avrio/delay.h>
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

/* main ===================================================================== */
int 
main (void) {
  struct ad7124_device my_ad7124;                    /* A new driver instance */
  struct ad7124_device *ad7124_handler = &my_ad7124; /* A driver handle to pass around */
  enum ad7124_registers regNr;                       /* Variable to iterate through registers */
  long timeout = 1000;                               /* Number of tries before a function times out */
  long ret = 0;                                      /* Return value */
  long sample;                                       /* Stores raw value read from the ADC */


  /* Initialize AD7124 device. */
  ret = AD7124_Setup (ad7124_handler, AD7124_SLAVE_ID, (ad7124_st_reg *) &ad7124_regs);
  if (ret < 0) {
    /* AD7124 initialization failed, check the value of ret! */
  }
  else {
    /* AD7124 initialization OK */
  }

  /* Read all registers */
  for (regNr = AD7124_Status; (regNr < AD7124_REG_NO) && ! (ret < 0); regNr++) {
    ret = AD7124_ReadRegister (ad7124_handler, &ad7124_regs[regNr]);
  }

  /* Read data from the ADC */
  ret = AD7124_WaitForConvReady (ad7124_handler, timeout);
  if (ret < 0) {
    /* Something went wrong, check the value of ret! */
  }

  ret = AD7124_ReadData (ad7124_handler, &sample);
  if (ret < 0) {
    /* Something went wrong, check the value of ret! */
  }
}
/* ========================================================================== */
