#ifndef __ALG_H__
#define __ALG_H__

#include "bimage.h"


void alg_init(void);
void alg_done(void);

/**
 * Calculates the curvature of the image
 * @param image The image to calculate the curvature of
 * @return The curvature score
 */
int alg_calculateCurvature( B_Image image );
int alg_calculateStraightness( B_Image image );

/**
 * Calculates a score based off of
 * the area used by the character:
 * area used divided by total area
 * @return 1/10000 fractionals
 */
int alg_calculateAreaUsed( B_Image image );

#endif // __ALG_H__

