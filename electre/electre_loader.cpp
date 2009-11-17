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

#include "electre_loader.h"

bool eloader::read_line(FILE *fp, char *buffer, int buflen)
{
  while (fgets(buffer, buflen, fp) != NULL)
    {
      char c = *buffer;
	if (c == '"' || c == '\n' || c == ' ')
	continue;
      return true;
    }
  return false;
}

std::string eloader::read_comment(FILE *fp)
{
  char buf[1024];
  while (fgets(buf,1024,fp) != NULL)
    {
      char c = *buf;
      if (c == '"')
	{
	  std::string ret = string(buf);
	  std::string rr = ret.substr(ret.find_first_of("\"", 0)+1, ret.find_last_of(string("\""), ret.size())-1);
	  return rr;
	}
    }
  return string ("");
}

electre *eloader::load_electre (FILE *fp) throw (std::string &)
{
  electre *e = 0;
  try {
    int no_alt = 0, no_crit = 0;
    int version = 0;
    double alpha = -1000.0, beta = -1000.0;
    int magic, magic2 = 0;
    
    std::vector<std::string> line1 = line_tokens(fp);
    if (line1.size() != 1) 
      throw std::string ("No version information.");

    version = atoi(line1[0].c_str());
    if (version != VERSION_III && version != VERSION_III_IV)
      throw std::string ("Wrong version. Program supports currently only ELECTRE-III.");

    std::vector<std::string> line2 = line_tokens(fp);
    if (line2.size() != 2)
      throw std::string ("Wrong number of arguments for distillation coefficients.");
    alpha = atof(line2[0].c_str());
    beta = atof(line2[1].c_str());
    if (alpha == -1000.0 || beta == -1000.0)
      throw std::string ("Distillation coefficients in incorrect format.");

    std::vector<std::string> line3 = line_tokens(fp);
    if (line3.size() != 1)
      throw std::string ("Wrong number of arguments for magic number.");
    magic = atoi(line3[0].c_str());

    std::vector<std::string> line4 = line_tokens(fp);
    if (line4.size() != 1)
      throw std::string ("Wrong number of arguments for magic number 2.");
    magic2 = atoi(line3[0].c_str());

    std::vector<std::string> line5 = line_tokens(fp);
    if (line5.size() != 1)
      throw std::string ("Wrong number of arguments for n:o of criteria.");
    no_crit = atoi(line5[0].c_str());
    if (no_crit < 1)
      throw std::string ("Invalid number of arguments.");

    std::vector<std::string> line6 = line_tokens(fp);
    if (line6.size() != 1)
      throw std::string ("Wrong number of arguments for n:o of alternatives.");
    no_alt = atoi(line6[0].c_str());
    if (no_alt < 1)
      throw std::string("Invalid number of alternatives.");

    e = new electre (no_alt, no_crit, alpha, beta);

    // now read criteria information
    for (int i=0;i<no_crit;i++)
      {
	errno = 0;
	electre_criterion *ec = 0;
	double cw = 0.0;
	int asc = -1, dir = -1;
	double ind_t_a = 0.0, ind_t_b = 0.0, pref_t_a = 0.0, pref_t_b = 0.0;
	double veto_t_a = 0.0, veto_t_b = 0.0;
	double veto = false;

	std::string desc = read_comment(fp);
	std::string name = read_comment(fp);
	if (name.size() < 1)
	  throw std::string ("Criterion has empty name.");

	std::vector<std::string> cl1 = line_tokens(fp);
	if (cl1.size() < 1) throw std::string ("Criterion line 1 incorrect amount of information.");
	cw = atof(cl1[0].c_str());
	if (cw < 0.0) throw std::string ("Criterion weight less than 0.");

	std::vector<std::string> cl2 = line_tokens(fp);
	if (cl2.size() != 1) throw std::string ("Criterion line 2 incorrect amount of information.");
	asc = atoi(cl2[0].c_str());
	if (asc != 0 && asc != 1) throw std::string ("Criterion ascending information not 0 or 1.");

	std::vector<std::string> cl3 = line_tokens(fp);
	if (cl3.size() != 1) throw std::string ("Criterion line 3 incorrect amount of information.");
	dir = atoi(cl3[0].c_str());
	if (dir != 0 && dir != 1) throw std::string ("Criterion direct information not 0 or 1.");
	
	std::vector<std::string> cl4 = line_tokens(fp);
	if (cl4.size() != 2) throw std::string ("Criterion line 4 incorrect amount of information.");
	
	errno = 0;
	ind_t_a = strtod(cl4[0].c_str(), NULL);
	if (errno)
	  throw std::string ("Criterion indifference threshold alpha in incorrect format.");
	ind_t_b = strtod(cl4[1].c_str(), NULL);
	if (errno)
	  throw std::string ("Criterion indifference threshold beta in incorrect format.");

	std::vector<std::string> cl5 = line_tokens(fp);
	if (cl5.size() != 2) throw std::string ("Criterion line 5 incorrect amount of information.");

	pref_t_a = strtod(cl5[0].c_str(), NULL);
	if (errno)
	  throw std::string ("Criterion preference threshold alpha in incorrect format.");
	pref_t_b = strtod(cl5[1].c_str(), NULL);
	if (errno)
	  throw std::string ("Criterion preference threshold beta in incorrect format.");

	std::vector<std::string> cl6 = line_tokens(fp);
	if (cl6.size() != 2) throw std::string ("Criterion line 6 incorrect amount of information.");
	if (cl6[0] != std::string("-"))
	  {
	    veto = true;
	    veto_t_a = strtod(cl6[0].c_str(), NULL);
	    if (errno)
	      throw std::string ("Criterion veto threshold alpha in incorrect format.");
	    veto_t_b = strtod(cl6[1].c_str(), NULL);
	    if (errno)
	      throw std::string ("Criterion veto threshold beta in incorrect format.");
	  }
	if (veto)
	  ec = new electre_criterion(name, desc, cw, asc, dir, ind_t_a, ind_t_b, pref_t_a, pref_t_b, veto_t_a, veto_t_b);
	else
	  ec = new electre_criterion(name, desc, cw, asc, dir, ind_t_a, ind_t_b, pref_t_a, pref_t_b);
	e->set_criterion(i, ec);
      }

    std::vector<electre_alternative *> alts(no_alt);
    for (int i=0;i<no_alt;i++)
      {
	std::string desc = read_comment(fp);
	std::string name = read_comment(fp);

	if (name.size() < 1)
	  throw std::string ("Alternative has empty name.");

	double *cvals = new double[no_crit];

	for (int j=0;j<no_crit;j++)
	  {
	    std::vector<double> xxx;
	    try {
	      std::vector<std::string> xx = line_tokens(fp);
	      xxx = elements_to_numbers(xx);
	      if (xxx.size() != 1)
		throw std::string ("Alternative's criteria value line incorrect amount of information.");	    
	    }
	    catch (string & s)
	      {
		delete cvals;
		throw s;
	      }
	    cvals[j] = xxx[0];
	  }
	electre_alternative *ea = new electre_alternative(name, desc, no_crit, cvals);

	e->set_alternative(i, ea);
      }
  }
  catch (std::string &s)
    {
      if (e) delete e;
      throw s;
    }
  return e;
}

electre_tri *eloader::load_electre_tri (FILE *fp) throw (std::string &)
{
  electre_tri *e = 0;
  try {
    int no_alt = 0, no_crit = 0, no_cat = 0;
    int version = 0;
    
    std::vector<std::string> line1 = line_tokens(fp);
    if (line1.size() != 1) 
      throw std::string ("No version information.");

    version = atoi(line1[0].c_str());
    if (version != VERSION_TRI)
      throw std::string ("Wrong version. Program supports only ELECTRE-TRI.");

    std::vector<std::string> line5 = line_tokens(fp);
    if (line5.size() != 1)
      throw std::string ("Wrong number of arguments for n:o of criteria.");
    no_crit = atoi(line5[0].c_str());
    if (no_crit < 1)
      throw std::string ("Invalid number of arguments.");

    std::vector<std::string> line6 = line_tokens(fp);
    if (line6.size() != 1)
      throw std::string ("Wrong number of arguments for n:o of alternatives.");
    no_alt = atoi(line6[0].c_str());
    if (no_alt < 1)
      throw std::string("Invalid number of alternatives.");

    std::vector<std::string> line7 = line_tokens(fp);
    if (line7.size() != 1)
      throw std::string ("Wrong number of arguments for n:o of categories.");
    no_cat = atoi(line7[0].c_str());
    if (no_cat < 1)
      throw std::string("Invalid number of categories.");

    e = new electre_tri (no_cat, no_crit, no_alt);

    // now read criteria information
    for (int i=0;i<no_crit;i++)
      {
	errno = 0;
	electre_criterion *ec = 0;
	double cw = 0.0;
	int asc = -1;
	double ind_t_a = 0.0, ind_t_b = 0.0, pref_t_a = 0.0, pref_t_b = 0.0;
	double veto_t_a = 0.0, veto_t_b = 0.0;
	double veto = false;

	std::string desc = read_comment(fp);
	std::string name = read_comment(fp);
	if (name.size() < 1)
	  throw std::string ("Criterion has empty name.");

	std::vector<std::string> cl1 = line_tokens(fp);
	if (cl1.size() < 1) throw std::string ("Criterion line 1 incorrect amount of information.");
	cw = atof(cl1[0].c_str());
	if (cw < 0.0) throw std::string ("Criterion weight less than 0.");

	std::vector<std::string> cl2 = line_tokens(fp);
	if (cl2.size() != 1) throw std::string ("Criterion line 2 incorrect amount of information.");
	asc = atoi(cl2[0].c_str());
	if (asc != 0 && asc != 1) throw std::string ("Criterion ascending information not 0 or 1.");

	
	std::vector<std::string> cl4 = line_tokens(fp);
	if (cl4.size() != 2) throw std::string ("Criterion line 4 incorrect amount of information.");
	
	errno = 0;
	ind_t_a = strtod(cl4[0].c_str(), NULL);
	if (errno)
	  throw std::string ("Criterion indifference threshold alpha in incorrect format.");
	ind_t_b = strtod(cl4[1].c_str(), NULL);
	if (errno)
	  throw std::string ("Criterion indifference threshold beta in incorrect format.");

	std::vector<std::string> cl5 = line_tokens(fp);
	if (cl5.size() != 2) throw std::string ("Criterion line 5 incorrect amount of information.");

	pref_t_a = strtod(cl5[0].c_str(), NULL);
	if (errno)
	  throw std::string ("Criterion preference threshold alpha in incorrect format.");
	pref_t_b = strtod(cl5[1].c_str(), NULL);
	if (errno)
	  throw std::string ("Criterion preference threshold beta in incorrect format.");

	std::vector<std::string> cl6 = line_tokens(fp);
	if (cl6.size() != 2) throw std::string ("Criterion line 6 incorrect amount of information.");
	if (cl6[0] != std::string("-"))
	  {
	    veto = true;
	    veto_t_a = strtod(cl6[0].c_str(), NULL);
	    if (errno)
	      throw std::string ("Criterion veto threshold alpha in incorrect format.");
	    veto_t_b = strtod(cl6[1].c_str(), NULL);
	    if (errno)
	      throw std::string ("Criterion veto threshold beta in incorrect format.");
	  }
	if (veto)
	  ec = new electre_criterion(name, desc, cw, asc, 1, ind_t_a, ind_t_b, pref_t_a, pref_t_b, veto_t_a, veto_t_b);
	else
	  ec = new electre_criterion(name, desc, cw, asc, 1, ind_t_a, ind_t_b, pref_t_a, pref_t_b);
	e->set_criterion(i, ec);
      }

    std::vector<electre_alternative *> alts(no_alt);
    for (int i=0;i<no_alt;i++)
      {
	std::string desc = read_comment(fp);
	std::string name = read_comment(fp);

	if (name.size() < 1)
	  throw std::string ("Alternative has empty name.");

	double *cvals = new double[no_crit];

	for (int j=0;j<no_crit;j++)
	  {
	    std::vector<double> xxx;
	    try {
	      std::vector<std::string> xx = line_tokens(fp);
	      xxx = elements_to_numbers(xx);
	      if (xxx.size() != 1)
		throw std::string ("Alternative's criteria value line incorrect amount of information.");	    
	    }
	    catch (string & s)
	      {
		delete cvals;
		throw s;
	      }
	    cvals[j] = xxx[0];
	  }
	electre_alternative *ea = new electre_alternative(name, desc, no_crit, cvals);

	e->set_alternative(i, ea);
      }

    // read names of classes
    for (int i=0;i<no_cat;i++)
      {
	string s = read_comment(fp);
	e->set_class_name(i, s);
      }
    // read profiles
    std::vector<electre_alternative *> profiles(no_cat-1);
    for (int i=0;i<no_cat-1;i++)
      {
	std::string desc = read_comment(fp);
	std::string name = read_comment(fp);

	if (name.size() < 1)
	  throw std::string ("Profile has empty name.");

	double *cvals = new double[no_crit];

	for (int j=0;j<no_crit;j++)
	  {
	    std::vector<double> xxx;
	    try {
	      std::vector<std::string> xx = line_tokens(fp);
	      xxx = elements_to_numbers(xx);
	      if (xxx.size() != 1)
		throw std::string ("Profiles criteria value line incorrect amount of information.");	    
	    }
	    catch (string & s)
	      {
		delete cvals;
		throw s;
	      }
	    cvals[j] = xxx[0];
	  }
	electre_alternative *ea = new electre_alternative(name, desc, no_crit, cvals);
	
	e->set_profile(i, ea);

      }
  }
  catch (std::string &s)
    {
      if (e) delete e;
      throw s;
    }
  return e;
}

std::vector<std::string> eloader::line_tokens (FILE *fp)
{
  char buf[1024];
  char nambuf[1024];
  char *tb;

  std::vector<std::string> res;
  
  if (!read_line (fp, buf, 1024))
    return res;

  tb = buf;
  while (true)
    {
      char * t = strsep (&tb, " ");
      if (t == NULL) break;
      if (sscanf(t, "%s", nambuf) != 1) break;
      if (strlen(nambuf) > 0)
	res.push_back(std::string(nambuf));
    }
  return res;
}

std::vector<double> eloader::elements_to_numbers(std::vector<std::string> &data) throw (std::string &)
{
  std::vector<double> res;

  std::vector<std::string>::iterator it = data.begin();
  while (it != data.end())
    {
      std::string & s = *it++;
      errno = 0;
      double d = strtod (s.c_str(), NULL);
      if (d == 0.0 && errno != 0)
	throw std::string ("Error converting element to number.");
      res.push_back(d);
    }
  return res;
}

std::vector<int> eloader::elements_to_ints(std::vector<std::string> &data) throw (std::string &)
{
  std::vector<int> res;

  std::vector<std::string>::iterator it = data.begin();
  while (it != data.end())
    {
      std::string & s = *it++;
      errno = 0;
      int d = (int) strtol (s.c_str(), NULL, 10);
      if (d == 0 && errno != 0)
	throw std::string ("Error converting element to number.");
      res.push_back(d);
    }
  return res;
}

