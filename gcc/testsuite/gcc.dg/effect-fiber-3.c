/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* A named handler whose clause takes a continuation must be performed
   through its name from a callee of the body, past a handler inside the
   body whose clause uses a copy of a local of the enclosing function.  */

#include <stdlib.h>

_Effect ask { int ask (int); };
_Effect stop { void stop (void); };

static int __attribute__((noipa))
run (_Handler (stop) h, int x)
{
  if (x == 2)
    _Perform h.stop ();
  return _Perform ask.ask (x);
}

int
main (void)
{
  int seen = 0, base = 100;
  int *pseen = &seen;

  _Try (stop h)
    {
      _Try
	{
	  for (int i = 0; ; i++)
	    *pseen += run (h, i);
	}
      _Handle (ask.ask (int x))
	{
	  _Resume (x + base);
	}
    }
  _Handle (stop.stop () _Continuation k)
    {
      _Discard (k);
    }
  if (seen != 201)
    abort ();
  return 0;
}
