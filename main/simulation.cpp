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

#include "simulation.h"

void create_sumto_rand(double *dest, int num_alts, double sumto)
{
  for (int i=0;i<num_alts-1;i++)
    dest[i] = rnd() * sumto;

  std::sort(dest, dest+num_alts-1);
  dest[num_alts-1] = sumto;
  
  double last = 0.0;
  for (int i=0;i<num_alts;i++)
    {
      double t = dest[i];
      dest[i] = t - last;
      last = t;
    }
}

void create_sumtoone_rand (double *dest, int num_alts)
{
  create_sumto_rand(dest, num_alts, 1.0);
}

void create_sumtoone_sorted(double *dest, int num_alts)
{
  create_sumtoone_rand(dest, num_alts);
  std::sort(dest, dest+num_alts,std::greater<double>());
}

double create_gaussian(double mean, double stdev)	// normally distributed random number
{	
  register double tmp= 0;
  for(int i= 12; i--;)
    tmp+= rnd();
  return mean+stdev*(tmp-6);
}
