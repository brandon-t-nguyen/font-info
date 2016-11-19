#include <stdlib.h>
#include <string.h>

#include <bmask.h>

#define B_MASK_INDEX(m,r,c) (c + r*(m->width))
#define B_MASK_CELL(m,r,c) ( m->matrix[B_MASK_INDEX(m,r,c)] )

struct B_Mask_Rec_str
{
    int *matrix;
    int divisor;
    int width;
    int height;
};

enum B_Mask_Op_enum
{
    B_MASK_CONVOLVE,
    B_MASK_CORRELATE
};

B_Mask B_Mask_new( const int matrix[], const int divisor,
                   const int width, const int height )
{
    if( !(width % 2) && !(height % 2) )
    {
        return NULL;
    }
    B_Mask mask = (B_Mask) malloc( sizeof(B_Mask_Rec) );
    mask->width = width;
    mask->height = height;
    mask->matrix = (int *) malloc( sizeof(int) * width * height );
    mask->matrix = (int *) memcpy( mask->matrix, matrix, sizeof(float)*width*height );
    mask->divisor = divisor;
    return mask;
}

void B_Mask_delete( B_Mask mask )
{
    free( mask->matrix );
    free( mask );
}


B_Mask B_Mask_transpose( const B_Mask input )
{
    B_Mask mask = (B_Mask) malloc( sizeof(B_Mask_Rec) );
    mask->width = input->height;
    mask->height = input->width;
    mask->matrix = (int *) malloc( sizeof(int) * mask->width * mask->height );
    mask->divisor = input->divisor;
    // transfer values
    int *iMatrix = input->matrix;
    int *cMatrix = mask->matrix;
    int iWidth = input->width;
    int iHeight = input->height;
    for( int iRow = 0; iRow < iWidth; iRow++ )
    {
        for( int iCol = 0; iCol < iWidth; iCol++ )
        {
            cMatrix[ iCol * iHeight + iRow ] = iMatrix[ iRow * iWidth + iCol];
        }
    }
    return mask;
}

B_Mask B_Mask_rotate( const B_Mask input )
{
    // transpose
    B_Mask mask = B_Mask_transpose( input );
    // for each row
    int *matrix = mask->matrix;
    int height = input->height;
    int width = input->width;
    for( int r = 0; r < height; r++ )
    {
        // reverse each row
        for( int cB = 0, cE = width-1; cB < cE; cB++, cE-- )
        {
            // swap
            int temp = matrix[ r*width + cE ];
            matrix[ r*width + cE ] = matrix[ r*width + cB ];
            matrix[ r*width + cB ] = temp;
        }
    }
    return mask;
}

B_Mask B_Mask_flipHor( const B_Mask input )
{
    // flip H
    B_Mask mask = (B_Mask) malloc( sizeof(B_Mask_Rec) );
    mask->width = input->height;
    mask->height = input->width;
    mask->matrix = (int *) malloc( sizeof(int) * mask->width * mask->height );
    mask->divisor = input->divisor;

    int *matrix = mask->matrix;
    int *matrixIn = input->matrix;
    int height = input->height;
    int width = input->width;
    for( int r = 0; r < height; r++ )
    {
        for( int c = 0; c < width; c++ )
        {
            matrix[ r*width + c ] = matrixIn[ r*width + (width-1-c) ];
        }
    }

    return mask;
}

B_Image B_Mask_mask( const B_Mask conv, const B_Image image,
                     int (*pixelOp)(const B_Mask, const B_Image, const int, const int) )
{
    int width = B_Image_getWidth(image);
    int height = B_Image_getHeight(image);

    // setup the output
    B_Image out = B_Image_new( width, height );

    for( int row = 0; row < height; row++ )
    {
        for( int col = 0; col < width; col++ )
        {
            // for each pixel convolve that shit
            int pixel = pixelOp( conv, image, row, col );
            if( pixel > 255 )
                pixel = 255;
            else if( pixel < 0 )
                pixel = 0;
            B_Image_setPixel( out, col, row, pixel&0xFF );
        }
    }
    return out;
}

int B_Mask_pixel( const B_Mask mask, const B_Image image,
                  const int row, const int col, enum B_Mask_Op_enum op )
{
    int height = mask->height;
    int width = mask->width;

    int midRow = mask->height / 2;

    int iHeight = B_Image_getHeight( image );
    int iWidth = B_Image_getWidth( image );

    int sum = 0;

    int coefSum = 0;

    int colStart;
    int rowStart;
    int inc;
    if( op == B_MASK_CONVOLVE )
    {
        rowStart = height-1;
        colStart = width-1;
        inc = -1;
    }
    else
    {
        rowStart = 0;
        colStart = 0;
        inc = +1;
    }

    for( int cRow = rowStart, iRow = row-midRow, countRow = 0; countRow < height; cRow += inc, iRow++, countRow++ )
    {
        for( int cCol = colStart, iCol = col-midRow, countCol = 0; countCol < width; cCol += inc, iCol++, countCol++ )
        {
            if( 0 <= iRow && iRow < iHeight && 0 <= iCol && iCol < iWidth )
            {
                int cellVal = B_MASK_CELL(mask,cRow,cCol);
                int pixelVal= (B_Image_getPixel( image, iCol, iRow )&0xFF);
                sum += cellVal * pixelVal;
                coefSum += cellVal<0?-cellVal:cellVal;
            }
        }
    }

    sum /= mask->divisor;       // divide by the divisor part
    sum /= coefSum?coefSum:1;   // divide by the coefficients
    return sum;
}

B_Image B_Mask_convolve( const B_Mask conv, const B_Image image )
{
    return B_Mask_mask( conv, image, &B_Mask_convolvePixel);
}


int B_Mask_convolvePixel( const B_Mask mask, const B_Image image,
                          const int row, const int col )
{
    return B_Mask_pixel( mask, image, row, col, B_MASK_CONVOLVE );
}

B_Image B_Mask_correlate( const B_Mask conv, const B_Image image )
{
    return B_Mask_mask( conv, image, &B_Mask_correlatePixel);
}


int B_Mask_correlatePixel( const B_Mask mask, const B_Image image,
                           const int row, const int col )
{
    return B_Mask_pixel( mask, image, row, col, B_MASK_CORRELATE );
}

int B_Mask_getWidth( const B_Mask mask )
{
    return mask->width;
}
int B_Mask_getHeight( const B_Mask mask )
{
    return mask->height;
}
int B_Mask_getDivisor( const B_Mask mask )
{
    return mask->divisor;
}
const int *  B_Mask_getMatrix( const B_Mask mask )
{
    return mask->matrix;
}

