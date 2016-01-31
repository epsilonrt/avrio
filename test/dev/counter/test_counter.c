#include <avrio/led.h>
#include <avrio/delay.h>
#include <avrio/serial.h>
#include <avrio/counter.h>

/* constants ================================================================ */
#define TEST_BAUDRATE     38400
#define TEST_SETUP        (SERIAL_DEFAULT + SERIAL_RW)
/*
 * Si le test mesure la sortie d'un TSL230 mettre USE_TSL230 à 1 et définir
 * les constantes TSL230_ correspondant aux broches
 */
#define USE_TSL230  1
#define TSL230_DDR  DDRB
#define TSL230_PORT PORTB
#define TSL230_S0 0
#define TSL230_S1 1
#define TSL230_S2 2
#define TSL230_S3 3
#define TSL230_OE 4

/* private variables ======================================================== */
static double dFreq;

/* private functions ======================================================== */
void vTimerInit(void);
void vTimerClear(void);
void vTimerEnable(bool En);
uint16_t usTimerRead(void);

/* main ===================================================================== */
int
main (void) {
  xCounter xMyMeter;
  xCounter * fm = &xMyMeter;
  xCounterOps ops = {
    .init = vTimerInit,
    .clear = vTimerClear,
    .enable = vTimerEnable,
    .read = usTimerRead
  };

  vLedInit ();
  ATOMIC_BLOCK (ATOMIC_FORCEON) {
    vSerialInit (TEST_BAUDRATE / 100, TEST_SETUP);
    stdout = &xSerialPort;
    vCounterInit (fm, &ops);
    vCounterSetWindow (fm, 100);
  }
  printf ("Frequency Meter Test\nWindow=%u ms\nCount,Freq\n", fm->usWindow);

  for (;;) {

    if (bCounterIsComplete (fm)) {
      dFreq = dCounterFreq (fm);
      printf ("%u,%.1f\n", usCounterCount(fm), dFreq);
      delay_ms (100);
      vCounterStart (fm);
    }
  }
  return 0;
}

// -----------------------------------------------------------------------------
void 
vTimerInit(void) {

#if USE_TSL230
  // Activation TSL230
  TSL230_DDR |= _BV(TSL230_OE)|_BV(TSL230_S3)|_BV(TSL230_S2)|_BV(TSL230_S1)|_BV(TSL230_S0);
  TSL230_PORT &= ~(_BV(TSL230_OE)|_BV(TSL230_S3)|_BV(TSL230_S2)|_BV(TSL230_S1));
  TSL230_PORT |= _BV(TSL230_S0);
#endif
  
  TCCR1A = 0;
  TCCR1B = 0;
}

// -----------------------------------------------------------------------------
void 
vTimerClear(void) {
  
  TCNT1 = 0;
}

// -----------------------------------------------------------------------------
void 
vTimerEnable(bool En) {
  if (En) {
    
    TCCR1B = 0x07; // Horloge T1 sur front montant
  }
  else {
    
    TCCR1B = 0; // Stop
  }
}

// -----------------------------------------------------------------------------
uint16_t 
usTimerRead(void) {
  
  return TCNT1;
}

/* ========================================================================== */
