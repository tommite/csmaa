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

#ifndef __NUMBERENTRY_H__
#define __NUMBERENTRY_H__

#include <gtkmm/entry.h>
#include <glibmm/ustring.h>
#include <ctype.h>
#include <stdlib.h>
#include <sstream>
#include <math.h>

class NumberEntry : public Gtk::Entry
{
 public:
  NumberEntry(bool allow_neg = true);

  NumberEntry(const char *text, bool allow_neg = true);

  double get_value() const;

  void set_value(double val);

  void set_discrete(bool disc);

  ~NumberEntry();

 protected:
  virtual void on_changed();
  
 private:
  bool allow_neg;
  bool updating;
  Glib::ustring old_s;
  bool discrete;
};

#endif
