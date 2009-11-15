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

#include "rankcomboboxset.h"

RankComboBoxSet::RankComboBoxSet(int _num, bool uniq, bool _managed, bool f)
{
  force = f;
  init(_num, uniq, _managed);
}

RankComboBoxSet::~RankComboBoxSet()
{
  if (managed)
    {
      for (unsigned int i=0;i<boxes.size();i++)
	delete boxes[i];
    }
}

Gtk::ComboBoxText * RankComboBoxSet::get_box(int index)
{
  return boxes[index];
}

void RankComboBoxSet::init(int _num, bool uniq, bool _managed)
{
  managed = _managed;
  changing = false;
  unique_ranks = uniq;
  num = _num;
  boxes.resize(num);

  for (int i=0;i<num;i++)
    boxes[i] = create_box(num, i);
}

void RankComboBoxSet::remove_last_box()
{
  Gtk::ComboBoxText *b = boxes[num-1];
  
  if (unique_ranks)
	b->set_active(num-1);
  boxes.resize(num-1);
  num--;
  if (managed)
    delete b;
  char textbuf[10];
  sprintf (textbuf, "%d", num+1);
  Glib::ustring ust(textbuf);
  for (int i=0;i<num;i++)
    {
      if (boxes[i]->get_active_row_number() == num)
	boxes[i]->set_active(num-1);
      boxes[i]->remove_text(ust);
    }
}

Gtk::ComboBoxText * RankComboBoxSet::create_box(int num_ranks, int index)
{
  Gtk::ComboBoxText *box = new Gtk::ComboBoxText();
  for (int j=0;j<num_ranks;j++)
    {
      char buf[3];
      sprintf (buf, "%d", (j+1));
      box->append_text(Glib::ustring(buf));
    }
  box->set_active(index);
  if (shandlers.size() < (unsigned int) index+1)
    shandlers.resize(index+1);
  shandlers[index] = box->signal_changed().connect(sigc::bind<int>(sigc::mem_fun(*this, &RankComboBoxSet::on_change), index));
  return box;
}

void RankComboBoxSet::disable_actionlisteners()
{
  for (unsigned int i=0;i<boxes.size();i++)
    shandlers[i].disconnect();
}

void RankComboBoxSet::enable_actionlisteners()
{
  for (unsigned int i=0;i<boxes.size();i++)
    shandlers[i] = boxes[i]->signal_changed().connect(sigc::bind<int>(sigc::mem_fun(*this, &RankComboBoxSet::on_change), i));
}


void RankComboBoxSet::on_change(int index)
{
  if (!force) return;

  if (changing) return;
  changing = true;

  Gtk::ComboBoxText *jcb = boxes[index];

  // update ranks so that there are no gaps (jumps over a rank)       
  for (int r=1;r<=num;r++) {
	bool found = false;
	for (int j=0;j<num;j++) {
	  Gtk::ComboBoxText *cb = boxes[j];
	  int val = cb->get_active_row_number() +1;
	  if (val == r) { found = true; break; }
	} // end for
	if (!found) {
	  if (unique_ranks) {
		for (int j=0;j<num;j++) {
		  if (j == index) continue;
		  Gtk::ComboBoxText *b2 = boxes[j];
		  int val = b2->get_active_row_number();
		  if (val == jcb->get_active_row_number()) {
			b2->set_active(r-1);
			changing = false;
			return;
		  }
		}
	  } // end if (uniqueRanks)
	  int maxr = 0;
	  int minoverr = 99999;
	  for (int j=0;j<num;j++) {
		Gtk::ComboBoxText *cb = boxes[j];
		int val = cb->get_active_row_number()+1;
		if (val > maxr) maxr = val;
		if (val > r && val < minoverr) minoverr = val;
	  }
	  if (maxr <= r)
		break;
	  int diff = minoverr - r;
	  for (int j=0;j<num;j++) {
		Gtk::ComboBoxText *cb = boxes[j];
		int val = cb->get_active_row_number()+1;
		if (val >= minoverr) cb->set_active(val-diff-1);
	  } // end for
	} // end while(true)
  } // end for   
  changing = false;
}

Gtk::ComboBoxText * RankComboBoxSet::add_box()
{
  Gtk::ComboBoxText * box = create_box(num+1, num);
  char buf[3];
  sprintf (buf, "%d", num+1);
  for (int i=0;i<num;i++)
	boxes[i]->append_text(Glib::ustring(buf));
  num++;
  boxes.push_back(box);
  return box;
}

std::ostream& RankComboBoxSet::operator<< (std::ostream &os)
{
  os << num;
  os << unique_ranks;
  os << managed;
  
  for (int i=0;i<num;i++)
    os << boxes[i]->get_active_row_number();
  return os;
}

std::istream& RankComboBoxSet::operator>> (std::istream &is)
{
  int n;
  is >> n;
  is >> unique_ranks;
  is >> managed;

  while(n > num)
    remove_last_box();

  while(n < num)
    add_box();

  for (int i=0;i<num;i++)
    {
      int ind;
      is >> ind;
      get_box(i)->set_active(ind);
    }
  return is;
}
