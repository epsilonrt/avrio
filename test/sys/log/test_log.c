#include <avrio/tc.h>
#include <avrio/log.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
#define PORT         "tty0"
#define BAUDRATE     500000
#define DATABIT      SERIAL_DATABIT_8 // 5 à 9 bits
#define PARITY       SERIAL_PARITY_NONE // NONE, EVEN, ODD
#define STOPBIT      SERIAL_STOPBIT_ONE // 1 ou 2
#define FLOWCTL      SERIAL_FLOW_NONE

/* macros =================================================================== */
#define test(t) do { \
    if (!t) { \
      PERROR ("Test %d failed !", test_count); \
      return (-1); \
    } \
    else { \
      PINFO ("Test %d success !", test_count); \
    } \
  } while (0)

/* private variables ======================================================== */
static int test_count;
static xSerialIos settings = {
  .baud = BAUDRATE, .dbits = DATABIT, .parity = PARITY,
  .sbits = STOPBIT, .flow = FLOWCTL
};

/* main ===================================================================== */
int
main (void) {
  static volatile int i;

  vLedInit();
  FILE * tc = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  stdout = tc;
  stderr = tc;
  stdin = tc;
  sei();

  for (;;) {

    test_count = 1;
    printf_P (PSTR ("\nAvrIO Log test\n"
                    "Press any key to proceed...\n"));
    while (getchar() == EOF)
      ;

    vLogSetMask (LOG_UPTO (LOG_INFO));
    i = iLogMask();
    test (i == LOG_UPTO (LOG_INFO));
    
    for (i = LOG_DEBUG; i >= LOG_EMERG; i--) {

      fprintf_P (stderr, PSTR("Priority up to %d\n"), i);
      vLogSetMask (LOG_UPTO (i));
      vLedSet (LED_LED1);
      PERROR ("Error %d", test_count++);
      vLedClear (LED_LED1);
      delay_ms (5);
      vLedSet (LED_LED1);
      PWARNING ("Warning %d", test_count++);
      vLedClear (LED_LED1);
      delay_ms (5);
      vLedSet (LED_LED1);
      PNOTICE ("Notice %d", test_count++);
      vLedClear (LED_LED1);
      delay_ms (5);
      vLedSet (LED_LED1);
      PINFO ("Info %d", test_count++);
      vLedClear (LED_LED1);
      PDEBUG ("Debug %d", test_count++);
      PERROR ("Error %d", test_count++);
      PWARNING ("Warning %d", test_count++);
      PNOTICE ("Notice %d", test_count++);
      PINFO ("Info %d", test_count++);
      PDEBUG ("Debug %d", test_count++);
    }

  }
  return 0;
}

/* ========================================================================== */
