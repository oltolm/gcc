/* { dg-do run { target x86_64-*-mingw* } } */
/* { dg-options "-O2" } */
/* In a function with __try, __C_specific_handler must run the cleanups of
   the cleanup attribute in order with the __finally blocks: when the
   exception leaves the function, when an __except of the function handles
   it, and on the normal path.  The cleanup must see the value its variable
   had at the fault.  */

#include <windows.h>
#include <stdlib.h>
#include <string.h>

static char order[8];
static int n;

static void
note (char c)
{
  order[n++] = c;
}

static void
release (int *p)
{
  note ((char) *p);
}

static volatile int * __attribute__((noipa))
null_pointer (void)
{
  return NULL;
}

static void __attribute__((noipa))
leave (volatile int *p)
{
  int r __attribute__((cleanup (release))) = 'c';
  __try
    {
      r = 'd';
      *p = 0;
      r = 'e';
    }
  __finally
    {
      note ('f');
    }
}

static int __attribute__((noipa))
handle (volatile int *p)
{
  __try
    {
      int r __attribute__((cleanup (release))) = 'c';
      __try
	{
	  *p = 0;
	}
      __finally
	{
	  note ('f');
	}
    }
  __except (EXCEPTION_EXECUTE_HANDLER)
    {
      note ('h');
      return 1;
    }
  return 0;
}

static void __attribute__((noipa))
normal (void)
{
  int r __attribute__((cleanup (release))) = 'c';
  __try
    {
      note ('t');
    }
  __finally
    {
      note ('f');
    }
}

int
main (void)
{
  __try
    {
      leave (null_pointer ());
    }
  __except (EXCEPTION_EXECUTE_HANDLER)
    {
      note ('h');
    }
  if (n != 3 || memcmp (order, "fdh", 3) != 0)
    abort ();

  n = 0;
  if (!handle (null_pointer ()) || n != 3 || memcmp (order, "fch", 3) != 0)
    abort ();

  n = 0;
  normal ();
  if (n != 3 || memcmp (order, "tfc", 3) != 0)
    abort ();
  return 0;
}
