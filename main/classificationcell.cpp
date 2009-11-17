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

#include "classificationcell.h"

ClassificationCell::ClassificationCell()
{
  Glib::RefPtr<Gdk::Colormap> colormap = get_default_colormap();
  
  white = Gdk::Color("white");
  green = Gdk::Color("green");
  lightgreen = Gdk::Color("light green");
  lightred= Gdk::Color("indian red");
  red = Gdk::Color("red");
  black = Gdk::Color("black");

  colormap->alloc_color(lightgreen);
  colormap->alloc_color(white);
  colormap->alloc_color(green);
  colormap->alloc_color(lightred);
  colormap->alloc_color(red);
  colormap->alloc_color(black);
  set_size_request(40, 30);

  value = 0.0;
}

ClassificationCell::~ClassificationCell()
{
  
}

void ClassificationCell::on_realize()
{
  Gtk::DrawingArea::on_realize();

  Glib::RefPtr<Gdk::Window> window = get_window();
  gc = Gdk::GC::create(window);
  window->set_background(white);
  window->clear();
  gc->set_foreground(black);
}

bool ClassificationCell::on_expose_event(GdkEventExpose *e)
{
  char buf[10];
  sprintf (buf, "%.0f", round(value));

  //  std::stringstream buf;
  //buf.precision(0);
  //buf.setf(std::ios::fixed, std::ios::floatfield);
  int w=0, h=0;

  Glib::RefPtr<Gdk::Window> window = get_window();
  if (value > 50.0)
    window->set_background(green);
  else if (value > 25.0)
    window->set_background(lightgreen);
  else if (value > 0.05)
    window->set_background(lightred);
  else
    window->set_background(white);

  window->clear();
  window->draw_rectangle(gc, false, 0, 0, 39, 29);

  //  buf << value;
  
  
  //  printf ("string %s\n", buf.str().c_str());

  Glib::RefPtr<Pango::Layout> p = create_pango_layout(Glib::ustring(buf));
  p->set_alignment(Pango::ALIGN_CENTER);
  p->get_pixel_size(w, h);
  int nx = (40-w) / 2 + 1;
  int ny = (30-h) / 2 + 1;
  window->draw_layout(gc, nx, ny, p);
  window->invalidate_rect(Gdk::Rectangle(0,0,40,30), false);
  
  return true;
}

void ClassificationCell::set_value(double val)
{
  value = val;
}
