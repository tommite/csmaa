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
