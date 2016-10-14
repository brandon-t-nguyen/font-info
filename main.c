#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "btypeface.h"
#include "bimage.h"
#include "bmask.h"
#include "alg.h"

const int id[] = { 0,0,0,
                   0,1,0,
                   0,0,0 };

const int sharp[] = { 0,-1,0,
                      -1,5,-1,
                      0,-1,0 };

const int edge[] = { 0, 1, 0,
                     1,-4, 1,
                     0, 1, 0 };

                    //  0,  0,  0,  0,  0,
const int curve[] = {
                        0, -4, -4,  8,  8,
                        0, -4,  4,  8, -4,
                        0, -4,  8,  4, -4,
                        0, -4,  8, -4, -4,
                        0, -4,  8, -4, -4,
                    };

void algTest(BT_Face face)
{
    alg_init();
    int cscore = 0;
    int sscore = 0;
    int ascore = 0;
    for( int c = '!'; c <= '~'; c++ )
    {
        B_Image character = BT_Face_renderChar( face, c );
        cscore += alg_calculateCurvature(character);
        sscore += alg_calculateStraightness(character);
        ascore += alg_calculateAreaUsed(character);
        B_Image_delete( character );
    }
    cscore /= ('~'-'!'+1);   // average
    sscore /= ('~'-'!'+1);   // average
    ascore /= ('~'-'!'+1);   // average
    printf("Curvature score: %d\n", cscore);
    printf("Straightness score: %d\n", sscore);
    printf("Area used score: %d\n", ascore);

    alg_done();
}

void convolveTest(BT_Face face)
{
    B_Mask mask = B_Mask_new( curve, 1, 5, 5 );
    B_Image image = BT_Face_renderChar( face, 'a' );
    printf("Before convolution:\n");
    B_Image_fprint( image, stdout );

    B_Image convImage = B_Mask_convolve( mask , image );
    printf("After convolution:\n");
    B_Image_fprint( convImage, stdout );


    B_Image_delete( image );
    B_Image_delete( convImage );
}

void curveTest(BT_Face face)
{
    alg_init();
    B_Image image = BT_Face_renderChar( face, 'A' );
    alg_calculateCurvature(image);
    B_Image_delete( image );
    alg_done();
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

    BT_Face face = BT_Face_new( &error, library, fontFilePath, 12);

    //convolveTest( face );
    algTest( face );
    //curveTest( face );

    // cleanup
    BT_Face_delete( face );
    FT_Done_FreeType( library );
}

