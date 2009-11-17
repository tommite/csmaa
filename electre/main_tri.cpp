/*
  Copyright (C) 2005 Tommi Tervonen

  This file is part of ELECTRE.

  ELECTRE is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  ELECTRE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with ELECTRE; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "electre.h"
#include "electre_loader.h"
#include <cstdlib>
#include <cstdio>
#include <unistd.h>

#define EXEFILE "electre_tri"

void print_license()
{
  fprintf (stderr, "ELECTRE-TRI\n");
  fprintf (stderr, "(C) 2005 Tommi Tervonen <tommi.tervonen@it.utu.fi>\n");
  fprintf (stderr, "Released under Gnu Public License v2.\n");
  fprintf (stderr, "------------------------------------------------\n");
}

void print_usage()
{
  fprintf (stderr, "Usage: %s rule lambda [input_file]\n", EXEFILE);
  fprintf (stderr, "where rule=[0/1], 0=pessimistic, 1=optimistic,\n");
  fprintf (stderr, "and lambda is the cutting level.\n");
}

int main (int argc, char **argv)
{
  FILE *inf;
  electre_tri *e;
  double lambda =0.0;
  bool optimistic = false;

  print_license();
  if (argc < 3 || argc > 4)
    {
      print_usage();
      exit(0);
    }
  if (argc == 3)
    inf = stdin;
  else // argc == 4
    {
      inf = fopen(argv[3], "r");
      if (inf == NULL)
	{
	  fprintf (stderr, "Error opening input file %s.\n", argv[1]);
	  exit(1);
	}
    }
  lambda = atof(argv[2]);
  optimistic = (atoi(argv[1]) == 1);

  // init rnumbergen
  try {
    e = eloader::load_electre_tri(inf);
  }
  catch (string &s)
    {
      fprintf (stderr, "Error loading model: %s\n", s.c_str());
      exit(1);
    }

  fprintf (stderr, "Model loaded.\n");

  //  e->print(stdout);

  fprintf (stderr, "Using %s rule.\n", optimistic ? "optimistic" : "pessimistic");

  e->set_lambda(lambda);
  e->set_rule(optimistic);

  e->analyze();
  
  fprintf (stderr, "Finished. Results:\n");

  e->print_results(stdout);

  delete e;

  return 0;
}
