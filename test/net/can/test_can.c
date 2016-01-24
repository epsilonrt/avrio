#include <stdio.h>
#include <string.h>
#include <avrio/can.h>
#include <avrio/serial.h>

/* main ===================================================================== */
int
main (void) {
  int iError;
  xCanFrame xTxFrame, xRxFrame;

  // Init AT90CAN128 CAN controller with 125 kbit/s
  vSerialInit (96, SERIAL_DEFAULT + SERIAL_WR);
  stdout = &xSerialPort;
  do {

    iError = eCanInit (CAN_SPEED_125K);
    if (iError != CAN_SUCCESS) {

      printf ("eCanInit error ! %d\n", iError);
    }
  } while (iError != CAN_SUCCESS);

  // Prepare a frame for sending
  memset (&xRxFrame, 0, sizeof (xRxFrame));
  memset (&xTxFrame, 0, sizeof (xTxFrame));
  xTxFrame.xArb.xId = 0x123;
  xTxFrame.xCtrl.xDlc = 8;
  xTxFrame.ucData[0] = 0x11;
  xTxFrame.ucData[1] = 0x22;
  xTxFrame.ucData[2] = 0x33;
  xTxFrame.ucData[3] = 0x44;
  xTxFrame.ucData[4] = 0x55;
  xTxFrame.ucData[5] = 0x66;
  xTxFrame.ucData[6] = 0x77;
  xTxFrame.ucData[7] = 0x88;

  // Simple send/receive loop
  printf ("Starting CAN RX/TX loop...\n");
  for (;;) {

    iError = eCanSendFrame (&xTxFrame);
    printf ("eCanSendFrame= %d", iError);

    // Check if we did receive a frame
    iError = eCanReceiveFrame (&xRxFrame);
    if (iError == CAN_SUCCESS) {
      uint8_t ucIndex;

      printf ("%lu ", xRxFrame.xArb.xId);
      for (ucIndex = 0; ucIndex < xRxFrame.xCtrl.xDlc; ucIndex++) {

        printf ("%02X ", xRxFrame.ucData[ucIndex]);
      }
      putchar ('\n');
    }
  }
}

/* ========================================================================== */
