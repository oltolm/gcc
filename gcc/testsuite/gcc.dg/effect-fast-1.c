/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects -Wtrampolines" } */
/* _Perform from a callee of a _Try body must call the matching _Handle
   clause, which reads and writes locals of the function containing the
   _Try, and _Resume must return its operand as the value of _Perform.
   The clauses must be called without trampolines.  */

#include <stdlib.h>

_Effect state { int get (void); void put (int); };

static int __attribute__((noipa))
increment (void)
{
  _Perform state.put (_Perform state.get () + 1);
  return _Perform state.get ();
}

int
main (void)
{
  int s = 41, r = 0;

  _Try
    {
      r = increment ();
    }
  _Handle (state.get ())
    {
      _Resume (s);
    }
  _Handle (state.put (int v))
    {
      s = v;
      _Resume ();
    }
  if (r != 42 || s != 42)
    abort ();
  return 0;
}
