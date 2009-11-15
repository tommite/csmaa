#include "resultstable.h"

ResultsTable::ResultsTable(int _no_alt, int _no_cat)
  : Gtk::Table (_no_cat+1, _no_alt+1)
{
  no_alts = _no_alt;
  no_cats = _no_cat;
  cells = NULL;

  altnames.resize(no_alts);
  catnames.resize(no_cats);
  for (int i=0;i<no_alts;i++)
    {
      altnames[i] = new Gtk::Label("");
      attach(*altnames[i], 0, i+1);
    }
  for (int i=0;i<no_cats;i++)
    {
      catnames[i] = new Gtk::Label("");
      attach(*catnames[i], i+1, 0);
    }

  cells = new ClassificationCell[no_alts*no_cats];

  for (int i=0;i<no_alts;i++)
    for (int j=0;j<no_cats;j++)
      {
	attach (cells[tf_index(i, j)], j+1, i+1);
	cells[tf_index(i,j)].show();
      }
}

ResultsTable::~ResultsTable()
{
  if (cells) delete[] cells;
  DEL_VECTOR_CONTENTS(altnames);
  DEL_VECTOR_CONTENTS(catnames);
}

int ResultsTable::get_no_alts() const
{
  return no_alts;
}

int ResultsTable::get_no_cats() const
{
  return no_cats;
}

void ResultsTable::resize(int noa, int noc)
{
  assert (noa >= 1);
  assert (noc >= 1);
  Gtk::Table::resize(noc+1, noa+1);

  for (int i=no_alts-1;i>=noa;i--)
    {
      remove(*altnames[i]);
      delete (altnames[i]);
    }
  
  altnames.resize(noa);

  for (int i=no_alts;i<noa;i++)
    {
      altnames[i] = new Gtk::Label("");
      attach(*altnames[i], 0, i+1);
      altnames[i]->show();
    }

  for (int i=no_cats-1;i>=noc;i--)
    {
      remove(*catnames[i]);
      delete (catnames[i]);
    }
  
  catnames.resize(noc);

  for (int i=no_cats;i<noc;i++)
    {
      catnames[i] = new Gtk::Label("");
      attach(*catnames[i], i+1, 0);
      catnames[i]->show();
    }

  int its = no_alts * no_cats;
  if (cells)
    {
      for (int i=0;i < its;i++)
	remove (cells[i]);
      delete[] cells;
    }
  no_alts = noa;
  no_cats = noc;
  cells = new ClassificationCell[noa*noc];
  
  for (int i=0;i<no_alts;i++)
    for (int j=0;j<no_cats;j++)
      {
	attach (cells[tf_index(i, j)], j+1, i+1);
	cells[tf_index(i,j)].show();
      }
}

void ResultsTable::set_alternative_name(int index, Glib::ustring nam)
{
  assert(index >= 0);
  assert(index < get_no_alts());

  altnames[index]->set_text(nam);
  altnames[index]->queue_draw();
}

void ResultsTable::set_corner_name(Glib::ustring nam)
{
  corner.set_text(nam);
  if (corner.get_parent() == 0)
    attach(corner, 0, 0);
}

void ResultsTable::set_category_name(int index, Glib::ustring nam)
{
  assert(index >= 0);
  assert(index < get_no_cats());

  catnames[index]->set_text(nam);
  catnames[index]->queue_draw();
}

void ResultsTable::attach (Gtk::Widget &w, int x, int y)
{
  Gtk::Table::attach(w, x, x+1, y, y+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
}

void ResultsTable::set_value(int alt, int cat, double val)
{
  cells[tf_index(alt, cat)].set_value(val);
  cells[tf_index(alt, cat)].queue_draw();
}
