/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-feffects" } */
/* With -feffects, _Dynamic declares a file-scope variable that may be
   redeclared extern, and outside any binding it reads and writes its
   global value.  */

#include <stdlib.h>

extern _Dynamic int width;
_Dynamic int width = 80;
extern _Dynamic int width;
static _Dynamic const int depth = 2;
_Dynamic struct { int a, b; } pair = { 1, 2 };
_Dynamic int *ptr;
static int target = 2;

int
main (void)
{
  if (width != 80 || depth != 2 || pair.b != 2 || ptr)
    abort ();
  width = 60;
  width++;
  pair.a += 2;
  ptr = &target;
  if (width != 61 || pair.a != 3 || *ptr != 2)
    abort ();
  return 0;
}
