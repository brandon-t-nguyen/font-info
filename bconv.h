#ifndef __B_CONV_H__
#define __B_CONV_H__

#include "bimage.h"

/**
 * Image convolutions
 */
typedef struct B_Conv_Rec_str B_Conv_Rec;
typedef B_Conv_Rec * B_Conv;

/**
 * Returns a new B_Conv object. Coupled with B_Conv_delete()
 * @param matrix Linear matrix of convolution values
 * @param width Width of matrix
 * @param height Height of matrix
 */
B_Conv B_Conv_new( const int matrix[], const int width, const int height,
                   const int divisor );

void B_Conv_delete( B_Conv conv );

/**
 * Returns a B_Image of the convolved image
 * @param conv The convolution
 * @param image The input image
 * @return The output, convolved image
 */
B_Image B_Conv_convolve( B_Conv conv, const B_Image image );

/**
 * Returns the integer value of the convolution of a single pixel
 * @param conv The Convolution object
 * @param image The Image object
 * @param row The row of the pixel to convolve
 * @param col The col of the pixel to convolve
 */
int B_Conv_convolvePixel( B_Conv conv, const B_Image image,
                          const int row, const int col );


#endif //__B_CONV_H__
