#include <stdio.h>
#include <string.h>
#include <ft2build.h>
#include FT_FREETYPE_H

#include <btypeface.h>
#include <bimage.h>
#include <bmask.h>
#include <alg.h>

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

    if( argc == 4 )
    {
        if( !strcmp(argv[1],"-m") )
        {
            int exitCode = 0;
            Metric metric = Metric_fromString(argv[2]);
            if( metric != Metric_Error )
            {
                BT_Face face = BT_Face_new( &error, argv[3], 12);
                if( error != BT_Err_Ok )
                {
                    fprintf(stderr, "Sorry, couldn't load face %s %s!\n", BT_Face_getFamilyName(face), BT_Face_getStyleName(face));
                    exitCode = 1;
                }
                else
                {
                    // do the algorithmic stuff
                    double metricScore = Alg_calculateMetric( alg, face, metric );
                    printf( "%f\n", metricScore );
                }
                BT_Face_delete( face );
            }
            else
            {
                fprintf(stderr,"Invalid metric: %s\n",argv[2]);
                exitCode = 2;
            }
            Alg_doneInstance(alg);
            return exitCode;
        }
    }

    Metrics_fprintHeader( stdout );
    for(int i = 1; i < argc; i++)
    {
        char *fontFilePath = argv[i];

        BT_Face face = BT_Face_new( &error, fontFilePath, 12);
        if( error != BT_Err_Ok )
        {
            fprintf(stderr, "Sorry, couldn't load face %s %s!\n", BT_Face_getFamilyName(face), BT_Face_getStyleName(face));
            BT_Face_delete( face );
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

