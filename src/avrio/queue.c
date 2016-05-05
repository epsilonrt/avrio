/**
 * Copyright © 2011-2015 epsilonRT. All rights reserved.
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
 * @file queue.c
 * @brief Pile d'octets FIFO
 */
#include <avrio/queue.h>

/* internal public functions ================================================ */
// ------------------------------------------------------------------------------
void
vQueueSetBuffer (struct xQueue *q, uint8_t * pucBuffer, size_t xBufferSize) {

  q->pxFirst  = pucBuffer;
  q->pxLast   = q->pxFirst + xBufferSize - 1;
  q->pxIn     = q->pxFirst;
  q->pxOut    = q->pxFirst;
  q->isEmpty  = true;
  q->isFull   = false;
  // TODO: suppression
  q->xLock    = MUTEX_INITIALIZER;
}

// ------------------------------------------------------------------------------
size_t
xQueueSize (xQueue * q) {

  return (size_t) (q->pxLast - q->pxFirst + 1);
}

// ------------------------------------------------------------------------------
size_t
xQueueLength (xQueue * q) {
  ssize_t xLength;

  if (q->isFull == false) {

    xLength = q->pxIn - q->pxOut;

    if (xLength < 0) {

      xLength += xQueueSize (q);
    }
  }
  else {

    xLength = xQueueSize (q);
  }

  return (size_t) xLength;
}

// ------------------------------------------------------------------------------
size_t
xQueueFree (xQueue * q) {

  return xQueueSize (q) - xQueueLength (q);
}

// ------------------------------------------------------------------------------
void
vQueueFlush (xQueue * q) {

  q->pxIn = q->pxOut = q->pxFirst;
  q->isEmpty  = true;
  q->isFull   = false;
}

// ------------------------------------------------------------------------------
void
vQueueDrop (xQueue * q) {

  if (q->pxOut < q->pxLast) {

    q->pxOut++;
  }
  else {

    q->pxOut = q->pxFirst;
  }
  q->isFull = false;
  if (q->pxOut == q->pxIn) {

    q->isEmpty = true;
  }
}

// ------------------------------------------------------------------------------
uint8_t
ucQueuePull (xQueue * q) {
  uint8_t ucByte;

  ucByte = *q->pxOut;
  vQueueDrop (q);
  return ucByte;
}

// ------------------------------------------------------------------------------
void
vQueuePush (xQueue * q, uint8_t ucByte) {

  *q->pxIn = ucByte;
  if (q->pxIn < q->pxLast) {

    q->pxIn++;
  }
  else {

    q->pxIn = q->pxFirst;
  }
  if (q->pxIn == q->pxOut) {

    q->isFull = true;
  }
  q->isEmpty = false;
}

// ------------------------------------------------------------------------------
void
vQueueDropBytes (xQueue * q, size_t xLength) {

  xLength = MIN (xLength, xQueueLength (q));

  q->pxOut += xLength;
  if (q->pxOut > q->pxLast) {

    q->pxOut -= xQueueSize (q);
  }
  q->isFull = false;
  if (q->pxOut == q->pxIn) {

    q->isEmpty = true;
  }
}

// ------------------------------------------------------------------------------
uint8_t
ucQueueRead (struct xQueue * q, size_t xIndex) {
  uint8_t *pxByte;

  xIndex = MIN (xIndex, xQueueLength (q) - 1);
  pxByte = q->pxOut + xIndex;

  if (pxByte > q->pxLast) {

    pxByte -= xQueueSize (q);
  }
  return *pxByte;
}

// ------------------------------------------------------------------------------
uint16_t
usQueueReadWord (struct xQueue * q, size_t xIndex) {
  uint16_t usWord;

  usWord = ucQueueRead (q, xIndex);
  usWord |= ucQueueRead (q, xIndex + 1) << 8;
  return usWord;
}

// ------------------------------------------------------------------------------
uint16_t
usQueuePullWord (struct xQueue * q) {
  uint16_t usWord;

  usWord = ucQueuePull (q);
  usWord |= ucQueuePull (q) << 8;

  return usWord;
}

// ------------------------------------------------------------------------------
size_t
xQueuePullBytes (struct xQueue *q, uint8_t * pucBytes, size_t xLength) {
  size_t xCount = 0;
  xLength = MIN(xQueueLength (q), xLength);

  while (xCount < xLength) {

    *pucBytes++ = ucQueuePull (q);
    xCount++;
  }

  return xCount;
}

// ------------------------------------------------------------------------------
size_t
xQueuePullAll (xQueue * q, uint8_t * pcBytes) {

  return xQueuePullBytes (q, pcBytes, xQueueLength(q));
}

// ------------------------------------------------------------------------------
int
iQueueCompare (struct xQueue *pxQueue1, struct xQueue *pxQueue2) {
  int iReturn = -1;
  uint8_t * pucQ1 = pxQueue1->pxOut;
  uint8_t * pucQ2 = pxQueue2->pxOut;
  size_t xQ1Length = xQueueLength(pxQueue1);

  while ((pucQ1 != pxQueue1->pxIn) && (pucQ2 != pxQueue2->pxIn) && (*pucQ1 == *pucQ2)) {

    iReturn++;
    if (pucQ1 < pxQueue1->pxLast) {

      pucQ1++;
    } else {

      pucQ1 = pxQueue1->pxFirst;
    }
    if (pucQ2 < pxQueue2->pxLast) {

      pucQ2++;
    } else {

      pucQ2 = pxQueue2->pxFirst;
    }

  }
  if ((xQ1Length == xQueueLength(pxQueue2)) && (iReturn == xQ1Length)) {

    iReturn = 0;
  }
  return iReturn;
}

// ------------------------------------------------------------------------------
void
vQueuePushWord (struct xQueue *q, uint16_t usWord) {

  vQueuePush (q, usWord & 0xFF);
  vQueuePush (q, usWord >> 8);
}

// ------------------------------------------------------------------------------
const char *
pcQueuePushString (xQueue * q, const char *pcString) {

  while ((*pcString) && !xQueueIsFull (q)) {

    vQueuePush (q, (uint8_t) * pcString++);
  }
  return pcString;
}

// ------------------------------------------------------------------------------
size_t
xQueuePushBytes (xQueue * q, const uint8_t * pcBytes, size_t xLength) {

  while ((xLength) && !xQueueIsFull (q)) {

    vQueuePush (q, *pcBytes++);
    xLength--;
  }

  return xLength;
}

// ------------------------------------------------------------------------------
size_t
xQueuePushBytesOfQueue (xQueue * pxDstQueue,
                        xQueue * pxSrcQueue, size_t xLength) {

  while (!xQueueIsEmpty (pxSrcQueue) && (xLength--) &&
         !xQueueIsFull (pxDstQueue)) {

    vQueuePush (pxDstQueue, ucQueuePull (pxSrcQueue));
  }

  return xLength;
}

#ifdef AVRIO_QUEUE_MALLOC_ENABLE
/* ========================================================================== */
#  include <stdlib.h>

// ------------------------------------------------------------------------------
xQueue *
pxQueueNew (size_t xBufferSize) {
  xQueue *q = malloc (sizeof (xQueue));

  if (q) {

    q->pxFirst = malloc (xBufferSize);
    if (q->pxFirst) {

      vQueueSetBuffer (q, q->pxFirst, xBufferSize);
    }
    else {

      free (q);
      q = 0;
    }
  }
  return q;
}

// ------------------------------------------------------------------------------
void
vQueueDelete (xQueue * q) {

  free (q->pxFirst);
  free (q);
}

/* ========================================================================== */
#endif /* AVRIO_QUEUE_MALLOC_ENABLE defined */

/* ========================================================================== */
