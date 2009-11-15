#ifndef __THRESHOLDBOX_H__
#define __THRESHOLDBOX_H__

#include <glibmm/ustring.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include "numberentry.h"
#include <gtkmm/label.h>
#include "changetrackable.h"
#include <fstream>

class ThresholdBox : public Gtk::HBox, public ChangeTrackable
{
 public:
  ThresholdBox(bool disablable = false);
  ~ThresholdBox();

  double get_base() const;
  double get_limit() const;
  int get_type() const;

  void set_base(double val);
  void set_limit(double val);
  void set_type(int typ);

  bool is_enabled() const;

  void set_discrete(bool dis);

  enum value_types{
    DET = 0,
    UNIF = 1,
    DETPROS = 2,
    UNIFPROS = 3
  };

  friend std::ofstream& operator<< (std::ofstream &os, ThresholdBox &t);
  friend std::ifstream& operator>> (std::ifstream &is, ThresholdBox &t);

 protected:  
  void on_enablebutton_clicked();

  bool discrete;
  bool disabled;
  bool disablable;
  Gtk::Button enablebutton;
  int type;
  NumberEntry base;
  NumberEntry limit;
  Gtk::Label limitlabel;
  Gtk::Label percentlabel1;
  Gtk::Label percentlabel2;
};

std::ofstream& operator<< (std::ofstream &os, ThresholdBox &t);
std::ifstream& operator>> (std::ifstream &is, ThresholdBox &t);
#endif
