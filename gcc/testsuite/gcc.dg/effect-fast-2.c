/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* A _Handle clause must run with its own handler removed from the chain,
   so that _Perform of the same effect inside the clause reaches the
   enclosing handler, and the handler must be removed from the chain when
   its _Try ends.  */

#include <stdlib.h>

_Effect ask { int ask (int); };

static int __attribute__((noipa))
query (int x)
{
  return _Perform ask.ask (x);
}

int
main (void)
{
  int outer_calls = 0, r1 = 0, r2 = 0;

  _Try
    {
      _Try
	{
	  r1 = query (1);
	}
      _Handle (ask.ask (int x))
	{
	  _Resume (query (x * 10) + 1);
	}
      r2 = query (2);
    }
  _Handle (ask.ask (int x))
    {
      outer_calls++;
      _Resume (x + 100);
    }
  if (r1 != 111 || r2 != 102 || outer_calls != 2)
    abort ();
  return 0;
}
