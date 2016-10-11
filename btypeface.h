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
#define DPI WEB_DPI
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

BT_Face BT_Face_new( FT_Error *errorHandle, FT_Library lib, const char * fontFilePath, int pointSize );
void BT_Face_delete( BT_Face face );
FT_Glyph BT_Face_getGlyph( BT_Face face, int charcode );


/**
 * Creates a B_Image from a string
 */
B_Image BT_Face_renderString( BT_Face face, const char * string );

/**
 * Creates a B_Image from a glyph
 */
B_Image BT_Face_renderGlyph( const FT_Glyph glyph );

/**
 * Creates a B_Image from a char
 */
B_Image BT_Face_renderChar( BT_Face face, const int code );

/**
 *
 */
void BT_Print_Bitmap( FT_Bitmap *bitmap );
void BT_Print_Glyph( FT_Glyph glyph);

#endif //__BTYPEFACE_H__
