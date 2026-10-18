/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* _Resume (k, v) must continue the suspended body with V as the value of
   its _Perform, each time it performs, while an operation without a
   continuation in the same _Try is still called directly.  */

#include <stdlib.h>

_Effect gen { int yield (int); int ask (void); };

static int last;

static void __attribute__((noipa))
produce (int n)
{
  for (int i = 0; i < n; i++)
    last = _Perform gen.yield (i * _Perform gen.ask ());
}

int
main (void)
{
  int total = 0, count = 0, factor = 3;

  _Try
    {
      produce (4);
    }
  _Handle (gen.yield (int v) _Continuation k)
    {
      total += v;
      count++;
      _Resume (k, total);
    }
  _Handle (gen.ask ())
    {
      _Resume (factor);
    }
  if (total != 18 || count != 4 || last != 18)
    abort ();
  return 0;
}
