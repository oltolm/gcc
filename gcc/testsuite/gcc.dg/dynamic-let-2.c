/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* Bindings made by _Let must follow coroutine switches: a body that runs
   as a coroutine sees the bindings of whoever resumed it plus its own,
   a clause sees only the bindings outside the _Try, and _Discard unwinds
   the body's bindings.  */

#include <stdlib.h>

_Dynamic int width = 80;

_Effect ask { int ask (void); };

static int __attribute__((noipa))
get_width (void)
{
  return width;
}

int
main (void)
{
  int seen = 0, rounds = 0;

  _Let (width = 1)
    {
      _Try
	{
	  if (get_width () != 1)
	    abort ();
	  _Let (width = 2)
	    {
	      int v = _Perform ask.ask ();
	      if (get_width () != 2 || v != 10)
		abort ();
	      _Perform ask.ask ();
	      abort ();
	    }
	}
      _Handle (ask.ask () _Continuation k)
	{
	  seen = get_width ();
	  if (rounds++ == 0)
	    _Resume (k, 10);
	  _Discard (k);
	}
      if (seen != 1 || rounds != 2 || get_width () != 1)
	abort ();
    }
  if (get_width () != 80)
    abort ();
  return 0;
}
