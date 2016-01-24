#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include <avrio/dlist.h>
#include <avrio/assert.h>
#include <avrio/serial.h>

/* constants ================================================================ */
#define SERIAL_BAUDRATE 38400
#define SERIAL_IOS (SERIAL_DEFAULT + SERIAL_WR)

/* private functions ======================================================== */

// -----------------------------------------------------------------------------
const void *
key (const xDListElmt * element) {

  return pvDListElmtData (element);
}

// -----------------------------------------------------------------------------
int
match (const void *key1, const void *key2) {
  int k1 = * (int *) key1;
  int k2 = * (int *) key2;

  return k1 - k2;
}


// -----------------------------------------------------------------------------
static void
vPrintList (const xDList *list) {
  xDListElmt          *element;
  int                *data, i;

  printf_P (PSTR("List size is %d\n"), iDListSize (list));

  for (element = pxDListHead (list), i = 0;
       element != NULL;
       element = pxDListElmtNext (element), i++) {

    data = pvDListElmtData (element);
    printf_P (PSTR("list[%03d]=%03d\n"), i, *data);
  }
}

/* main ===================================================================== */
int main (int argc, char **argv) {
  xDList              list;
  xDListElmt          *element;
  int                *data, i, j;

  vSerialInit (SERIAL_BAUDRATE / 100, SERIAL_IOS);
  stdout = &xSerialPort;
  stderr = &xSerialPort;

  //  Initialize the doubly-linked list.
  iDListInit (&list, free);
  iDListInitSearch (&list, key, match);

  //  Test 1 - Perform some doubly-linked list operations.
  element = pxDListHead (&list);
  for (i = 10; i > 0; i--) {

    if ( (data = (int *) malloc (sizeof (int))) == NULL) {
      return 1;
    }

    *data = i;
    if (iDListInsertBefore (&list, pxDListHead (&list), data) != 0) {
      return 1;
    }
  }
  vPrintList (&list);

  element = pxDListHead (&list);
  for (i = 1; i <= iDListSize (&list); i++) {

    j = *pxDListElmtDataPtr (element, int);
    assert (i == j);
    element = pxDListElmtNext (element);
  }
  printf_P (PSTR("Test %d: %s\n"), 1, "Ok");

  //  Test 2
  element = pxDListHead (&list);
  for (i = 0; i < 8; i++) {
    element = pxDListElmtNext (element);
  }
  data = pvDListElmtData (element);

  printf_P (PSTR("\nRemoving an element after the one containing %03d\n"), *data);

  if (iDListRemove (&list, element, (void **) &data) != 0) {
    return 1;
  }
  vPrintList (&list);

  //  Test 3
  printf_P (PSTR("\nInserting 011 at the tail of the list\n"));
  *data = 11;
  if (iDListInsertAfter (&list, pxDListTail (&list), data) != 0) {
    return 1;
  }
  vPrintList (&list);

  //  Test 4
  printf_P (PSTR("\nRemoving an element at the tail of the list\n"));
  element = pxDListTail (&list);
  if (iDListRemove (&list, element, (void **) &data) != 0) {
    return 1;
  }
  vPrintList (&list);

  //  Test 5
  printf_P (PSTR("\nInserting 012 just before the tail of the list\n"));
  *data = 12;
  if (iDListInsertBefore (&list, pxDListTail (&list), data) != 0) {
    return 1;
  }
  vPrintList (&list);

  //  Test 6
  printf_P (PSTR("\nIterating and removing the fourth element\n"));
  element = pxDListHead (&list);
  element = pxDListElmtNext (element);
  element = pxDListElmtPrev (element);
  element = pxDListElmtNext (element);
  element = pxDListElmtPrev (element);
  element = pxDListElmtNext (element);
  element = pxDListElmtNext (element);
  element = pxDListElmtNext (element);
  if (iDListRemove (&list, element, (void **) &data) != 0) {
    return 1;
  }
  vPrintList (&list);

  //  Test 7
  printf_P (PSTR("\nInserting 013 before the first element\n"));
  *data = 13;
  if (iDListInsertBefore (&list, pxDListHead (&list), data) != 0) {
    return 1;
  }
  vPrintList (&list);

  //  Test 8
  printf_P (PSTR("\nRemoving an element at the head of the list\n"));
  if (iDListRemove (&list, pxDListHead (&list), (void **) &data) != 0) {
    return 1;
  }
  vPrintList (&list);

  //  Test 9
  printf_P (PSTR("\nInserting 014 just after the head of the list\n"));
  *data = 14;
  if (iDListInsertAfter (&list, pxDListHead (&list), data) != 0) {
    return 1;
  }
  vPrintList (&list);

  //  Test 10
  printf_P (PSTR("\nInserting 015 two elements after the head of the list\n"));
  if ( (data = (int *) malloc (sizeof (int))) == NULL) {
    return 1;
  }
  *data = 15;
  element = pxDListHead (&list);
  element = pxDListElmtNext (element);
  if (iDListInsertAfter (&list, element, data) != 0) {
    return 1;
  }
  vPrintList (&list);

  //  Test 11
  i = bDListElmtIsHead (pxDListHead (&list));
  printf_P (PSTR("\nTesting bDListElmtIsHead...Value=%d (1=OK)\n"), i);
  i = bDListElmtIsHead (pxDListTail (&list));
  printf_P (PSTR("Testing bDListElmtIsHead...Value=%d (0=OK)\n"), i);
  i = bDListElmtIsTail (pxDListTail (&list));
  printf_P (PSTR("Testing bDListElmtIsTail...Value=%d (1=OK)\n"), i);
  i = bDListElmtIsTail (pxDListHead (&list));
  printf_P (PSTR("Testing bDListElmtIsTail...Value=%d (0=OK)\n"), i);

  // Test  12
  printf_P (PSTR("Test %d: "), 12);
  j = 6;
  element = pxDListFindFirst (&list, &j);
  printf_P (PSTR("Search %d, found %d ... "), j, *pxDListElmtDataPtr (element, int));
  assert (*pxDListElmtDataPtr (element, int) == 6);
  printf_P (PSTR("%s\n"), "Ok");

  //  Destroy the doubly-linked list.                                           *
  printf_P (PSTR("\nDestroying the list\n"));
  iDListDestroy (&list);

  return 0;

}

/* ========================================================================== */
