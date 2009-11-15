#include "ordinalpreferencestable.h"

OrdinalPreferencesTable::OrdinalPreferencesTable(int num_crit)
  : Gtk::Table(2, num_crit+1),
    boxes(num_crit, true, true)
{
  nc = num_crit;

  critnames.resize(nc);
  for (int i=0;i<num_crit;i++)
    {
      Gtk::ComboBoxText *b = boxes.get_box(i);
      attach(*b, 1, 2, i+1, i+2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
      b->show();
      critnames[i] = new Gtk::Label("");
      attach(*critnames[i], 0, 1, i+1, i+2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
      critnames[i]->show();
    }

  l1 = new Gtk::Label("Criterion");
  l2 = new Gtk::Label("Rank");

  attach(*l1, 0, 1, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  attach(*l2, 1, 2, 0, 1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);

  l1->show();
  l2->show();
}

void OrdinalPreferencesTable::set_criterion_name(int index, Glib::ustring name)
{
  assert(index >= 0);
  assert(index < nc);

  critnames[index]->set_text(name);
}

OrdinalPreferencesTable::~OrdinalPreferencesTable()
{
  delete l1;
  delete l2;
  DEL_VECTOR_CONTENTS(critnames);
}

void OrdinalPreferencesTable::resize(int num_crit)
{
  if (num_crit == nc) return;

  Gtk::Table::resize(2, num_crit+1);
  
  for (int i=nc-1;i>=num_crit;i--)
    {
      boxes.remove_last_box();
      delete critnames[i];
    }

  critnames.resize(num_crit);
  for (int i=nc;i<num_crit;i++)
    {
      Gtk::ComboBoxText *b = boxes.add_box();
      attach(*b, 1, 2, i+1, i+2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
      b->show();
      critnames[i] = new Gtk::Label("");
      attach(*critnames[i], 0, 1, i+1, i+2, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
      critnames[i]->show();
    }
  nc = num_crit;
}

std::vector<int> OrdinalPreferencesTable::get_criterion_ranks()
{
  std::vector<int> res (nc);
  
  for (int i=0;i<nc;i++)
    res[i] = boxes.get_box(i)->get_active_row_number() + 1;
  return res;
}

std::ofstream& operator<< (std::ofstream &os, OrdinalPreferencesTable &pt)
{
  os << pt.nc << std::endl;
  std::vector<int> v = pt.get_criterion_ranks();
  for (int i=0;i<pt.nc;i++)
    {
      os << pt.critnames[i]->get_text().raw() << std::endl;
      os << v[i] << std::endl;
    }
  return os;
}

std::ifstream& operator>> (std::ifstream &is, OrdinalPreferencesTable &pt)
{
  int n;
  char buf[256];
  is >> n;
  pt.resize(n);
  std::vector<int> v(n);
  for (int i=0;i<n;i++)
    {
      std::ws(is);
      is.getline(buf, 256);
      pt.set_criterion_name(i, buf);
      is >> v[i];
    }
  pt.set_criterion_ranks(v);  
  return is;
}

void OrdinalPreferencesTable::set_criterion_ranks(std::vector<int> &v)
{
  boxes.disable_actionlisteners();

  for (int i=0;i<nc;i++)
    {
      boxes.get_box(i)->set_active(v[i]-1);
    }

  boxes.enable_actionlisteners();
}
