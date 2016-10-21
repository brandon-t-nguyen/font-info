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
    Algorithm alg = Alg_getInstance();
    int cscore = 0;
    int sscore = 0;
    int ascore = 0;
    for( int c = '!'; c <= '~'; c++ )
    {
        B_Image character = BT_Face_renderChar( face, c );
        cscore += Alg_calculateCurvature(alg, character);
        sscore += Alg_calculateStraightness(alg, character);
        ascore += Alg_calculateAreaUsed(alg, character);
        B_Image_delete( character );
    }
    cscore /= ('~'-'!'+1);   // average
    sscore /= ('~'-'!'+1);   // average
    ascore /= ('~'-'!'+1);   // average
    printf("Curvature score: %d\n", cscore);
    printf("Straightness score: %d\n", sscore);
    printf("Area used score: %d\n", ascore);

    Alg_doneInstance(alg);
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
    Algorithm alg = Alg_getInstance();
    B_Image image = BT_Face_renderChar( face, 'A' );
    Alg_calculateCurvature(alg, image);
    B_Image_delete( image );
    Alg_doneInstance(alg);
}

void doAll(int argc, char *argv[])
{
    Algorithm alg = Alg_getInstance();
    for(int i = 1; i < argc; i++)
    {
        char *fontFilePath = argv[i];
        char *name = fontFilePath + strlen(fontFilePath)-1;
        while( *name != '/' && name >= fontFilePath)
        {
            --name;
        }
        name++;

        BT_Error error;
        BT_Face face = BT_Face_new( &error, fontFilePath, 12);
        if( !face )
        {
            fprintf(stderr, "What the fuck, couldn't make it!\n");
            continue;
        }
        int cscore = 0;
        for( int c = '!'; c <= '~'; c++ )
        {
            B_Image character = BT_Face_renderChar( face, c );
            cscore += Alg_calculateCurvature(alg, character);
            B_Image_delete( character );
        }
        cscore /= ('~'-'!'+1);   // average
        printf("%s,%d\n",name,cscore);

        BT_Face_delete( face );
    }
    Alg_doneInstance(alg);
}

int main(int argc, char *argv[])
{
    doAll(argc, argv);
    /*
    printf("Hello, freetype!\n");

    if( !(argc >= 2) )
    {
        fprintf(stderr,"Not enough arguments (%d provided)\n",argc);
        exit(1);
    }
    const char *fontFilePath = argv[1];

    BT_Error error;
    BT_Face face = BT_Face_new( &error, fontFilePath, 12);

    //convolveTest( face );
    algTest( face );
    //curveTest( face );

    // cleanup
    BT_Face_delete( face );
    */
}

