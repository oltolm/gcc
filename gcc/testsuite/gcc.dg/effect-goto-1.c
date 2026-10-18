/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* A nonlocal goto from a _Handle clause to a label in the body of its
   _Try must run the cleanups of the frames in between: the cleanup
   variables of the performer and the restore of the handler chain by
   _Perform, so that the _Try's handler is found again.  */

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
  __label__ inside;
  struct __gcc_effect_handler *before = __gcc_effect_top ();
  int last = -1;

  _Try
    {
      range (0, 10);
    inside:
      _Perform gen.yield (100);
    }
  _Handle (gen.yield (int v))
    {
      if (v == 2)
	goto inside;
      last = v;
      _Resume ();
    }
  if (last != 100 || cleanups != 1 || __gcc_effect_top () != before)
    abort ();
  return 0;
}
