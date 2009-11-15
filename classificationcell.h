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
