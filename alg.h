#ifndef __ALG_H__
#define __ALG_H__

#include "bimage.h"

typedef struct Algorithm_Rec_str Algorithm_Rec;
typedef Algorithm_Rec * Algorithm;

/*
void alg_init(void);
void alg_done(void);
*/

/**
 * Returns the instance of the Algorithm. Increases the ref count.
 * @return instance of algorithm struct
 */
Algorithm Alg_getInstance(void);

/**
 * Deletes the instance if no more references
 * @param alg The Algorithm instance
 */
void Alg_deleteInstance( Algorithm alg );

/**
 * Calculates the curvature of the image
 * @param image The image to calculate the curvature of
 * @return The curvature score
 */
int Alg_calculateCurvature( Algorithm alg, B_Image image );

/**
 * Calculates the straightness of the image
 * @param image The image to calculate the curvature of
 * @return The curvature score
 */
int Alg_calculateStraightness( Algorithm alg, B_Image image );

/**
 * Calculates a score based off of
 * the area used by the character:
 * area used divided by total area
 * @return 1/10000 fractionals
 */
int Alg_calculateAreaUsed( Algorithm alg, B_Image image );

#endif // __ALG_H__

