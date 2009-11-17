/*
  Copyright (C) 2005-2006 Tommi Tervonen

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

electre_alternative::electre_alternative(string &n, string &d, int nc, double *cvals)
{
  assert(nc > 0);
  assert (n.size() > 0);

  name = n;
  desc = d;
  no_crit = nc;
  crit_vals = cvals;
}

void electre_alternative::print (FILE *f)
{
  fprintf (f, "%s", name.c_str());
  for (int i=0;i<no_crit;i++)
    fprintf (f, "\t%f", crit_vals[i]);
  printf ("\n");
}

void electre::print (FILE *f)
{
  fprintf (f, "ALT\n");
  for (int i=0;i<get_no_alts();i++)
    if (alts[i])
      alts[i]->print(f);
    else fprintf (f, "NULL\n");
  print_preorders(f);
}

void electre_tri::print (FILE *f)
{
  fprintf (f, "ALT\t");
  for (int i=0;i<get_no_crit();i++)
    {
      if (crits[i])
	fprintf (f, "%s(%s)", crits[i]->get_name().c_str(), crits[i]->is_ascending() ? "a" : "d");
      else fprintf (f, "NULL");
      if (i != get_no_crit()-1)
	fprintf (f, "\t");
      else fprintf (f, "\n");
    }
  for (int i=0;i<get_no_alts();i++)
    if (alts[i])
      alts[i]->print(f);
    else fprintf (f, "NULL\n");
  fprintf (f, "WEIGHT\t");
  for (int i=0;i<get_no_crit();i++)
    {
      if (crits[i])
	fprintf (f, "%f", crits[i]->get_weight());
      else fprintf (f, "NULL");
      if (i != get_no_crit()-1)
	fprintf (f, "\t");
      else fprintf (f, "\n");
    }

  fprintf (f, "PROFILE\t");
  for (int i=0;i<get_no_crit();i++)
    {
      if (crits[i])
	fprintf (f, "%s", crits[i]->get_name().c_str());
      else fprintf (f, "NULL");
      if (i != get_no_crit()-1)
	fprintf (f, "\t");
      else fprintf (f, "\n");
    }
  for (int i=0;i<num_classes-1;i++)
    if (profiles[i])
      profiles[i]->print(f);
    else fprintf(f, "NULL\n");

  fprintf (f, "THRESHOLD\t");
  for (int i=0;i<get_no_crit();i++)
    {
      if (crits[i])
	fprintf (f, "%s", crits[i]->get_name().c_str());
      else fprintf (f, "NULL");
      if (i != get_no_crit()-1)
	fprintf (f, "\t");
      else fprintf (f, "\n");
    }
  fprintf (f, "P\t");
  for (int i=0;i<get_no_crit();i++)
    {
      if (crits[i])
	fprintf (f, "%f,%f", crits[i]->get_pref_th_a(), crits[i]->get_pref_th_b());
      else fprintf (f, "NULL");
      if (i != get_no_crit()-1)
	fprintf (f, "\t");
      else fprintf (f, "\n");
    }
  fprintf (f, "Q\t");
  for (int i=0;i<get_no_crit();i++)
    {
      if (crits[i])
	fprintf (f, "%f,%f", crits[i]->get_ind_th_a(), crits[i]->get_ind_th_b());
      else fprintf (f, "NULL");
      if (i != get_no_crit()-1)
	fprintf (f, "\t");
      else fprintf (f, "\n");
    }
  fprintf (f, "V\t");
  for (int i=0;i<get_no_crit();i++)
    {
      if (crits[i])
	fprintf (f, "%f,%f", crits[i]->get_veto_th_a(), crits[i]->get_veto_th_b());
      else fprintf (f, "NULL");
      if (i != get_no_crit()-1)
	fprintf (f, "\t");
      else fprintf (f, "\n");
    }

  fprintf (f, "CLASS\n");
  for (int i=0;i<num_classes;i++)
    fprintf (f, "%s\n", class_names[i].c_str());
}



electre_alternative::electre_alternative(electre_alternative &e)
{
  name = e.name;
  desc = e.desc;
  no_crit = e.no_crit;
  crit_vals = new double[no_crit];
  copy (e.crit_vals, e.crit_vals+no_crit, crit_vals);
}

electre_alternative::~electre_alternative()
{
  delete[] crit_vals;
}

electre::~electre()
{
  for (int i=0;i<get_no_crit();i++)
    delete[] disc_matrices[i];

  delete[] conc_matrix;
  delete[] cred_matrix;
}

electre_criterion::electre_criterion (string &_name, string &_desc, double _weight, bool _ascending, double _ind_threshold_a, double _ind_threshold_b, double _pref_threshold_a, double _pref_threshold_b, double _veto_threshold_a, double _veto_threshold_b)
{
  assert (_name.size() > 0);
  name = _name;
  desc = _desc;
  ascending = _ascending;
  weight = _weight;
  ind_threshold_a = _ind_threshold_a;
  ind_threshold_b = _ind_threshold_b;
  pref_threshold_a = _pref_threshold_a;
  pref_threshold_b = _pref_threshold_b;
  veto_threshold_a = _veto_threshold_a;
  veto_threshold_b = _veto_threshold_b;
  veto = true;

}

electre_criterion::electre_criterion (string &_name, string & _desc, double _weight, bool _ascending, double _ind_threshold_a, double _ind_threshold_b, double _pref_threshold_a, double _pref_threshold_b)
{
  assert (_name.size() > 0);

  name = _name;
  desc = _desc;
  ascending = _ascending;
  weight = _weight;
  ind_threshold_a = _ind_threshold_a;
  ind_threshold_b = _ind_threshold_b;
  pref_threshold_a = _pref_threshold_a;
  pref_threshold_b = _pref_threshold_b;
  veto_threshold_a = 0.0;
  veto_threshold_b = 0.0;
  veto = false;
}

void electre_base::set_criterion(int crit_index, electre_criterion *ec)
{
  assert(crit_index >= 0 & crit_index < get_no_crit());
  if (crits[crit_index])
    delete crits[crit_index];
  crits[crit_index] = ec;
}

void electre_base::set_alternative(int alt_index, electre_alternative *ea)
{
  assert(alt_index >= 0 & alt_index < get_no_alts());
  if (alts[alt_index] != 0)
    delete alts[alt_index];
  alts[alt_index] = ea;
}

void electre_tri::set_class_name(int index, const string &name)
{
  assert(index >= 0 & index < num_classes);
  class_names[index] = name;
}

void electre_tri::set_profile(int alt_index, electre_alternative *ea) throw (string &)
{
  assert(alt_index >= 0 & alt_index < num_classes-1);
  if (profiles[alt_index] != 0)
    delete profiles[alt_index];
  profiles[alt_index] = ea;
}

electre_base::electre_base(int noa, int noc)
{
  assert (noa > 0);
  assert (noc > 0);

  crits.resize(noc);
  alts.resize(noa);
  
  fill (crits.begin(), crits.end(), (electre_criterion *) 0);
  fill (alts.begin(), alts.end(), (electre_alternative *) 0);
}

electre_base::~electre_base()
{
  for (unsigned int i=0;i<crits.size();i++)
    delete crits[i];
  for (unsigned int i=0;i<alts.size();i++)
    delete alts[i];
}

electre::electre(int noa, int noc, double a, double b)
  : electre_base(noa, noc)
{
  alpha = a;
  beta = b;
  rule_maximin = false;

  conc_matrix = new double[get_no_alts()*get_no_alts()];
  cred_matrix = new double[get_no_alts()*get_no_alts()];
  disc_matrices.resize(get_no_crit());
  
  for (int i=0;i<get_no_crit();i++)
    disc_matrices[i] = new double[get_no_alts()*get_no_alts()];
}

void electre::set_rule_maximin()
{
  rule_maximin = true;
}

void electre::analyze()
{
  // calc sum_w
  sum_w = 0.0;
  for (int i=0;i<get_no_crit();i++)
    sum_w += crits[i]->get_weight();

  calculate_concordance_matrix();

  if (rule_maximin)
    {
      vector<pair<double,int> >minc;
      
      for (int i=0;i<get_no_alts();i++)
	minc.push_back(pair<double,int>(minconcs[i], i));
      sort(minc.begin(), minc.end(), greater<pair<double,int> >());
      vector<set<int> >res(0);
      for (int i=0;i<get_no_alts();i++)
	{
	  set<int> s;
	  s.insert(minc[i].second);
	  int newi = i;
	  for (int j=i+1;j<get_no_alts();j++)
	    {
	      if (minc[j].second == minc[i].second)
		{
		  s.insert(minc[j].second);
		  newi++;
		}
	      else
		break;
	    }
	  i = newi;
	  res.push_back(s);
	}
      final_preorder = res;
      return;
    }

  calculate_discordance_matrices();
  calculate_credibility_matrix();

  set<int> A;
  for (int i=0;i<get_no_alts();i++)
	A.insert(i);
  
  downdist = distillate(A, true);
  updist = distillate(A, false);
  
  reverse(updist.begin(), updist.end());

  combine_distillations();

}

void electre_tri::analyze()
{
  int i=0;
  // calc sum_w
  sum_w = 0.0;
  for (int j=0;j<get_no_crit();j++)
    sum_w += crits[j]->get_weight();

  // classify alternatives
  for (int j=0;j<get_no_alts();j++)
    {
      //      printf ("Classifying alternative %s...\n", alts[j]->get_name().c_str());
      if (optimistic_rule)
	i=0;
      else // pessimistic
	i=num_classes-2;
      while (true)
	{
	  if (optimistic_rule && i==num_classes-1)
	    break;
	  else if (!optimistic_rule && i==-1)
	    break;
	  double sum = 0.0;
	  double sum2 = 0.0;
	  for (int k=0;k<get_no_crit();k++)
	    {
	      double conc = concordance(k, alts[j], profiles[i]);
	      double conc2 = concordance(k, profiles[i], alts[j]);

	      sum += (crits[k]->get_weight() * conc);
	      sum2 += (crits[k]->get_weight() * conc2);
	    }
	  sum /= sum_w;
	  sum2 /= sum_w;
	  // concordance calc'd, calc discordance
	  double disprod = 1.0;
	  double disprod2 = 1.0;
	  for (int k=0;k<get_no_crit();k++)
	    {
	      if (crits[k]->has_veto())
		{
		  double dis = discordance(k, alts[j], profiles[i]);
		  double dis2 = discordance(k, profiles[i], alts[j]);

		  if (dis > sum)
		    disprod *= ((1.0 - dis) / (1.0 - sum));
		  
		  if (dis2 > sum2)
		    disprod2 *= ((1.0 - dis2) / (1.0 - sum2));
		}
	    }
	  sum *= disprod;
	  sum2 *= disprod2;
	  
	  //	  printf ("disp %f %f sums %f %f\n", disprod, disprod2, sum, sum2);
	  
	  if (!optimistic_rule && sum >= lambda)
	    break;
	  else if (optimistic_rule && sum2 >= lambda & sum < lambda)
	    break;
	  if (optimistic_rule) i++;
	  else i--;
	}
      // ok, let's classify, class found
      if (optimistic_rule)
	assign_to_class(j, i);
      else
	assign_to_class(j,i+1);
    } // end for

}

void electre_tri::assign_to_class (int alt_index, int class_index)
{
  results[alt_index] = class_index;
}

void electre::print_preorders(FILE *f)
{
  printf ("Descending (downward) distillations:\n");
  for (unsigned int i=0;i<downdist.size();i++)
    {
      fprintf (f, "\t[%d]: ", i+1);
      set<int> ss = downdist[i];
      set<int>::iterator it = ss.begin();
      while (it != ss.end())
	{
	  int k = *it++;
	  fprintf (f, "%s ", alts[k]->get_name().c_str());
	}
      fprintf (f, "\n");
    }
  printf ("Ascending (upward) distillations:\n");
  for (unsigned int i=0;i<updist.size();i++)
    {
      fprintf (f, "\t[%d]: ", i+1);
      set<int> ss = updist[i];
      set<int>::iterator it = ss.begin();
      while (it != ss.end())
	{
	  int k = *it++;
	  fprintf (f, "%s ", alts[k]->get_name().c_str());
	}
      fprintf (f, "\n");
    }
  printf ("Final partial preorder:\n");
  for (unsigned int i=0;i<final_preorder.size();i++)
    {
      fprintf (f, "\t[%d]: ", i+1);
      set<int> ss = final_preorder[i];
      set<int>::iterator it = ss.begin();
      while (it != ss.end())
	{
	  int k = *it++;
	  fprintf (f, "%s ", alts[k]->get_name().c_str());
	}
      fprintf (f, "\n");
    }

  printf ("Median complete preorder:\n");
  for (unsigned int i=0;i<median_preorder.size();i++)
    {
      fprintf (f, "\t[%d]: ", i+1);
      set<int> ss = median_preorder[i];
      set<int>::iterator it = ss.begin();
      while (it != ss.end())
	{
	  int k = *it++;
	  fprintf (f, "%s ", alts[k]->get_name().c_str());
	}
      fprintf (f, "\n");
    }
}

void electre::combine_distillations()
{
  vector<set<int> > res;
  double *m1 = new double[get_no_alts()*get_no_alts()];
  double *m2 = new double[get_no_alts()*get_no_alts()];
  double *m3 = new double[get_no_alts()*get_no_alts()];

  fill (m1, m1+(get_no_alts()*get_no_alts()), 0.0);
  fill (m2, m2+(get_no_alts()*get_no_alts()), 0.0);
  fill (m3, m3+(get_no_alts()*get_no_alts()), 0.0);

  // form the matrices
  // first down
  vector<set<int> >::iterator di = downdist.begin();
  list<int> dl;
  while (di != downdist.end())
    {
      set<int> ds = *di++;
      set<int>::iterator dsit = ds.begin();
      while (dsit != ds.end())
	{
	  int i = *dsit++;
	  list<int>::iterator lit = dl.begin();
	  while (lit != dl.end())
	    m1[matloc(*lit++, i)] = 1.0; // outranks
	}
      dsit = ds.begin();
      while (dsit != ds.end())
	dl.push_back(*dsit++);
    }
  // then up
  vector<set<int> >::iterator ui = updist.begin();
  list<int> ul;
  while (ui != updist.end())
    {
      set<int> us = *ui++;
      set<int>::iterator usit = us.begin();
      while (usit != us.end())
	{
	  int i = *usit++;
	  list<int>::iterator lit = ul.begin();
	  while (lit != ul.end())
	    m2[matloc(*lit++, i)] = 1.0; // outranks
	}
      usit = us.begin();
      while (usit != us.end())
	ul.push_back(*usit++);
    }
  // outranking matrices constructed, perform combination  
  for (int i=0;i<get_no_alts();i++)
    for (int j=0;j<get_no_alts();j++)
      {
	if (i == j)
	  continue;
	double r1 = m1[matloc(i, j)];
	double r2 = m2[matloc(i, j)];

	double r3 = m1[matloc(j, i)];
	double r4 = m2[matloc(j, i)];

	if (r1 == 1.0)
	  {
	    assert (r3 == 0.0);
	    if (r4 != 1.0)
	      m3[matloc(i, j)] = 1.0;
	  }
	if (r2 == 1.0)
	  {
	    assert (r4 == 0.0);
	    if (r3 != 1.0)
	      m3[matloc(i, j)] = 1.0;
	  }
	if (r3 == 1.0)
	  {
	    assert (r1 == 0.0);
	    if (r2 != 1.0)
	      m3[matloc(j, i)] = 1.0;
	  }
	if (r4 == 1.0)
	  {
	    assert(r2 == 0.0);
	    if (r1 != 1.0)
	      m3[matloc(j, i)] = 1.0;
	  }
      }
  // now construct the final preorder
  vector<pair<int,int> > m;
  for (int i=0;i<get_no_alts();i++)
    {
      double tot = 0.0;
      for (int j=0;j<get_no_alts();j++)
	tot += m3[matloc(i, j)];
      pair<int,int>p((int)tot, i);
      m.push_back(p);
    }
  sort(m.begin(), m.end(), greater<pair<int,int> >());
  vector<pair<int,int> >::iterator mi = m.begin();
  pair<int,int> p1 = *mi++;
  set<int> s1;
  s1.insert(p1.second);
  int last_qual = p1.first;
  while (mi != m.end())
    {
      pair<int,int> p2 = *mi++;
      if (p2.first == last_qual)
	{
	  s1.insert(p2.second);
	  if (mi == m.end())
	    res.push_back(s1);
	}
      else
	{
	  last_qual = p2.first;
	  res.push_back(s1);
	  s1 = set<int>();
	  s1.insert(p2.second);
	  if (mi == m.end())
	    res.push_back(s1);
	}
    }
  
  delete[] m1;
  delete[] m2;
  delete[] m3;
  final_preorder = res;

  vector<set<int> > medres;
  vector<set<int> >::iterator mit = res.begin();
  while (mit != res.end())
    {
      set<int> s = *mit++;
      if (s.size() == 1)
	medres.push_back(s);
      else // we have more than 1 element in the rank, see the rank differences
	{
	  set<int>::iterator si = s.begin();
	  vector<pair<int,int> > tempvec;
	  while (si != s.end())
	    {
	      int elem = *si++;
	      int i1 = 0, i2 = 0;
	      for (i1=0;i1<(int) downdist.size();i1++)
		if (find(downdist[i1].begin(), downdist[i1].end(), elem) != downdist[i1].end())
		  break;
	      for (i2=0;i2<(int) updist.size();i2++)
		if (find(updist[i2].begin(), updist[i2].end(), elem) != updist[i2].end())
		  break;

	      int df = i1 - i2;
	      if (df < 0) df = 0-df;
	      tempvec.push_back (pair<int, int>(df, elem));
	    }
	  // ok got the rank differences
	  vector<pair<int,int> >::iterator i3 = tempvec.begin();
	  sort (tempvec.begin(), tempvec.end());
	  int lastrk = -1;
	  set<int> tempset;
	  while (i3 != tempvec.end())
	    {
	      pair<int,int> p = *i3++;
	      if (p.first != lastrk)
		{
		  if (tempset.size() > 0)
		    {
		      medres.push_back(tempset);
		      tempset = set<int>();
		    }
		  lastrk = p.first;
		}
	      tempset.insert(p.second);
	    }
	  if (tempset.size() > 0)
	    medres.push_back(tempset);
	} // end else
    }
  median_preorder = medres;
}

void electre_base::set_weights (const double *w)
{
  for (int i=0;i<get_no_crit();i++)
    crits[i]->set_weight(w[i]);
}

vector<set<int> >electre::distillate(set<int> A, bool down)
{
  set<int>::iterator it = A.begin();
  set<int>::iterator it2 = A.begin();

  set<int> D = A;

  // find max credibility
  set<int>::iterator it1 = D.begin();
  double max_cred = 0.0;
  while (it1 != D.end())
	{
	  set<int>::iterator it2 = it1;
	  it2++;
	  while (it2 != D.end())
		{
		  double c1 = cred_matrix[matloc(*it1, *it2)];
		  double c2 = cred_matrix[matloc(*it2, *it1)];
		  //		  printf ("c1 %f c2 %f\n", c1, c2);
		  if (c1 > max_cred) max_cred = c1;
		  if (c2 > max_cred) max_cred = c2;
		  it2++;
		}
	  it1++;
	}

  double cutlevel = max_cred;
  do
	{
	  // first, lower the cutting level
	  double cut_thr = cutlevel - s(cutlevel);
	  cutlevel = 0.0;

	  // then find the highest cutting level below the threshold
	  it1 = D.begin();
	  while (it1 != D.end())
		{
		  set<int>::iterator it2 = it1;
		  it2++;
		  while (it2 != D.end())
			{
			  double c1 = cred_matrix[matloc(*it1, *it2)];
			  double c2 = cred_matrix[matloc(*it2, *it1)];
			  //		  printf ("c1 %f c2 %f\n", c1, c2);
			  if ((c1 > cutlevel) && (c1 < cut_thr)) cutlevel = c1;
			  if ((c2 > cutlevel) && (c2 < cut_thr)) cutlevel = c2;
			  it2++;
			}
		  it1++;
		}
	  //	  printf ("cut_thr %f\n", cut_thr);
	  //printf ("cutlevel %f\n", cutlevel);
	  
	  if (cutlevel <= 0.0) break;

	  vector<int> quals (D.size());
	  int maxormin = calculate_qualifications(D, quals, cutlevel, down);

	  set<int> nset;
	  set<int>::iterator ai = D.begin();
	  for (unsigned int i=0;i<quals.size();i++)
		{
		  if (quals[i] == maxormin)
			nset.insert(*ai);
		  ai++;
		  //		  printf ("quals %d %d\n", i, quals[i]);
		}
	  D = nset;
	}
  while (D.size() > 1);

  set<int> diffset;
  set_difference(A.begin(), A.end(), D.begin(), D.end(), inserter(diffset, diffset.begin()));

  vector<set<int> > res;
  res.push_back(D);
  if (diffset.size() > 0) 
	{	  
	  vector<set<int> > dists = distillate(diffset, down);
	  for (unsigned int i=0;i<dists.size();i++)
		res.push_back(dists[i]);
	}
  return res;
}

vector<set<int> >electre::distillate(bool down)
{

  double lam_k = 0.0;
  double lam_k1 = 0.0;
  //  int r = 0;


  set<int> A;
  vector<set<int> > D;
  vector<set<int> > C;
  vector<int> quals(get_no_alts());

  for (int i=0;i<get_no_alts();i++)
    A.insert(i);

  do
    {
      int k = 0;
      D.resize(k+1);
      D[k] = A;
	  
      set<int>::iterator it = A.begin();
	  
      // lam_k = max(...)
      while (it != A.end())
		{
		  int ind1 = *it;
		  set<int>::iterator it2 = ++it;
		  while (it2 != A.end())
			{
			  int ind2 = *it2;
			  
			  double credval = cred_matrix[matloc(ind1, ind2)];
			  if (credval > lam_k) lam_k = credval;
			  credval = cred_matrix[matloc(ind2, ind1)];
			  if (credval > lam_k) lam_k = credval;
			  
			  it2++;
			}
		}
      // begin inner repeat
      do
		{
		  set<int>::iterator it = D[k].begin();
		  double diff = lam_k - s(lam_k);
		  lam_k1 = 0.0;
		  // lam_k+1 = max(...)
		  while (it != D[k].end())
			{
			  int ind1 = *it;
			  set<int>::iterator it2 = ++it;
			  while (it2 != D[k].end())
				{
				  int ind2 = *it2;
				  
				  double credval = cred_matrix[matloc(ind1, ind2)];
				  if (credval > lam_k1 && credval < diff) lam_k1 = credval;
				  credval = cred_matrix[matloc(ind2, ind1)];
				  if (credval > lam_k1 && credval < diff) lam_k1 = credval;
				  
				  it2++;
				}
			}
		  // calc qualification scores
		  int maxormin = calculate_qualifications(D[k], quals, lam_k, down);
		  set<int> nset;
		  for (unsigned int i=0;i<quals.size();i++)
			{
			  if (quals[i] == maxormin)
				nset.insert(i);
			}
		  D.resize(k+2);
		  D[k+1] = nset;
		  k++;
		  lam_k = lam_k1;
		}
      while (D[k].size() != 1 && lam_k != 0.0);
      C.push_back(D[k]);
      set<int> diffset;
      set_difference (A.begin(), A.end(), D[k].begin(), D[k].end(), inserter(diffset, diffset.begin()));
      A = diffset;
	  
    }
  while (A.size() != 0);
  return C;
}

void electre::calculate_concordance_matrix()
{
  minconcs.resize(get_no_alts());
  // for all pairs of alternatives
  for (int i=0;i<get_no_alts();i++)
    {
      double min = 1.0;
      for (int j=0;j<get_no_alts();j++)
	{
	  if (i == j) {conc_matrix[matloc(i,j)] = 1.0; continue; }
	  double sum = 0.0;
	  // for all criteria
	  for (int k=0;k<get_no_crit();k++)
	    {
	      double conc = concordance(k, alts[i], alts[j]);
	      sum += crits[k]->get_weight() * conc;
	    }
	  sum /= sum_w;
	  conc_matrix[matloc(i,j)] = sum;
	  if (sum < min) min = sum;
	}
      minconcs[i] = min;
    }
}

void electre::calculate_credibility_matrix()
{
  // for all pairs of alternatives
  for (int i=0;i<get_no_alts();i++)
    for (int j=0;j<get_no_alts();j++)
      {
	if (i == j) {cred_matrix[matloc(i, j)] = 1.0; continue; }
	// for all criteria
	double conc = conc_matrix[matloc(i,j)];
	double prod = conc;
	for (int k=0;k<get_no_crit();k++)
	  {
	    double disc = disc_matrices[k][matloc(i,j)];
	    if (disc > conc)
	      prod *= (1.0 - disc) / (1.0 - conc);
	  }

	cred_matrix[matloc(i, j)] = prod;
      }
}

double electre_base::concordance(int crit_index, electre_alternative *a, electre_alternative *b) const
{
  assert (crit_index >= 0 & crit_index < get_no_crit() && a && b);

  electre_criterion *c = crits[crit_index];

  double val_a = a->get_value(crit_index);
  double val_b = b->get_value(crit_index);

  double qa = q(crit_index, val_a);
  double pa = p(crit_index, val_a);

  if (!c->is_ascending())
    {
      val_a = 0.0 - val_a;
      val_b = 0.0 - val_b;
    }
  
  if ((val_b - val_a) <= qa) return 1.0;
  else if ((val_b - val_a) > pa) return 0.0;
  else return (val_a + pa - val_b) / (pa - qa);

}

double electre_base::discordance(int crit_index, electre_alternative *a, electre_alternative *b) const
{
  assert (crit_index >= 0 & crit_index < get_no_crit() && a && b);

  electre_criterion *c = crits[crit_index];

  double val_a = a->get_value(crit_index);
  double val_b = b->get_value(crit_index);

  double va = v(crit_index, val_a);
  double pa = p(crit_index, val_a);

  if (!c->is_ascending())
    {
      val_a = 0.0 - val_a;
      val_b = 0.0 - val_b;
    }


  if ((val_b - val_a) >= va) return 1.0;
  else if ((val_b - val_a) < pa) return 0.0;
  else return ((val_b - val_a + pa) / (va - pa));
}

void electre::calculate_discordance_matrices()
{
  // for all criteria
  for (int k=0;k<get_no_crit();k++)
    {
      // for all pairs of alternatives
      for (int i=0;i<get_no_alts();i++)
	for (int j=0;j<get_no_alts();j++)
	  {
	    if (i == j) {disc_matrices[k][matloc(i,j)] = 0.0; continue; }
	    double disc = 0.0;
	    if (crits[k]->has_veto())
	      disc = discordance(k, alts[i], alts[j]);
	    disc_matrices[k][matloc(i,j)] = disc;
	  }
    }
}

double electre_base::p(int crit_ind, double val) const
{
  double c_th_a = 0.0, c_th_b = 0.0;
  c_th_a = crits[crit_ind]->get_pref_th_a();
  c_th_b = crits[crit_ind]->get_pref_th_b();

  return (c_th_a * val) + c_th_b; 
}

double electre_base::q(int crit_ind, double val) const
{
  double c_th_a = 0.0, c_th_b = 0.0;
  
  c_th_a = crits[crit_ind]->get_ind_th_a();
  c_th_b = crits[crit_ind]->get_ind_th_b();

  return (c_th_a * val) + c_th_b; 
}

double electre_base::v(int crit_ind, double val) const
{
  double c_th_a = 0.0, c_th_b = 0.0;
  
  c_th_a = crits[crit_ind]->get_veto_th_a();
  c_th_b = crits[crit_ind]->get_veto_th_b();

  return (c_th_a * val) + c_th_b; 
}

int electre::calculate_qualifications (set<int> indices, vector<int> &result, double cutlevel, bool down)
{
  
  int maxqual = 0;

  set<int>::iterator it = indices.begin();

  if  (down)
    fill (result.begin(), result.end(), NOVAL_MIN);
  else // up
    fill (result.begin(), result.end(), NOVAL_MAX);

  int i = 0;
  while (it != indices.end())
    {
      int ind = *it++;
      int P = 0;
      int F = 0;

      set<int>::iterator it2 = indices.begin();
      
      while (it2 != indices.end())
		{
		  int ind2 = *it2++;
		  if (ind == ind2) continue;
		  
		  double ab = cred_matrix[matloc(ind, ind2)];
		  double ba = cred_matrix[matloc(ind2, ind)];
		  
		  if ((ab > cutlevel) &&( ab > (ba + s(ab))))
			P++;
		  if ((ba > cutlevel) && (ba > (ab + s(ba))))
			F++;
		}
      int res = P - F;
      result[i] = res;
	  i++;
      if (down)
		{
		  if (res > maxqual) maxqual = res;
		}
      else // up
		if (res < maxqual) maxqual = res;
    }
  return maxqual;
}

int electre::get_uprank(int altind) const
{
  for (unsigned int i=0;i<updist.size();i++)
    {
      set<int> s = updist[i];
      if (find(s.begin(), s.end(), altind) != s.end())
	return i+1;
    }
  assert(0);
  return -1;
}

int electre::get_preorderrank(int altind) const
{
  for (unsigned int i=0;i<median_preorder.size();i++)
    {
      set<int> s = median_preorder[i];
      if (find(s.begin(), s.end(), altind) != s.end())
	return i+1;
    }
  assert(0);
  return -1;
}

int electre::get_final_preorderrank(int altind) const
{
  for (unsigned int i=0;i<final_preorder.size();i++)
    {
      set<int> s = final_preorder[i];
      if (find(s.begin(), s.end(), altind) != s.end())
	return i+1;
    }
  assert(0);
  return -1;
}

int electre::get_downrank(int altind) const
{
  for (unsigned int i=0;i<downdist.size();i++)
    {
      set<int> s = downdist[i];
      if (find(s.begin(), s.end(), altind) != s.end())
	{
	  return i+1;
	}
    }
  assert(0);
  return -1;
}

electre_tri::electre_tri (int num_cl, int num_cr, int num_al)
  : electre_base(num_al, num_cr)
{
  assert (num_cl > 0);
  num_classes = num_cl;
  results = vector<int>(num_al);
  profiles = vector<electre_alternative *>(num_cl-1);
  class_names = vector<string>(num_cl);
}

electre_tri::~electre_tri()
{
  for (unsigned int i=0;i<profiles.size();i++)
    delete profiles[i];
}

void electre_tri::print_results (FILE *f)
{
  fprintf (stderr, "Alternative\tClass\n");
  for (int i=0;i<get_no_alts();i++)
    {
      string nam = alts[i]->get_name();
      fprintf (stderr, "%s\t%s\n", nam.c_str(), class_names[results[i]].c_str());
    }
}

void electre_base::set_measurement(int alt, int crit, double value)
{
  assert (alt >= 0);
  assert (alt < get_no_alts());
  assert (crit >= 0);
  assert (crit < get_no_crit());

  alts[alt]->set_value(crit, value);
}

void electre_base::set_weights(double *w)
{
  for (int i=0;i<get_no_crit();i++)
    crits[i]->set_weight(w[i]);
}

void electre_base::set_direct_pref_th(int crit, double val)
{
  crits[crit]->set_pref_th_a(0.0);
  crits[crit]->set_pref_th_b(val);
}

void electre_base::set_direct_indif_th(int crit, double val)
{
  crits[crit]->set_ind_th_a(0.0);
  crits[crit]->set_ind_th_b(val);
}

void electre_base::set_direct_veto_th(int crit, double val)
{
  crits[crit]->set_veto_th_a(0.0);
  crits[crit]->set_veto_th_b(val);
}

void electre_base::set_pros_pref_th(int crit, double val)
{
  crits[crit]->set_pref_th_a(val);
  crits[crit]->set_pref_th_b(0.0);
}

void electre_base::set_pros_indif_th(int crit, double val)
{
  crits[crit]->set_ind_th_a(val);
  crits[crit]->set_ind_th_b(0.0);
}

void electre_base::set_pros_veto_th(int crit, double val)
{
  crits[crit]->set_veto_th_a(val);
  crits[crit]->set_veto_th_b(0.0);
}

bool electre::are_incomparable(int a1, int a2) const
{
  assert (a1 >= 0);
  assert (a2 >= 0);
  assert (a1 < get_no_alts());
  assert (a2 < get_no_alts());
  if (a1 == a2) return false;

  int ua1 = get_uprank(a1);
  int ua2 = get_uprank(a2);

  int da1 = get_downrank(a1);
  int da2 = get_downrank(a2);

  if ((ua1 < ua2) && (da1 > da2)) return true;
  if ((ua1 > ua2) && (da1 < da2)) return true;
  return false;
}
