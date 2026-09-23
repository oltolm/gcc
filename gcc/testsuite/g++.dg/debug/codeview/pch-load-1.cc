#include <pch-load-1.hh>

int
pch_use (int x)
{
  return pch_parent (x) + pch_child (x);
}
