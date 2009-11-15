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

#ifndef __CARDINALPREFERENCESTABLE_H__
#define __CARDINALPREFERENCESTABLE_H__

#include <gtkmm/table.h>
#include <gtkmm/button.h>
#include "numberentry.h"
#include <gtkmm/label.h>
#include <glibmm/ustring.h>
#include <cassert>
#include <vector>
#include "changetrackable.h"
#include "globals.h"
#include <fstream>

class CardinalPreferencesTable : public Gtk::Table, public ChangeTrackable
{
 public:
  CardinalPreferencesTable(int num_crit);
  ~CardinalPreferencesTable();

  void set_criterion_name(int index, const char *c)
    { set_criterion_name(index, Glib::ustring(c)); }
  void set_criterion_name(int index, Glib::ustring name);

  void resize(int num_crit);

  bool has_lower_bound(int index) const;
  bool has_upper_bound(int index) const;
  double get_lower_bound(int index) const;
  double get_upper_bound(int index) const;

  friend std::ostream& operator<< (std::ostream &os, CardinalPreferencesTable &pt);
  friend std::ofstream& operator<< (std::ofstream &os, CardinalPreferencesTable &pt);
  friend std::ifstream& operator>> (std::ifstream &is, CardinalPreferencesTable &pt);

 protected:
  void add_components(int row_index);
  void lbb_clicked(int index);
  void ubb_clicked(int index);

  void rlb_clicked(int index);
  void rub_clicked(int index);

  std::vector<Gtk::Label *> critnames;

  int nc;
  std::vector<std::vector<Gtk::Button *> > buttons;
  std::vector<std::vector<NumberEntry *> > bounds_fields;
  std::vector<std::vector<Gtk::Button *> > remove_buttons;
  

};

std::ostream& operator<< (std::ostream &os, CardinalPreferencesTable &pt);
std::ofstream& operator<< (std::ofstream &os, CardinalPreferencesTable &pt);
std::ifstream& operator>> (std::ifstream &is, CardinalPreferencesTable &pt);

#endif
