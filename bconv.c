#include <stdlib.h>
#include "bconv.h"

#define B_CONV_CELL(m,r,c) (c + r*m->width)

struct B_Conv_Rec_str
{
    int *matrix;
    int divisor;
    int width;
    int height;
};

B_Conv B_Conv_new( const int matrix[], const int width, const int height,
                   const int divisor )
{
    if( !(width % 2) && !(height % 2) )
    {
        return NULL;
    }
    B_Conv conv = (B_Conv) malloc( sizeof(B_Conv_Rec) );
    conv->width = width;
    conv->height = height;
    conv->matrix = (int *) malloc( sizeof(int) * width * height );
    conv->matrix = memcpy( conv->matrix, matrix, sizeof(float)*width*height );
    conv->divisor = divisor;
    return conv;
}

B_Conv B_Conv_new2d( const int **matrix, const int width, const int height,
                     const int divisor )
{
}

void B_Conv_delete( B_Conv conv )
{
}


B_Image B_Conv_convolve( B_Conv conv, const B_Image image )
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
            int pixel = B_Conv_convolvePixel( conv, image, row, col );
            B_Image_setPixel( out, col, row, (pixel>255?255:pixel&0xFF) );
        }
    }
    return out;
}


int B_Conv_convolvePixel( B_Conv conv, const B_Image image,
                          const int row, const int col )
{
    int height = conv->height;
    int width = conv->width;

    int midRow = conv->height / 2;
    int midCol = conv->width / 2;

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
            sum += B_CONV_CELL(conv,cRow,cCol) *
                   B_Image_getPixel( image, srcCol, srcRow );
            coefSum += B_CONV_CELL(conv,cRow,cCol);
        }
    }

    sum /= conv->divisor;
    sum /= coefSum?coefSum:1;
    return sum;
}
