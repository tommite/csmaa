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

#ifndef __RANKCOMBOBOXSET_H__
#define __RANKCOMBOBOXSET_H__

#include <gtkmm/comboboxtext.h>
#include <glibmm/ustring.h>
#include <vector>
#include <iostream>

class RankComboBoxSet
{
 public:
  // managed = true means that this object will delete the boxes also 
  // (otherwise not, and they have to be deleted from outside)
  RankComboBoxSet(int num_boxes, bool uniqs, bool managed, bool force=true);
  ~RankComboBoxSet();
  
  Gtk::ComboBoxText * get_box(int index);

  Gtk::ComboBoxText * get_last_box()
    { return get_box(get_num_boxes()-1); }

  // adds a box to this set, returns a pointer to it
  Gtk::ComboBoxText * add_box();

  // removes the last box of the set, deleting it also if necessary
  void remove_last_box();

  int get_num_boxes() const
  { return num; }


  std::ostream& operator<< (std::ostream &os);
  std::istream& operator>> (std::istream &os);

  void disable_actionlisteners();
  void enable_actionlisteners();


 protected:
  bool force;
  int num;
  bool unique_ranks;
  bool changing;
  std::vector<Gtk::ComboBoxText *> boxes;
  bool managed;
  std::vector<sigc::connection> shandlers;
  
  void init(int num_boxes, bool unique, bool _managed);
  
  void on_change(int index);

 private:
  Gtk::ComboBoxText * create_box(int num_ranks, int index);
};


#endif
