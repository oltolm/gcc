/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* _Let must bind a _Dynamic variable for the dynamic extent of its body:
   callees see the innermost binding, assignments change the binding and
   not the global value, the initializers are evaluated before any of the
   bindings takes effect, and leaving the body by break, return or
   falling off its end restores the previous binding.  */

#include <stdlib.h>

_Dynamic int width = 80;
_Dynamic int depth = 1;
_Dynamic const int fixed = 7;
_Dynamic struct { int a, b; } pair = { 1, 2 };

static int __attribute__((noipa))
get_width (void)
{
  return width;
}

static void __attribute__((noipa))
set_width (int w)
{
  width = w;
}

static int __attribute__((noipa))
bound_return (void)
{
  _Let (width = 5)
    {
      return get_width ();
    }
}

int
main (void)
{
  _Let (width = 40)
    {
      if (get_width () != 40 || depth != 1)
	abort ();
      set_width (41);
      if (width != 41)
	abort ();
      _Let (width = width + 1, depth = width)
	{
	  if (get_width () != 42 || depth != 41)
	    abort ();
	  width++;
	  depth += 10;
	  if (get_width () != 43 || depth != 51)
	    abort ();
	}
      if (get_width () != 41 || depth != 1)
	abort ();
    }
  if (width != 80)
    abort ();

  for (int i = 0; i < 3; i++)
    _Let (width = i)
      {
	if (i == 1)
	  break;
	if (get_width () != i)
	  abort ();
      }
  if (get_width () != 80 || bound_return () != 5 || get_width () != 80)
    abort ();

  _Let (fixed = 8, pair = (typeof (pair)) { 3, 4 })
    {
      pair.a += fixed;
      if (fixed != 8 || pair.a != 11 || pair.b != 4)
	abort ();
    }
  if (fixed != 7 || pair.a != 1 || pair.b != 2)
    abort ();

  set_width (90);
  if (width != 90)
    abort ();
  return 0;
}
