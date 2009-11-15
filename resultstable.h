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

#ifndef __SMAATRIRESULTSTABLE_H__
#define __SMAATRIRESULTSTABLE_H__

#include "globals.h"
#include <cassert>
#include <gtkmm/label.h>
#include "classificationcell.h"
#include <gtkmm/table.h>
#include <map>

class ResultsTable : public Gtk::Table
{
 public:
  ResultsTable(int no_alt, int no_cat);
  ~ResultsTable();
  
  int get_no_alts() const;
  int get_no_cats() const;

  void set_alternative_name(int index, const char *c)
    { set_alternative_name(index, Glib::ustring(c)); }
  void set_category_name(int index, const char *c)
    { set_category_name(index, Glib::ustring(c)); }

  void set_alternative_name(int index, Glib::ustring nam);
  void set_category_name(int index, Glib::ustring nam);
  void resize(int no_alt, int no_cat);

  void set_corner_name(const char *n)
    { set_corner_name(Glib::ustring(n)); }
  void set_corner_name(Glib::ustring name);

  void set_value(int alt, int cat, double val);
  
 protected:
  void attach(Gtk::Widget &w, int x, int y);
  
  std::vector<Gtk::Label *> altnames;
  std::vector<Gtk::Label *> catnames;
  
  inline int tf_index(int alt, int cat)
    { return cat*no_alts + alt; }
  
  ClassificationCell *cells;
  int no_alts;
  int no_cats;
  Gtk::Label corner;
};

#endif
