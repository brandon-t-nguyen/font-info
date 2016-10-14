#include <stdlib.h>
#include <string.h>
#include "bmask.h"

#define B_MASK_INDEX(m,r,c) (c + r*(m->width))
#define B_MASK_CELL(m,r,c) ( m->matrix[B_MASK_INDEX(m,r,c)] )

struct B_Mask_Rec_str
{
    int *matrix;
    int divisor;
    int width;
    int height;
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
    mask->matrix = memcpy( mask->matrix, matrix, sizeof(float)*width*height );
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

B_Image B_Mask_convolve( B_Mask conv, const B_Image image )
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
            int pixel = B_Mask_convolvePixel( conv, image, row, col );
            if( pixel > 255 )
                pixel = 255;
            else if( pixel < 0 )
                pixel = 0;
            B_Image_setPixel( out, col, row, pixel&0xFF );
        }
    }
    return out;
}


int B_Mask_convolvePixel( B_Mask mask, const B_Image image,
                          const int row, const int col )
{
    int height = mask->height;
    int width = mask->width;

    int midRow = mask->height / 2;

    int iHeight = B_Image_getHeight( image );
    int iWidth = B_Image_getWidth( image );

    int sum = 0;

    int srcRow = 0; // source row to get image pixel
    int srcCol = 0; // source col to get image pixel
    int coefSum = 0;

    for( int cRow = 0, iRow = row-midRow; cRow < height; cRow++, iRow++ )
    {
        // extend the borders
        if( iRow < 0 )
            srcRow = 0;
        else if( iRow >= iHeight )
            srcRow = iHeight-1;
        else    // inside the image
            srcRow = iRow;

        for( int cCol = 0, iCol = col-midRow; cCol < width; cCol++, iCol++ )
        {
            // for each cell in the conv
            if( iCol < 0 )
                srcCol = 0;
            else if( iCol >= iWidth )
                srcCol = iWidth-1;
            else
                srcCol = iCol;

            int cellVal = B_MASK_CELL(mask,cRow,cCol);
            int pixelVal = (B_Image_getPixel( image, srcCol, srcRow )&0xFF);

            sum += cellVal * pixelVal;
            coefSum += cellVal<0?-cellVal:cellVal;
        }
    }

    sum /= mask->divisor;       // divide by the divisor part
    sum /= coefSum?coefSum:1;   // divide by the coefficients
    return sum;
}
