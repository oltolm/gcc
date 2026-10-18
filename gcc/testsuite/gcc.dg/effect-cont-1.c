/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* A continuation stored by a clause must outlive the _Try and the
   function that started it, and _Try { _Resume (k, v); } must continue
   it under the new _Try's clauses, with V as the value of the _Perform.
   The body must keep using its copy of a parameter of the function that
   started it.  */

#include <stdlib.h>

_Effect gen { int yield (int); };

static _Continuation (gen.yield) saved;
static int have, value;

static void __attribute__((noipa))
start (int n)
{
  _Try
    {
      int got = 0;
      for (int i = 0; i < n; i++)
	got += _Perform gen.yield (i);
      if (got != 10 + 11 + 12)
	abort ();
    }
  _Handle (gen.yield (int v) _Continuation k)
    {
      value = v;
      saved = k;
      have = 1;
    }
}

int
main (void)
{
  int sum = 0, count = 0;

  start (3);
  while (have)
    {
      sum += value;
      have = 0;
      _Try
	{
	  _Resume (saved, 10 + count++);
	}
      _Handle (gen.yield (int v) _Continuation k)
	{
	  value = v;
	  saved = k;
	  have = 1;
	}
    }
  if (sum != 3 || count != 3)
    abort ();
  return 0;
}
