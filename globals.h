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

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define VERSION "1.0"

#ifdef DEMO
#define MAXALTS 5
#define MAXCATS 3
#define MAXCRIT 3
#define APPNAME "CSMAA v1.0 (Demo version)"
#else
#define MAXALTS 50
#define MAXCATS 20
#define MAXCRIT 20
#define APPNAME "CSMAA v1.0 (Full version)"
#endif

#define MAXNAMELENGTH 30
#define ENTRYWIDTH 20

typedef enum {
  TYPE_NONE = -1,
  TYPE_UNIF_REAL = 0,
  TYPE_UNIF_DISC = 1,
  TYPE_GAUS = 2,
  TYPE_ORD = 3
} crittype;

#define DEL_VECTOR_CONTENTS(v) { for (unsigned int i=0;i<v.size();i++) { if (v[i]) {delete v[i]; v[i] = NULL; }}}

#endif
