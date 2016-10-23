#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "btypeface.h"
#include "bimage.h"
#include "bmask.h"
#include "alg.h"

void curveTest(BT_Face face)
{
    Algorithm alg = Alg_getInstance();
    B_Image image = BT_Face_getChar( face, 'A' );
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
            B_Image character = BT_Face_getChar( face, c );
            cscore += Alg_calculateCurvature(alg, character);
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
}

