#include <stdlib.h>
#include "alg.h"
#include "bimage.h"
#include "bmask.h"

struct Algorithm_Rec_str
{
    B_Mask edge;
    B_Mask slant[2];
    B_Mask curve[4];
};

#define DIM_NORM 100.0

static double Alg_calculate( const B_Image image, B_Mask masks[], int numConvs )
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
    area = area>0?area:1;
    double finalScore = score/area;

    // normalize
    finalScore = finalScore >= 1.0 ? 1.0 : finalScore;
    return finalScore;
}

// wrapper for doing operations that require entire glyph set
static double Alg_CalcAll( Algorithm alg, const BT_Face face, double (*algorithm)( Algorithm, const B_Image ) )
{
    double scoreTotal = 0.0;
    int num = 0;
    for( int c = FIRST_GLYPH; c <= LAST_GLYPH; c++ )
    {
        const B_Image image = BT_Face_getChar( face, c );
        if( image )
        {
            scoreTotal += algorithm( alg, image );
            ++num;
        }
    }
    double scoreAverage = scoreTotal/num;
    // normalize
    scoreAverage = scoreAverage > 1.0? 1.0 : scoreAverage;
    scoreAverage = scoreAverage < 0.0? 0.0 : scoreAverage;
    return scoreAverage;
}

/////// Dimensional /////
static void Alg_WidthInit( Algorithm alg ){}
static void Alg_WidthDone( Algorithm alg ){}
static double Alg_WidthAlg( Algorithm alg, const B_Image image )
{
    double width = (double)B_Image_getWidth( image );
    // normalize
    return width / DIM_NORM;
}
static double Alg_WidthCalc( Algorithm alg, const BT_Face face )
{
    return Alg_CalcAll( alg, face, &Alg_WidthAlg );
}

static void Alg_HeightInit( Algorithm alg ){}
static void Alg_HeightDone( Algorithm alg ){}
static double Alg_HeightAlg( Algorithm alg, const B_Image image )
{
    double height = (double)B_Image_getHeight( image );
    // normalize
    return height / DIM_NORM;
}
static double Alg_HeightCalc( Algorithm alg, const BT_Face face )
{
    return Alg_CalcAll( alg, face, &Alg_HeightAlg );
}

static void Alg_AspectRatioInit( Algorithm alg ){}
static void Alg_AspectRatioDone( Algorithm alg ){}
static double Alg_AspectRatioAlg( Algorithm alg, const B_Image image )
{
    int widthInt = B_Image_getWidth( image );
    int heightInt = B_Image_getHeight( image );
    double width = (double)widthInt;
    double height = (double)heightInt;
    double ar = 0.0;    // aspect ratio
    if( widthInt == heightInt )
    {
        return ar = 0.5;
    }
    else
    {
        if( heightInt > widthInt )
        {
            ar = 0.5 + width/height;
        }
        else
        {
            ar = 0.5 - height/width;
        }
    }
    return ar;
}
static double Alg_AspectRatioCalc( Algorithm alg, const BT_Face face )
{
    return Alg_CalcAll( alg, face, &Alg_AspectRatioAlg );
}

static void Alg_xHeightInit( Algorithm alg ){}
static void Alg_xHeightDone( Algorithm alg ){}
/*
static double Alg_xHeightAlg( Algorithm alg, const B_Image image )
{
    return 0.0;
}
*/
static double Alg_xHeightCalc( Algorithm alg, const BT_Face face )
{
    B_Image x = BT_Face_getChar(face, 'x');
    double height  = B_Image_getHeight(x);
    return height / DIM_NORM;
}


/////// Density /////////
static void Alg_DensityInit( Algorithm alg ){}
static void Alg_DensityDone( Algorithm alg ){}
static double Alg_DensityAlg( Algorithm alg, B_Image image )
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
                ++calc;
        }
    }
    area = area>0?area:1;
    double density = (double)calc / (double)area;
    return density;
}
static double Alg_DensityCalc( Algorithm alg, const BT_Face face )
{
    return Alg_CalcAll( alg, face, &Alg_DensityAlg );
}

/////// Slant //////
static int slant_l[] = {
                           0,  0,  0,  0,  9,
                           0,  0,  0,  9,  0,
                           0,  0,  9,  0,  0,
                           0,  9,  0,  0,  0,
                           9,  0,  0,  0,  0,
                        };
#define NUM_SLANT_MASKS 2
static void Alg_SlantInit( Algorithm alg )
{
    alg->slant[0] = B_Mask_new( slant_l, 1, 5, 5 );
    alg->slant[1] = B_Mask_rotate( alg->slant[0] );
}
static void Alg_SlantDone( Algorithm alg )
{
    B_Mask_delete( alg->slant[0] );
    B_Mask_delete( alg->slant[1] );
}
static double Alg_SlantAlg( Algorithm alg, const B_Image image )
{
    B_Image edge = B_Mask_convolve( alg->edge, image );
    double score = Alg_calculate( edge, alg->slant, NUM_SLANT_MASKS );
    B_Image_delete(edge);
    return score;
}
static double Alg_SlantCalc( Algorithm alg, const BT_Face face )
{
    return Alg_CalcAll( alg, face, &Alg_SlantAlg );
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
static void Alg_CurveInit( Algorithm alg )
{
    alg->curve[0] = B_Mask_new( curve_tl, 1, 5, 5 );
    alg->curve[1] = B_Mask_rotate( alg->curve[0] );
    alg->curve[2] = B_Mask_rotate( alg->curve[1] );
    alg->curve[3] = B_Mask_rotate( alg->curve[2] );
}
static void Alg_CurveDone( Algorithm alg )
{
    for( int i = 0; i < NUM_CURVE_MASKS; i++ )
    {
        B_Mask_delete( alg->curve[i] );
    }
}
static double Alg_CurveAlg( Algorithm alg, const B_Image image )
{
    // get an edge detected image
    B_Image edge = B_Mask_convolve( alg->edge, image );
    double score = Alg_calculate( edge, alg->curve, NUM_CURVE_MASKS );
    B_Image_delete(edge);
    return score;
}
static double Alg_CurveCalc( Algorithm alg, const BT_Face face )
{
    return Alg_CalcAll( alg, face, &Alg_CurveAlg );
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
    Alg_WidthInit( alg );
    Alg_HeightInit( alg );
    Alg_AspectRatioInit( alg );
    Alg_xHeightInit( alg );
    Alg_DensityInit( alg );
    Alg_CurveInit( alg );
    Alg_SlantInit( alg );
}

static void Alg_done( Algorithm alg )
{
    B_Mask_delete( alg->edge );
    Alg_WidthDone( alg );
    Alg_HeightDone( alg );
    Alg_AspectRatioDone( alg );
    Alg_xHeightDone( alg );
    Alg_DensityDone( alg );
    Alg_CurveDone( alg );
    Alg_SlantDone( alg );
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
    if( algRefCount == 0 && algorithm )
    {
        Alg_releaseInstance();
    }
}
void Alg_releaseInstance(void)
{
    if( !algorithm )
        return;
    Alg_done( algorithm );
    free(algorithm);
    algorithm = NULL;
    algRefCount = 0;
}

double Alg_calculateMetric( Algorithm alg, const BT_Face face, const Metric metric )
{
    switch( metric )
    {
    case Metric_Width:
        return Alg_WidthCalc( alg, face );
        break;
    case Metric_Height:
        return Alg_HeightCalc( alg, face );
        break;
    case Metric_AspectRatio:
        return Alg_AspectRatioCalc( alg, face );
        break;
    case Metric_xHeight:
        return Alg_xHeightCalc( alg, face );
        break;
    case Metric_Density:
        return Alg_DensityCalc( alg, face );
        break;
    case Metric_Curve:
        return Alg_CurveCalc( alg, face );
        break;
    case Metric_Slant:
        return Alg_SlantCalc( alg, face );
        break;
    default:
        fprintf(stderr,"Not a valid metric!\n");
        break;
    }
    return 0.0;
}

void Alg_calculateMetrics( Algorithm alg, const BT_Face face, Metrics * results )
{
    for( int i = 0; i < NUM_METRICS; i++ )
    {
        results->metrics[i] = Alg_calculateMetric( alg, face, i );
    }
}

void Metrics_fprintHeader( FILE * file )
{
    fprintf(file,"Family Name,Style Name,");
    for( int i = 0; i < NUM_METRICS; i++ )
    {
        fprintf(file,"%s,", Alg_strMetric(i));
    }
    fprintf(file,"\n");
}

void Metrics_fprint( FILE * file, const BT_Face face, const Metrics * results )
{
    fprintf(file,"%s,%s,", BT_Face_getFamilyName(face), BT_Face_getStyleName(face));
    for( int i = 0; i < NUM_METRICS; i++ )
    {
        fprintf(file,"%f,", results->metrics[i]);
    }
    fprintf(file,"\n");
}

const char * Alg_strMetric( Metric metric )
{
    switch( metric )
    {
    case Metric_Width:
        return "Width";
        break;
    case Metric_Height:
        return "Height";
        break;
    case Metric_AspectRatio:
        return "Aspect Ratio";
        break;
    case Metric_xHeight:
        return "x-height";
        break;
    case Metric_Density:
        return "Density";
        break;
    case Metric_Curve:
        return "Curve";
        break;
    case Metric_Slant:
        return "Slant";
        break;
    default:
        return "Not a valid metric!";
        break;
    }
    return "";
}
