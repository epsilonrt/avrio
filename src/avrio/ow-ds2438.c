/**
 * Copyright © 2017 epsilonRT. All rights reserved.
 *
 * This file is part of AvrIO.
 *
 * This software is governed by the CeCILL license under French law and
 * abiding by the rules of distribution of free software.  You can  use,
 * modify and/ or redistribute the software under the terms of the CeCILL
 * license as circulated by CEA, CNRS and INRIA at the following URL
 * <http://www.cecill.info>.
 *
 * The fact that you are presently reading this means that you have had
 * knowledge of the CeCILL license and that you accept its terms.
 *
 * @file
 * @brief
 */
#include "ow-ds2438.h"

/* constants ================================================================ */
#define PAGE_0   0x00
#define PAGE_1   0x01
#define PAGE_2   0x02
#define PAGE_3   0x03
#define PAGE_4   0x04
#define PAGE_5   0x05
#define PAGE_6   0x06
#define PAGE_7   0x07

// Page 0
#define STATUS   0
#define TEMP_LSB 1
#define TEMP_MSB 2
#define VOLT_LSB 3
#define VOLT_MSB 4
#define CURR_LSB 5
#define CURR_MSB 6
#define THRESH   7
#define CRC8     8

// Page 1
#define ICA      4

// Page 7
#define CCA_LSB 4
#define CCA_MSB 5
#define DCA_LSB 6
#define DCA_MSB 7

// Commands
#define RECALL_E_E        0xB8
#define WRITE_SCRATCHPAD  0x4E
#define READ_SCRATCHPAD   0xBE
#define COPY_SCRATCHPAD   0x48
#define CONV_VOLT         0xB4
#define CONV_TEMP         0x44

/* private functions ======================================================== */
// -----------------------------------------------------------------------------
static int
iDoCmdPage (const xOwDevice *device, uint8_t cmd, uint8_t page_n) {

  if (bOwReset() == 0) {
    return -1;
  }
  vOwMatch (device);
  vOwWrite (cmd);
  vOwWrite (page_n);
  return 0;
}

// -----------------------------------------------------------------------------
int
iStartConvert (xOwDevice *device, uint8_t conv) {

  if (bOwReset() == 0) {

    return -1;
  }

  vOwMatch (device);
  vOwWrite (conv);

  return 0;
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
int
iDs2438ReadPage (xOwDevice * device, uint8_t page_n, uint8_t * page_buf) {

  if (iDoCmdPage (device, READ_SCRATCHPAD, page_n) != 0) {

    return -1;
  }

  for (uint8_t i = 0; i < 9; i++) {

    page_buf[i] = ucOwRead();
  }

  return iOwCheckCRC8 (page_buf, 8);
}

// -----------------------------------------------------------------------------
int
iDs2438WritePage (xOwDevice * device, uint8_t page_n, const uint8_t * buffer, uint8_t len) {

  if (iDoCmdPage (device, WRITE_SCRATCHPAD, page_n) != 0) {

    return -1;
  }

  for (uint8_t i = 0; i < len; i++) {

    vOwWrite (buffer[i]);
  }

  return 0;
}

// -----------------------------------------------------------------------------
int
iDs2438CopyPage (xOwDevice * device, uint8_t page_n) {

  return iDoCmdPage (device, COPY_SCRATCHPAD, page_n);
}

// -----------------------------------------------------------------------------
int
iDs2438RecallPage (xOwDevice * device, uint8_t page_n) {

  return iDoCmdPage (device, RECALL_E_E, page_n);
}

// -----------------------------------------------------------------------------
int
iDs2438StartConvert (xOwDevice *d, eDs2438Convertion conv) {
  int ret = 0;

  if (conv & eDs2438ConvVolt) {

    ret = iStartConvert (d, CONV_VOLT);
  }
  if ( (ret == 0) && (conv & eDs2438ConvTemp)) {

    ret = iStartConvert (d, CONV_TEMP);
  }
  return ret;
}

// -----------------------------------------------------------------------------
int
iDs2438Configure (xOwDevice * device, uint8_t config) {

  if (iDs2438WritePage (device, PAGE_0, &config, 1) == 0) {
    uint8_t page[9];

    if (iDs2438ReadPage (device, PAGE_0, page) == 0) {

      return iDs2438CopyPage (device, PAGE_0);
    }
  }

  return -1;
}

// -----------------------------------------------------------------------------
int
iDs2438DataGetRaw (xOwDevice * device, xDs2438DataRaw * raw) {

  if (iDs2438RecallPage (device, PAGE_0) == 0) {
    uint8_t page[9];

    if (iDs2438ReadPage (device, PAGE_0, page) == 0) {
      raw->temp = (page[TEMP_MSB] << 8) + page[TEMP_LSB];
      raw->voltage = (page[VOLT_MSB] << 8) + page[VOLT_LSB];
      raw->current = (page[CURR_MSB] << 8) + page[CURR_LSB];
      return 0;
    }
  }

  return -1;
}

// -----------------------------------------------------------------------------
int
iDs2438CapacityGetRaw (xOwDevice * device, xDs2438CapacityRaw * raw) {

  if (iDs2438RecallPage (device, PAGE_1) == 0) {
    uint8_t page[9];

    if (iDs2438ReadPage (device, PAGE_1, page) == 0) {
      raw->ica = page[ICA];

      if (iDs2438RecallPage (device, PAGE_7) == 0) {

        if (iDs2438ReadPage (device, PAGE_7, page) == 0) {

          raw->cca = (page[CCA_MSB] << 8) + page[CCA_LSB];
          raw->dca = (page[DCA_MSB] << 8) + page[DCA_LSB];
          return 0;
        }
      }
    }
  }

  return -1;
}

// -----------------------------------------------------------------------------
void
vDs2438CapacityRawToMes (xDs2438CapacityMes * mes, double rsens, const xDs2438CapacityRaw * raw) {

  mes->remaining = ( (double) raw->ica) / (2048.0 * rsens);
  mes->charging = ( (double) raw->cca) / (64.0 * rsens);
  mes->discharging = ( (double) raw->dca) / (64.0 * rsens);
}

// -----------------------------------------------------------------------------
void
vDs2438DataRawToMes (xDs2438DataMes * mes, double rsens, const xDs2438DataRaw * raw) {

  mes->temp = ( (double) raw->temp) * 0.00390625;
  mes->voltage = ( (double) raw->voltage) * 0.01;
  mes->current = ( (double) raw->current) / (4096.0 * rsens);
}

/* ========================================================================== */
