#include "alg.h"
#include "bimage.h"
#include "bmask.h"

int alg_calculate( B_Image image, B_Mask masks[], int numConvs )
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
                int toAdd = B_Mask_convolvePixel( masks[i], image, r, c );
                score += toAdd<0?0:toAdd;
            }
        }
    }
    /*
    B_Image_fprint(image,stdout);
    for( int i = 0; i < numConvs; i++)
    {
        B_Image lol = B_Mask_convolve( masks[i], image );
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
B_Mask straight_vert;
B_Mask straight_horz;
B_Mask straight_masks[2];
void alg_straightnessInit(void)
{
    straight_masks[0] = B_Mask_new( straight, 1, 5, 1 );
    straight_masks[1] = B_Mask_new( straight, 1, 1, 5 );
}
void alg_straightnessDone(void)
{
    B_Mask_delete( straight_masks[0] );
    B_Mask_delete( straight_masks[1] );
}
int alg_calculateStraightness( B_Image image )
{
    return alg_calculate( image, straight_masks, 2 );
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
#define NUM_CURVE_MASKS 4
B_Mask curve_edge;
B_Mask curve_masks[NUM_CURVE_MASKS];
void alg_curvatureInit(void)
{
    curve_edge = B_Mask_new( curve_edge_mat, 1, 3, 3 );
    curve_masks[0] = B_Mask_new( curve_tl, 1, 5, 5 );
    curve_masks[1] = B_Mask_rotate( curve_masks[0] );
    curve_masks[2] = B_Mask_rotate( curve_masks[1] );
    curve_masks[3] = B_Mask_rotate( curve_masks[2] );
    /*
    curve_masks[4] = B_Mask_new( curve_tl_inv, 1, 5, 5 );
    curve_masks[5] = B_Mask_rotate( curve_masks[4] );
    curve_masks[6] = B_Mask_rotate( curve_masks[5] );
    curve_masks[7] = B_Mask_rotate( curve_masks[6] );
    */
}
void alg_curvatureDone(void)
{
    B_Mask_delete( curve_edge );
    for( int i = 0; i < NUM_CURVE_MASKS; i++ )
    {
        B_Mask_delete( curve_masks[i] );
    }
}

int alg_calculateCurvature( B_Image image )
{
    // EDGE DETECT FIRST!!!!!!
    B_Image edge = B_Mask_convolve( curve_edge, image );
    int score = alg_calculate( edge, curve_masks, NUM_CURVE_MASKS );
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


