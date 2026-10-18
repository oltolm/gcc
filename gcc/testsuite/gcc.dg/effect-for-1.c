/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* A for statement over an effect operation must run its body for each
   _Perform of the operation by the expression.  continue must resume
   the expression; break and goto out of the body must unwind it,
   running its cleanups, and leave the handler chain as before.  */

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
}

int
main (void)
{
  __label__ out;
  struct __gcc_effect_handler *before = __gcc_effect_top ();
  int sum = 0, pairs = 0;

  for (gen.yield (int v) : range (0, 10))
    {
      if (v % 2)
	continue;
      sum += v;
    }
  if (sum != 0 + 2 + 4 + 6 + 8 || cleanups != 1
      || __gcc_effect_top () != before)
    abort ();

  sum = 0;
  for (gen.yield (int v) : range (0, 10))
    {
      if (v == 4)
	break;
      sum += v;
    }
  if (sum != 0 + 1 + 2 + 3 || cleanups != 2
      || __gcc_effect_top () != before)
    abort ();

  /* break leaves only the innermost loop.  */
  for (gen.yield (int i) : range (0, 3))
    for (gen.yield (int j) : range (0, 10))
      {
	if (j > i)
	  break;
	pairs++;
      }
  if (pairs != 1 + 2 + 3 || cleanups != 2 + 1 + 3
      || __gcc_effect_top () != before)
    abort ();

  for (gen.yield (int v) : range (0, 10))
    if (v == 5)
      goto out;
  abort ();
 out:
  if (cleanups != 7 || __gcc_effect_top () != before)
    abort ();
  return 0;
}
