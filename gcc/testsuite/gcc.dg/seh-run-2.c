/* { dg-do run { target x86_64-*-mingw* } } */
/* { dg-options "-O2" } */
/* A __try in a nested function: its filter must reach both the locals of
   the nested function and those of the function containing it, through
   the frame object of the nested function.  */

#include <windows.h>
#include <stdlib.h>

static volatile int * __attribute__((noipa))
null_pointer (void)
{
  return NULL;
}

int
main (void)
{
  int outer = 5;

  int nested (volatile int *p)
  {
    int got = 0, inner = 2;
    __try
      {
	got = *p;
      }
    __except (outer == 5 && inner == 2
	      && GetExceptionCode () == EXCEPTION_ACCESS_VIOLATION)
      {
	got = -outer - inner;
      }
    return got;
  }

  if (nested (null_pointer ()) != -7)
    abort ();
  return 0;
}
