/**
 * @file
 * @brief Test d'accès SPI à un module RFM69
 * Effectue des lectures et écritures dans les registres du module RFM69 et
 * affiche les résultats sur la liaison série.
 * -----------------------------<<WARNING>>------------------------------------- 
 * Un module RFM69 est alimenté en 3.3V et n'est donc pas compatible
 * avec ARDUINO sans adaptation de niveau... on utilisera plutôt une carte xplbee !
 * -----------------------------------------------------------------------------
    RFM69 SPI test

    Test 1 - Read registers (xfer)>
    #,register,value
    0x00,RegFifo,0x00
    0x01,RegOpMode,0x04
    ....
    Success

    Test 2 - Read registers (read)>
    #,register,value
    0x00,RegFifo,0x00
    0x01,RegOpMode,0x04
    0x02,RegDataModul,0x00
    0x03,RegBitrateMsb,0x1A
    ....
    Success

    Test 3 - Read block>
    0x3E,RegAesKey1,16,0x000102030405060708090A0B0C0D0E0F
    Success

    Test 4 - Write register>
    0x3E,RegAesKey1,0x5A
    Success

    Test 5 - Write block>
    0x3E,RegAesKey1,16,0x000102030405060708090A0B0C0D0E0F
    Press any key to repeat...
 * 
 */
#include <avrio/delay.h>
#include <avrio/tc.h>
#include <avrio/spi.h>
#include <avrio/assert.h>
#include <avr/pgmspace.h>
#include <string.h>

/* constants ================================================================ */
#define BAUDRATE 115200
#define PORT     "tty0"

/* private variables ======================================================== */
static const char *   regs[] = {
  "RegFifo", "RegOpMode", "RegDataModul", "RegBitrateMsb", "RegBitrateLsb",
  "RegFdevMsb", "RegFdevLsb", "RegFrfMsb", "RegFrfMid", "RegFrfLsb",
  "RegOsc1", "RegAfcCtrl", "Reserved0C", "RegListen1", "RegListen2",
  "RegListen3", "RegVersion", "RegPaLevel", "RegPaRamp", "RegOcp",
  "Reserved14", "Reserved15", "Reserved16", "Reserved17", "RegLna",
  "RegRxBw", "RegAfcBw", "RegOokPeak", "RegOokAvg", "RegOokFix",
  "RegAfcFei", "RegAfcMsb", "RegAfcLsb", "RegFeiMsb", "RegFeiLsb",
  "RegRssiConfig", "RegRssiValue", "RegDioMapping1", "RegDioMapping2", "RegIrqFlags1",
  "RegIrqFlags2", "RegRssiThresh", "RegRxTimeout1", "RegRxTimeout2", "RegPreambleMsb",
  "RegPreambleLsb", "RegSyncConfig", "RegSyncValue1", "RegSyncValue2", "RegSyncValue3",
  "RegSyncValue4", "RegSyncValue5", "RegSyncValue6", "RegSyncValue7", "RegSyncValue8",
  "RegPacketConfig1", "RegPayloadLength", "RegNodeAdrs", "RegBroadcastAdrs", "RegAutoModes",
  "RegFifoThresh", "RegPacketConfig2", "RegAesKey1", "RegAesKey2", "RegAesKey3",
  "RegAesKey4", "RegAesKey5", "RegAesKey6", "RegAesKey7", "RegAesKey8",
  "RegAesKey9", "RegAesKey10", "RegAesKey11", "RegAesKey12", "RegAesKey13",
  "RegAesKey14", "RegAesKey15", "RegAesKey16", "RegTemp1", "RegTemp2",
  "RegTestLna", "RegTestPa1", "RegTestPa2", "RegTestDagc", "RegTestAfc",
  NULL // must be last
};

/* main ===================================================================== */
int
main (void) {
  int t = 1, ret;
  int addr = 0;
  static uint8_t rx_buffer[16];
  static uint8_t tx_buffer[16];

  // Configuration du port série par défaut (8N1, sans RTS/CTS)
  xSerialIos settings = SERIAL_SETTINGS (BAUDRATE);
  // Ouverture du port série en entrée et en sortie
  FILE * serial_port = xFileOpen (PORT, O_RDWR | O_NONBLOCK, &settings);
  stdout = serial_port; // le port série est la sortie standard
  stderr = serial_port; // le port série est la sortie d'erreur
  stdin = serial_port; // le port série est l'entrée standard
  sei();

  vSpiSetSsAsOutput(); 
  vSpiMasterInit (SPI_DIV4);
  vSpiSetMode (eSpiMode0);
  vSpiSetBitOrder (eSpiBitOrderMsb);

  printf_P (PSTR ("RFM69 SPI test\n\n"));

  for (;;) {

    printf_P (PSTR ("Test %d - Read registers (xfer)>\n#,register,value\n"), t++);
    addr = 0;
    while (regs[addr] != NULL) {

      tx_buffer[0] = addr;
      vSpiMasterXfer (tx_buffer, 2, rx_buffer, 2);
      printf_P (PSTR ("0x%02X,%s,0x%02X\n"), addr, regs[addr], rx_buffer[1]);
      addr++;
    }
    printf_P (PSTR ("Success\n\n"));

    printf_P (PSTR ("Test %d - Read registers (read)>\n#,register,value\n"), t++);
    addr = 0;
    while (regs[addr] != NULL) {

      ret = ucSpiMasterReadReg8 (addr);
      printf_P (PSTR ("0x%02X,%s,0x%02X\n"), addr, regs[addr], ret);
      addr++;
    }
    printf_P (PSTR ("Success\n\n"));

    printf_P (PSTR ("Test %d - Read block>\n"), t++);
    addr = 0;
    while ( (strcmp (regs[addr], "RegAesKey1") != 0) && regs[addr] != NULL) {

      addr++;
    }
    if (regs[addr]) {
      uint8_t buffer[16];

      vSpiMasterReadRegBlock (addr, buffer, 16);

      printf_P (PSTR ("0x%02X,%s,%d,0x"), addr, regs[addr], 16);
      for (int i = 0; i < 16; i++) {
        printf_P (PSTR ("%02X"), buffer[i]);
      }
      putchar ('\n');
    }
    printf_P (PSTR ("Success\n\n"));

    printf_P (PSTR ("Test %d - Write register>\n"), t++);
    if (regs[addr]) {

      vSpiMasterWriteReg8 (addr, 0x5A);
      ret = ucSpiMasterReadReg8 (addr);
      printf_P (PSTR ("0x%02X,%s,0x%02X\n"), addr, regs[addr], ret);
      assert (ret == 0x5A);
    }
    printf_P (PSTR ("Success\n\n"));


    printf_P (PSTR ("Test %d - Write block>\n"), t++);
    if (regs[addr]) {

      for (int i = 0; i < 16; i++) {
        tx_buffer[i] = i;
      }
      vSpiMasterWriteRegBlock (addr, tx_buffer, 16);
      vSpiMasterReadRegBlock (addr, rx_buffer, 16);
      printf_P (PSTR ("0x%02X,%s,%d,0x"), addr, regs[addr], 16);
      for (int i = 0; i < 16; i++) {
        printf_P (PSTR ("%02X"), rx_buffer[i]);
      }
      putchar ('\n');
      assert (memcmp (tx_buffer, rx_buffer, 16) == 0);
    }

    printf_P (PSTR ("Press any key to repeat..."));
    while (getchar() == EOF)
      ;
  }

  return 0;
}

/* ========================================================================== */
