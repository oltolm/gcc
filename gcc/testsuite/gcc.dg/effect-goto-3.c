/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* A nonlocal goto from a _Handle clause to a label after an enclosing
   _Try must pop the records of both _Try statements, whether the inner
   _Try is in the body of the outer one or in one of its clauses.  */

#include <stdlib.h>

struct __gcc_effect_handler;
extern struct __gcc_effect_handler *__gcc_effect_top (void);

_Effect gen { void yield (int); };
_Effect ask { int ask (void); };

static int cleanups;

static void
count_cleanup (int *p __attribute__((unused)))
{
  cleanups++;
}

static void __attribute__((noipa))
range (int lo, int hi)
{
  int guard __attribute__((cleanup (count_cleanup))) = 0;
  for (int i = lo; i < hi; i++)
    _Perform gen.yield (i);
  abort ();
}

static int __attribute__((noipa))
query (void)
{
  return _Perform ask.ask ();
}

int
main (void)
{
  __label__ out1, out2;
  struct __gcc_effect_handler *before = __gcc_effect_top ();

  _Try
    {
      _Try
	{
	  range (0, 10);
	}
      _Handle (gen.yield (int v))
	{
	  if (v == 3)
	    goto out1;
	  _Resume ();
	}
    }
  _Handle (ask.ask ())
    {
      _Resume (0);
    }
 out1:
  if (cleanups != 1 || __gcc_effect_top () != before)
    abort ();

  _Try
    {
      query ();
    }
  _Handle (ask.ask ())
    {
      _Try
	{
	  range (0, 10);
	}
      _Handle (gen.yield (int v))
	{
	  if (v == 3)
	    goto out2;
	  _Resume ();
	}
      _Resume (0);
    }
 out2:
  if (cleanups != 2 || __gcc_effect_top () != before)
    abort ();
  return 0;
}
