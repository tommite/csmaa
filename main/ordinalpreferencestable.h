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

#ifndef __ORDINALPREFERENCESTABLE_H__
#define __ORDINALPREFERENCESTABLE_H__

#include "rankcomboboxset.h"
#include <gtkmm/combobox.h>
#include <gtkmm/table.h>
#include <glibmm/ustring.h>
#include <gtkmm/label.h>
#include "changetrackable.h"
#include <vector>
#include "globals.h"
#include <fstream>

class OrdinalPreferencesTable : public Gtk::Table, public ChangeTrackable
{
 public:
  OrdinalPreferencesTable(int num_crit);
  ~OrdinalPreferencesTable();

  void set_criterion_name(int index, const char *n)
    { set_criterion_name(index, Glib::ustring(n)); }
  void set_criterion_name(int index, Glib::ustring name);

  void resize(int num_crit);

  std::vector<int> get_criterion_ranks();

  friend std::ofstream& operator<< (std::ofstream &os, OrdinalPreferencesTable &pt);
  friend std::ifstream& operator>> (std::ifstream &is, OrdinalPreferencesTable &pt);

  void set_criterion_ranks(std::vector<int> &v);

 protected:
  int nc;
  RankComboBoxSet boxes;
  std::vector<Gtk::Label *> critnames;
  Gtk::Label *l1, *l2;
};

std::ofstream& operator<< (std::ofstream &os, OrdinalPreferencesTable &pt);
std::ifstream& operator>> (std::ifstream &is, OrdinalPreferencesTable &pt);

#endif
