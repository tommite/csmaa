#include "numberentry.h"

NumberEntry::NumberEntry(bool allow_n)
  : Entry()
{
  allow_neg = allow_n;
  set_max_length(50);
  set_width_chars(4);
  updating = false;
  set_value(0.0);
  discrete = false;
}

NumberEntry::NumberEntry(const char *text, bool allow_n)
  : Entry()
{
  allow_neg = allow_n;
  set_max_length(50);
  set_width_chars(4);
  updating = false;
  set_text(Glib::ustring(text));
  discrete = false;
}

void NumberEntry::set_discrete(bool disc)
{
  if (disc == discrete) return;

  if (disc)
    {
      double v = get_value();
      v = floor(v);
      set_value(v);
    }
  discrete = disc;
}

void NumberEntry::on_changed()
{
  if (updating) return;
  updating = true;

  Glib::ustring s = get_text();

  bool dotfound = false;

  for (unsigned int i=0;i<s.length();i++)
	{
	  char c = s[i];
	  if (c == '.')
		{
		  if (dotfound || discrete)
			{
			  set_text(old_s);
			  updating = false;
			  return;
			}
		  else 
			{
			  dotfound = true;
			}
		}
	  else if (allow_neg && c == '-')
	    {
	      if (i != 0)
		{
		  set_text(old_s);
		  updating = false;
		  return;
		}
	    }
	  else if (!isdigit(c))
		{
		  set_text(old_s);
		  updating = false;
		  return;
		}
	}
  old_s = s;
  if (s.size() > 4)
    set_width_chars(s.size());
  else
    set_width_chars(4);
  updating = false;
}

NumberEntry::~NumberEntry()
{

}

double NumberEntry::get_value() const
{
  std::stringstream s;
  double ff;

  s << get_text();
  s >> ff;
  return ff;
}

void NumberEntry::set_value(double val)
{
  std::stringstream s;

  s << val;

  set_text(Glib::ustring(s.str()));
}
