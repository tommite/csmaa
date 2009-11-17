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

#ifndef __EXACTPREFERENCESTABLE_H__
#define __EXACTPREFERENCESTABLE_H__

#include <gtkmm/table.h>
#include "numberentry.h"
#include <gtkmm/label.h>
#include <fstream>
#include <vector>
#include <assert.h>
#include <glibmm/ustring.h>
#include "changetrackable.h"
#include "globals.h"

class ExactPreferencesTable: public Gtk::Table, public ChangeTrackable
{
 public:
  ExactPreferencesTable(int num_crit);
  ~ExactPreferencesTable();

  void set_criterion_name(int index, const char *c)
    { set_criterion_name(index, Glib::ustring(c)); }
  void set_criterion_name(int index, Glib::ustring name);

  void resize(int num_crit);

  double get_weight(int index) const;
  void set_weight(int index, double val);

  friend std::ofstream& operator<< (std::ofstream &os, ExactPreferencesTable &pt);
  friend std::ifstream& operator>> (std::ifstream &is, ExactPreferencesTable &pt);

 protected:
  int nc;
  std::vector<Gtk::Label *> critnames;
  std::vector<NumberEntry *> weights;
  Gtk::Label critlabel;
  Gtk::Label wlabel;
};

std::ofstream& operator<< (std::ofstream &os, ExactPreferencesTable &pt);
std::ifstream& operator>> (std::ifstream &is, ExactPreferencesTable &pt);

#endif
