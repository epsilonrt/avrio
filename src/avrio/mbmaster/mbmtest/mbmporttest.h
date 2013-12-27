/* 
 * MODBUS Library: Port testing utility
 * Copyright (c) 2007 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * $Id$
 */

#ifndef _MBM_PORT_TEST_H
#define _MBM_PORT_TEST_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif

/*! \addtogroup mbm_port_test
 * @{
 */

/* ----------------------- Function prototypes ------------------------------*/

void            vMBPTestDebugLED( UBYTE ubIdx, BOOL bTurnOn );
void            vMBPTestRun( void );

#ifdef __cplusplus
PR_END_EXTERN_C
#endif

/*! @} */

#endif
