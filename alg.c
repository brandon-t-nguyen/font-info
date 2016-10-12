#include "alg.h"
#include "bimage.h"
#include "bconv.h"

int alg_calculate( B_Image image, B_Conv convs[], int numConvs )
{
    int width = B_Image_getWidth( image );
    int height = B_Image_getHeight( image );
    int area = width * height;
    int score = 0;
    for( int r = 0; r < height; r++ )
    {
        for( int c = 0; c < width; c++ )
        {
            for( int i = 0; i < numConvs; i++ )
            {
                score += B_Conv_convolvePixel( convs[i], image, r, c );
            }
        }
    }
    return score*1000/area;
}

/////// Area Used    /////////
void alg_areaUsedInit(void)
{
}
void alg_areaUsedDone(void)
{
}
int alg_calculateAreaUsed( B_Image image )
{
    int width = B_Image_getWidth( image );
    int height = B_Image_getHeight( image );
    int area = width * height;
    int calc = 0;
    for( int r = 0; r < height; r++ )
    {
        for( int c = 0; c < width; c++ )
        {
            if( B_Image_getPixel( image, c, r ) > 128 )
                calc++;
        }
    }
    int fraction = calc * 10000 / area;
    return fraction;
}

/////// Straightness /////////
static int straight[] = { 1, 1, 1, 1, 1 };
B_Conv straight_vert;
B_Conv straight_horz;
B_Conv straight_convs[2];
void alg_straightnessInit(void)
{
    straight_convs[0] = B_Conv_new( straight, 1, 5, 1 );
    straight_convs[1] = B_Conv_new( straight, 1, 1, 5 );
}
void alg_straightnessDone(void)
{
    B_Conv_delete( straight_convs[0] );
    B_Conv_delete( straight_convs[1] );
}
int alg_calculateStraightness( B_Image image )
{
    return alg_calculate( image, straight_convs, 2 );
}

///////   Curvature  /////////
static int curve_tl[] = {
                           0, 0, 0, 0, 2,
                           0, 0, 1, 2,-2,
                           0, 0, 2, 1,-2,
                           0, 0, 2,-2,-2,
                           0, 0, 2,-2,-2,
                        };
static int curve_tr[] = {
                           2, 0, 0, 0, 0,
                          -2, 2, 1, 0, 0,
                          -2, 1, 2, 0, 0,
                          -2,-2, 2, 0, 0,
                          -2,-2, 2, 0, 0,
                        };
static int curve_br[] = {
                          -2,-2, 2, 0, 0,
                          -2,-2, 2, 0, 0,
                          -2, 1, 2, 0, 0,
                          -2, 2, 1, 0, 0,
                           2, 0, 0, 0, 0,
                        };
static int curve_bl[] = {
                           0, 0, 2,-2,-2,
                           0, 0, 2,-2,-2,
                           0, 0, 2, 1,-2,
                           0, 0, 1, 2,-2,
                           0, 0, 0, 0, 2
                        };

B_Conv curve_convs[4];
void alg_curvatureInit(void)
{
    curve_convs[0] = B_Conv_new( curve_tl, 1, 5, 5 );
    curve_convs[1] = B_Conv_new( curve_tr, 1, 5, 5 );
    curve_convs[2] = B_Conv_new( curve_bl, 1, 5, 5 );
    curve_convs[3] = B_Conv_new( curve_br, 1, 5, 5 );
}
void alg_curvatureDone(void)
{
    B_Conv_delete( curve_convs[0] );
    B_Conv_delete( curve_convs[1] );
    B_Conv_delete( curve_convs[2] );
    B_Conv_delete( curve_convs[3] );
}

int alg_calculateCurvature( B_Image image )
{
    return alg_calculate( image, curve_convs, 4 );
}
//////////////////////////////

void alg_init(void)
{
    alg_curvatureInit();
    alg_straightnessInit();
    alg_areaUsedInit();
}

void alg_done(void)
{
    alg_curvatureDone();
    alg_straightnessDone();
    alg_areaUsedDone();
}


