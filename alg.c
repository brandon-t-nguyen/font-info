#include <stdlib.h>
#include "alg.h"
#include "bimage.h"
#include "bmask.h"

struct Algorithm_Rec_str
{
    B_Mask edge;
    B_Mask curve[4];
    B_Mask straight[2];
};

static int Alg_calculate( B_Image image, B_Mask masks[], int numConvs )
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
    area = area>0?area:1;
    return score*1000/area;
}

/////// Area Used    /////////
static void Alg_areaUsedInit( Algorithm alg )
{
}
static void Alg_areaUsedDone( Algorithm alg )
{
}
int Alg_calculateAreaUsed( Algorithm alg, B_Image image )
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
    area = area>0?area:1;
    int fraction = calc * 10000 / area;
    return fraction;
}

/////// Straightness /////////
static int straight[] = { 1, 1, 1, 1, 1 };
void Alg_straightnessInit( Algorithm alg )
{
    alg->straight[0] = B_Mask_new( straight, 1, 5, 1 );
    alg->straight[1] = B_Mask_new( straight, 1, 1, 5 );
}
void Alg_straightnessDone( Algorithm alg )
{
    B_Mask_delete( alg->straight[0] );
    B_Mask_delete( alg->straight[1] );
}
int Alg_calculateStraightness( Algorithm alg, B_Image image )
{
    return Alg_calculate( image, alg->straight, 2 );
}

///////   Curvature  /////////

static int curve_tl[] = {
                          -9, -9, -9,  1,  1,
                          -9, -9,  3,  7,  7,
                          -9,  3,  7,  3, -9,
                           1,  7,  3, -9, -9,
                           1,  7, -9, -9, -9,
                        };
#define NUM_CURVE_MASKS 4
static void Alg_curvatureInit( Algorithm alg )
{
    alg->curve[0] = B_Mask_new( curve_tl, 1, 5, 5 );
    alg->curve[1] = B_Mask_rotate( alg->curve[0] );
    alg->curve[2] = B_Mask_rotate( alg->curve[1] );
    alg->curve[3] = B_Mask_rotate( alg->curve[2] );
}
static void Alg_curvatureDone( Algorithm alg )
{
    for( int i = 0; i < NUM_CURVE_MASKS; i++ )
    {
        B_Mask_delete( alg->curve[i] );
    }
}

int Alg_calculateCurvature( Algorithm alg, B_Image image )
{
    B_Image edge = B_Mask_convolve( alg->edge, image );
    int score = Alg_calculate( edge, alg->curve, NUM_CURVE_MASKS );
    B_Image_delete(edge);
    return score;
}
//////////////////////////////

static Algorithm algorithm = NULL;
static int algRefCount = 0;

static int edge_mat[] = {
                             -1, -1, -1,
                             -1,  8, -1,
                             -1, -1, -1,
                        };


static void Alg_init( Algorithm alg )
{
    alg->edge = B_Mask_new( edge_mat, 1, 3, 3 );
    Alg_curvatureInit( alg );
    Alg_straightnessInit( alg );
    Alg_areaUsedInit( alg );
}

static void Alg_done( Algorithm alg )
{
    B_Mask_delete( alg->edge );
    Alg_curvatureDone( alg );
    Alg_straightnessDone( alg );
    Alg_areaUsedDone( alg );
}

Algorithm Alg_getInstance(void)
{
    if( !algorithm )
    {
        // new instance!
        algRefCount = 0;
        algorithm = malloc(sizeof(Algorithm_Rec));
        Alg_init( algorithm );
    }
    ++algRefCount;
    return algorithm;
}

void Alg_doneInstance( Algorithm alg )
{
    // Check if correct pointer
    if( alg != algorithm )
        return;

    if( algRefCount > 0 )
    {
        --algRefCount;
    }

    // delete if no more references
    if( algRefCount == 0 && !algorithm )
    {
        Alg_done( algorithm );
        algorithm = NULL;
        algRefCount = 0;
    }
}
