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

#include "measurementstable.h"

MeasurementsTable::MeasurementsTable(int _no_crit, int _no_alts, bool un_r, bool hide_ord, bool forc_upd)
  : Gtk::Table(_no_crit+1, _no_alts+1)
{
  assert(_no_alts >=0);
  assert(_no_crit >=0);

  force_update = forc_upd;
  hide_ordinal = hide_ord;
  no_alts = _no_alts;
  no_crit = _no_crit;

  Gtk::Label alt_label ("Alternative");
  attach (alt_label, 0, 0);
  alt_label.show();

  types.resize(no_crit);
  std::fill (types.begin(), types.end(), TYPE_NONE);
  unique_ranks = un_r;
  ranksets.resize(no_crit);
  std::fill(ranksets.begin(), ranksets.end(), (RankComboBoxSet *) NULL);
  labels.resize(no_crit);
  std::fill(labels.begin(), labels.end(), (Gtk::Label *) NULL);
  crit_names.resize(no_crit);
  std::fill(crit_names.begin(), crit_names.end(), (Gtk::Label *) NULL);
}

MeasurementsTable::~MeasurementsTable()
{
  for (unsigned int i=0;i<ranksets.size();i++)
    {
      if (ranksets[i] != NULL)
	delete ranksets[i];
    }
  
  for (unsigned int i=0;i<labels.size();i++)
    {
      if (labels[i] != NULL)
	delete labels[i];
    }

  for (unsigned int i=0;i<crit_names.size();i++)
    {
      if (crit_names[i] != NULL)
	delete crit_names[i];
    }
}

void MeasurementsTable::set_alternative_name(int index, Glib::ustring name)
{
  assert (index >= 0);
  assert (index < get_no_alts());
  if (labels[index] == NULL) 
    {
      labels[index] = new Gtk::Label();
      attach (*labels[index], 0, index+1);
      labels[index]->show();
    }
  labels[index]->set_text(name);
}

void MeasurementsTable::set_criterion_name(int index, Glib::ustring name)
{
  assert (index >= 0);
  assert (index < get_no_crit());
  if (crit_names[index] == NULL) 
    {
      crit_names[index] = new Gtk::Label();
      attach (*crit_names[index], index+1, 0);
      crit_names[index]->show();
    }
  crit_names[index]->set_text(name);
}

void MeasurementsTable::attach (Gtk::Widget &w, int x, int y)
{
  Gtk::Table::attach(w, x, x+1, y, y+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
}

void MeasurementsTable::set_criterion_type(int index, crittype type)
{
  assert (index >= 0);
  assert (index < get_no_crit());
  
  crittype old_type = types[index];
  if (old_type == type) return;

  if (old_type == TYPE_UNIF_REAL && type == TYPE_UNIF_DISC)
    {
      std::map<int, std::vector<NumberEntry *> >::iterator iter = n_entries.find(index);
      for (int i=0;i<get_no_alts();i++)
	(*iter).second[i]->set_discrete(true);
      types[index] = type;
      return;
    }
  else if (type == TYPE_UNIF_REAL && old_type == TYPE_UNIF_DISC)
    {
      std::map<int, std::vector<NumberEntry *> >::iterator iter = n_entries.find(index);
      for (int i=0;i<get_no_alts();i++)
	(*iter).second[i]->set_discrete(false);
      types[index] = type;
      return;
    }

  if (old_type == TYPE_UNIF_REAL || old_type == TYPE_UNIF_DISC || old_type == TYPE_GAUS)
    {
      if (type == TYPE_UNIF_REAL || type == TYPE_UNIF_DISC || type == TYPE_GAUS)
	return;

      std::map<int, std::vector<NumberEntry *> >::iterator iter = n_entries.find(index);
      if (iter != n_entries.end())
	{
	  std::vector<NumberEntry *> &v = (*iter).second;
	  for (unsigned int k=0;k<v.size();k++)
	    {
	      remove (*v[k]);
	      delete v[k];
	    }
	  n_entries.erase(iter);
	}
    }
  else if (old_type == TYPE_ORD)
    {
      if (!hide_ordinal)
	{
	  RankComboBoxSet *rs = ranksets[index];
	  for (int i=0;i<rs->get_num_boxes();i++)
	    remove(*(rs->get_box(i)));
	  delete ranksets[index];
	  ranksets[index] = NULL;
	}
    }

  types[index] = type;
  if (type == TYPE_UNIF_REAL || type == TYPE_GAUS || type == TYPE_UNIF_DISC)
    {
      std::vector<NumberEntry *> ev(get_no_alts());
      for (int i=0;i<get_no_alts();i++)
	{
	  NumberEntry *e = new NumberEntry("0.0");
	  ev[i] = e;
	  attach (*e, index+1, i+1);
	  e->signal_changed().connect(sigc::mem_fun(*this, &MeasurementsTable::set_changed));
	  e->show();
	}
      n_entries[index] = ev;
    }
  else  if (type == TYPE_ORD)
    {
      if (!hide_ordinal)
	{
	  ranksets[index] = new RankComboBoxSet(get_no_alts(), unique_ranks, true, force_update);
	  for (int i=1;i<=get_no_alts();i++)
	    {
	      attach(*ranksets[index]->get_box(i-1), index+1, i);
	      ranksets[index]->get_box(i-1)->signal_changed().connect(sigc::mem_fun(*this, &MeasurementsTable::set_changed));
	      ranksets[index]->get_box(i-1)->show();
	    }
	}
    }
  else assert(0); // we should never get here
}

int MeasurementsTable::get_no_alts() const
{
  return no_alts;
}

int MeasurementsTable::get_no_crit() const
{
  return no_crit;
}

void MeasurementsTable::resize(int no_crit, int no_alts)
{
  assert(no_crit >= 1);
  assert(no_alts >= 1);
  int oc = get_no_crit();
  int oa = get_no_alts();
  Gtk::Table::resize(no_crit+1, no_alts+1);

  this->no_crit = no_crit;
  this->no_alts = no_alts;

  for (int i=oa-1;i >= no_alts;i--)
    {
      delete labels[i];
      for (unsigned int j=0;j<ranksets.size();j++)
	{
	  if (ranksets[j] != NULL)
	    {
	      Gtk::ComboBoxText *c = ranksets[j]->get_last_box();
	      remove (*c);
	      ranksets[j]->remove_last_box();
	    }
	}
      std::map<int, std::vector<NumberEntry *> >::iterator it = n_entries.begin();
      while (it != n_entries.end())
	{
	  std::vector<NumberEntry *> &v = (*it).second;
	  NumberEntry *ne = v.back();
	  remove (*ne);
	  v.pop_back();
	  it++;
	}
    }
  labels.resize(no_alts, NULL);
  for (int i=oc-1;i >= no_crit;i--)
	{
	  if (hide_ordinal && (types[i] == 2)) continue;
	  delete crit_names[i];
	  if (ranksets[i] != NULL)
	    delete ranksets[i];
	  std::map<int, std::vector<NumberEntry *> >::iterator it = n_entries.find(i);
	  if (it != n_entries.end())
	    {
	      std::vector<NumberEntry *> & v = (*it).second;
	      for (unsigned int i=0;i<v.size();i++)
		{
		  remove (*v[i]);
		  delete v[i];
		}
	      n_entries.erase(it);
	    }
	}
  crit_names.resize(no_crit, NULL);
  ranksets.resize(no_crit);
  types.resize(no_crit, TYPE_NONE);
  
  // if we have to add some components
  int addl = oc < no_crit ? oc : no_crit;
  while (oa < no_alts)
    {
      for (int i=0;i<addl;i++)
	{
	  switch (types[i])
	    {
	    case TYPE_UNIF_REAL:
	    case TYPE_UNIF_DISC:
	    case TYPE_GAUS:
	      {
		std::map<int, std::vector<NumberEntry *> >::iterator iter = n_entries.find(i);
		assert (iter != n_entries.end());
		std::vector<NumberEntry *> &v = (*iter).second;

		NumberEntry * ne = new NumberEntry("0.0");
		v.push_back(ne);
		attach(*ne, i+1, oa+1);
		ne->signal_changed().connect(sigc::mem_fun(*this, &MeasurementsTable::set_changed));
		ne->show();
	      }
	      break;
	    case TYPE_ORD:
	      {
		if (!hide_ordinal)
		  {
		    Gtk::ComboBoxText *c = ranksets[i]->add_box();
		    attach(*c, i+1, oa+1);
		    c->signal_changed().connect(sigc::mem_fun(*this, &MeasurementsTable::set_changed));
		    c->show();
		  }
	      }
	      break;
	    default:
	      assert(0);
	    }
	}
      oa++;
    }

  while (oc < no_crit)
    {
      set_criterion_type(oc, TYPE_UNIF_REAL);
      oc++;
    }
}

void MeasurementsTable::set_measurement(double val, int altind, int critind)
{
  assert (altind >= 0);
  assert (critind >= 0);
  assert (altind < no_alts);
  assert (critind < no_crit);

  int typ = types[critind];

  if (typ == TYPE_UNIF_REAL || typ == TYPE_UNIF_DISC || typ == TYPE_GAUS) // cardinal
    {
      std::map<int, std::vector<NumberEntry *> >::iterator iter = n_entries.find(critind);
      assert (iter != n_entries.end());
      (*iter).second[altind]->set_value(val);      
    }
  else // ordinal
    {
      int ind = (int) val;
      ind--;
      ranksets[critind]->get_box(altind)->set_active(ind);
    }
}

double MeasurementsTable::get_measurement (int altind, int critind)
{
  assert (altind >= 0);
  assert (critind >= 0);
  assert (altind < no_alts);
  assert (critind < no_crit);

  int typ = types[critind];

  if (typ == TYPE_UNIF_REAL || typ == TYPE_UNIF_DISC || typ == TYPE_GAUS) // cardinal
    {
      std::map<int, std::vector<NumberEntry *> >::iterator iter = n_entries.find(critind);
      assert (iter != n_entries.end());
      double val = (*iter).second[altind]->get_value();
      return val;
    }
  else // ordinal
    {
      if (hide_ordinal && (types[critind] == 2)) return 0.0;
      Glib::ustring v = ranksets[critind]->get_box(altind)->get_active_text();
      std::string s = v;
      return atof(s.c_str());
    }
}

std::ofstream & operator<< (std::ofstream &os, MeasurementsTable &mt)
{
  os << mt.unique_ranks << std::endl;
  os << mt.hide_ordinal << std::endl;
  os << mt.force_update << std::endl;
  os << mt.no_alts << std::endl;
  os << mt.no_crit << std::endl;
  
  for (int i=0;i<mt.no_crit;i++)
    os << mt.types[i] << std::endl;
  
  for (int i=0;i<mt.no_alts;i++)
    os << mt.labels[i]->get_text().raw() << std::endl;

  for (int i=0;i<mt.no_crit;i++)
    os << mt.crit_names[i]->get_text().raw() << std::endl;

  for (int i=0;i<mt.no_alts;i++)
    for (int j=0;j<mt.no_crit;j++)
      {
	if (mt.hide_ordinal && (mt.types[j] == 2)) continue;
	double val = mt.get_measurement(i, j);
	os << val << std::endl;
      }
  return os;
}

std::ifstream& operator>> (std::ifstream &is, MeasurementsTable &mt)
{
  char buf[256];
  is >> mt.unique_ranks;
  is >> mt.hide_ordinal;
  is >> mt.force_update;

  int noa, noc;
  is >> noa;
  is >> noc;

  mt.resize(noc, noa);
  
  for (int i=0;i<mt.no_crit;i++)
    {
      int t;
      std::ws(is);
      is >> t;
      mt.set_criterion_type(i, (crittype) t);
    }

  for (int i=0;i<mt.no_alts;i++)
    {
      std::ws(is);
      is.getline(buf, 256);
      mt.set_alternative_name(i, buf);
    }
  for (int i=0;i<mt.no_crit;i++)
    {
      std::string s;
      std::ws(is);
      is.getline(buf, 256);
      mt.set_criterion_name(i, buf);
    }
  for (int i=0;i<mt.no_alts;i++)
    for (int j=0;j<mt.no_crit;j++)
      {
	if (mt.hide_ordinal && (mt.types[j] == 2)) continue;
	double val;
	is >> val;
	mt.set_measurement(val, i, j);
      }

  return is;
}

std::ostream & operator<< (std::ostream &os, MeasurementsTable &mt)
{
  os << "MeasurementsTable" << std::endl;
  os << "Unique ranks: " << (mt.unique_ranks ? "on" : "off") << std::endl;
  os << "Number of alternatives: " << mt.no_alts << ", number of criteria: " << mt.no_crit << std::endl;
  
  os << "Criterion\tType" << std::endl;
  for (int i=0;i<mt.no_crit;i++)
    {
      os << mt.crit_names[i]->get_text().raw() << "\t";
      switch (mt.types[i])
	{
	case TYPE_UNIF_REAL:
	  os << "UNIFreal";
	  break;
	case TYPE_UNIF_DISC:
	  os << "UNIFdisc";
	  break;
	case TYPE_GAUS:
	  os << "GAUS";
	  break;
	case TYPE_ORD:
	  os << "ORD";
	  break;
	case TYPE_NONE:
	  break;
	}
      os << std::endl;
    }
  
  os << "Alternative";
  for (int i=0;i<mt.no_crit;i++)
    os << "\t" << mt.crit_names[i]->get_text().raw();
  os << std::endl;

  for (int i=0;i<mt.no_alts;i++)
    {
      os << mt.labels[i]->get_text().raw();
      
      for (int j=0;j<mt.no_crit;j++)
	{
	  double val = mt.get_measurement(i, j);
	  os << "\t" << val;
	}
      os << std::endl;
    }
  return os;
}
