/**
 * Copyright © 2015 Pascal JEAN aka epsilonRT. All rights reserved.
 *
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
 *
 * @file net.h
 * @brief Utiliaires réseau
 */
#ifndef _AVRIO_NET_H_
#define _AVRIO_NET_H_

#include <avrio/defs.h>
__BEGIN_C_DECLS
/* ======================================================================== */
/**
 * @addtogroup net_group
 * @{
 *
 *  @defgroup net_module Utiliaires réseau
 *
 *  Ce module fournit des fonctions utilitaires réseau.
 *  @{
 */

/* constants ================================================================ */

#if defined(__DOXYGEN__)
/*
 * __DOXYGEN__ defined
 * Partie documentation ne devant pas être compilée.
 * =============================================================================
 */

/* internal public functions ================================================ */

/**
 * Convertit un mot de l'ordre réseau à l'ordre hôte
 */
uint16_t ntohs(uint16_t netshort);
/**
 * Convertit un mot de l'ordre hôte à l'ordre réseau
 */
uint16_t htons(uint16_t hostshort);

/**
 * Convertit un long mot de l'ordre réseau à l'ordre hôte
 */
uint32_t ntohl(uint32_t netlong);
/**
 * Convertit un long mot de l'ordre hôte à l'ordre réseau
 */
uint32_t htonl(uint32_t hostlong);

/**
 * Convertit un très long mot de l'ordre réseau à l'ordre hôte
 */
uint64_t ntohll(uint64_t netlonglong);
/**
 * Convertit un très long mot de l'ordre hôte à l'ordre réseau
 */
uint64_t htonll(uint64_t hostlonglong);

/**
 * empile un mot d'une pile de données réseau
 */
uint8_t * pushs (uint8_t * netdata, uint16_t hostshort);
/**
 * dépile un mot d'une pile de données réseau
 */
uint8_t * pops (uint8_t * netdata, uint16_t * hostshort);

/**
 * empile un long mot d'une pile de données réseau
 */
uint8_t * pushl (uint8_t * netdata, uint32_t hostlong);
/**
 * dépile un long mot d'une pile de données réseau
 */
uint8_t * popl (uint8_t * netdata, uint32_t * hostlong);
/**
 * empile un très long mot d'une pile de données réseau
 */
uint8_t * pushll (uint8_t * netdata, uint32_t hostlonglong);
/**
 * dépile un très long mot d'une pile de données réseau
 */
uint8_t * popll (uint8_t * netdata, uint32_t * hostlonglong);

/**
 *   @}
 * @}
 */
#else
/*
 * __DOXYGEN__ not defined
 * Partie ne devant pas être documentée.
 * =============================================================================
 */

#if !defined(ntohs) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
//------------------------------------------------------------------------------
#include <avrio/util.h> // vSwapBytes() from avrio/util_swapbytes.S
INLINE uint16_t
ntohs (uint16_t n) {

  vSwapBytes ( (uint8_t *) &n, sizeof (n));
  return n;
}
# define htons(n) ntohs(n)
#elif !defined(ntohs)
//------------------------------------------------------------------------------
# define ntohs(n) ((uint16_t)(n))
# define htons(n) ntohs(n)
#endif

#if !defined(ntohl) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
//------------------------------------------------------------------------------
#include <avrio/util.h> // vSwapBytes() from avrio/util_swapbytes.S
INLINE uint32_t
ntohl (uint32_t n) {

  vSwapBytes ( (uint8_t *) &n, sizeof (n));
  return n;
}
# define htonl(n) ntohl(n)
#elif !defined(ntohl)
//------------------------------------------------------------------------------
# define ntohl(n) ((uint32_t)(n))
# define htonl(n) ntohl(n)
#endif

#if !defined(ntohll) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
//------------------------------------------------------------------------------
#include <avrio/util.h> // vSwapBytes() from avrio/util_swapbytes.S
INLINE uint64_t
ntohll (uint64_t n) {

  vSwapBytes ( (uint8_t *) &n, sizeof (n));
  return n;
}
# define htonll(n) ntohll(n)

#elif !defined(ntohll)
//------------------------------------------------------------------------------
# define ntohll(n) ((uint64_t)(n))
# define htonll(n) ntohll(n)
#endif


//------------------------------------------------------------------------------
INLINE uint8_t * 
pushs (uint8_t * netdata, uint16_t hostshort) {
  *PUINT16(netdata) = htons (hostshort);
  netdata += sizeof(hostshort);
  return netdata;
}

//------------------------------------------------------------------------------
INLINE uint8_t * 
pops (uint8_t * netdata, uint16_t * hostshort) {
  *hostshort = ntohs (*PUINT16(netdata));
  netdata += sizeof(hostshort);
  return netdata;
}

//------------------------------------------------------------------------------
INLINE uint8_t * 
pushl (uint8_t * netdata, uint32_t hostlong) {
  *PUINT32(netdata) = htonl (hostlong);
  netdata += sizeof(hostlong);
  return netdata;
}

//------------------------------------------------------------------------------
INLINE uint8_t * 
popl (uint8_t * netdata, uint32_t * hostlong) {
  *hostlong = ntohl (*PUINT32(netdata));
  netdata += sizeof(hostlong);
  return netdata;
}

//------------------------------------------------------------------------------
INLINE uint8_t * 
pushll (uint8_t * netdata, uint32_t hostlonglong) {
  *PUINT64(netdata) = htonll (hostlonglong);
  netdata += sizeof(hostlonglong);
  return netdata;
}

//------------------------------------------------------------------------------
INLINE uint8_t * 
popll (uint8_t * netdata, uint32_t * hostlonglong) {
  *hostlonglong = ntohll (*PUINT32(netdata));
  netdata += sizeof(hostlonglong);
  return netdata;
}

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
__END_C_DECLS
#endif /* _AVRIO_NET_H_ */
