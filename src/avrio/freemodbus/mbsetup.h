/* Copyright _ 2009-2012 epsilonRT. All rights reserved.
 *  $Id$ */
#ifndef _MP2011_MBSETUP_H_
#  define _MP2011_MBSETUP_H_
/* ========================================================================== */
#  include <avrio/defs.h>
#  include "mb.h"
#  include "mbport.h"

/* internal public functions ================================================ */
void vMBSetupInit (void);
ULONG ulMBSetupBaudRate (void);
eMBParity eMBSetupParity (void);
UCHAR ucMBSetupSlaveAddress (void);

/* inline public functions ================================================== */

/* ========================================================================== */
#endif /* _MP2011_MBSETUP_H_ */
