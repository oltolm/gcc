/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects -Wtrampolines" } */
/* _Perform h.op must call the clause of the handler named h, even from a
   callee and past an inner handler for the same effect, with h and the
   handlers inside it removed from the chain.  _Perform eff.op must skip
   named handlers.  */

#include <stdlib.h>

_Effect ask { int ask (int); };

static int __attribute__((noipa))
by_name (_Handler (ask) h, int x)
{
  return _Perform h.ask (x);
}

static int __attribute__((noipa))
by_search (int x)
{
  return _Perform ask.ask (x);
}

int
main (void)
{
  int r1 = 0, r2 = 0, r3 = 0;

  _Try
    {
      _Try (ask h)
	{
	  r1 = by_name (h, 1);
	  r2 = by_search (2);
	  _Try
	    {
	      r3 = by_name (h, 3);
	    }
	  _Handle (ask.ask (int x))
	    {
	      _Resume (x + 1000);
	    }
	}
      _Handle (ask.ask (int x))
	{
	  _Resume (x + 10 + by_search (0));
	}
    }
  _Handle (ask.ask (int x))
    {
      _Resume (x + 100);
    }
  if (r1 != 111 || r2 != 102 || r3 != 113)
    abort ();
  return 0;
}
