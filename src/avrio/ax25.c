/**
 * @file ax25.c
 * @brief Couche liaison du protocole AX.25 (Implémentation).
 * @author Francesco Sacchi <batt@develer.com>
 *          @copyright 2009 GNU General Public License version 2
 *          See the notice below.
 * @author Pascal JEAN <pjean@btssn.net>
 *          @copyright 2014 GNU Lesser General Public License version 3
 *          <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20120519 - Initial version from BeRTOS
 * -----------------------------------------------------------------------------
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
 */
#include "avrio-config.h"

/* ========================================================================== */
#include <avrio/ax25.h>
#include <avrio/crc.h>
#include <avrio/led.h>
#include <avrio/delay.h>
#include <string.h> //memset, memcmp
#include <ctype.h>  //isalnum, toupper
#include <stdlib.h>

/* constants ================================================================ */
/* structures =============================================================== */
/* types ==================================================================== */

/* private variables ======================================================== */
/* public variables ========================================================= */

#if 0
#define LOG_LEVEL  AX25_LOG_LEVEL
#define LOG_FORMAT AX25_LOG_FORMAT
#include <cfg/log.h>
#else
#define LOG_INFO(str,...)
#define LOG_ERR(str,...)
#define LOG_WARN(str,...)
#endif

/* macros =================================================================== */
#if AX25_MAX_RPT > 8
#error "AX25_MAX_RPT must be less than equal to 8."
#endif

#if CONFIG_AX25_RPT_LST
  #define AX25_SET_REPEATED(msg, idx, val) \
    do \
    { \
      if (val) \
        (msg)->rpt_flags |= _BV(idx) ; \
      else \
        (msg)->rpt_flags &= ~_BV(idx) ; \
    } while(0)
#endif

#define DECODE_CALL(buf, addr) \
  for (unsigned i = 0; i < sizeof((addr)); i++) \
  { \
    char c = (*(buf)++ >> 1); \
    (addr)[i] = (c == ' ') ? '\x0' : c; \
  }

/* private functions ======================================================== */
// ------------------------------------------------------------------------------
static void
vAssert (bool test) {

  if (test == false) {

    for (;;) {

      vLedSet (LED_LED1);
      delay_ms (50);
      vLedClear (LED_LED1);
      delay_ms (150);
    }
  }
}

// -----------------------------------------------------------------------------
static void
prvvDecode(xAx25Context *ctx) {
  xAx25Msg msg;
  uint8_t *buf = ctx->buf;

  DECODE_CALL(buf, msg.dst.call);
  msg.dst.ssid = (*buf++ >> 1) & 0x0F;

  DECODE_CALL(buf, msg.src.call);
  msg.src.ssid = (*buf >> 1) & 0x0F;

  LOG_INFO("SRC[%.6s-%d], DST[%.6s-%d]\n", msg.src.call, msg.src.ssid, msg.dst.call, msg.dst.ssid);

  /* Repeater addresses */
  #if CONFIG_AX25_RPT_LST
    for ( msg.rpt_cnt = 0;
          !(*buf++ & 0x01) && (msg.rpt_cnt < countof(msg.rpt_lst));
          msg.rpt_cnt++) {
      DECODE_CALL(buf, msg.rpt_lst[msg.rpt_cnt].call);
      msg.rpt_lst[msg.rpt_cnt].ssid = (*buf >> 1) & 0x0F;
      AX25_SET_REPEATED(&msg, msg.rpt_cnt, (*buf & 0x80));

      LOG_INFO("RPT%d[%.6s-%d]%c\n", msg.rpt_cnt,
        msg.rpt_lst[msg.rpt_cnt].call,
        msg.rpt_lst[msg.rpt_cnt].ssid,
        (AX25_REPEATED(&msg, msg.rpt_cnt) ? '*' : ' '));
    }
  #else
    while (!(*buf++ & 0x01)) {

      char rpt[6];
      uint8_t ssid;
      DECODE_CALL(buf, rpt);
      ssid = (*buf >> 1) & 0x0F;
      LOG_INFO("RPT[%.6s-%d]\n", rpt, ssid);
    }
  #endif

  msg.ctrl = *buf++;
  if (msg.ctrl != AX25_CTRL_UI) {

    LOG_WARN("Only UI frames are handled, got [%02X]\n", msg.ctrl);
    return;
  }

  msg.pid = *buf++;
  if (msg.pid != AX25_PID_NOLAYER3) {

    LOG_WARN("Only frames without layer3 protocol are handled, got [%02X]\n", msg.pid);
    return;
  }

  msg.len = ctx->frm_len - 2 - (buf - ctx->buf);
  msg.info = buf;
  LOG_INFO("DATA: %.*s\n", msg.len, msg.info);

  if (ctx->hook)
    ctx->hook(&msg);
}

// -----------------------------------------------------------------------------
static void
prvvPutChar(xAx25Context *ctx, uint8_t c) {

  if (c == HDLC_FLAG || c == HDLC_RESET
    || c == AX25_ESC)
    fputc (AX25_ESC, ctx->ch);
  ctx->crc_out = usCrcCcittUpdate(c, ctx->crc_out);
  fputc(c, ctx->ch);
}

// -----------------------------------------------------------------------------
static void
prvvSendCall(xAx25Context *ctx, const xAx25Call *addr, bool last) {
  unsigned len = MIN(sizeof(addr->call), strlen(addr->call));

  for (unsigned i = 0; i < len; i++) {

    uint8_t c = addr->call[i];
    vAssert(isalnum(c) || c == ' ');
    c = toupper(c);
    prvvPutChar(ctx, c << 1);
  }

  /* Fill with spaces the rest of the CALL if it's shorter */
  if (len < sizeof(addr->call))
    for (unsigned i = 0; i < sizeof(addr->call) - len; i++)
      prvvPutChar(ctx, ' ' << 1);

  /* The bit7 "has-been-repeated" flag is not implemented here */
  /* Bits6:5 should be set to 1 for all SSIDs (0x60) */
  /* The bit0 of last call SSID should be set to 1 */
  uint8_t ssid = 0x60 | (addr->ssid << 1) | (last ? 0x01 : 0);
  prvvPutChar(ctx, ssid);
}

// -----------------------------------------------------------------------------
static void
prvvPrintCall (FILE *ch, const xAx25Call *call) {

  fprintf (ch, "%.6s", call->call);
  if (call->ssid)
    fprintf(ch, "-%d", call->ssid);
}

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vAx25Poll(xAx25Context *ctx) {
  int c;

  while ((c = fgetc(ctx->ch)) != EOF) {

    if (!ctx->escape && c == HDLC_FLAG) {

      if (ctx->frm_len >= AX25_MIN_FRAME_LEN) {

        if (ctx->crc_in == AX25_CRC_CORRECT) {

          LOG_INFO("Frame found!\n");
          prvvDecode(ctx);
        }
        else {

          LOG_INFO("CRC error, computed [%04X]\n", ctx->crc_in);
        }
      }
      ctx->sync = true;
      ctx->crc_in = CRC_CCITT_INIT_VAL;
      ctx->frm_len = 0;
      continue;
    }

    if (!ctx->escape && c == HDLC_RESET) {

      LOG_INFO("HDLC reset\n");
      ctx->sync = false;
      continue;
    }

    if (!ctx->escape && c == AX25_ESC) {

      ctx->escape = true;
      continue;
    }

    if (ctx->sync) {

      if (ctx->frm_len < CONFIG_AX25_FRAME_BUF_LEN) {

        ctx->buf[ctx->frm_len++] = c;
        ctx->crc_in = usCrcCcittUpdate(c, ctx->crc_in);
      }
      else {

        LOG_INFO("Buffer overrun");
        ctx->sync = false;
      }
    }
    ctx->escape = false;
  }

  if (ferror(ctx->ch)) {

    LOG_ERR("Channel error [%04x]\n", ferror(ctx->ch));
    clearerr(ctx->ch);
  }
}

// -----------------------------------------------------------------------------
void
vAx25SendVia(xAx25Context *ctx, const xAx25Call *path, size_t path_len,
              const void *_buf, size_t len) {
  const uint8_t *buf = (const uint8_t *)_buf;

  vAssert(path);
  vAssert(path_len >= 2);

  ctx->crc_out = CRC_CCITT_INIT_VAL;
  fputc(HDLC_FLAG, ctx->ch);


  /* Send call */
  for (size_t i = 0; i < path_len; i++)
    prvvSendCall(ctx, &path[i], (i == path_len - 1));

  prvvPutChar(ctx, AX25_CTRL_UI);
  prvvPutChar(ctx, AX25_PID_NOLAYER3);

  while (len--)
    prvvPutChar(ctx, *buf++);

  /*
   * According to AX25 protocol,
   * CRC is sent in reverse order!
   */
  uint8_t crcl = (ctx->crc_out & 0xff) ^ 0xff;
  uint8_t crch = (ctx->crc_out >> 8) ^ 0xff;
  prvvPutChar(ctx, crcl);
  prvvPutChar(ctx, crch);

  vAssert(ctx->crc_out == AX25_CRC_CORRECT);

  fputc (HDLC_FLAG, ctx->ch);
}

// -----------------------------------------------------------------------------
void
vAx25Print(FILE *ch, const xAx25Msg *msg) {

  prvvPrintCall(ch, &msg->src);
  fputc('>', ch);
  prvvPrintCall(ch, &msg->dst);

  #if CONFIG_AX25_RPT_LST
  for (int i = 0; i < msg->rpt_cnt; i++) {

    fputc(',', ch);
    prvvPrintCall(ch, &msg->rpt_lst[i]);
    /* Print a '*' if packet has already been transmitted
     * by this repeater */
    if (AX25_REPEATED(msg, i))
      fputc('*', ch);
  }
  #endif

  fprintf(ch, ":%.*s\n", msg->len, msg->info);
}

// -----------------------------------------------------------------------------
void
vAx25Init(xAx25Context *ctx, FILE *channel, vAx25CallBack hook) {

  vAssert(ctx);
  vAssert(channel);

  memset(ctx, 0, sizeof(*ctx));
  ctx->ch = channel;
  ctx->hook = hook;
  ctx->crc_in = ctx->crc_out = CRC_CCITT_INIT_VAL;
}

/* ========================================================================== */
