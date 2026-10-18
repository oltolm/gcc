/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* The body of a _Try is saved as tokens and parsed after its clauses, so
   a pragma in it must still apply to the statement after it, in both
   kinds of _Try.  */

#include <stdlib.h>

_Effect ask { int ask (int); };

int
main (void)
{
  int s = 0;
  int *ps = &s;

  _Try
    {
#pragma GCC unroll 4
      for (int i = 0; i < 8; i++)
	s += _Perform ask.ask (i);
    }
  _Handle (ask.ask (int x))
    {
      _Resume (x);
    }
  if (s != 28)
    abort ();

  _Try
    {
#pragma GCC ivdep
      for (int i = 0; i < 8; i++)
	*ps += _Perform ask.ask (i);
    }
  _Handle (ask.ask (int x) _Continuation k)
    {
      _Resume (k, x);
    }
  if (s != 56)
    abort ();
  return 0;
}
