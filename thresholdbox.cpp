#include "thresholdbox.h"

ThresholdBox::ThresholdBox(bool dis)
{
  discrete = false;
  type = 0;
  disablable = dis;
  disabled = dis;
  limitlabel.set_text("+-");
  percentlabel1.set_text("%");
  percentlabel2.set_text("%");
  base.signal_changed().connect(sigc::mem_fun(*this, &ThresholdBox::set_changed));
  limit.signal_changed().connect(sigc::mem_fun(*this, &ThresholdBox::set_changed));
  enablebutton.set_label("Enable");

  if (dis)
    pack_start(enablebutton, Gtk::PACK_SHRINK, 5, 5);
  else
    {
      pack_start(base, Gtk::PACK_SHRINK, 0, 5);
    }

  enablebutton.signal_clicked().connect(sigc::mem_fun(*this, &ThresholdBox::on_enablebutton_clicked));
  show_all_children();

}

std::ofstream& operator<< (std::ofstream &os, ThresholdBox &b)
{
  os << b.disabled << std::endl;
  os << b.disablable << std::endl;
  os << b.discrete << std::endl;
  os << b.type << std::endl;
  os << b.base.get_value() << std::endl;
  os << b.limit.get_value() << std::endl;
  return os;
}

std::ifstream& operator>> (std::ifstream &is, ThresholdBox &b)
{
  double ba, li;
  int ty;
  double disc;

  is >> b.disabled;
  is >> b.disablable;
  is >> disc;
  is >> ty;
  is >> ba;
  is >> li;
  b.base.set_value(ba);
  b.limit.set_value(li);

  b.set_type(ty);
  b.set_discrete(disc);
  
  if (b.disabled)
    {
      if (b.base.get_parent())
	b.remove(b.base);
      if (b.limit.get_parent())
	b.remove(b.limit);
      if (b.limitlabel.get_parent())
	b.remove(b.limitlabel);
      if (b.percentlabel1.get_parent())
	b.remove(b.percentlabel1);
      if (b.percentlabel2.get_parent())
	b.remove(b.percentlabel2);
      if (b.enablebutton.get_parent() == 0)
	{
	  b.pack_start(b.enablebutton, Gtk::SHRINK, 0, 5);
	  b.enablebutton.show();
	  b.enablebutton.set_label("Enable");
	}
    }
  else
    {
      if (b.disablable)
	{
	  b.enablebutton.set_label("X");
	  if (b.enablebutton.get_parent() == 0)
	    {
	      b.pack_end(b.enablebutton, Gtk::SHRINK, 0, 5);
	      b.enablebutton.show();
	    }
	}
    } // end else
  return is;
}

void ThresholdBox::on_enablebutton_clicked()
{
  set_changed();

  disabled = !disabled;
  set_type(type);
}

void ThresholdBox::set_type(int typ)
{
  type = typ;

  // remove all first
  if (base.get_parent())
    remove(base);
  if (limit.get_parent())
    remove(limit);
  if (limitlabel.get_parent())
    remove(limitlabel);
  if (enablebutton.get_parent())
    remove(enablebutton);
  if (percentlabel1.get_parent())
    remove(percentlabel1);
  if (percentlabel2.get_parent())
    remove(percentlabel2);

  if (disablable)
    {
      pack_end(enablebutton, Gtk::SHRINK, 0, 5);
      if (disabled)
	enablebutton.set_label("Enable");
      else
	enablebutton.set_label("X");
      enablebutton.show();
    }
  if (disabled) return;
  
  base.set_discrete(discrete);
  limit.set_discrete(discrete);
      
  pack_start(base, Gtk::SHRINK, 0, 5);
  if (type == DETPROS || type == UNIFPROS)
    pack_start(percentlabel1, Gtk::SHRINK, 0, 5);
  if (type == UNIF || type == UNIFPROS)
    {
      pack_start(limitlabel, Gtk::SHRINK, 0, 5);
      pack_start(limit, Gtk::SHRINK, 0, 5);
      if (type == UNIFPROS)
	{
	  pack_start(percentlabel2, Gtk::SHRINK, 0, 5);
	}
    }
  show_all_children();
}

ThresholdBox::~ThresholdBox()
{

}

double ThresholdBox::get_base() const
{
  return base.get_value();
}

double ThresholdBox::get_limit() const
{
  return limit.get_value();
}

int ThresholdBox::get_type() const
{
  return type;
}

void ThresholdBox::set_base(double val)
{
  base.set_value(val);
}

void ThresholdBox::set_limit(double val)
{
  limit.set_value(val);
}

bool ThresholdBox::is_enabled() const
{
  return !disabled;
}

void ThresholdBox::set_discrete(bool disc)
{
  discrete = disc;
  set_type(type);
}
