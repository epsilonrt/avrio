/**
 * @file demo_draw.c
 * @brief CMG DRAW Demo
 * @author Christian Merkle. Copyright _ 2oo6 - 2oo8 All rights reserved.
 * @author Copyright _ 2011-2012 epsilonRT. All rights reserved.
 * @copyright GNU Lesser General Public License version 3
 *            <http://www.gnu.org/licenses/lgpl.html>
 * Revision History ------------------------------------------------------------
 *    20120307 - Initial version by epsilonRT
 */
#include "CMG/CMG.h"

/* public variables ========================================================= */

// pen dash dot dot - 16 (2 bytes)
cmg_u8  dataPenDashDotDot[] = { 0xfc, 0xcc };

// brush - 50% - 8x2 (1 byte)
cmg_u8  dataBrush50[] = { 0xaa, 0x55 };

// brush2 - light gray- 24x6 (3 bytes)
cmg_u8  dataBrushLight[] = { 0x92, 0x49, 0x24, 0x00, 0x00, 0x00, 0x24, 0x92, 0x49, 0x00, 0x00, 0x00, 0x49, 0x24, 0x92, 0x00, 0x00, 0x00 };

// HelloWorld - 41x6 (6 bytes)  
cmg_u8    dataHelloWorld[] = { 0xae, 0x88, 0x42, 0x93, 0xa3, 0x00, 0xa8, 0x88, 0xa2, 0xaa, 0xa2, 0x80, 0xee, 0x88, 0xa3, 0xab, 0x22, 0x80, 0xa8, 0x88, 0xa3, 0xaa, 0xa2, 0x80, 0xae, 0xee, 0x42, 0x92, 0xbb, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

// chars - 8x14 (1 byte)
cmg_u8    dataChar_C[] = { 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xc0, 0xc2, 0x66, 0x3c, 0x00, 0x00 };
cmg_u8    dataChar_M[] = { 0x00, 0x00, 0x00, 0xc6, 0xee, 0xfe, 0xd6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x00, 0x00 };
cmg_u8    dataChar_G[] = { 0x00, 0x00, 0x00, 0x3c, 0x66, 0xc2, 0xc0, 0xc0, 0xde, 0xc6, 0x66, 0x3a, 0x00, 0x00 };


cmg_Pen     PenDashDotDot;

cmg_Brush   Brush50;
cmg_Brush   BrushLight;
cmg_Brush   BrushHelloWorld;


/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void SpoolDemo( void )
{
    CMG_SetPen( &PenDashDotDot );
    CMG_Line( 0, 0, 100, 0 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 0.9f );
    CMG_Line( 0, 5, 100, 5 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 0.8f );
    CMG_Line( 0, 10, 100, 10 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 0.7f );
    CMG_Line( 0, 15, 100, 15 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 0.6f );
    CMG_Line( 0, 20, 100, 20 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 0.5f );
    CMG_Line( 0, 25, 100, 25 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 0.4f );
    CMG_Line( 0, 30, 100, 30 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 0.3f );
    CMG_Line( 0, 35, 100, 35 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 0.2f );
    CMG_Line( 0, 40, 100, 40 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 0.1f );
    CMG_Line( 0, 45, 100, 45 );


    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 1.1f );
    CMG_Line( 130, 0, 230, 0 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 1.2f );
    CMG_Line( 130, 5, 230, 5 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 1.3f );
    CMG_Line( 130, 10, 230, 10 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 1.4f );
    CMG_Line( 130, 15, 230, 15 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 1.5f );
    CMG_Line( 130, 20, 230, 20 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 1.6f );
    CMG_Line( 130, 25, 230, 25 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 1.7f );
    CMG_Line( 130, 30, 230, 30 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 1.8f );
    CMG_Line( 130, 35, 230, 35 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 1.9f );
    CMG_Line( 130, 40, 230, 40 );
    PenDashDotDot.sfpPenScaleFactor = FLOAT_TO_SFP( 2.0f );
    CMG_Line( 130, 45, 230, 45 );
}



// -----------------------------------------------------------------------------
int main( void )
{
    cmg_Result	iResult;
    cmg_Coord   iScreenSizeX, iScreenSizeY;

    iResult = CMG_DRAW_Init();
    if ( iResult != CMG_OK )
    {
        //printf( " *** ERROR 0x%02x ***\n\n", iResult );
        return -1;
    }
    CMG_SetContrast (184);
    CMG_GetScreenSize( &iScreenSizeX, &iScreenSizeY );


    CMG_CreatePen( &PenDashDotDot, dataPenDashDotDot, 2 );

    CMG_CreateBrush( &Brush50, dataBrush50, 1, 2 );
    CMG_CreateBrush( &BrushLight, dataBrushLight, 3, 6 );
    CMG_CreateBrush( &BrushHelloWorld, dataHelloWorld, 6, 6 );



    CMG_SetColor( 1 );
    CMG_SetFillColor( 1 );

    CMG_SetPen( &PenDashDotDot );

    CMG_SetBrush( &Brush50 );
    CMG_SetBrush( &BrushLight );
    CMG_SetBrush( &BrushHelloWorld );

    CMG_SetDrawStyle( DRAWSTYLE_BORDER );
    CMG_SetDrawStyle( DRAWSTYLE_FILL );
    CMG_SetDrawStyle( DRAWSTYLE_BOTH );

    SpoolDemo();

    

    while ( 1 );

    CMG_DRAW_Exit();
    return 0;
}

/* ========================================================================== */
