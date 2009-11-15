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
