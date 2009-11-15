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

#include "changetrackable.h"

ChangeTrackable::ChangeTrackable()
{
  _changed = false;
}

bool ChangeTrackable::has_changed() const
{
  return _changed;
}

void ChangeTrackable::set_changed()
{
  _changed = true;
}

void ChangeTrackable::clear_changed()
{
  _changed = false;
}

ChangeTrackable::~ChangeTrackable()
{
}
