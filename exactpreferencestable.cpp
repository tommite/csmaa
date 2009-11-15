#include "exactpreferencestable.h"

ExactPreferencesTable::ExactPreferencesTable(int n_c)
{
  assert (n_c > 0);
  nc = n_c;
  critnames.resize(nc);
  weights.resize(nc);

  critlabel.set_text("Criterion");
  wlabel.set_text("Weight");

  Gtk::Table::resize(2, nc+1);

  attach (critlabel, 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  attach (wlabel, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);

  for (int i=0;i<nc;i++)
    {
      critnames[i] = new Gtk::Label("");
      weights[i] = new NumberEntry();
      weights[i]->signal_changed().connect(sigc::mem_fun(*this, &ExactPreferencesTable::set_changed));
      attach(*critnames[i], 0, 1, i+1, i+2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
      attach(*weights[i], 1, 2, i+1, i+2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
    }
  show_all_children();
}

ExactPreferencesTable::~ExactPreferencesTable()
{
  DEL_VECTOR_CONTENTS(critnames);
  DEL_VECTOR_CONTENTS(weights);
}

void ExactPreferencesTable::set_criterion_name(int index, Glib::ustring name)
{
  assert (index >= 0);
  assert (index < nc);

  critnames[index]->set_text(name);
}

void ExactPreferencesTable::resize(int n_c)
{
  assert (n_c > 0);
  if (n_c == nc) return;

  Gtk::Table::resize(2, n_c+1);

  if (n_c > nc)
    {
      critnames.resize(n_c);
      weights.resize(n_c);
      for (int i=nc;i<n_c;i++)
	{
	  critnames[i] = new Gtk::Label("");
	  weights[i] = new NumberEntry();
	  weights[i]->signal_changed().connect(sigc::mem_fun(*this, &ExactPreferencesTable::set_changed));

	  attach (*critnames[i], 0, 1, i+1, i+2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
	  attach (*weights[i], 1, 2, i+1, i+2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
	}
      show_all_children();
    }
  else // n_c < nc
    {
      for (int i=nc-1;i>=n_c;i--)
	{
	  Gtk::Label *l = critnames[i];
	  if (l->get_parent())
	    remove (*l);
	  delete l;
	  NumberEntry *e = weights[i];
	  if (e->get_parent())
	    remove (*e);
	  delete e;
	}
      critnames.resize(n_c);
      weights.resize(n_c);
    }
  nc = n_c;
}

double ExactPreferencesTable::get_weight(int index) const
{
  assert (index >= 0);
  assert (index < nc);

  return weights[index]->get_value();
}

void ExactPreferencesTable::set_weight(int index, double val)
{
  assert (index >= 0);
  assert (index < nc);

  weights[index]->set_value(val);
}

std::ofstream& operator<< (std::ofstream &os, ExactPreferencesTable &pt)
{
  os << pt.nc << std::endl;

  for (int i=0;i<pt.nc;i++)
    {
      os << pt.critnames[i]->get_text().raw() << std::endl;
      os << pt.weights[i]->get_value() << std::endl;
    }
  return os;
}

std::ifstream& operator>> (std::ifstream &is, ExactPreferencesTable &pt)
{
  char buf[256];
  int n;
  double w;
  is >> n;

  pt.resize(n);
  for (int i=0;i<n;i++)
    {
      ws(is);
      is.getline(buf, 256);
      pt.set_criterion_name(i, buf);
      is >> w;
      pt.set_weight(i, w);
    }
  return is;
}
