#include <stdio.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "btypeface.h"
#include "bimage.h"
#include "bmask.h"
#include "alg.h"

extern void visualize( BT_Face face1, BT_Face face2, int charcode );
int main(int argc, char *argv[])
{
    BT_Error error;
    #ifdef VISUALIZE
    BT_Face face1 = BT_Face_new( &error, argv[1], 12);
    BT_Face face2 = BT_Face_new( &error, argv[2], 12);
    visualize( face1, face2, argv[3][0] );
    BT_Face_delete(face1);
    BT_Face_delete(face2);
    #else
    Algorithm alg = Alg_getInstance();
    Metrics_fprintHeader( stdout );
    for(int i = 1; i < argc; i++)
    {
        char *fontFilePath = argv[i];
        char *name = fontFilePath + strlen(fontFilePath)-1;
        while( *name != '/' && name >= fontFilePath)
        {
            --name;
        }
        name++;

        BT_Face face = BT_Face_new( &error, fontFilePath, 12);
        if( !face )
        {
            fprintf(stderr, "What the fuck, couldn't make it!\n");
            continue;
        }
        // do the algorithmic stuff
        Metrics metrics;
        Alg_calculateMetrics( alg, face, &metrics );

        Metrics_fprint( stdout, face, &metrics );

        BT_Face_delete( face );
    }

    Alg_doneInstance(alg);
    #endif
}

