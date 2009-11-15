#ifndef __SIMULATION_H__
#define __SIMULATION_H__

#include <stdlib.h>
#include <algorithm>
#include <functional>

//! Random macro, returns a random number within the range [0.0,1.0].
inline double rnd()
{ return (double) rand() / (double) RAND_MAX; }

#ifndef MIN
#define MIN(x,y) ( x <= y ? x : y )
#endif

/**
 * Generates random numbers that have a certain sum.
 *
 * \param dest destination vector.
 * \param num_alts number of numbers to be generated.
 * \param sumto sum of the numbers.
 */
void create_sumto_rand(double *dest, int num_alts, double sumto);

/**
 * Generates random numbers that sum to 1.0.
 *
 * \param dest destination vector. The numbers will be written to this.
 * \param num_alts number of numbers to be generated.
 */
void create_sumtoone_rand(double *dest, int num_alts);

/**
 * Generates random numbers that sum to 1.0, and are sorted in ascending order.
 *
 * \param dest destination vector. The numbers will be written to this.
 * \param num_alts number of numbers to be generated.
 */
void create_sumtoone_sorted(double *dest, int num_alts);

// normally distributed random number
double create_gaussian(double mean, double stdev);

inline double rand_meanpm(double mean, double plusminus)
{ return mean - plusminus + (2.0 * plusminus * rnd()); }

inline double rand_interval(double min, double max)
{ return min + (rnd() * (max - min)); }

#endif

