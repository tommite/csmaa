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

#include "cardinalpreferencestable.h"

CardinalPreferencesTable::CardinalPreferencesTable(int num_crit)
{
  nc = num_crit;

  buttons.resize(2);
  remove_buttons.resize(2);
  bounds_fields.resize(2);

  buttons[0].resize(num_crit);
  buttons[1].resize(num_crit);

  remove_buttons[0].resize(num_crit);
  remove_buttons[1].resize(num_crit);

  bounds_fields[0].resize(num_crit);
  bounds_fields[1].resize(num_crit);

  Gtk::Table::resize(5,nc);

  critnames.resize(nc);

  for (int i=0;i<nc;i++)
	add_components(i);
}

void CardinalPreferencesTable::add_components(int i)
{
  Gtk::Button *b = new Gtk::Button("Add lowerbound");
  buttons[0][i] = b;
  attach(*b, 1, 3, i, i+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  b->show();
  b->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &CardinalPreferencesTable::lbb_clicked), i));
  b = new Gtk::Button("Add upperbound");
  buttons[1][i] = b;
  attach(*b, 3, 5, i, i+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  b->show();
  b->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &CardinalPreferencesTable::ubb_clicked), i));

  b = new Gtk::Button("Remove lowerbound");
  remove_buttons[0][i] = b;
  b->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &CardinalPreferencesTable::rlb_clicked), i));
  b = new Gtk::Button("Remove upperbound");
  remove_buttons[1][i] = b;
  b->signal_clicked().connect(sigc::bind<int>(sigc::mem_fun(*this, &CardinalPreferencesTable::rub_clicked), i));

  NumberEntry *n = new NumberEntry();
  bounds_fields[0][i] = n;
  n->signal_changed().connect(sigc::mem_fun(*this, &CardinalPreferencesTable::set_changed));
  n = new NumberEntry();
  bounds_fields[1][i] = n;
  n->signal_changed().connect(sigc::mem_fun(*this, &CardinalPreferencesTable::set_changed));

  critnames[i] = new Gtk::Label("");
  attach(*critnames[i], 0, 1, i, i+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  critnames[i]->show();
}

void CardinalPreferencesTable::lbb_clicked(int i)
{
  remove(*buttons[0][i]);
  attach(*bounds_fields[0][i], 1, 2, i, i+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  bounds_fields[0][i]->show();
  attach(*remove_buttons[0][i], 2, 3, i, i+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  remove_buttons[0][i]->show();
  set_changed();
}

void CardinalPreferencesTable::rlb_clicked(int i)
{
  remove(*remove_buttons[0][i]);
  remove(*bounds_fields[0][i]);
  attach(*buttons[0][i], 1, 3, i, i+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  buttons[0][i]->show();
  set_changed();
}

void CardinalPreferencesTable::rub_clicked(int i)
{
  remove(*remove_buttons[1][i]);
  remove(*bounds_fields[1][i]);
  attach(*buttons[1][i], 3, 5, i, i+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  buttons[1][i]->show();
  set_changed();
}

void CardinalPreferencesTable::ubb_clicked(int i)
{
  remove(*buttons[1][i]);
  attach(*bounds_fields[1][i], 3, 4, i, i+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  bounds_fields[1][i]->show();
  attach(*remove_buttons[1][i], 4, 5, i, i+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
  remove_buttons[1][i]->show();
  set_changed();
}

void CardinalPreferencesTable::set_criterion_name(int index, Glib::ustring name)
{
  assert(index >= 0);
  assert(index < nc);

  critnames[index]->set_text(name);
}

CardinalPreferencesTable::~CardinalPreferencesTable()
{
  DEL_VECTOR_CONTENTS(critnames);
  for (unsigned int i=0;i<buttons[0].size();i++)
	{
	  //	  remove(*buttons[0][i]);
	  delete buttons[0][i];
	  //	  remove(*buttons[1][i]);
	  delete buttons[1][i];
	}

  for (unsigned int i=0;i<remove_buttons[0].size();i++)
	{
	  //	  remove(*buttons[0][i]);
	  delete remove_buttons[0][i];
	  //	  remove(*buttons[1][i]);
	  delete remove_buttons[1][i];
	}
  for (unsigned int i=0;i<bounds_fields[0].size();i++)
	{
	  //remove(*bounds_fields[0][i]);
	  delete bounds_fields[0][i];
	  //remove(*bounds_fields[0][i]);
	  delete bounds_fields[1][i];
	}
}

void CardinalPreferencesTable::resize(int num_crit)
{
  if (num_crit == nc) return;

  Gtk::Table::resize(5, num_crit);

  for (int i=nc-1;i>=num_crit;i--)
    {
      delete buttons[0][i];
      delete buttons[1][i];
      delete remove_buttons[0][i];
      delete remove_buttons[1][i];
      delete bounds_fields[0][i];
      delete bounds_fields[1][i];
      delete critnames[i];
    }

  buttons[0].resize(num_crit);
  buttons[1].resize(num_crit);
  remove_buttons[0].resize(num_crit);
  remove_buttons[1].resize(num_crit);
  bounds_fields[0].resize(num_crit);
  bounds_fields[1].resize(num_crit);
  critnames.resize(num_crit);

  for (int i=nc;i<num_crit;i++)
    add_components(i);

  nc = num_crit;
}

bool CardinalPreferencesTable::has_lower_bound(int index) const
{
  assert (index >= 0);
  assert (index < nc);

  return (bounds_fields[0][index]->get_parent() != NULL);
}

bool CardinalPreferencesTable::has_upper_bound(int index) const
{
  assert (index >= 0);
  assert (index < nc);

  return (bounds_fields[1][index]->get_parent() != NULL);
}

double CardinalPreferencesTable::get_lower_bound(int index) const
{
  assert (index >= 0);
  assert (index < nc);
  assert(has_lower_bound(index));
  
  return bounds_fields[0][index]->get_value();
}

double CardinalPreferencesTable::get_upper_bound(int index) const
{
  assert (index >= 0);
  assert (index < nc);
  assert(has_upper_bound(index));
  
  return bounds_fields[1][index]->get_value();
}

std::ofstream& operator<< (std::ofstream &os, CardinalPreferencesTable &pt)
{
  os << pt.nc << std::endl;
  for (int i=0;i<pt.nc;i++)
    {
      os << pt.critnames[i]->get_text().raw() << std::endl;
      os << pt.has_lower_bound(i) << std::endl;
      if (pt.has_lower_bound(i))
	os << pt.get_lower_bound(i) << std::endl;
      os << pt.has_upper_bound(i) << std::endl;
      if (pt.has_upper_bound(i))
	os << pt.get_upper_bound(i) << std::endl;
    }
  return os;
}

std::ifstream& operator>> (std::ifstream &is, CardinalPreferencesTable &pt)
{
  int n;
  char buf[256];
  is >> n;
  pt.resize(n);

  for (int i=0;i<n;i++)
    {
      bool has_l, has_u;
      std::ws(is);
      is.getline(buf, 256);
      pt.set_criterion_name(i, buf);
      is >> has_l;
      if (has_l)
	{
	  if (!pt.has_lower_bound(i))
	    pt.lbb_clicked(i);
	  double b;
	  is >> b;
	  pt.bounds_fields[0][i]->set_value(b);
	}
      else
	{
	  if (pt.has_lower_bound(i))
	    pt.rlb_clicked(i);
	}
      is >> has_u;
      if (has_u)
	{
	  if (!pt.has_upper_bound(i))
	    pt.ubb_clicked(i);
	  double b;
	  is >> b;
	  pt.bounds_fields[1][i]->set_value(b);
	}
      else
	{
	  if (pt.has_upper_bound(i))
	    pt.rub_clicked(i);
	}
    }
  return is;
}

std::ostream& operator<< (std::ostream &os, CardinalPreferencesTable &pt)
{
  os << "CardinalPreferencesTable" << std::endl;
  os << "Number of criteria: " << pt.nc << std::endl;
  os << "Criterion\tLower bound\tUpper bound\n";
  for (int i=0;i<pt.nc;i++)
    {
      os << pt.critnames[i]->get_text().raw() << "\t";
      if (pt.has_lower_bound(i))
	os << pt.get_lower_bound(i);
      else
	os << "-";
      os << "\t";
      if (pt.has_upper_bound(i))
	os << pt.get_upper_bound(i);
      else
	os << "-";
      os << "\n";
    }
  return os;
}
