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
 */
#include <string.h>
#include <stdio.h>
#include <avr/pgmspace.h>
#include <avrio/assert.h>
#include <avrio/heap.h>

/* constants ================================================================ */
// #define HEAP_DEBUG 1
#define FREE_FILL_CODE     0xFE
#define ALLOC_FILL_CODE    0xA0

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void
vHeapInit (xHeap* h, void* memory, size_t size) {

#ifdef HEAP_DEBUG
  memset (memory, FREE_FILL_CODE, size);
#endif

  if ( ( (size_t) memory % alignof (heap_buf_t)) != 0) {
    fprintf_P (stderr, PSTR("memory buffer is unaligned, please use the "
                        "HEAP_DEFINE_BUF() macro to declare heap buffers !\n"));
    abort();
  }

  /* Initialize heap with a single big chunk */
  h->freelist = (xMemChunk *) memory;
  h->freelist->next = NULL;
  h->freelist->size = size;
}

// -----------------------------------------------------------------------------
void *
pvHeapAllocMem (xHeap* h, size_t size) {
  xMemChunk *chunk, *prev;

  /* Round size up to the allocation granularity */
  size = ROUND_UP2 (size, sizeof (xMemChunk));

  /* Handle allocations of 0 bytes */
  if (!size) {
    size = sizeof (xMemChunk);
  }

  /* Walk on the free list looking for any chunk big enough to
   * fit the requested block size.
   */
  for (prev = (xMemChunk *) &h->freelist, chunk = h->freelist;
       chunk;
       prev = chunk, chunk = chunk->next) {
    if (chunk->size >= size) {
      if (chunk->size == size) {
        /* Just remove this chunk from the free list */
        prev->next = chunk->next;
#ifdef HEAP_DEBUG
        memset (chunk, ALLOC_FILL_CODE, size);
#endif
        return (void *) chunk;
      }
      else {
        /* Allocate from the END of an existing chunk */
        chunk->size -= size;
#ifdef HEAP_DEBUG
        memset ( (uint8_t *) chunk + chunk->size, ALLOC_FILL_CODE, size);
#endif
        return (void *) ( (uint8_t *) chunk + chunk->size);
      }
    }
  }

  return NULL; /* fail */
}

// -----------------------------------------------------------------------------
void
vHeapFreeMem (xHeap* h, void *mem, size_t size) {
  xMemChunk *prev;
  assert (mem);

#ifdef HEAP_DEBUG
  memset (mem, FREE_FILL_CODE, size);
#endif

  /* Round size up to the allocation granularity */
  size = ROUND_UP2 (size, sizeof (xMemChunk));

  /* Handle allocations of 0 bytes */
  if (!size) {
    size = sizeof (xMemChunk);
  }

  /* Special cases: first chunk in the free list or memory completely full */
  assert ( (uint8_t*) mem != (uint8_t*) h->freelist);
  if ( ( (uint8_t *) mem) < ( (uint8_t *) h->freelist) || !h->freelist) {
    /* Insert memory block before the current free list head */
    prev = (xMemChunk *) mem;
    prev->next = h->freelist;
    prev->size = size;
    h->freelist = prev;
  }
  else { /* Normal case: not the first chunk in the free list */
    /*
     * Walk on the free list. Stop at the insertion point (when mem
     * is between prev and prev->next)
     */
    prev = h->freelist;
    while (prev->next < (xMemChunk *) mem && prev->next) {
      prev = prev->next;
    }

    /* Make sure mem is not *within* prev */
    assert ( (uint8_t*) mem >= (uint8_t*) prev + prev->size);

    /* Should it be merged with previous block? */
    if ( ( (uint8_t *) prev) + prev->size == ( (uint8_t *) mem)) {
      /* Yes */
      prev->size += size;
    }
    else { /* not merged with previous chunk */
      xMemChunk *curr = (xMemChunk*) mem;

      /* insert it after the previous node
       * and move the 'prev' pointer forward
       * for the following operations
       */
      curr->next = prev->next;
      curr->size = size;
      prev->next = curr;

      /* Adjust for the following test */
      prev = curr;
    }
  }

  /* Also merge with next chunk? */
  if ( ( (uint8_t *) prev) + prev->size == ( (uint8_t *) prev->next)) {
    prev->size += prev->next->size;
    prev->next = prev->next->next;

    /* There should be only one merge opportunity, becuase we always merge on free */
    assert ( (uint8_t*) prev + prev->size != (uint8_t*) prev->next);
  }
}

// -----------------------------------------------------------------------------
size_t
ulHeapFreeSpace (xHeap *h) {
  size_t free_mem = 0;
  for (xMemChunk *chunk = h->freelist; chunk; chunk = chunk->next) {
    free_mem += chunk->size;
  }

  return free_mem;
}

// -----------------------------------------------------------------------------
void *
pvHeapMalloc (xHeap* h, size_t size) {
  size_t *mem;

  size += sizeof (size_t);
  if ( (mem = (size_t*) pvHeapAllocMem (h, size))) {
    *mem++ = size;
  }

  return mem;
}

// -----------------------------------------------------------------------------
void *
pvHeapCalloc (xHeap* h, size_t nmemb, size_t size) {
  void *mem;

  if ( (mem = pvHeapMalloc (h, nmemb * size))) {
    memset (mem, 0, nmemb * size);
  }

  return mem;
}

// -----------------------------------------------------------------------------
void
vHeapFree (xHeap *h, void *mem) {
  size_t *_mem = (size_t *) mem;

  if (_mem) {
    --_mem;
    vHeapFreeMem (h, _mem, *_mem);
  }
}
/* ========================================================================== */
