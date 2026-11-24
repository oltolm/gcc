#include "pch-load-1-child.hh"

struct pch_record
{
  struct { int get () { return 1; } } member;
};

inline int
pch_parent (int x)
{
  return x + 3;
}
