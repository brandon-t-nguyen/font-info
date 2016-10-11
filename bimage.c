#include <stdint.h>
#include <stdlib.h>
#include "bimage.h"


// the strcut definition
struct B_Image_Rec_str
{
    int width, height;
    uint8_t *buffer;
    int widthCap, heightCap;
};

#define B_IMAGE_INDEX(x,y) (x + y*image->widthCap)


B_Image B_Image_new( int width, int height )
{
    int area = width * height;

    // make the object
    B_Image image = malloc( sizeof(B_Image_Rec) );
    image->width = width;
    image->height = height;
    image->widthCap = width;
    image->heightCap = height;

    // allocate and initialize the buffer
    image->buffer = (uint8_t *) malloc( area * sizeof(uint8_t) );
    uint8_t *buffer = image->buffer;    // save on deref cost
    for( int i = 0; i < area; i++ )
    {
        buffer[i] = 0;
    }

    return image;
}

void B_Image_delete( B_Image image )
{
    free(image->buffer);
    free(image);
}

uint8_t B_Image_getPixel( B_Image image, int x, int y )
{
    if( x < image->width && y < image->height )
    {
        return image->buffer[ B_IMAGE_INDEX(x,y) ];
    }
    else
    {
        fprintf(stderr,"%s: Pixel out of bounds: (%d,%d)", __FUNCTION__, x, y);
        return 0;
    }
}


void B_Image_setPixel( B_Image image, int x, int y, uint8_t pixel )
{
    if( x < image->width && y < image->height )
    {
        image->buffer[ B_IMAGE_INDEX(x,y) ] = pixel;
    }
    else
    {
        fprintf(stderr,"%s: Pixel out of bounds: (%d,%d)", __FUNCTION__, x, y);
    }
}

void B_Image_drawBitmap( B_Image image, uint8_t buffer[], int width, int height, int x, int y )
{
    uint8_t *row;
    for( int r = y, rBuffer = 0; r < image->height && rBuffer < height; r++, rBuffer++ )
    {
        row = &image->buffer[ B_IMAGE_INDEX(0,r) ]; // get a pointer to the row everytime
        for( int c = x, cBuffer = 0; c < image->width && cBuffer < width; c++, cBuffer++ )
        {
            row[c] = buffer[ cBuffer + rBuffer * width ];
        }
    }
}

void B_Image_fprint( B_Image image, FILE * file )
{
    int width = image->width;
    int height = image->height;
    uint8_t *row;
    for(int r = 0; r < height; r++)
    {
        row = &image->buffer[ B_IMAGE_INDEX(0,r) ];
        for(int c = 0; c < width; c++)
        {
            fputc( B_Image_grayToAscii(row[c]), file );
        }
        fputc( '\n', file );
    }
}

// Getters
inline int B_Image_getWidth( B_Image image )
{
    return image->width;
}

inline int B_Image_getHeight( B_Image image )
{
    return image->height;
}

