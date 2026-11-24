#include <pch-load-1.hh>

int
pch_use (int x)
{
  pch_record r;
  return pch_parent (x) + pch_child (x) + r.member.get ();
}
