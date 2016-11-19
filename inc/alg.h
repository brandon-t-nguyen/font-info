#ifndef __ALG_H__
#define __ALG_H__

#include <btypeface.h>
#include <bimage.h>

typedef struct Algorithm_Rec_str Algorithm_Rec;
typedef Algorithm_Rec * Algorithm;

typedef enum Metric_enum
{
    Metric_Width,
    Metric_Height,
    Metric_AspectRatio,
    Metric_xHeight,
    Metric_Density,
    Metric_Slant,
    Metric_Curve,
    Metric_Serif,
    NUM_METRICS,
    Metric_Error
} Metric;

typedef struct Metrics_str
{
    double metrics[NUM_METRICS];
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

/**
 * Prints the metrics hedaer out in CSV form. Adds a new line
 */
void Metrics_fprintHeader( FILE * file );

/**
 * Prints the metrics out in CSV form. Adds a new line
 */
void Metrics_fprint( FILE * file, const BT_Face face, const Metrics * results );

/**
 * Returns a const string for a metric name
 */
const char * Metric_toString( Metric metric );

/**
 * Returns a const string for a metric name
 */
Metric Metric_fromString( const char * name );

#endif // __ALG_H__

