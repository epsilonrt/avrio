/**
 * Bertos is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * As a special exception, you may use this file as part of a free software
 * library without restriction.  Specifically, if other files instantiate
 * templates or use macros or inline functions from this file, or you compile
 * this file and link it with other files to produce an executable, this
 * file does not by itself cause the resulting executable to be covered by
 * the GNU General Public License.  This exception does not however
 * invalidate any other reasons why the executable file might be covered by
 * the GNU General Public License.
 *
 * Copyright 2009 Develer S.r.l. (http://www.develer.com/)
 *
 * @file ax25.c
 * @brief Couche liaison du protocole AX.25 (Implémentation).
 *
 * Revision History ------------------------------------------------------------
 *    20120519 - Initial version from BeRTOS
 */
#include "avrio-config.h"

/* ========================================================================== */
#include <avrio/ax25.h>
#include <avrio/crc.h>
#include <string.h> //memset, memcmp
#include <ctype.h>  //isalnum, toupper
#include <stdlib.h>

/* constants ================================================================ */
/*
 * CRC computation on correct AX25 packets should
 * give this result (don't ask why).
 */
#define AX25_CRC_CORRECT  0xF0B8

/* macros =================================================================== */
#if AX25_MAX_RPT > 8
#error "AX25_MAX_RPT must be less than equal to 8."
#endif

// -----------------------------------------------------------------------------
#if CONFIG_AX25_RPT_LST
  #define AX25_SET_REPEATED(frm, idx, val) \
    do \
    { \
      if (val) \
        (frm)->rpt_flags |= _BV(idx) ; \
      else \
        (frm)->rpt_flags &= ~_BV(idx) ; \
    } while(0)
#endif

// -----------------------------------------------------------------------------
#define DECODE_CALL(buf, addr) \
  for (unsigned i = 0; i < sizeof((addr)); i++) \
  { \
    char c = (*(buf)++ >> 1); \
    (addr)[i] = (c == ' ') ? '\x0' : c; \
  }

// -----------------------------------------------------------------------------
#if 0
#include <avrio/led.h>
#include <avrio/delay.h>
#define ASSERT(test)  do { \
  if (test == false) { \
    for (;;) { \
      vLedSet (LED_LED1); \
      delay_ms (50); \
      vLedClear (LED_LED1); \
      delay_ms (150); \
    } \
  } \
} while (0)
#endif

// -----------------------------------------------------------------------------
#ifndef ASSERT
#define ASSERT(b)
#endif

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
static void
prvvDecode(xAx25 *p) {
  xAx25Frame frm;
  uint8_t *buf = p->buf;

  DECODE_CALL(buf, frm.dst.call);
  frm.dst.ssid = (*buf++ >> 1) & 0x0F;

  DECODE_CALL(buf, frm.src.call);
  frm.src.ssid = (*buf >> 1) & 0x0F;

  /* Repeater addresses */
  #if CONFIG_AX25_RPT_LST
  for ( frm.rpt_cnt = 0;
        !(*buf++ & 0x01) && (frm.rpt_cnt < countof(frm.rpt_lst));
        frm.rpt_cnt++) {
    DECODE_CALL(buf, frm.rpt_lst[frm.rpt_cnt].call);
    frm.rpt_lst[frm.rpt_cnt].ssid = (*buf >> 1) & 0x0F;
    AX25_SET_REPEATED(&frm, frm.rpt_cnt, (*buf & 0x80));
  }
  #else
  while (!(*buf++ & 0x01)) {
    char rpt[6];
    uint8_t ssid;

    DECODE_CALL(buf, rpt);
    ssid = (*buf >> 1) & 0x0F;
  }
  #endif

  frm.ctrl = *buf++;
  if (frm.ctrl != AX25_CTRL_UI) {

    return;
  }

  frm.pid = *buf++;
  if (frm.pid != AX25_PID_NOLAYER3) {

    return;
  }

  frm.len = p->frm_len - 2 - (buf - p->buf);
  frm.info = buf;

  if (p->hook)
    p->hook(&frm);
}

// -----------------------------------------------------------------------------
static void
prvvPutChar(xAx25 *p, uint8_t c) {

  if (c == HDLC_FLAG || c == HDLC_RESET || c == HDLC_ESC) {

    // Ajout d'un caractère d'échappement si le code correspond à un flag HDLC
    fputc (HDLC_ESC, p->fout);
  }
  p->crc_out = usCrcCcittUpdate(c, p->crc_out);
  fputc(c, p->fout);
}

// -----------------------------------------------------------------------------
static void
prvvSendCall (xAx25 *p, const xAx25Node *addr, bool last) {
  unsigned len = MIN(sizeof(addr->call), strlen(addr->call));

  for (unsigned i = 0; i < len; i++) {

    uint8_t c = addr->call[i];
    ASSERT(isalnum(c) || c == ' ');
    c = toupper(c);
    prvvPutChar(p, c << 1);
  }

  /* Fill with spaces the rest of the CALL if it's shorter */
  if (len < sizeof(addr->call)) {

    for (unsigned i = 0; i < sizeof(addr->call) - len; i++) {

      prvvPutChar(p, ' ' << 1);
    }
  }
  /* The bit7 "has-been-repeated" flag is not implemented here */
  /* Bits6:5 should be set to 1 for all SSIDs (0x60) */
  /* The bit0 of last call SSID should be set to 1 */
  uint8_t ssid = 0x60 | (addr->ssid << 1) | (last ? 0x01 : 0);
  prvvPutChar(p, ssid);
}

// -----------------------------------------------------------------------------
static void
prvvPrintCall (FILE *f, const xAx25Node *call) {

  fprintf (f, "%.6s", call->call);
  if (call->ssid)
    fprintf(f, "-%d", call->ssid);
}

// -----------------------------------------------------------------------------
static void
prvvPrintInfo (FILE *f, const xAx25Frame *frm) {
  const uint8_t *p = frm->info;
  size_t len = frm->len;

  while (len--) {

    fputc (*p++, f);
  }
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vAx25Poll(xAx25 *p) {
  int c;

  while ((c = fgetc(p->fin)) != EOF) {

    if (!p->escape && c == HDLC_FLAG) {

      if (p->frm_len >= AX25_MIN_FRAME_LEN) {

        if (p->crc_in == AX25_CRC_CORRECT) {

          vAx25HwFrmLedOn();
          prvvDecode(p);
          vAx25HwFrmLedOff();
        }
      }
      p->sync = true;
      p->crc_in = CRC_CCITT_INIT_VAL;
      p->frm_len = 0;
      continue;
    }

    if (!p->escape && c == HDLC_RESET) {

      p->sync = false;
      continue;
    }

    if (!p->escape && c == HDLC_ESC) {

      p->escape = true;
      continue;
    }

    if (p->sync) {

      if (p->frm_len < CONFIG_AX25_FRAME_BUF_LEN) {

        p->buf[p->frm_len++] = c;
        p->crc_in = usCrcCcittUpdate(c, p->crc_in);
      }
      else {

        p->sync = false;
      }
    }
    p->escape = false;
  }

  if (ferror(p->fin)) {

    clearerr(p->fin);
  }
}

// -----------------------------------------------------------------------------
void
vAx25SendVia(xAx25 *p, const xAx25Node *path, size_t path_len,
              const void *_buf, size_t len) {
  const uint8_t *buf = (const uint8_t *)_buf;

  ASSERT(path);
  ASSERT(path_len >= 2);

  p->crc_out = CRC_CCITT_INIT_VAL;
  fputc(HDLC_FLAG, p->fout);


  /* Send call */
  for (size_t i = 0; i < path_len; i++)
    prvvSendCall(p, &path[i], (i == path_len - 1));

  prvvPutChar(p, AX25_CTRL_UI);
  prvvPutChar(p, AX25_PID_NOLAYER3);

  while (len--)
    prvvPutChar(p, *buf++);

  /*
   * According to AX25 protocol,
   * CRC is sent in reverse order!
   */
  uint8_t crcl = (p->crc_out & 0xff) ^ 0xff;
  uint8_t crch = (p->crc_out >> 8) ^ 0xff;
  prvvPutChar(p, crcl);
  prvvPutChar(p, crch);

  ASSERT(p->crc_out == AX25_CRC_CORRECT);

  fputc (HDLC_FLAG, p->fout);
}

// -----------------------------------------------------------------------------
void
vAx25Print (FILE *f, const xAx25Frame *frm) {

  prvvPrintCall(f, &frm->src);
  fputc('>', f);
  prvvPrintCall(f, &frm->dst);

  #if CONFIG_AX25_RPT_LST
  for (int i = 0; i < frm->rpt_cnt; i++) {

    fputc(',', f);
    prvvPrintCall(f, &frm->rpt_lst[i]);
    /* Print a '*' if packet has already been transmitted
     * by this repeater */
    if (AX25_REPEATED(frm, i))
      fputc('*', f);
  }
  #endif
  fputc(':', f);
  prvvPrintInfo (f, frm);
  fputc('\n', f);
}

// -----------------------------------------------------------------------------
void
vAx25Init(xAx25 *p, FILE *fin, FILE *fout, vAx25CallBack hook) {

  ASSERT (p);
  ASSERT (fin);
  ASSERT (fout);

  memset(p, 0, sizeof(*p));
  p->fin = fin;
  p->fout = fout;
  p->hook = hook;
  p->crc_in = p->crc_out = CRC_CCITT_INIT_VAL;
  vAx25HwInit();
}

/* ========================================================================== */
