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
                int toAdd = B_Conv_convolvePixel( convs[i], image, r, c );
                score += toAdd<0?0:toAdd;
            }
        }
    }
    /*
    B_Image_fprint(image,stdout);
    for( int i = 0; i < numConvs; i++)
    {
        B_Image lol = B_Conv_convolve( convs[i], image );
        B_Image_fprint(lol,stdout);
        B_Image_delete(lol);
    }
    */
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
static int curve_edge_mat[] = {
                               -1, -1, -1,
                               -1,  8, -1,
                               -1, -1, -1,
                          };
static int curve_tl[] = {
                          -9, -9, -9,  1,  1,
                          -9, -9,  3,  7,  7,
                          -9,  3,  7,  3, -9,
                           1,  7,  3, -9, -9,
                           1,  7, -9, -9, -9,
                        };
/*
static int curve_tl_inv[] = {
                          -9, -9, -9,  1,  1,
                          -9, -9,  3,  7,  7,
                          -9,  3,  7,  3,  0,
                           1,  7,  3,  0,  0,
                           1,  7,  0,  0,  0,
                        };
*/
#define NUM_CURVE_CONVS 4
B_Conv curve_edge;
B_Conv curve_convs[NUM_CURVE_CONVS];
void alg_curvatureInit(void)
{
    curve_edge = B_Conv_new( curve_edge_mat, 1, 3, 3 );
    curve_convs[0] = B_Conv_new( curve_tl, 1, 5, 5 );
    curve_convs[1] = B_Conv_rotate( curve_convs[0] );
    curve_convs[2] = B_Conv_rotate( curve_convs[1] );
    curve_convs[3] = B_Conv_rotate( curve_convs[2] );
    /*
    curve_convs[4] = B_Conv_new( curve_tl_inv, 1, 5, 5 );
    curve_convs[5] = B_Conv_rotate( curve_convs[4] );
    curve_convs[6] = B_Conv_rotate( curve_convs[5] );
    curve_convs[7] = B_Conv_rotate( curve_convs[6] );
    */
}
void alg_curvatureDone(void)
{
    B_Conv_delete( curve_edge );
    for( int i = 0; i < NUM_CURVE_CONVS; i++ )
    {
        B_Conv_delete( curve_convs[i] );
    }
}

int alg_calculateCurvature( B_Image image )
{
    // EDGE DETECT FIRST!!!!!!
    B_Image edge = B_Conv_convolve( curve_edge, image );
    int score = alg_calculate( edge, curve_convs, NUM_CURVE_CONVS );
    B_Image_delete(edge);
    return score;
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


