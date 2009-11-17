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

#ifndef __ELECTRE_H__
#define __ELECTRE_H__

#include <cassert>
#include <string>
#include <algorithm>
#include <vector>
#include <set>
#include <list>

#ifdef DEBUG
#include <iostream>
#include <iterator>
#endif

#ifndef MIN
#define MIN(a,b) ( a < b ? a : b)
#endif

#ifndef MAX
#define MAX(a,b) ( a > b ? a : b)
#endif

#define NOVAL_MIN -30000
#define NOVAL_MAX 30000

using namespace std;

class electre_criterion
{
 public:
  // with veto
  electre_criterion (string &name, string &desc, double weight, bool ascending, double ind_threshold_a, double ind_threshold_b, double pref_threshold_a, double pref_threshold_b, double veto_threshold_a, double veto_threshold_b);

  // without veto
  electre_criterion (string &name, string &desc, double weight, bool ascending, double ind_threshold_a, double ind_threshold_b, double pref_threshold_a, double pref_threshold_b);

  inline const string & get_name() const { return name; }
  inline double get_weight() const { return weight; }
  inline void set_weight(double w) { weight = w; }
  inline bool is_ascending() const { return ascending; }
  inline double get_ind_th_a() const {return ind_threshold_a; }
  inline double get_ind_th_b() const {return ind_threshold_b; }
  inline double get_pref_th_a() const {return pref_threshold_a; }
  inline double get_pref_th_b() const {return pref_threshold_b; }
  inline double get_veto_th_a() const {return veto_threshold_a; }
  inline double get_veto_th_b() const {return veto_threshold_b; }

  inline void set_ind_th_a(double x) {ind_threshold_a = x; }
  inline void set_ind_th_b(double x) {ind_threshold_b = x; }
  inline void set_pref_th_a(double x) {pref_threshold_a = x; }
  inline void set_pref_th_b(double x) {pref_threshold_b = x; }
  inline void set_veto_th_a(double x) {veto_threshold_a = x; }
  inline void set_veto_th_b(double x) {veto_threshold_b = x; }

  inline bool has_veto() const { return veto; }

 protected:
  string name;
  string desc;
  double weight;
  bool ascending;
  double ind_threshold_a;
  double ind_threshold_b;
  double pref_threshold_a;
  double pref_threshold_b;
  bool veto;
  double veto_threshold_a;
  double veto_threshold_b;
};

class electre_alternative
{
 public:
  // takes responsibility of cvals-vector
  electre_alternative(string &name, string &desc, int no_crit, double *cvals);
  ~electre_alternative();

  inline const string & get_name() const { return name; }
  inline double get_value(int crit_index) const { assert(crit_index >= 0 && crit_index < no_crit); return crit_vals[crit_index]; }
  inline void set_value(int crit_index, double val) const { assert(crit_index >= 0 && crit_index < no_crit); crit_vals[crit_index] = val; }

  electre_alternative(electre_alternative &e);

  void print (FILE *f);

 protected:
  string name;
  string desc;
  int no_crit;
  double *crit_vals;
};


class electre_base
{
 public:
  electre_base(int num_alts, int num_crits);
  virtual ~electre_base();

  // sets criterion, moves responsibility for this class
  virtual void set_criterion (int index, electre_criterion *ec);
  electre_criterion * get_criterion(int index) const { return crits[index]; }

  // sets alternative, moves responsibility for this class
  virtual void set_alternative (int index, electre_alternative *ec);
  electre_alternative * get_alternative(int index) const { return alts[index]; }

  virtual void set_weights (const double *w);

  int get_no_alts() const { return alts.size(); }
  int get_no_crit() const { return crits.size(); }

  virtual void analyze() = 0;

  virtual void set_direct_pref_th(int crit, double val);
  virtual void set_direct_indif_th(int crit, double val);
  virtual void set_direct_veto_th(int crit, double val);

  virtual void set_pros_pref_th(int crit, double val);
  virtual void set_pros_indif_th(int crit, double val);
  virtual void set_pros_veto_th(int crit, double val);

  virtual void set_measurement(int alt, int crit, double value);
  virtual void set_weights(double *w);

 protected:

  double concordance(int crit_index, electre_alternative * alt_a, electre_alternative * alt_b) const;
  double discordance(int crit_index, electre_alternative * alt_a, electre_alternative * alt_b) const;

  double p(int crit_ind, double val) const;
  double q(int crit_ind, double val) const;
  double v(int crit_ind, double val) const;
  
  vector<electre_criterion *>crits;
  vector<electre_alternative *>alts;
  vector<double> minconcs;

};

class electre : public electre_base
{
 public:
  electre(int num_alts, int num_crit, double alpha, double beta);
  ~electre();

  virtual void analyze();
  void print (FILE *f);

  int get_no_upranks() const { return updist.size(); }
  int get_no_preorderranks() const { return median_preorder.size(); }
  int get_no_downranks() const { return downdist.size(); }

  int get_uprank(int altind) const;
  int get_preorderrank(int altind) const;
  int get_downrank(int altind) const;
  int get_final_preorderrank(int altind) const;

  bool are_incomparable(int altind1, int altind2) const;

  void print_preorders(FILE *f);

  void set_rule_maximin();

 protected:
  void calculate_concordance_matrix();
  void calculate_discordance_matrices();
  void calculate_credibility_matrix();

  // down = true for downward (top-down) distillations, false for upward (bottom-up) distillations
  vector<set<int> >distillate(bool down);

  vector<set<int> >distillate(set<int> s, bool down);
  
  void combine_distillations();

  int calculate_qualifications (set<int> indices, vector<int> &result, double cutlevel, bool down);

  inline double s(double val) const { return alpha * val + beta; }
  // cutting levels
  double alpha;
  double beta;

  double * conc_matrix;
  double * cred_matrix;
  vector<double *> disc_matrices;

  inline int matloc(int row, int col) const { return row * get_no_alts() + col; }

  int num_upranks;
  int num_downranks;

  double sum_w;

  double rule_maximin;

  vector<set<int> > downdist;
  vector<set<int> > updist;
  vector<set<int> > final_preorder;
  vector<set<int> > median_preorder;
};

class electre_tri : public electre_base
{
 public:
  electre_tri (int num_classes, int num_crit, int num_alts);
  ~electre_tri();

  // sets profile, moves responsibility for this class
  // profile 0 is between classes 0 and 1, etc.
  virtual void set_profile (int index, electre_alternative *ec) throw (string &);
  electre_alternative * get_profile(int index) const { return profiles[index]; }
  virtual void set_class_name(int index, const string &name);
  string get_class_name(int index) const {return class_names[index]; }

  virtual void analyze();
  void print_results(FILE *f);

  void set_lambda(double lam) {lambda = lam; }
  void set_rule(bool optimistic) {optimistic_rule = optimistic; }

  int get_class(int alt) {return results[alt]; }

  int get_no_classes() { return num_classes; }

  void print(FILE *f);
 protected:
  double lambda;
  bool optimistic_rule;
  void assign_to_class (int alt_index, int class_index);
  vector<electre_alternative *> profiles;
  vector<string> class_names;
  int num_classes;
  vector<int> results;
  double sum_w;
};

#endif
