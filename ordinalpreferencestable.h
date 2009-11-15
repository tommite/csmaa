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
