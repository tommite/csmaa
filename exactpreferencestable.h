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
