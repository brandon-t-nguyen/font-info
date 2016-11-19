#ifndef __B_MASK_H__
#define __B_MASK_H__

#include <bimage.h>

/**
 * Image masks
 */
typedef struct B_Mask_Rec_str B_Mask_Rec;
typedef B_Mask_Rec * B_Mask;

/**
 * Returns a new B_Mask object. Coupled with B_Mask_delete()
 * @param matrix Linear matrix of convolution values
 * @param width Width of matrix
 * @param height Height of matrix
 */
B_Mask B_Mask_new( const int matrix[], const int divisor,
                   const int width, const int height );

void B_Mask_delete( B_Mask mask );

/**
 * Returns a new B_Mask that is the input transposed
 * @param input The input mask
 * @return Rotated convolution
 */
B_Mask B_Mask_transpose( const B_Mask input );

/**
 * Returns a new B_Mask that's rotated 90 degrees
 */
B_Mask B_Mask_rotate( const B_Mask input );

/**
 * Returns a new B_Mask that's flipped horizontally
 */
B_Mask B_Mask_flipHor( const B_Mask input );

/**
 * Returns a B_Image of the convolved image
 * @param mask The mask
 * @param image The input image
 * @return The output, convolved image
 */
B_Image B_Mask_convolve( const B_Mask mask, const B_Image image );

/**
 * Returns the integer value of the convolution of a single pixel
 * @param mask The Mask object
 * @param image The Image object
 * @param row The row of the pixel to convolve
 * @param col The col of the pixel to convolve
 */
int B_Mask_convolvePixel( const B_Mask mask, const B_Image image,
                          const int row, const int col );

/**
 * Returns a B_Image of the correlated image
 * @param mask The mask
 * @param image The input image
 * @return The output, correlated image
 */
B_Image B_Mask_correlate( const B_Mask mask, const B_Image image );

/**
 * Returns the integer value of the convolution of a single pixel
 * @param mask The Mask object
 * @param image The Image object
 * @param row The row of the pixel to correlate
 * @param col The col of the pixel to correlate
 */
int B_Mask_correlatePixel( const B_Mask mask, const B_Image image,
                           const int row, const int col );

int B_Mask_getWidth( const B_Mask mask );
int B_Mask_getHeight( const B_Mask mask );
int B_Mask_getDivisor( const B_Mask mask );
const int *  B_Mask_getMatrix( const B_Mask mask );

#endif //__B_MASK_H__
