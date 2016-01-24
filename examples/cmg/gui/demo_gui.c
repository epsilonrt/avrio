/**
 * CMG Gui demo
 * @author Christian Merkle. Copyright _ 2oo6 - 2oo8 All rights reserved.
 */
#include "CMG/CMG.h"

/* public variables ========================================================= */
// If these fonts files do not exist, you must create them in the 
// GMGFonts directory. You can do this using  GMGFontConverter !
extern cmg_FontRawData    g_Font_Elephant_7[];
extern cmg_FontRawData    g_Font_Garamond_7[];
extern cmg_FontRawData    g_Font_Pristina_7[];
extern cmg_FontRawData    g_Font_Tahoma_7[];
extern cmg_FontRawData    g_Font_Times_New_Roman_7[];
extern cmg_FontRawData    g_Font_Verdana_7[];

/* private variables ======================================================== */

// pen dash dot dot - 16 (2 bytes)
cmg_u8  dataPenDashDotDot[] = { 0xfc, 0xcc };

// brush - 50% - 8x2 (1 byte)
cmg_u8  dataBrush50[] = { 0xaa, 0x55 };

// brush2 - light gray- 24x6 (3 bytes)
cmg_u8  dataBrushLight[] = { 
  0x92, 0x49, 0x24, 
  0x00, 0x00, 0x00, 
  0x24, 0x92, 0x49, 
  0x00, 0x00, 0x00, 
  0x49, 0x24, 0x92,
  0x00, 0x00, 0x00 };

// HelloWorld - 41x6 (6 bytes)  
cmg_u8    dataHelloWorld[] = { 
  0xae, 0x88, 0x42, 0x93, 0xa3, 0x00, 
  0xa8, 0x88, 0xa2, 0xaa, 0xa2, 0x80, 
  0xee, 0x88, 0xa3, 0xab, 0x22, 0x80, 
  0xa8, 0x88, 0xa3, 0xaa, 0xa2, 0x80, 
  0xae, 0xee, 0x42, 0x92, 0xbb, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


// chars - 8x14 (1 byte)
cmg_u8    dataChar_C[] = { 
  0x00, 0x00, 0x00, 0x3c, 0x66, 
  0xc2, 0xc0, 0xc0, 0xc0, 0xc2, 
  0x66, 0x3c, 0x00, 0x00 };
  
cmg_u8    dataChar_M[] = { 
  0x00, 0x00, 0x00, 0xc6, 0xee, 
  0xfe, 0xd6, 0xc6, 0xc6, 0xc6, 
  0xc6, 0xc6, 0x00, 0x00 };
  
cmg_u8    dataChar_G[] = { 
  0x00, 0x00, 0x00, 0x3c, 0x66, 
  0xc2, 0xc0, 0xc0, 0xde, 0xc6, 
  0x66, 0x3a, 0x00, 0x00 };

cmg_Pen     PenDashDotDot;
cmg_Brush   Brush50;
cmg_Brush   BrushLight;
cmg_Brush   BrushHelloWorld;
cmg_Coord   iScreenSizeX, iScreenSizeY;

cmg_Font Font_Tahoma_7;

/* internal public functions ================================================ */

// -----------------------------------------------------------------------------
void Button( cmg_Coord x1, cmg_Coord y1, cmg_Coord iWidth, cmg_char *strText )
{
    cmg_Coord   iTextOffset;

    CMG_SetROPMode( ROPMODE_COPY );

    // element
    CMG_SetDrawStyle( DRAWSTYLE_BOTH );
    CMG_SetColor( 1 );
    CMG_SetFillColor( 0 );
    CMG_RoundedRectangle( x1, y1, x1 + iWidth, y1 + 14, 5 );

    // text
    iTextOffset = ( iWidth - CMG_TEXT_Measure_GetTextWidth( strText ) ) >> 1;
    CMG_TEXT_Render( x1 + iTextOffset, y1 + 3, strText );
}


// -----------------------------------------------------------------------------
void TextBox( cmg_Coord x1, cmg_Coord y1, cmg_Coord iWidth, cmg_char *strText )
{
    cmg_Coord   iCusorPos;

    CMG_SetROPMode( ROPMODE_COPY );

    // element
    CMG_SetDrawStyle( DRAWSTYLE_BOTH );
    CMG_SetColor( 1 );
    CMG_SetFillColor( 0 );
    CMG_Rectangle( x1, y1, x1 + iWidth, y1 + 15 );

    // text
    CMG_TEXT_Render( x1 + 3, y1 + 4, strText );

    // cursor
    iCusorPos = CMG_TEXT_Measure_GetTextWidth( strText ) + 3;
    CMG_Line( x1 + iCusorPos, y1 + 3, x1 + iCusorPos, y1 + 12 );
}


// -----------------------------------------------------------------------------
void Dialog( cmg_Coord x1, cmg_Coord y1, cmg_Coord x2, cmg_Coord y2, cmg_char *strTitle )
{
    CMG_SetROPMode( ROPMODE_COPY );

    // dialog window
    CMG_SetDrawStyle( DRAWSTYLE_BOTH );
    CMG_SetColor( 1 );
    CMG_SetFillColor( 0 );
    CMG_Rectangle( x1, y1 + 7, x2, y2 ); 

    // titlebar
    CMG_SetDrawStyle( DRAWSTYLE_FILL );
    CMG_SetFillColor( 1 );
    CMG_RoundedRectangle( x1, y1, x2, y1 + 14, 6 ); 
    
    // title text
    CMG_SetROPMode( ROPMODE_XOR );
    CMG_TEXT_Render( x1 + 10, y1 + 3, strTitle );
    CMG_SetROPMode( ROPMODE_COPY );

    // close Button
    CMG_SetFillColor( 0 );
    CMG_Circle( x2 - 7, y1 + 7, 5 );
    CMG_Line( x2 - 9, y1 + 5, x2 - 5, y1 + 9 );
    CMG_Line( x2 - 5, y1 + 5, x2 - 9, y1 + 9 );


    // text
    CMG_TEXT_Render( x1 + 10, y1 + 25, "Please enter some text:" );

    // textbox
    TextBox( x1 + 10, y1 + 40, x2 - x1 - 20, "What do you think about this one?" );

    // buttons
    Button( x1 + 10, y2 - 25, 60, "Cool" );
    Button( x2 - 90, y2 - 25, 80, "Awesome!" );

}

// -----------------------------------------------------------------------------
void Test( void )
{
    // desktop background
    CMG_SetROPMode( ROPMODE_COPY );
    CMG_SetDrawStyle( DRAWSTYLE_FILL );
    CMG_SetBrush( &BrushLight );
    CMG_Rectangle( 0, 0, iScreenSizeX - 1, iScreenSizeY - 1 ); 

    Dialog( 0, 0, 110, 50, "CMG" );
    Dialog( 10, 10, 80, 40, "Hello" );
}



// -----------------------------------------------------------------------------
int main( void )
{
    cmg_Result	iResult;

    iResult = CMG_TEXT_DrawAndTextInit();
    if ( iResult != CMG_OK )
    {
        //printf( " *** ERROR 0x%02x ***\n\n", iResult );
        return -1;
    }
    CMG_SetContrast (184);

    CMG_Sleep_ms( 500 );

    CMG_GetScreenSize( &iScreenSizeX, &iScreenSizeY );

    CMG_CreatePen( &PenDashDotDot, dataPenDashDotDot, 2 );

    CMG_CreateBrush( &Brush50, dataBrush50, 1, 2 );
    CMG_CreateBrush( &BrushLight, dataBrushLight, 3, 6 );
    CMG_CreateBrush( &BrushHelloWorld, dataHelloWorld, 6, 6 );


    CMG_TEXT_Font_Create( &Font_Tahoma_7, g_Font_Tahoma_7, 1 );
    CMG_TEXT_Font_Set( &Font_Tahoma_7 );

    Test();

    while ( 1 );

    CMG_TEXT_Exit();
    return 0;
}
/* ========================================================================== */


