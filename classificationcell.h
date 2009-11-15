#ifndef __CLASSIFICATIONCELL_H__
#define __CLASSIFICATIONCELL_H__

#include <gtkmm/drawingarea.h>
#include <gdkmm/colormap.h>
#include <gdkmm/window.h>
#include <sstream>
#include <math.h>

class ClassificationCell : public Gtk::DrawingArea
{
 public:
  ClassificationCell();
  virtual ~ClassificationCell();

  void set_value(double val);

 protected:
  virtual void on_realize();
  virtual bool on_expose_event(GdkEventExpose* e);

  Glib::RefPtr<Gdk::GC> gc;
  Gdk::Color white, green, lightgreen, red, black, lightred;

  double value;
};

#endif
