#ifndef __INPUTCRITERIONTABLE_H__
#define __INPUTCRITERIONTABLE_H__

#include <gtkmm/box.h>
#include <gtkmm/entry.h>
#include <gtkmm/table.h>
#include "globals.h"
#include "numberentry.h"
#include <gtkmm/comboboxtext.h>
#include <vector>
#include <cassert>
#include "changetrackable.h"
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include "thresholdbox.h"
//#include <gtkmm/tooltips.h>

using namespace std;

class InputCriterionTable : public Gtk::Table, public ChangeTrackable
{
 public:
  InputCriterionTable(int no_crit, int no_alt, bool only_cardinal = false);
  ~InputCriterionTable();
  
  crittype get_type(int index);
  int get_card_threshold_type(int index);
  Glib::ustring get_name(int index);
  
  int get_no_crit() const;
  int get_no_alts() const;
  
  void enable_thresholds();
  void disable_thresholds();
  
  void resize(int no_crit);
  
  void create_criterion();
  void remove_criterion();

  void set_no_alts(int alts);

  bool is_ascending(int index);

  bool is_threshold_percentage(int index);

  bool is_discrete(int index);

  double get_indif_threshold_base(int index);
  double get_pref_threshold_base(int index);
  double get_veto_threshold_base(int index);

  double get_indif_threshold_limit(int index);
  double get_pref_threshold_limit(int index);
  double get_veto_threshold_limit(int index);

  void set_only_cardinal(bool only_cardinal);

  friend std::ofstream& operator<< (std::ofstream &os, InputCriterionTable &t);
  friend std::ifstream& operator>> (std::ifstream &is, InputCriterionTable &t);

  friend std::ostream& operator<<(std::ostream &os, InputCriterionTable &t);

  bool has_veto(int index);
  
  virtual bool has_changed() const;
  virtual void clear_changed();

 protected:
  void attach(Gtk::Widget &w, int x, int y);

  void on_name_change(int index);
  void on_type_change(int index);
  void on_typeth_change(int index);

  Gtk::ComboBoxText * create_typechooser(int index);
  
  int no_crit;
  int no_alts;
  bool thresholds;
  bool only_cardinal;
  
  Gtk::Label *header_name;
  Gtk::Label *header_type;
  Gtk::Label *header_smin;
  Gtk::Label *header_smax;
  Gtk::Label *header_indif_th;
  Gtk::Label *header_pref_th;
  Gtk::Label *header_veto_th;
  Gtk::Label *header_type_th;
  Gtk::Label *header_ascending;
  
  vector<Gtk::Entry *> names;
  vector<Gtk::ComboBoxText *> types;
  vector<NumberEntry *> scaling_mins;
  vector<NumberEntry *> scaling_maxs;
  vector<ThresholdBox *> indif_th_cards;
  vector<ThresholdBox *> pref_th_cards;
  vector<ThresholdBox *> veto_th_cards;
  vector<Gtk::ComboBoxText *> card_th_types;
  vector<Gtk::ComboBoxText *> ascending;

  //  Gtk::Tooltips tips;

};

std::ofstream& operator<< (std::ofstream &os, InputCriterionTable &t);
std::ifstream& operator>> (std::ifstream &is, InputCriterionTable &t);

std::ostream& operator<<(std::ostream &os, InputCriterionTable &t);  

#endif
