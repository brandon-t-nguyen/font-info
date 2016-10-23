#ifndef __ALG_H__
#define __ALG_H__

#include "btypeface.h"
#include "bimage.h"

typedef struct Algorithm_Rec_str Algorithm_Rec;
typedef Algorithm_Rec * Algorithm;

typedef enum Metric_enum
{
    Metric_Density,
    Metric_Curve,
    Metric_Angle,

} Metric;

typedef struct Metrics_str
{
    double density;
    double curve;
    double slant;
} Metrics;

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
void Alg_doneInstance( Algorithm alg );

/**
 * Releases the instance, reseting the Singleton pattern
 */
void Alg_releaseInstance(void);

/**
 * Calculates the value for a given metric
 */
double Alg_calculateMetric( const Algorithm alg, const BT_Face face, const Metric metric );

/**
 * Calculates all metrics for a font
 */
void Alg_calculateMetrics( const Algorithm alg, const BT_Face face, Metrics * results );


#endif // __ALG_H__

