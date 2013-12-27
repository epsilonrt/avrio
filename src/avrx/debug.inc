/**
 * @file debug.inc
 * @author Larry Barello, <larry@barello.net>. 
 *         Copyright © 1998-2002 All rights reserved.
 * @author Pascal Jean, epsilonRT. Copyright © 2011 All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * @version $Id$
 * Revision History ---
 *    20110905 - Initial version by epsilonRT
 *    20050918 - It is recommended that these routines not be used for user
 *               code.  See the sample AvrXIo for more generic, flexible 
 *               serial I/O routines.
 * ---
 * @brief Module pour le débugage
 *
 * Note: although AvrX is processor independent, Serialio.s is not. You MUST
 * compile this file for the particular CPU being used as GCC no longer places
 * interrupt vectors at link time.
 */

  .extern vDebugInit
  .extern cDebugPutChar
  .extern cDebugGetChar
  .extern vDebugPushChar

// -----------------------------------------------------------------------------
  .extern cDebugGetCharEcho
  .extern vDebugPutCR
  .extern vDebugPutSpace
  .extern vDebugPutString
  .extern xDebugGetHex
  .extern usDebugGetHexWord
  .extern ucDebugGetHexByte
  .extern ucDebugGetHexDigit
  .extern vDebugPutHex
  .extern vDebugPutHexWord
  .extern vDebugPutHexByte
  .extern vDebugPutHexDigit
  .extern xDebugIsHex 
  .extern xDebugIsWhite

/* ========================================================================== */

