#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/serial.h>
#include <util/atomic.h>
#include <avrio/bdcm.h>
#include <avrio/encoder.h>
#include <avrio/task.h>

/* constants ================================================================ */
#define MOTOR_REDUCTION 50
#define ENCODER_CPR     100
#define DISPLAY_PERIOD  SECONDS(5)

/* private variables ======================================================== */
static volatile uint8_t xDisplayFlag;
static xTaskHandle xDisplayTimer;

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static uint8_t
prvucAtomicReadByte (volatile uint8_t * pucByte) {
  uint8_t ucValue;

  ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
    ucValue = *pucByte;
  }
  return ucValue;
}

// ------------------------------------------------------------------------------
// Calcul de la vitesse de rotation d'un moteur en RPM
static int16_t
prvsMotorRpm (uint8_t ucMotor) {
  int32_t lRpm;

  lRpm = ((int32_t) xEncoderSpeed (ucMotor) * 600L) /
     (MOTOR_REDUCTION * ENCODER_CPR);
  return (int16_t) lRpm;
}

// ------------------------------------------------------------------------------
// Affiche le message d'invite
static void
prvvDisplayInviteMsg (void) {

  printf_P (PSTR
            ("\n\nPress 0 or 1 to set the duty cycle of this brush DC motor\nPress F to set PWM frequency ? "));
}

// ------------------------------------------------------------------------------
// Cadence l'affichage des infos des encodeurs
static void
prvvDisplayTask (xTaskHandle xDummy __attribute__ ((unused))) {

  xDisplayFlag = 1;
  vTaskStart (xDisplayTimer);
}

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
  vEncoderInit ();
  xDisplayTimer =
     xTaskCreate (xTaskConvertMs (DISPLAY_PERIOD), prvvDisplayTask);
  vTaskStart (xDisplayTimer);
  printf_P (PSTR
            ("\n\n**** Quadrature Encoder Test ****\n\n\nPWM Frequency: %d\n"),
            usFreq);
  prvvDisplayInviteMsg ();

  for (;;) {

    if (usSerialHit ()) {

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
          prvvDisplayInviteMsg ();
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
          prvvDisplayInviteMsg ();
          vBdcmSetRatio (0, iDutyCycle[0]);
          vBdcmSetRatio (1, iDutyCycle[1]);
          break;

        default:
          break;
      }
    }

    if (prvucAtomicReadByte (&xDisplayFlag)) {

      if (iDutyCycle[0]) {

        printf_P (PSTR ("\nC0=%08ld - S0=%04d RPM"),
                  xEncoderCount (0), prvsMotorRpm (0));
      }
      if (iDutyCycle[1]) {

        printf_P (PSTR ("\nC1=%08ld - S1=%04d RPM"),
                  xEncoderCount (1), prvsMotorRpm (1));
      }
      ATOMIC_BLOCK (ATOMIC_RESTORESTATE) {
        xDisplayFlag = 0;
      }
    }
  }
}

/* ========================================================================== */
