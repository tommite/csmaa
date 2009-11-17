/*
	This file is part of CSMAA.
	(c) Tommi Tervonen, 2009	

    CSMAA is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CSMAA is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CSMAA.  If not, see <http://www.gnu.org/licenses/>.
*/

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

