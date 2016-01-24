/*
 * This file is part of AvrIO.
 *
 * AvrIO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * AvrIO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with AvrIO.  If not, see <http://www.gnu.org/licenses/lgpl.html>
 */
#ifndef _AVRIO_WUSB_CONFIG_H_
#define _AVRIO_WUSB_CONFIG_H_

#include <avrio/defs.h>
#include <avrio/wusb/cywusb693_.h>

/* *INDENT-OFF* */
__BEGIN_C_DECLS
/* ========================================================================== */
/*
 * @addtogroup wusb_module
 * @{
 *
 *  @defgroup wusb_config
 *  @{
 */

/* macros =================================================================== */

/* constants ================================================================ */

/*===========================================================================*
 *                         Device Config                                     *
 *===========================================================================*/
//#define WUSB_CONFIG_DEVICE_ADJUST_XTAL

#define WUSB_CONFIG_DEVICE_DEFAULT_DATARATE             WDEV_RATE_16KBPS    /* Vitesse de transmission */
#define WUSB_CONFIG_DEVICE_DEFAULT_CHANNEL              2      /* Channel: 2 - 79 */
#define WUSB_CONFIG_DEVICE_DEFAULT_PA_BIAS              5      /* Power Amplifier Bias: 0 - 7 */
#define WUSB_CONFIG_DEVICE_DEFAULT_EOF_BITS             3      /* EOF Length: 0 - 7 */
#define WUSB_CONFIG_DEVICE_DEFAULT_XTL                  0x00   /* Crystal Adjust: 0x00 - 0x3F */
#define WUSB_CONFIG_DEVICE_DEFAULT_VCO_ADJUST           bMINUS5_PLUS5
#define WUSB_CONFIG_DEVICE_DEFAULT_SYNTH_SETTLE_COUNT   150

/*===========================================================================*
 *                         Packet Config                                     *
 *===========================================================================*/
//#define WUSB_CONFIG_PACKET_USE_MALLOC             1   /*< Allocation dynamique de mémoire */
#define WUSB_CONFIG_PACKET_USE_INLINE             1   /*< Utilisation de fonctions inline */
#define WUSB_CONFIG_PACKET_PAYLOAD_LENGTH_SIZEOF  1   /*< Taille du champ "longueur du payload data" */ 

/*===========================================================================*
 *                         Network Config                                    *
 *===========================================================================*/
#define WUSB_CONFIG_NETWORK_USE_INLINE             1   /*< Utilisation de fonctions inline */
#define WUSB_CONFIG_NETWORK_PAYLOAD_SIZE        32
#define WUSB_CONFIG_NETWORK_CHANNEL_CONFIG      4
#define WUSB_CONFIG_NETWORK_SEEDED_BIND_CHANNEL 1
#define WUSB_CONFIG_NETWORK_SEEDED_BIND_PNCODE  1

#define WUSB_CONFIG_NETWORK_DEFAULT_THRESHOLD_L_16KBPS  12  /*  8 - 12 */
#define WUSB_CONFIG_NETWORK_DEFAULT_THRESHOLD_H_16KBPS  52  /* 56 - 52 */

#define WUSB_CONFIG_NETWORK_DEFAULT_THRESHOLD_L_64KBPS   8  /*  8 -  2 */
#define WUSB_CONFIG_NETWORK_DEFAULT_THRESHOLD_H_64KBPS  56  /* 56 - 30 */

#define WUSB_CONFIG_NETWORK_DEFAULT_TIMEOUT_ACK  5
#define WUSB_CONFIG_NETWORK_DEFAULT_TIMEOUT_BIND 6
#define WUSB_CONFIG_NETWORK_DEFAULT_TIMEOUT_PING 6
#define WUSB_CONFIG_NETWORK_CORRUPTPKT_THRESHOLD 20

/* This is setup to ensure a guard ban at the low end Channel 0 - 2.402, 
 * a 2 MHz guard band to 2.400
 * The upper guard band is protected by the use of NUM_CHANNELS in the protocol 
 * and application Channel 78 - 2.480, a 3.5MHz guard to 2.4835
 */
#define WUSB_CONFIG_NETWORK_CHANNEL_OFFSET 2

/*===========================================================================*
 *                         Sensor Config                                     *
 *===========================================================================*/
#define WUSB_CONFIG_SENSOR_BIND_TX_PER_CH  2
#define WUSB_CONFIG_SENSOR_BIND_CYCLES     2

#define WUSB_CONFIG_SENSOR_NACK_RETRY_ATTEMPTS   7   // 1-7,Number of TX with random delay
#define WUSB_CONFIG_SENSOR_SEARCH_PACKET_REPEATS 3   // 1-255,Number of TX on each ch
#define WUSB_CONFIG_SENSOR_SEARCH_PACKET_DELAY   20  // Number of ms between each TX

/*===========================================================================*
 *                         Hub    Config                                     *
 *===========================================================================*/
/*
 * @brief WUSB_CONFIG_HUB_NOISE_THRESHOLD_MAX Valeur max de bruit pour un bon canal
 * WUSB_CONFIG_HUB_SEARCH_PING_REPEATS Nombre de PING envoyés avant de considérer un canal libre
 * WUSB_CONFIG_HUB_SEARCH_PING_PERIOD Période en ms d'envoi des PING dans le mode de recherche de canal
 * WUSB_CONFIG_HUB_SERVICE_TICK  Unité de temps en ms pour la période des services
 * WUSB_CONFIG_HUB_SERVICE_PERIOD_NOISE Période en ticks du service de mesure du bruit (max 255)
 */
#define WUSB_CONFIG_HUB_NOISE_THRESHOLD_MAX   17
#define WUSB_CONFIG_HUB_SEARCH_PING_REPEATS   6 
#define WUSB_CONFIG_HUB_SEARCH_PING_PERIOD    20 
#define WUSB_CONFIG_HUB_SERVICE_TICK          50
#define WUSB_CONFIG_HUB_SERVICE_PERIOD_NOISE  10
/*
 * @brief Taille max des données envoyées à un capteur
 */
#define WUSB_CONFIG_HUB_BACKDATA_SIZE WUSB_CONFIG_NETWORK_PAYLOAD_SIZE
#define WUSB_CONFIG_HUB_UNKNOWN_CMD_ENABLED 1

/*@}*/
/* ========================================================================== */
__END_C_DECLS
/* *INDENT-ON* */
#endif  /* _AVRIO_WUSB_CONFIG_H_ */

