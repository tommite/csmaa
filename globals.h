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
