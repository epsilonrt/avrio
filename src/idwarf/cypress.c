/**
 * @file cypress.c
 * @brief Réimplémente les fonctions du DVK nécessaires à l'évolution
 * @author Copyright © 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120311 - Initial version by epsilonRT * 
 * --------------------------------------------------------------------------
 *               
 *  Copyright 2004, Cypress Semiconductor Corporation.
 * 
 *  This software is owned by Cypress Semiconductor Corporation (Cypress)
 *  and is protected by and subject to worldwide patent protection (United
 *  States and foreign), United States copyright laws and international
 *  treaty provisions. Cypress hereby grants to licensee a personal,
 *  non-exclusive, non-transferable license to copy, use, modify, create
 *  derivative works of, and compile the Cypress Source Code and derivative
 *  works for the sole purpose of creating custom software in support of
 *  licensee product to be used only in conjunction with a Cypress integrated
 *  circuit as specified in the applicable agreement. Any reproduction,
 *  modification, translation, compilation, or representation of this
 *  software except as specified above is prohibited without the express
 *  written permission of Cypress.
 * 
 *  Disclaimer: CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
 *  WITH REGARD TO THIS MATERIAL, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *  Cypress reserves the right to make changes without further notice to the
 *  materials described herein. Cypress does not assume any liability arising
 *  out of the application or use of any product or circuit described herein.
 *  Cypress does not authorize its products for use as critical components in
 *  life-support systems where a malfunction or failure may reasonably be
 *  expected to result in significant injury to the user. The inclusion of
 *  Cypress' product in a life-support systems application implies that the
 *  manufacturer assumes all risk of such use and in doing so indemnifies
 *  Cypress against all charges.
 * 
 *  Use may be limited by and subject to the applicable Cypress software
 *  license agreement.
 * 
 * --------------------------------------------------------------------------
 */
#include "cypress.h"
#include "fram.h"

#ifdef HUB
/* ========================================================================== */

/* public variables ========================================================= */
extern U16 gu16NextFreeDeviceId;

/* internal public functions ================================================ */

//--------------------------------------------------------------------------
//
//  Function:    HubFindNextFreeDeviceId
//
//  Description: After this function executes gu16NextFreeDeviceId will
//               contain the next free Device Id or 0xFFFF is none
//               is available
//
//--------------------------------------------------------------------------
void HubFindNextFreeDeviceId(void)
{
    U16 Loop = (FRAM_LENGTH >> 4);
    U8  Status;

    // Meaured while(loop) on 27-May-2004. It took 94.3ms worst case for 512
    // itererations. Commented out return and used scope toggles on both ends.
    while (Loop) // Check every memory location
    {
        --Loop;

        ++gu16NextFreeDeviceId;

        // Wrap if necessary
        if (gu16NextFreeDeviceId > ((FRAM_LENGTH >> 4) - 1))
            gu16NextFreeDeviceId = 0;

        Status = FramReadByte((gu16NextFreeDeviceId << 4) + offsetof(FRAM_STRUCT, sStatus));

        if ((*(FRAM_STATUS*)&Status).sFlags.Enabled == FALSE)
        {
            // We've found a free Device Id, exit.
            return;
        }
    }

    // We've check every status byte and nothing is available.
    // Doing this lets the protocol know we're full.
    gu16NextFreeDeviceId = 0xFFFF;
}

/*
000013a0 <HubFindNextFreeDeviceId>:
    13a0:	cf 93       	push	r28
    13a2:	df 93       	push	r29
    13a4:	c9 e1       	ldi	r28, 0x19	; 25
    13a6:	d0 e0       	ldi	r29, 0x00	; 0
    13a8:	80 91 5f 03 	lds	r24, 0x035F
    13ac:	90 91 60 03 	lds	r25, 0x0360
    13b0:	01 96       	adiw	r24, 0x01	; 1
    13b2:	90 93 60 03 	sts	0x0360, r25
    13b6:	80 93 5f 03 	sts	0x035F, r24
    13ba:	89 31       	cpi	r24, 0x19	; 25
    13bc:	91 05       	cpc	r25, r1
    13be:	20 f0       	brcs	.+8      	; 0x13c8 <HubFindNextFreeDeviceId+0x28>
    13c0:	10 92 60 03 	sts	0x0360, r1
    13c4:	10 92 5f 03 	sts	0x035F, r1
    13c8:	80 91 5f 03 	lds	r24, 0x035F
    13cc:	90 91 60 03 	lds	r25, 0x0360
    13d0:	24 e0       	ldi	r18, 0x04	; 4
    13d2:	88 0f       	add	r24, r24
    13d4:	99 1f       	adc	r25, r25
    13d6:	2a 95       	dec	r18
    13d8:	e1 f7       	brne	.-8      	; 0x13d2 <HubFindNextFreeDeviceId+0x32>
    13da:	0e 94 2a 09 	call	0x1254	; 0x1254 <FramReadByte>
    13de:	80 72       	andi	r24, 0x20	; 32
    13e0:	41 f0       	breq	.+16     	; 0x13f2 <HubFindNextFreeDeviceId+0x52>
    13e2:	21 97       	sbiw	r28, 0x01	; 1
    13e4:	09 f7       	brne	.-62     	; 0x13a8 <HubFindNextFreeDeviceId+0x8>
    13e6:	8f ef       	ldi	r24, 0xFF	; 255
    13e8:	9f ef       	ldi	r25, 0xFF	; 255
    13ea:	90 93 60 03 	sts	0x0360, r25
    13ee:	80 93 5f 03 	sts	0x035F, r24
    13f2:	df 91       	pop	r29
    13f4:	cf 91       	pop	r28
    13f6:	08 95       	ret
*/
//--------------------------------------------------------------------------
//
//  Function:    ResetDeviceIdTable
//
//  Description: Clear the first byte in each page of the FRAM and reset
//               gu16NextFreeDeviceId to 0
//
//--------------------------------------------------------------------------
void ResetDeviceIdTable(void)
{
    U16 DeviceId;
    
    for (DeviceId = 0; DeviceId < (FRAM_LENGTH >> 4); ++DeviceId)
    {
        FramWriteByte(DeviceId << 4, 0x00); // Clear status byte of each page
    }

    // Reset the Device ID numbers
    gu16NextFreeDeviceId = 0x0000;
}
/*
00001472 <ResetDeviceIdTable>:
    1472:	cf 93       	push	r28
    1474:	df 93       	push	r29
    1476:	c0 e0       	ldi	r28, 0x00	; 0
    1478:	d0 e0       	ldi	r29, 0x00	; 0
    147a:	ce 01       	movw	r24, r28
    147c:	44 e0       	ldi	r20, 0x04	; 4
    147e:	88 0f       	add	r24, r24
    1480:	99 1f       	adc	r25, r25
    1482:	4a 95       	dec	r20
    1484:	e1 f7       	brne	.-8      	; 0x147e <ResetDeviceIdTable+0xc>
    1486:	60 e0       	ldi	r22, 0x00	; 0
    1488:	0e 94 3a 09 	call	0x1274	; 0x1274 <FramWriteByte>
    148c:	21 96       	adiw	r28, 0x01	; 1
    148e:	c9 31       	cpi	r28, 0x19	; 25
    1490:	d1 05       	cpc	r29, r1
    1492:	99 f7       	brne	.-26     	; 0x147a <ResetDeviceIdTable+0x8>
    1494:	10 92 60 03 	sts	0x0360, r1
    1498:	10 92 5f 03 	sts	0x035F, r1
    149c:	df 91       	pop	r29
    149e:	cf 91       	pop	r28
    14a0:	08 95       	ret

*/

//--------------------------------------------------------------------------
//
//  Function:    HubDeleteSensor
//
//  Description: Will delete the requested Sensor from FRAM
//
//  Inputs:      DeviceId - Number of Device Id to delete or 0xFFFF to delete all
//
//--------------------------------------------------------------------------
U8 rfDeleteSensor(U16 DeviceId)
{
    U16         Page;
    FRAM_STATUS Sensor;
    U8          Temp;

    if (DeviceId == 0xFFFF) // Delete All
    {
        ResetDeviceIdTable();

        return CMD_STATUS_SUCCESS;
    }

    // Delete Single Device
    if (DeviceId >= (FRAM_LENGTH >> 4)) // Bounds Check
    {
        return CMD_STATUS_UNKNOWN_ID;
    }

    Page    = DeviceId << 4;
    Temp    = FramReadByte(Page + offsetof(FRAM_STRUCT, sStatus));
    Sensor  = *(FRAM_STATUS*)&Temp;

    if (Sensor.sFlags.Enabled)
    {
        FramWriteByte(Page, 0x00);
        return CMD_STATUS_SUCCESS;
    }
    else
    {
        return CMD_STATUS_UNKNOWN_ID;
    }
}
/*
  <<<<<<<<<<<<<<<<<<< Vérifié, conforme ! >>>>>>>>>>>>>>>>>>>>>>>
000002f0 <rfDeleteSensor>:
     2f0:	cf 93       	push	r28
     2f2:	df 93       	push	r29
     2f4:	2f ef       	ldi	r18, 0xFF	; 255
     2f6:	8f 3f       	cpi	r24, 0xFF	; 255
     2f8:	92 07       	cpc	r25, r18
     2fa:	19 f4       	brne	.+6      	; 0x302 <rfDeleteSensor+0x12>
     2fc:	0e 94 39 0a 	call	0x1472	; 0x1472 <ResetDeviceIdTable>
     300:	12 c0       	rjmp	.+36     	; 0x326 <rfDeleteSensor+0x36>
     302:	89 31       	cpi	r24, 0x19	; 25
     304:	91 05       	cpc	r25, r1
     306:	88 f4       	brcc	.+34     	; 0x32a <rfDeleteSensor+0x3a>
     308:	ec 01       	movw	r28, r24
     30a:	44 e0       	ldi	r20, 0x04	; 4
     30c:	cc 0f       	add	r28, r28
     30e:	dd 1f       	adc	r29, r29
     310:	4a 95       	dec	r20
     312:	e1 f7       	brne	.-8      	; 0x30c <rfDeleteSensor+0x1c>
     314:	ce 01       	movw	r24, r28
     316:	0e 94 2a 09 	call	0x1254	; 0x1254 <FramReadByte>
     31a:	80 72       	andi	r24, 0x20	; 32
     31c:	31 f0       	breq	.+12     	; 0x32a <rfDeleteSensor+0x3a>
     31e:	ce 01       	movw	r24, r28
     320:	60 e0       	ldi	r22, 0x00	; 0
     322:	0e 94 3a 09 	call	0x1274	; 0x1274 <FramWriteByte>
     326:	80 e0       	ldi	r24, 0x00	; 0
     328:	01 c0       	rjmp	.+2      	; 0x32c <rfDeleteSensor+0x3c>
     32a:	82 e0       	ldi	r24, 0x02	; 2
     32c:	df 91       	pop	r29
     32e:	cf 91       	pop	r28
     330:	08 95       	ret

*/

//--------------------------------------------------------------------------
U16 
rfGetMaximumDeviceId (void) {

  return (FRAM_LENGTH >> 4) - 1;
}

#endif /* HUB defined */
/* ========================================================================== */

