/**
 * @file queue.c
 * @brief Pile d'octets FIFO
 * @author Copyright _ 2011-2014 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 */
#include <avrio/queue.h>

/* internal public functions ================================================ */
// ------------------------------------------------------------------------------
void
vQueueSetBuffer (struct xQueue *pxQueue, uint8_t * pucBuffer, size_t xBufferSize) {

  pxQueue->pxFirst  = pucBuffer;
  pxQueue->pxLast   = pxQueue->pxFirst + xBufferSize - 1;
  pxQueue->pxIn     = pxQueue->pxFirst;
  pxQueue->pxOut    = pxQueue->pxFirst;
  pxQueue->xLock    = QUEUE_MUTEX_INITIALIZER;
}

// ------------------------------------------------------------------------------
size_t
xQueueSize (xQueue * pxQueue) {

  return (size_t) (pxQueue->pxLast - pxQueue->pxFirst + 1);
}

// ------------------------------------------------------------------------------
size_t
xQueueLength (xQueue * pxQueue) {

  if (xQueueIsFull(pxQueue) == false) {
    ssize_t xLength;

    xLength = pxQueue->pxIn - pxQueue->pxOut;

    if (xLength < 0) {

      xLength += xQueueSize (pxQueue);
    }
    return (size_t) xLength;
  }
  // Pile pleine
  return xQueueSize (pxQueue);
}

// ------------------------------------------------------------------------------
size_t
xQueueFree (xQueue * pxQueue) {

  return xQueueSize (pxQueue) - xQueueLength (pxQueue);
}

// ------------------------------------------------------------------------------
void
vQueueFlush (xQueue * pxQueue) {

  pxQueue->pxIn = pxQueue->pxOut = pxQueue->pxFirst;
  pxQueue->xLock = QUEUE_MUTEX_INITIALIZER;
}

// ------------------------------------------------------------------------------
void
vQueueDrop (xQueue * pxQueue) {


  if (pxQueue->pxOut < pxQueue->pxLast) {

    pxQueue->pxOut++;
  }
  else {

    pxQueue->pxOut = pxQueue->pxFirst;
  }
  vQueueUnlock (pxQueue, QUEUE_LOCK_FULL);
  if (pxQueue->pxOut == pxQueue->pxIn) {

    vQueueLock (pxQueue, QUEUE_LOCK_EMPTY);
  }
}

// ------------------------------------------------------------------------------
void
vQueueDropBytes (xQueue * pxQueue, size_t xLength) {

  xLength = MIN (xLength, xQueueLength (pxQueue));

  pxQueue->pxOut += xLength;
  if (pxQueue->pxOut > pxQueue->pxLast) {

    pxQueue->pxOut -= xQueueSize (pxQueue);
  }
  vQueueUnlock (pxQueue, QUEUE_LOCK_FULL);
  if (pxQueue->pxOut == pxQueue->pxIn) {

    vQueueLock (pxQueue, QUEUE_LOCK_EMPTY);
  }
}

// ------------------------------------------------------------------------------
uint8_t
ucQueuePull (xQueue * pxQueue) {
  uint8_t ucByte;

  ucByte = *pxQueue->pxOut;
  vQueueDrop (pxQueue);

  return ucByte;
}

// ------------------------------------------------------------------------------
uint8_t
ucQueueRead (struct xQueue * pxQueue, size_t xIndex) {
  uint8_t *pxByte;

  xIndex = MIN (xIndex, xQueueLength (pxQueue) - 1);
  pxByte = pxQueue->pxOut + xIndex;

  if (pxByte > pxQueue->pxLast) {

    pxByte -= xQueueSize (pxQueue);
  }

  return *pxByte;
}

// ------------------------------------------------------------------------------
uint16_t
usQueueReadWord (struct xQueue * pxQueue, size_t xIndex) {
  uint16_t usWord;

  usWord = ucQueueRead (pxQueue, xIndex);
  usWord |= ucQueueRead (pxQueue, xIndex + 1) << 8;

  return usWord;
}

// ------------------------------------------------------------------------------
uint16_t
usQueuePullWord (struct xQueue * pxQueue) {
  uint16_t usWord;

  usWord = ucQueuePull (pxQueue);
  usWord |= ucQueuePull (pxQueue) << 8;

  return usWord;
}

// ------------------------------------------------------------------------------
size_t
xQueuePullBytes (struct xQueue *pxQueue, uint8_t * pucBytes, size_t xLength) {
  size_t xCount = 0;
  xLength = MIN(xQueueLength (pxQueue), xLength);

  while (xCount < xLength) {

    *pucBytes++ = ucQueuePull (pxQueue);
    xCount++;
  }

  return xCount;
}

// ------------------------------------------------------------------------------
size_t
xQueuePullAll (xQueue * pxQueue, uint8_t * pcBytes) {

  return xQueuePullBytes (pxQueue, pcBytes, xQueueLength(pxQueue));
}

// ------------------------------------------------------------------------------
void
vQueuePush (xQueue * pxQueue, uint8_t ucByte) {

  *pxQueue->pxIn = ucByte;

  if (pxQueue->pxIn < pxQueue->pxLast) {

    pxQueue->pxIn++;
  }
  else {

    pxQueue->pxIn = pxQueue->pxFirst;
  }
  if (pxQueue->pxIn == pxQueue->pxOut) {

    vQueueLock (pxQueue, QUEUE_LOCK_FULL);
  }
  vQueueUnlock (pxQueue, QUEUE_LOCK_EMPTY);
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
vQueuePushWord (struct xQueue *pxQueue, uint16_t usWord) {

  vQueuePush (pxQueue, usWord & 0xFF);
  vQueuePush (pxQueue, usWord >> 8);
}

// ------------------------------------------------------------------------------
const char *
pcQueuePushString (xQueue * pxQueue, const char *pcString) {

  while ((*pcString) && !xQueueIsFull (pxQueue)) {

    vQueuePush (pxQueue, (uint8_t) * pcString++);
  }

  return pcString;
}

// ------------------------------------------------------------------------------
size_t
xQueuePushBytes (xQueue * pxQueue, const uint8_t * pcBytes, size_t xLength) {

  while ((xLength) && !xQueueIsFull (pxQueue)) {

    vQueuePush (pxQueue, *pcBytes++);
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
  xQueue *pxQueue = malloc (sizeof (xQueue));

  if (pxQueue) {

    pxQueue->pxFirst = malloc (xBufferSize);
    if (pxQueue->pxFirst) {

      vQueueSetBuffer (pxQueue, pxQueue->pxFirst, xBufferSize);
    }
    else {

      free (pxQueue);
      pxQueue = 0;
    }
  }
  return pxQueue;
}

// ------------------------------------------------------------------------------
void
vQueueDelete (xQueue * pxQueue) {

  free (pxQueue->pxFirst);
  free (pxQueue);
}

/* ========================================================================== */
#endif /* AVRIO_QUEUE_MALLOC_ENABLE defined */

/* ========================================================================== */
