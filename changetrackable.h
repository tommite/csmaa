#ifndef __CHANGETRACKABLE_H__
#define __CHANGETRACKABLE_H__


class ChangeTrackable
{
 public:
  ChangeTrackable();
  virtual ~ChangeTrackable();

  virtual bool has_changed() const;
  virtual void clear_changed();

 protected:
  virtual void set_changed();

 private:
  bool _changed;
};

#endif
