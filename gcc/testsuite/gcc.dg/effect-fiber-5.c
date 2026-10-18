/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* The body of a _Try is parsed after its clauses, so identifiers in it
   must be classified as typedef names or not in the body's own scopes,
   in both kinds of _Try.  */

#include <stdlib.h>

typedef int U;

_Effect ask { int ask (int); };

static int result;

int
main (void)
{
  _Try
    {
      typedef long T;
      T t = 1;
      int U = 2;
      result = _Perform ask.ask ((int) t + U);
    }
  _Handle (ask.ask (int x))
    {
      _Resume (x * 10);
    }
  if (result != 30)
    abort ();

  int *presult = &result;
  _Try
    {
      typedef long T;
      T t = 4;
      int U = 5;
      *presult = _Perform ask.ask ((int) t + U);
    }
  _Handle (ask.ask (int x) _Continuation k)
    {
      _Resume (k, x * 10);
    }
  if (result != 90)
    abort ();
  return 0;
}
