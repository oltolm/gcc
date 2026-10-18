/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* _Discard (k) in a clause must end the _Try: the suspended body must not
   continue, and its cleanups must run, without -fexceptions since
   -feffects enables it.  The body must see copies of the locals of the
   enclosing function, taken when the _Try starts.  */

#include <stdlib.h>

_Effect fail { void fail (int); };

static int cleanups;

static void
count_cleanup (int *p __attribute__((unused)))
{
  cleanups++;
}

static int __attribute__((noipa))
check (int x)
{
  if (x > 2)
    _Perform fail.fail (x);
  return x;
}

int
main (void)
{
  int sum = 0, n = 10, failed = 0;
  int *psum = &sum;

  _Try
    {
      int guard __attribute__((cleanup (count_cleanup))) = 0;
      int limit = n;
      n = -1;
      for (int i = 0; i < limit; i++)
	*psum += check (i);
      *psum = -1000;
    }
  _Handle (fail.fail (int x) _Continuation k)
    {
      failed = x;
      _Discard (k);
    }
  if (sum != 3 || failed != 3 || n != 10 || cleanups != 1)
    abort ();
  return 0;
}
