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
