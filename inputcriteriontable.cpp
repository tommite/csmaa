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

#include "inputcriteriontable.h"

void InputCriterionTable::resize(int no_c)
{
  if (no_c == no_crit) return;
  
  if (no_c < no_crit)
    {
      while (no_crit > no_c)
	remove_criterion();
    }  
  else
    {
      while (no_crit < no_c)
	create_criterion();
    }
}

void InputCriterionTable::remove_criterion()
{
  assert (no_crit > 0);

  Gtk::Entry *n = names.back();
  names.pop_back();
  remove (*n);
  delete n;

  Gtk::ComboBoxText *t = types.back();
  types.pop_back();
  remove (*t);
  delete t;

  NumberEntry *n1 = scaling_mins.back();
  scaling_mins.pop_back();
  if (n1->get_parent())
    remove (*n1);
  delete n1;

  NumberEntry *n2 = scaling_maxs.back();
  scaling_maxs.pop_back();
  if (n2->get_parent())
    remove (*n2);
  delete n2;
  
  ThresholdBox *n3 = indif_th_cards.back();
  indif_th_cards.pop_back();
  if (n3->get_parent())
    remove (*n3);
  delete n3;
  
  ThresholdBox *n4 = pref_th_cards.back();
  pref_th_cards.pop_back();
  if (n4->get_parent())
    remove (*n4);
  delete n4;

  Gtk::ComboBoxText *cc = card_th_types.back();
  card_th_types.pop_back();
  if (cc->get_parent())
    remove (*cc);
  delete cc;

  ThresholdBox *n5 = veto_th_cards.back();
  veto_th_cards.pop_back();
  if (n5->get_parent())
    remove (*n5);
  delete n5;

  Gtk::ComboBoxText *asc = ascending.back();
  ascending.pop_back();
  if (asc->get_parent())
    remove (*asc);
  delete asc;

  if (thresholds)
    Gtk::Table::resize(7, no_crit);
  else
    Gtk::Table::resize(5, no_crit);

  no_crit--;

  set_changed();
}

bool InputCriterionTable::has_changed() const
{
  if (ChangeTrackable::has_changed()) return true;
  for (int i=0;i<no_crit;i++)
    {
      if (indif_th_cards[i]->has_changed()) return true;
      if (pref_th_cards[i]->has_changed()) return true;
      if (veto_th_cards[i]->has_changed()) return true;
    }
  return false;
}

void InputCriterionTable::clear_changed()
{
  ChangeTrackable::clear_changed();
  for (int i=0;i<no_crit;i++)
    {
      indif_th_cards[i]->clear_changed();
      pref_th_cards[i]->clear_changed();
      veto_th_cards[i]->clear_changed();
    }
}

InputCriterionTable::InputCriterionTable(int _no_crit, int _no_alts, bool _only_cardinal)
  : Gtk::Table(4, 1)
{
  no_crit = 0;
  no_alts = 0;
  only_cardinal = _only_cardinal;

  thresholds = false;

  // create header
  header_name = new Gtk::Label();
  header_type = new Gtk::Label();
  header_smin = new Gtk::Label();
  header_smax = new Gtk::Label();
  header_indif_th = new Gtk::Label();
  header_pref_th = new Gtk::Label();
  header_veto_th = new Gtk::Label();
  header_ascending = new Gtk::Label();
  header_type_th = new Gtk::Label();

  header_name->set_text ("Name");
  header_type->set_text ("Type");
  header_smin->set_text ("Scaling MIN");
  header_smax->set_text ("Scaling MAX");
  header_indif_th->set_text ("Indifference TH");
  header_pref_th->set_text ("Preference TH");
  header_veto_th->set_text ("Veto TH");
  header_ascending->set_text("Direction");
  header_type_th->set_text("Threshold type");

  attach (*header_name, 0, 0);
  attach (*header_type, 1, 0);
  attach (*header_ascending, 2, 0);
  attach (*header_smin, 3, 0);
  attach (*header_smax, 4, 0);
  header_name->show();
  header_type->show();
  header_ascending->show();
  header_smin->show();
  header_smax->show();

  /*
  tips.set_tip(*header_name, "Name of the criterion");
  tips.set_tip(*header_type, "Type of the criterion.\nUNIF = Cardinal criterion, uniformly distributed values (e.g. upper and lower bounds)\nGAUS = Cardinal criterion, Gaussian distributed values\nOrdinal = Ordinal criterion");
  tips.set_tip(*header_smin, "Scale minimum value for the criterion (worst value)");
  tips.set_tip(*header_smax, "Scale maximum value for the criterion (best value)");
  tips.set_tip(*header_indif_th, "Indifference threshold");
  tips.set_tip(*header_pref_th, "Preference threshold");
  tips.set_tip(*header_veto_th, "Veto threshold");
  tips.set_tip(*header_ascending, "Whether the criterion scale is ascending or descending\nASC = Ascending, higher values are better\nDESC = Descending, lower values are better");
  tips.set_tip(*header_type_th, "Threshold type\nABS, DET = Absolute value, deterministic (no uncertainty\nABS, UNCERT = Absolute uncertain value\nPROS, DET = Value is percentage of the measurement, deterministic (no uncertainty\nPROS, UNCERT = Uncertain value, percentage of the measurement");
  */
  
  while (no_crit < _no_crit)
    create_criterion();
  set_no_alts(_no_alts);
}

void InputCriterionTable::attach(Gtk::Widget &w, int x, int y)
{
  Gtk::Table::attach(w, x, x+1, y, y+1, Gtk::SHRINK, Gtk::SHRINK, 5, 5);
}

InputCriterionTable::~InputCriterionTable()
{
  delete header_name;
  delete header_type;
  delete header_smin;
  delete header_smax;
  delete header_indif_th;
  delete header_pref_th;
  delete header_veto_th;
  delete header_ascending;
  delete header_type_th;

  while (no_crit > 0)
    remove_criterion();
}

crittype InputCriterionTable::get_type(int index)
{
  assert(index >= 0);
  assert(index < no_crit);
  return (crittype) types[index]->get_active_row_number();
}

int InputCriterionTable::get_card_threshold_type(int index)
{
  assert(index >= 0);
  assert(index < no_crit);
  return card_th_types[index]->get_active_row_number();
}

Glib::ustring InputCriterionTable::get_name(int index)
{
  assert(index >= 0);
  assert(index < no_crit);
  return names[index]->get_text();
}

int InputCriterionTable::get_no_crit() const
{
  return no_crit;
}

void InputCriterionTable::on_type_change(int index)
{
  assert(index >= 0);
  assert(index < no_crit);

  crittype newrow = (crittype) types[index]->get_active_row_number();

  if (newrow == TYPE_UNIF_DISC)
    {
      scaling_mins[index]->set_discrete(true);
      scaling_maxs[index]->set_discrete(true);
      indif_th_cards[index]->set_discrete(true);
      pref_th_cards[index]->set_discrete(true);
      veto_th_cards[index]->set_discrete(true);	

      card_th_types[index]->remove_text("PROS, DET");
      card_th_types[index]->remove_text("PROS, UNCERT");
    }
  else if (newrow == TYPE_UNIF_REAL)
    {
      scaling_mins[index]->set_discrete(false);
      scaling_maxs[index]->set_discrete(false);
      indif_th_cards[index]->set_discrete(false);
      pref_th_cards[index]->set_discrete(false);
      veto_th_cards[index]->set_discrete(false);

      card_th_types[index]->remove_text("PROS, DET");
      card_th_types[index]->remove_text("PROS, UNCERT");
      card_th_types[index]->append_text("PROS, DET");
      card_th_types[index]->append_text("PROS, UNCERT");	
    }

  if (!thresholds)
    {
      if (newrow == TYPE_ORD) // not a cardinal criterion
	{
	  scaling_mins[index]->hide();
	  scaling_maxs[index]->hide();
	  scaling_mins[index]->set_child_visible(false);
	  scaling_maxs[index]->set_child_visible(false);
	  ascending[index]->hide();
	  ascending[index]->set_child_visible(false);
	}
      else // is cardinal
	{
	  scaling_mins[index]->show();
	  scaling_maxs[index]->show();
	  scaling_mins[index]->set_child_visible(true);
	  scaling_maxs[index]->set_child_visible(true);
	  ascending[index]->show();
	  ascending[index]->set_child_visible(true);
	}
    }
  else    
    {
      if (newrow == TYPE_ORD) // ordinal
	{
	  if (indif_th_cards[index]->get_parent() != 0)
	    {
	      remove (*indif_th_cards[index]);
	      remove (*pref_th_cards[index]);
	      remove (*veto_th_cards[index]);
	      remove (*card_th_types[index]);
	    }
	  ascending[index]->hide();
	  ascending[index]->set_child_visible(false);
	}
      else
	{
	  if (indif_th_cards[index]->get_parent() == 0)
	    {
	      attach(*indif_th_cards[index], 3, index+1);
	      attach(*pref_th_cards[index], 4, index+1);
	      attach(*veto_th_cards[index], 5, index+1);
	      attach(*card_th_types[index], 6, index+1);
	      indif_th_cards[index]->show();
	      pref_th_cards[index]->show();
	      veto_th_cards[index]->show();
	    }
	  ascending[index]->show();
	  ascending[index]->set_child_visible(true);
	}
    }
  set_changed();
}

void InputCriterionTable::on_typeth_change(int index)
{
  assert(index >= 0);
  assert(index < no_crit);
  assert(thresholds);

  int newrow = card_th_types[index]->get_active_row_number();

  indif_th_cards[index]->set_type(newrow);
  pref_th_cards[index]->set_type(newrow);
  veto_th_cards[index]->set_type(newrow);

  set_changed();
}

void InputCriterionTable::on_name_change(int index)
{ 
  set_changed(); 
}

void InputCriterionTable::create_criterion()
{
  char textbuf[20];
  no_crit++;
  sprintf (textbuf, "Criterion %d", no_crit);

  if (thresholds)
    Gtk::Table::resize(7, no_crit+1);
  else
    Gtk::Table::resize(5, no_crit+1);

  // name
  Gtk::Entry * nam = new Gtk::Entry();
  nam->set_width_chars(ENTRYWIDTH);
  nam->set_max_length(MAXNAMELENGTH);
  nam->set_text(Glib::ustring(textbuf));
  nam->signal_changed().connect(sigc::bind<int>(sigc::mem_fun(*this, &InputCriterionTable::on_name_change), no_crit-1));
  names.push_back(nam);
  attach (*nam, 0, no_crit);
  nam->show();

  // type
  Gtk::ComboBoxText * typ = create_typechooser(no_crit-1);
  typ->set_active(0);
  typ->signal_changed().connect(sigc::bind<int>(sigc::mem_fun(*this, &InputCriterionTable::on_type_change), no_crit-1));
  types.push_back(typ);
  attach (*typ, 1, no_crit);
  typ->show();

  // scalings
  NumberEntry *smin = new NumberEntry();
  smin->set_text("0.0");
  scaling_mins.push_back(smin);
  NumberEntry *smax = new NumberEntry();
  smax->set_text("100.0");
  scaling_maxs.push_back(smax);
  smin->signal_changed().connect(sigc::mem_fun(*this, &InputCriterionTable::set_changed));
  smax->signal_changed().connect(sigc::mem_fun(*this, &InputCriterionTable::set_changed));

  // thresholds
  ThresholdBox * itc = new ThresholdBox();
  indif_th_cards.push_back(itc);
  ThresholdBox * ptc = new ThresholdBox();
  pref_th_cards.push_back(ptc);
  ThresholdBox * vtc = new ThresholdBox(true);
  veto_th_cards.push_back(vtc);
  Gtk::ComboBoxText *cc = new Gtk::ComboBoxText();
  cc->append_text("ABS, DET");
  cc->append_text("ABS, UNCERT");
  cc->append_text("PROS, DET");
  cc->append_text("PROS, UNCERT");
  cc->set_active(0);
  card_th_types.push_back(cc);
  
  cc->signal_changed().connect(sigc::bind<int>(sigc::mem_fun(*this, &InputCriterionTable::on_typeth_change), no_crit-1));

  Gtk::ComboBoxText *as = new Gtk::ComboBoxText();
  as->append_text("ASC");
  as->append_text("DESC");
  as->set_active(0);
  as->signal_changed().connect(sigc::mem_fun(*this, &InputCriterionTable::set_changed));
  ascending.push_back(as);
  attach(*as, 2, no_crit);
  as->show();

  if (thresholds)
    {
      attach(*itc, 3, no_crit);
      attach(*ptc, 4, no_crit);
      attach(*vtc, 5, no_crit);
      attach(*cc, 6, no_crit);
      itc->show();
      ptc->show();
      vtc->show();
      cc->show();
    }
  else
    {
      attach (*smin, 3, no_crit);
      attach (*smax, 4, no_crit);
      smin->show();
      smax->show();
    }
}

void InputCriterionTable::set_no_alts(int alts)
{
  assert (alts >= 0);
  no_alts = alts;
}


Gtk::ComboBoxText * InputCriterionTable::create_typechooser(int index)
{
  Gtk::ComboBoxText *typechooser = new Gtk::ComboBoxText();
  typechooser->append_text("UNIF, real");
  typechooser->append_text("UNIF, discrete");
  typechooser->append_text("GAUS");
  if (!only_cardinal)
    typechooser->append_text("Ordinal");
  typechooser->set_active(0);
  typechooser->signal_changed().connect(sigc::bind<int>(sigc::mem_fun(*this, &InputCriterionTable::on_type_change), index));
  return typechooser;
}

void InputCriterionTable::enable_thresholds()
{
  if (thresholds) return;
  thresholds = true;

  remove (*header_smin);
  remove (*header_smax);

  for (int i=0;i<get_no_crit();i++)
    {
      remove (*scaling_mins[i]);
      remove (*scaling_maxs[i]);
    }

  Gtk::Table::resize(7, no_crit+1);

  attach(*header_indif_th, 3, 0);
  attach(*header_pref_th, 4, 0);
  attach(*header_veto_th, 5, 0);
  attach(*header_type_th, 6, 0);
  header_indif_th->show();
  header_pref_th->show();
  header_veto_th->show();
  header_type_th->show();

  for (int i=0;i<no_crit;i++)
    {
      if (get_type(i) < 2)
	{
	  attach(*indif_th_cards[i], 3, i+1);
	  attach(*pref_th_cards[i], 4, i+1);
	  attach(*veto_th_cards[i], 5, i+1);
	  attach(*card_th_types[i], 6, i+1);
	  indif_th_cards[i]->show();
	  pref_th_cards[i]->show();
	  veto_th_cards[i]->show();
	  card_th_types[i]->show();
	}
    }
}

void InputCriterionTable::disable_thresholds()
{
  if (!thresholds) return;
  remove (*header_indif_th);
  remove (*header_pref_th);
  remove (*header_veto_th);
  remove (*header_type_th);
  for (int i=0;i<no_crit;i++)
    {
      if (get_type(i) == TYPE_ORD)
	{
	  remove (*indif_th_cards[i]);
	  remove (*pref_th_cards[i]);
	  remove (*veto_th_cards[i]);
	  remove (*card_th_types[i]);
	}
    }
  Gtk::Table::resize(5, no_crit+1);
  
  attach (*header_smin, 3, 0);
  header_smin->show();
  attach (*header_smax, 4, 0);
  header_smax->show();
  for (int i=0;i<no_crit;i++)
    {
      if (get_type(i) == TYPE_ORD)
	{
	  attach(*scaling_mins[i], 3, i+1);
	  attach(*scaling_maxs[i], 4, i+1);
	  scaling_mins[i]->show();
	  scaling_maxs[i]->show();
	}
    }
  thresholds = false;
}

double InputCriterionTable::get_pref_threshold_base(int index)
{
  assert (thresholds);
  if (get_type(index) == TYPE_ORD) // ordinal
    return 0.0;
  else // cardinal
    return pref_th_cards[index]->get_base();
}

double InputCriterionTable::get_indif_threshold_base(int index)
{
  assert (thresholds);
  if (get_type(index)== TYPE_ORD) // ordinal
    return 0.0;
  else // cardinal
    return indif_th_cards[index]->get_base();
}

double InputCriterionTable::get_veto_threshold_base(int index)
{
  assert (thresholds);
  if (get_type(index) == TYPE_ORD) // ordinal
    return 0.0;
  else // cardinal
    return veto_th_cards[index]->get_base();
}

double InputCriterionTable::get_pref_threshold_limit(int index)
{
  assert (thresholds);
  if (get_type(index) == TYPE_ORD) // ordinal
    return 0.0;
  else // cardinal
    return pref_th_cards[index]->get_limit();
}


double InputCriterionTable::get_indif_threshold_limit(int index)
{
  assert (thresholds);
  if (get_type(index) == TYPE_ORD) // ordinal
    return 0.0;
  else // cardinal
    return indif_th_cards[index]->get_limit();
}


double InputCriterionTable::get_veto_threshold_limit(int index)
{
  assert (thresholds);
  if (get_type(index) == TYPE_ORD) // ordinal
    return 0.0;
  else // cardinal
    return veto_th_cards[index]->get_limit();
}


void InputCriterionTable::set_only_cardinal(bool c)
{
  if (c == only_cardinal) return;
      
  for (unsigned int i=0;i<types.size();i++)
    {
      if (c == false)
	types[i]->append_text("Ordinal");
      else
	{
	  if (get_type(i) == TYPE_ORD)
	    types[i]->set_active(0);
	  types[i]->remove_text("Ordinal");
	}
    }
  only_cardinal = c;
}

std::ofstream& operator<< (std::ofstream &os, InputCriterionTable &it)
{
  os << it.no_crit << std::endl;
  os << it.no_alts << std::endl;
  os << it.thresholds << std::endl;
  os << it.only_cardinal << std::endl;
  
  for (int i=0;i<it.no_crit;i++)
    {
      os << it.get_name(i).raw() << std::endl;
      os << it.get_type(i) << std::endl;
      os << it.is_ascending(i) << std::endl;
      os << it.scaling_mins[i]->get_value() << std::endl;
      os << it.scaling_maxs[i]->get_value() << std::endl;

      os << it.card_th_types[i]->get_active_row_number() << std::endl;

      os << *it.indif_th_cards[i] << std::endl;
      os << *it.pref_th_cards[i] << std::endl;
      os << *it.veto_th_cards[i] << std::endl;
    }
  return os;
}

std::ifstream& operator>> (std::ifstream &is, InputCriterionTable &it)
{
  int a = 0, c = 0;
  char buf[256];
  is >> c;
  is >> a;

  it.resize(c);
  it.set_no_alts(a);

  bool th = false;
  is >> th;
  if (th) it.enable_thresholds();
  else it.disable_thresholds();
  bool oc = false;
  is >> oc;
  it.set_only_cardinal(oc);
  
  for (int i=0;i<c;i++)
    {
      int typ;
      double smin, smax;
      int thtyp;
      bool as;
      ws(is);
      is.getline (buf, 256);
      std::string nam (buf);
      is >> typ;
      is >> as;
      is >> smin;
      is >> smax;

      is >> thtyp;

      is >> *it.indif_th_cards[i];
      is >> *it.pref_th_cards[i];
      is >> *it.veto_th_cards[i];

      it.names[i]->set_text(nam);
      it.types[i]->set_active(typ);
      if (as)
	it.ascending[i]->set_active(0);
      else
	it.ascending[i]->set_active(1);
      it.scaling_mins[i]->set_value(smin);
      it.scaling_maxs[i]->set_value(smax);

      it.card_th_types[i]->set_active(thtyp);
    }      
  return is;
}

std::ostream& operator<< (std::ostream &os, InputCriterionTable &it)
{
  os << "InputCriterionTable" << std::endl;
  os << it.no_crit << " criteria, " << it.no_alts << " alternatives.";
  os << " Thresholds: " << (it.thresholds ? "enabled" : "disabled") << ".";
  os << " Only cardinal criteria: " << (it.only_cardinal ? "on" : "off") << std::endl;
  os << "Name\tType\tScalingMIN\tScalingMAX\tAscending\n";
  os << "\tIndifTH_Card\tPrefTH_Card\tVetoTH_Card\n";
  for (int i=0;i<it.no_crit;i++)
    {
      os << it.get_name(i).raw() << "\t";
      if (it.get_type(i) == TYPE_UNIF_REAL) os << "UNIF,real";
      else if (it.get_type(i) == TYPE_UNIF_DISC) os << "UNIF,disc";
      else if (it.get_type(i) == TYPE_GAUS) os << "GAUS";
      else if (it.get_type(i) == TYPE_ORD) os << "ORD";
      os << "\t";
      os << it.scaling_mins[i]->get_value() << "\t";
      os << it.scaling_maxs[i]->get_value() << "\t";
      os << (it.is_ascending(i) ? "Yes" : "No") << "\n";
      os << "\t\t";
      os << it.indif_th_cards[i]->get_base() << " +- " << it.indif_th_cards[i]->get_limit() << "\t";
      os << it.pref_th_cards[i]->get_base() << " +- " << it.pref_th_cards[i]->get_limit() << "\t";
      os << it.veto_th_cards[i]->get_base() << " +- " << it.veto_th_cards[i]->get_limit() << "\n";
    }
  return os;  
}

bool InputCriterionTable::is_ascending(int in)
{
  assert (in >= 0);
  assert (in < no_crit);

  return ascending[in]->get_active_row_number() == 0;
}

bool InputCriterionTable::has_veto(int index)
{
  return veto_th_cards[index]->is_enabled();
}

bool InputCriterionTable::is_threshold_percentage(int index)
{
  int typ = card_th_types[index]->get_active_row_number();
  if (typ == ThresholdBox::DETPROS || typ == ThresholdBox::UNIFPROS)
    return true;
  return false;
}

