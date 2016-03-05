#include <avrio/tc.h>
#include <avrio/log.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <avr/pgmspace.h>

/* constants ================================================================ */
#define PORT         "tty0"
#define BAUDRATE     500000

static const char flashstr[] PROGMEM = "FLASH String %d";
static const char ramstr[]  = "SRAM String %d";

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
static xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);

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

    vLog_P (LOG_INFO, flashstr, ++test_count);
    vLog (LOG_INFO, ramstr, ++test_count);

    for (i = LOG_DEBUG; i >= LOG_EMERG; i--) {

      fprintf_P (stderr, PSTR ("\nPriority up to %s\n"), sLogPriorityString(i));
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
