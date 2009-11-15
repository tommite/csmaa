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
