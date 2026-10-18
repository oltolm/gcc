/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* _Discard of a stored continuation must run the cleanups of its body,
   and resuming a continuation again after the body has suspended at a
   later one must abort.  */

#include <signal.h>
#include <stdlib.h>

_Effect gen { void yield (void); };

static _Continuation (gen.yield) first, second;
static int cleanups;

static void
count_cleanup (int *p __attribute__((unused)))
{
  cleanups++;
}

static void
on_abort (int sig __attribute__((unused)))
{
  _Exit (cleanups == 1 ? 0 : 1);
}

int
main (void)
{
  _Try
    {
      int guard __attribute__((cleanup (count_cleanup))) = 0;
      _Perform gen.yield ();
      _Perform gen.yield ();
    }
  _Handle (gen.yield () _Continuation k)
    {
      first = k;
    }
  _Discard (first);
  if (cleanups != 1)
    abort ();

  _Try
    {
      _Perform gen.yield ();
      _Perform gen.yield ();
    }
  _Handle (gen.yield () _Continuation k)
    {
      first = k;
    }
  _Try
    {
      _Resume (first);
    }
  _Handle (gen.yield () _Continuation k)
    {
      second = k;
    }
  signal (SIGABRT, on_abort);
  _Discard (first);
  return 1;
}
