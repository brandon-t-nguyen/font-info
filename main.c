#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "btypeface.h"
#include "bimage.h"
#include "bconv.h"

const int id[] = { 0,0,0,
                   0,1,0,
                   0,0,0 };

void convolveTest(BT_Face face)
{
    B_Conv conv = B_Conv_new( id, 3, 3, 1 );
    B_Image image = BT_Face_renderChar( face, 'A' );
    printf("Before convolution:\n");
    B_Image_fprint( image, stdout );

    B_Image convImage = B_Conv_convolve( conv, image );
    printf("After identity convolution:\n");
    B_Image_fprint( convImage, stdout );


    B_Image_delete( image );
    B_Image_delete( convImage );
}

int main(int argc, char *argv[])
{
    FT_Error error;

    printf("Hello, freetype!\n");

    if( !(argc >= 2) )
    {
        fprintf(stderr,"Not enough arguments (%d provided)\n",argc);
        exit(1);
    }
    const char *fontFilePath = argv[1];

    FT_Library library;
    error = FT_Init_FreeType( &library );
    if( error )
    {
        fprintf(stderr,"Problem occurred during freetype library initialization");
        exit(1);
    }
    //BT_Print_Bitmap( &face->glyph->bitmap );

    BT_Face face = BT_Face_new( &error, library, fontFilePath, 12);
    /*
    for(int c = '!'; c <= '~'; c++)
    {
        //FT_Glyph glyph = BT_Face_getGlyph( face, c );
        //BT_Print_Bitmap( &((FT_BitmapGlyph)glyph)->bitmap );
        BT_Print_Glyph( BT_Face_getGlyph( face, c) );
        putchar('\n');
    }
    */
    B_Image image = BT_Face_renderString( face, "Hello, team!" );
    B_Image_fprint( image, stdout );
    B_Image_delete( image );


    convolveTest( face );

    BT_Face_delete( face );
    // cleanup
    FT_Done_FreeType( library );
}

