/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* A goto from a _Handle clause or the body of a _Try that runs as a
   coroutine to a label outside the _Try must destroy the coroutine,
   running the cleanups of its frames, and leave the handler chain as
   before: from a clause with a continuation, which runs on the stack of
   the _Try; from a clause without one or from the body, which run on
   the stack of the coroutine; and from a coroutine running inside the
   body of another.  */

#include <stdlib.h>

struct __gcc_effect_handler;
extern struct __gcc_effect_handler *__gcc_effect_top (void);

_Effect ev { void stop (int); void ping (int); };

static int cleanups;

static void
count_cleanup (int *p __attribute__((unused)))
{
  cleanups++;
}

static void __attribute__((noipa))
work (int n)
{
  int guard __attribute__((cleanup (count_cleanup))) = 0;
  for (int i = 0; i < n; i++)
    {
      _Perform ev.ping (i);
      if (i == 3)
	_Perform ev.stop (i);
    }
  abort ();
}

int
main (void)
{
  __label__ out1, out2, out3, out4;
  struct __gcc_effect_handler *before = __gcc_effect_top ();
  int pings = 0;

  _Try
    {
      work (10);
    }
  _Handle (ev.stop (int x) _Continuation k)
    {
      if (x == 3)
	goto out1;
      _Resume (k);
    }
  _Handle (ev.ping (int x))
    {
      pings++;
      _Resume ();
    }
 out1:
  if (pings != 4 || cleanups != 1 || __gcc_effect_top () != before)
    abort ();

  _Try
    {
      work (10);
    }
  _Handle (ev.stop (int x) _Continuation k)
    {
      _Resume (k);
    }
  _Handle (ev.ping (int x))
    {
      if (x == 2)
	goto out2;
      _Resume ();
    }
 out2:
  if (cleanups != 2 || __gcc_effect_top () != before)
    abort ();

  _Try
    {
      int guard __attribute__((cleanup (count_cleanup))) = 0;
      goto out3;
    }
  _Handle (ev.stop (int x) _Continuation k)
    {
      _Discard (k);
    }
  _Handle (ev.ping (int x))
    {
      _Resume ();
    }
 out3:
  if (cleanups != 3 || __gcc_effect_top () != before)
    abort ();

  _Try
    {
      int guard __attribute__((cleanup (count_cleanup))) = 0;
      _Try
	{
	  work (10);
	}
      _Handle (ev.stop (int x) _Continuation k)
	{
	  _Resume (k);
	}
      _Handle (ev.ping (int x))
	{
	  if (x == 1)
	    goto out4;
	  _Resume ();
	}
    }
  _Handle (ev.stop (int x) _Continuation k)
    {
      _Discard (k);
    }
  _Handle (ev.ping (int x))
    {
      _Resume ();
    }
 out4:
  if (cleanups != 5 || __gcc_effect_top () != before)
    abort ();
  return 0;
}
