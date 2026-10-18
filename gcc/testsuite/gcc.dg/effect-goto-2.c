/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* A nonlocal goto from a _Handle clause to a label after its _Try must
   run the cleanups of the performer and pop the _Try's record from the
   handler chain.  */

#include <stdlib.h>

struct __gcc_effect_handler;
extern struct __gcc_effect_handler *__gcc_effect_top (void);

_Effect gen { void yield (int); };

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

int
main (void)
{
  __label__ outside;
  struct __gcc_effect_handler *before = __gcc_effect_top ();

  _Try
    {
      range (0, 10);
    }
  _Handle (gen.yield (int v))
    {
      if (v == 3)
	goto outside;
      _Resume ();
    }
 outside:
  if (cleanups != 1 || __gcc_effect_top () != before)
    abort ();
  return 0;
}
