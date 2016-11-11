#ifndef __BTYPEFACE_H__
#define __BTYPEFACE_H__

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "bimage.h"

/**
 * This is a set of abstractions for accessing some things from FreeType2
 *
 */
#define WEB_DPI 72
#define PRINT_DPI 300
#define DPI PRINT_DPI
#define point(x) (x*64)
//#define grayToAscii(x) (x==0?' ':(x<128?'.':'*'))
#define grayToAscii(x) (x==0?' ':(x<64?'.':(x<128?'*':'#')))

// enough for the printable ASCII characters
// from ! to ~
#define FIRST_GLYPH ' '
#define LAST_GLYPH '~'
#define NUM_GLYPHS (LAST_GLYPH - FIRST_GLYPH + 1)


typedef struct BT_Face_Rec_str BT_Face_Rec;
typedef BT_Face_Rec * BT_Face;

typedef enum BT_Err_enum
{
    BT_Err_Ok,
    BT_Err_Unknown_File_Format,
    BT_Err_Set_Char_Size,
    BT_Err_Load_Char,
    BT_Err_Cache_Char,
    BT_Err_Render_Char,
    BT_Err_Unknown_Error
} BT_Error;

BT_Face BT_Face_new( BT_Error *errorHandle, const char * fontFilePath, int pointSize );
void BT_Face_delete( BT_Face face );

const char * BT_Face_getFamilyName( const BT_Face face );
const char * BT_Face_getStyleName( const BT_Face face );

/**
 * Returns a B_Image from a char
 */
const B_Image BT_Face_getChar( const BT_Face face, const int code );

/**
 * Returns a B_Image from a char, edge detected
 */
const B_Image BT_Face_getCharEdge( const BT_Face face, const int code );

#endif //__BTYPEFACE_H__
