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

#ifndef __ELECTRE_LOADER_H__
#define __ELECTRE_LOADER_H__

#include <cstdio>
#include <cstdarg>
#include <cstdlib>
#include <string>
#include <vector>
#include <cerrno>
#include "electre.h"
#include <typeinfo>

#define READ_ERROR throw "Error reading from file pointer"
#define VERSION_TRI 5
#define VERSION_III 3
#define VERSION_III_IV 1

namespace eloader {
  
  template<class T>
    int find_index (std::vector<T> &v, T dat)
    {
      for (int i=0;i<v.size();i++)
	if (v[i] == dat) return i;
      return -1;
    }
  
  electre *load_electre (FILE *fp) throw (std::string &);
  electre_tri *load_electre_tri (FILE *fp) throw (std::string &);
  
  bool read_line(FILE *fp, char *buffer, int buflen);
  std::string read_comment(FILE *fp);
  
  std::vector<std::string> line_tokens(FILE *fp);
  
  std::vector<double> elements_to_numbers(std::vector<std::string> &data) throw (std::string &);
  std::vector<int> elements_to_ints(std::vector<std::string> &data) throw (std::string &);
  
}

#endif
