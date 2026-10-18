/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* return in the body of a for statement over an effect operation must
   return from the function containing the loop, also from nested loops,
   after unwinding the expressions of the loops and restoring the handler
   chain.  */

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

struct pair { int a, b; };

static int __attribute__((noipa))
find (int x)
{
  for (gen.yield (int v) : range (0, 10))
    if (v == x)
      return v * 10;
  return -1;
}

static struct pair __attribute__((noipa))
find_pair (int sum)
{
  for (gen.yield (int i) : range (0, 10))
    for (gen.yield (int j) : range (0, 10))
      if (i + j == sum && i < j)
	return (struct pair) { i, j };
  return (struct pair) { -1, -1 };
}

static int seen;

static void __attribute__((noipa))
stop_at (int x)
{
  for (gen.yield (int v) : range (0, 10))
    {
      if (v == x)
	return;
      seen++;
    }
  seen = -1;
}

int
main (void)
{
  struct __gcc_effect_handler *before = __gcc_effect_top ();

  if (find (4) != 40 || cleanups != 1 || __gcc_effect_top () != before)
    abort ();
  if (find (20) != -1 || cleanups != 2 || __gcc_effect_top () != before)
    abort ();

  struct pair p = find_pair (5);
  if (p.a != 0 || p.b != 5 || cleanups != 4
      || __gcc_effect_top () != before)
    abort ();

  stop_at (3);
  if (seen != 3 || cleanups != 5 || __gcc_effect_top () != before)
    abort ();
  return 0;
}
