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

#ifndef __MEASUREMENTSTABLE_H__
#define __MEASUREMENTSTABLE_H__

#include <cassert>
#include <gtkmm/label.h>
#include <glibmm/ustring.h>
#include <gtkmm/table.h>
#include "globals.h"
#include <vector>
#include <algorithm>
#include "numberentry.h"
#include "rankcomboboxset.h"
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "changetrackable.h"

class MeasurementsTable : public Gtk::Table, public ChangeTrackable
{
 public:
  MeasurementsTable(int no_crit, int no_alts, bool unique_ranks = false, bool hide_ordinal = false, bool force_ordinal_update = true);
  virtual ~MeasurementsTable();


  void set_alternative_name(int index, const char *n)
    { set_alternative_name(index, Glib::ustring(n)); }
    
  void set_criterion_name(int index, const char *n)
    { set_criterion_name(index, Glib::ustring(n)); }
  
  void set_alternative_name(int index, Glib::ustring name);
  void set_criterion_name(int index, Glib::ustring name);
  
  double get_measurement(int altind, int critind);
  void set_measurement(double value, int altind, int critind);

  // criterion types are defined in globals.h
  void set_criterion_type(int index, crittype type);

  int get_no_alts() const;
  int get_no_crit() const;

  virtual void resize(int no_crit, int no_alts); 

  friend std::ofstream& operator<< (std::ofstream &os, MeasurementsTable &mt);
  friend std::ifstream& operator>> (std::ifstream &is, MeasurementsTable &mt);

  friend std::ostream& operator<< (std::ostream &os, MeasurementsTable &mt);

 protected:
  bool force_update;
  bool hide_ordinal;
  bool unique_ranks;
  std::vector<crittype> types;
  std::vector<RankComboBoxSet *> ranksets;
  std::vector<Gtk::Label *> labels;
  std::vector<Gtk::Label *> crit_names;
  int no_alts;
  int no_crit;
  std::map<int, std::vector<NumberEntry *> >n_entries;

  void attach(Gtk::Widget &w, int x, int y);
};

std::ofstream& operator<< (std::ofstream &os, MeasurementsTable &mt);
std::ifstream& operator>> (std::ifstream &is, MeasurementsTable &mt);

std::ostream& operator<< (std::ostream &os, MeasurementsTable &mt);


#endif
