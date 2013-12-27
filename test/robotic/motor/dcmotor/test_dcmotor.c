/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
/* ========================================================================== */
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/serial.h>
#include <util/atomic.h>
#include <avrio/bdcm.h>

/* main ===================================================================== */
int
main (void) {
  char cChoice;
  int16_t iDutyCycle[2] = { 0, 0 };
  uint16_t usFreq = 1000;

  vSerialInit (96, SERIAL_DEFAULT + SERIAL_RW);
  stdout = &xSerialPort;
  stdin = &xSerialPort;
  vBdcmInit (usFreq);
  printf_P (PSTR
            ("\n\n**** Brush DC Motor Test ****\n\n\nPWM Frequency: %d\n"),
            usFreq);

  for (;;) {

    printf_P (PSTR
              ("\n\nPress 0 or 1 to set the duty cycle of this brush DC motor\nPress F to set PWM frequency ? "));
    cChoice = getchar ();
    switch (cChoice) {

      case '0':
      case '1':{
        uint8_t ucMotor = cChoice - '0';
        int16_t iDuty;

        do {
          printf_P (PSTR ("\nDuty cycle (M%d=%d) [-10000, 10000] ? "),
                    ucMotor, iDutyCycle[ucMotor]);
          scanf ("%i", &iDuty);
        } while ((iDuty < -10000) || (iDuty > 10000));
        iDutyCycle[ucMotor] = iDuty;
        vBdcmSetRatio (ucMotor, iDuty);
      }
        break;

      case 'F':
      case 'f':
        printf_P (PSTR ("\nPWM Frequency (F=%u) ? "), usFreq);
        scanf ("%u", &usFreq);
        vBdcmSetRatio (0, 0);
        vBdcmSetRatio (1, 0);
        vBdcmInit (usFreq);
        printf_P (PSTR ("\nPress any key to restart the motors ?"));
        getchar ();
        vBdcmSetRatio (0, iDutyCycle[0]);
        vBdcmSetRatio (1, iDutyCycle[1]);
        break;

      default:
        break;
    }
  }
}

/* ========================================================================== */
