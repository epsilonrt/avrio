/**
 * Copyright © 2015 epsilonRT. All rights reserved.
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
 * @file net.h
 * @brief Utiliaires réseau
 */
#ifndef _AVRIO_NET_H_
#define _AVRIO_NET_H_

#include <avrio/defs.h>
#if defined(__cplusplus)
  extern "C" {
#endif
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
 * @brief Convertir entier court de l'ordre des octets du réseau vers l'hôte
 *
 * @param n entier court 16-bit venant du réseau
 * @return entier court 16-bit dans l'ordre de l'hôte
 */
uint16_t ntohs(uint16_t n);

/**
 * @brief Convertir entier court de l'ordre des octets de l'hôte vers le réseau
 *
 * @param h entier court 16-bit venant de l'hôte
 * @return entier court 16-bit dans l'ordre du réseau
 */
uint16_t htons(uint16_t h);

/**
 * @brief Convertir entier long de l'ordre des octets du réseau vers l'hôte
 *
 * @param n entier long 32-bit venant du réseau
 * @return entier long 32-bit dans l'ordre de l'hôte
 */
uint32_t ntohl(uint32_t n);

/**
 * @brief Convertir entier long de l'ordre des octets de l'hôte vers le réseau
 *
 * @param h entier long 32-bit venant de l'hôte
 * @return entier long 32-bit dans l'ordre du réseau
 */
uint32_t htonl(uint32_t h);

/**
 * @brief Convertir entier long long de l'ordre des octets du réseau vers l'hôte
 *
 * @param n entier long 64-bit venant du réseau
 * @return entier long 64-bit dans l'ordre de l'hôte
 */
uint64_t ntohll(uint64_t n);

/**
 * @brief Convertir entier long long de l'ordre des octets de l'hôte vers le réseau
 *
 * @param h entier long 64-bit venant de l'hôte
 * @return entier long 64-bit dans l'ordre du réseau
 */
uint64_t htonll(uint64_t h);

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
#if defined(__cplusplus)
  }
#endif
#endif /* _AVRIO_NET_H_ */
