#ifndef __BIMAGE_H__
#define __BIMAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>

/**
 * The B_Image is a class that represents an 8 bit matrix of values. It's a row->major
 */

typedef struct B_Image_Rec_str B_Image_Rec;
typedef B_Image_Rec * B_Image;

#define B_Image_grayToAscii(x) (x==0?' ':(x<64?'.':(x<128?'*':'#')))

/**
 * Returns a new B_Image object. Coupled with B_Image_delete()
 * @param width The width of the image
 * @param height The height of the image
 * @return The newly created B_Image
 */
B_Image B_Image_new( int width, int height );
void B_Image_delete( B_Image image );

/**
 * Gets a pixel from the image
 * @param image The image object
 * @param x The column of the pixel (0 is left border)
 * @param y The row of the pixel (0 is top border)
 * @return The pixel value
 */
uint8_t B_Image_getPixel( B_Image image, int x, int y );

/**
 * Sets a pixel from the image
 * @param image The image object
 * @param x The column of the pixel (0 is left border)
 * @param y The row of the pixel (0 is top border)
 * @param pixel The pixel value
 */
void B_Image_setPixel( B_Image image, int x, int y, uint8_t pixel );

/**
 * Draws onto the image with a row-major linear array (bitmap)
 * @param image Image object
 * @param buffer The row-major array to draw with
 * @param width The width of the rows
 * @param height The height of the columns
 * @param x The x-position of the bitmap (0 is left border of bitmap)
 * @param y The y-position of the bitmap (0 is top border of bitmap)
 */
void B_Image_drawBitmap( B_Image image, const uint8_t buffer[], int width, int height, int x, int y );


/**
 * Print the buffer out to an open file
 * @param image The image object
 * @param file File pointer to file to print to
 */
void B_Image_fprint( B_Image image, FILE * file );
#define B_Image_print(image) B_Image_fprint( image, stdout )

// Getters

int B_Image_getWidth( const B_Image image );
int B_Image_getHeight( const B_Image image );

#ifdef __cplusplus
}
#endif

#endif //__BIMAGE_H__
