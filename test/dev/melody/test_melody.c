#include <avr/pgmspace.h>
#include <avrio/task.h>
#include <avrio/melody.h>
#include <avrio/button.h>
#include <avrio/led.h>
#include <avrio-config.h>

// #define TEST_TIMER
// #define TEST_PWM

/* private variables ======================================================== */
static const xNote pxStarwarsMelody[] PROGMEM = {
  {29, 12}, {PITCH_PAUSE, 4}, {29, 12}, {PITCH_PAUSE, 4}, {29, 12},
  {PITCH_PAUSE, 4}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 12},
  {PITCH_PAUSE, 4}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 25},
  {PITCH_PAUSE, 8}, {36, 12}, {PITCH_PAUSE, 4}, {36, 12}, {PITCH_PAUSE, 4},
  {36, 12}, {PITCH_PAUSE, 4}, {37, 8}, {PITCH_PAUSE, 4}, {32, 4}, {28, 12},
  {PITCH_PAUSE, 4}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 25},
  {PITCH_PAUSE, 8}, {41, 12}, {PITCH_PAUSE, 4}, {29, 8}, {PITCH_PAUSE, 4},
  {29, 4}, {41, 8}, {PITCH_PAUSE, 8}, {40, 8}, {PITCH_PAUSE, 4}, {39, 4},
  {38, 4}, {37, 4}, {38, 8}, {PITCH_PAUSE, 8}, {30, 4}, {PITCH_PAUSE, 4},
  {35, 8}, {PITCH_PAUSE, 8}, {34, 8}, {PITCH_PAUSE, 4}, {33, 4}, {32, 4},
  {31, 4}, {32, 8}, {PITCH_PAUSE, 8}, {25, 4}, {PITCH_PAUSE, 4}, {28, 8},
  {PITCH_PAUSE, 8}, {25, 8}, {PITCH_PAUSE, 4}, {28, 4}, {32, 12},
  {PITCH_PAUSE, 4}, {29, 8}, {PITCH_PAUSE, 4}, {32, 4}, {36, 25},
  {PITCH_PAUSE, 8}, {41, 12}, {PITCH_PAUSE, 4}, {29, 8}, {PITCH_PAUSE, 4},
  {29, 4}, {41, 8}, {PITCH_PAUSE, 8}, {40, 8}, {PITCH_PAUSE, 4}, {39, 4},
  {38, 4}, {37, 4}, {38, 8}, {PITCH_PAUSE, 8}, {30, 4}, {PITCH_PAUSE, 4},
  {35, 8}, {PITCH_PAUSE, 8}, {34, 8}, {PITCH_PAUSE, 4}, {33, 4}, {32, 4},
  {31, 4}, {32, 8}, {PITCH_PAUSE, 8}, {25, 4}, {PITCH_PAUSE, 4}, {28, 8},
  {PITCH_PAUSE, 8}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 12},
  {PITCH_PAUSE, 4}, {25, 8}, {PITCH_PAUSE, 4}, {32, 4}, {29, 25}, {PITCH_END,
                                                                   0}
};

static const xNote pxIndianaJonesMelody[] PROGMEM = {
  {31, 7}, {PITCH_PAUSE, 3}, {32, 3}, {34, 3}, {PITCH_PAUSE, 3}, {39, 22},
  {PITCH_PAUSE, 3}, {29, 7}, {PITCH_PAUSE, 3}, {31, 3}, {32, 30},
  {PITCH_PAUSE, 7}, {34, 7}, {PITCH_PAUSE, 3}, {36, 3}, {38, 3},
  {PITCH_PAUSE, 3}, {44, 22}, {PITCH_PAUSE, 7}, {36, 7}, {PITCH_PAUSE, 3},
  {38, 3}, {39, 15}, {41, 15}, {43, 15}, {31, 7}, {PITCH_PAUSE, 3}, {32, 3},
  {34, 3}, {PITCH_PAUSE, 3}, {39, 30}, {PITCH_PAUSE, 7}, {41, 7},
  {PITCH_PAUSE, 3}, {43, 3}, {44, 30}, {PITCH_PAUSE, 15}, {34, 7},
  {PITCH_PAUSE, 3}, {34, 3}, {43, 11}, {PITCH_PAUSE, 3}, {41, 7},
  {PITCH_PAUSE, 3}, {34, 3}, {43, 11}, {PITCH_PAUSE, 3}, {41, 7},
  {PITCH_PAUSE, 3}, {34, 3}, {44, 11}, {PITCH_PAUSE, 3}, {43, 7},
  {PITCH_PAUSE, 3}, {41, 3}, {39, 30}, {PITCH_END, 0}
};

#ifdef  TEST_TIMER
# include <avrio-board-melody.h>
#endif

/* private functions ======================================================== */

// ------------------------------------------------------------------------------
static void
prvvLedBlink (uint8_t ucCount) {

  while (ucCount--) {

    vLedSet (LED_LED1);
    delay_ms (100);
    vLedClear (LED_LED1);
    delay_ms (100);
  }
}

#ifdef TEST_PWM
// ------------------------------------------------------------------------------
static void
prvvPwmInit (void) {

  DDRB |= _BV(5) | _BV(6);
  TCCR1A |= _BV (COM1A1) | _BV (COM1B1);
}

// ------------------------------------------------------------------------------
void 
vMelodyTopChangedCB (uint16_t usNewTop) {

  OCR1A = usNewTop / 4;
  OCR1B = usNewTop / 8;
}
#else
# define prvvPwmInit()
# ifdef AVRIO_MELODY_TIMERCB_ENABLE
    // -------------------------------------------------------------------------
    void 
    vMelodyTopChangedCB (uint16_t usNewTop) {
    }
# endif 
#endif

// ------------------------------------------------------------------------------
static uint8_t prvucMelodyTest (void) {
  static uint8_t ucMelodyIndex = 0;

  if (++ucMelodyIndex > 6)
    ucMelodyIndex = 1;

  switch (ucMelodyIndex) {

    case 1:
      vMelodyBeep ();
      break;
    case 2:
      vMelodyPlaySystem (MELODY_WELCOME);
      break;
    case 3:
      vMelodyPlaySystem (MELODY_WARNING);
      break;
    case 4:
      vMelodyPlaySystem (MELODY_ERROR);
      break;
    case 5:
      vMelodyPlayFlash (pxStarwarsMelody);
      break;
    case 6:
      vMelodyPlayFlash (pxIndianaJonesMelody);
      break;
    default:
      break;
  }
  return ucMelodyIndex;
}

/* main ===================================================================== */
int
main (void) {

  vLedInit ();
  vButInit ();
  vMelodyInit (30, 0);
  prvvPwmInit ();

#ifdef  TEST_TIMER
  vMelodyHardwarePlay (4000);
  while (xButGet (BUTTON_BUTTON1) == BUTTON_NO_BUTTON);
#endif

  vMelodyPlaySystem (MELODY_WELCOME);
  prvvLedBlink (10);

  for (;;) {
#ifdef AVRIO_BUTTON_ENABLE
    if (xButGet (BUTTON_BUTTON1)) {

      prvvLedBlink (prvucMelodyTest());
    }
#else
    prvvLedBlink (prvucMelodyTest());
    while (xMelodyIsPlay())
      ;
    delay_ms (1000);
#endif
  }
  return 0;
}
/* ========================================================================== */
