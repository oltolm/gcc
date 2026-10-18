/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* return in the body of a _Try that runs as a coroutine must return from
   the function containing the _Try, also from a coroutine running inside
   the body of another, after destroying the coroutines, running the
   cleanups of their frames, and restoring the handler chain.  */

#include <stdlib.h>

struct __gcc_effect_handler;
extern struct __gcc_effect_handler *__gcc_effect_top (void);

_Effect confirm { int ask (int); };

static int cleanups;

static void
count_cleanup (int *p __attribute__((unused)))
{
  cleanups++;
}

struct pair { int a, b; };

static int __attribute__((noipa))
pick (int n, int want)
{
  _Try
    {
      int guard __attribute__((cleanup (count_cleanup))) = 0;
      for (int i = 0; i < n; i++)
	if (_Perform confirm.ask (i))
	  return i * 10;
    }
  _Handle (confirm.ask (int i) _Continuation k)
    {
      _Resume (k, i == want);
    }
  return -1;
}

static struct pair __attribute__((noipa))
pick_pair (int want)
{
  _Try
    {
      int guard __attribute__((cleanup (count_cleanup))) = 0;
      _Try
	{
	  int guard2 __attribute__((cleanup (count_cleanup))) = 0;
	  for (int i = 0; i < 10; i++)
	    if (_Perform confirm.ask (i))
	      return (struct pair) { i, i + 1 };
	}
      _Handle (confirm.ask (int i) _Continuation k)
	{
	  _Resume (k, i == want);
	}
    }
  _Handle (confirm.ask (int i) _Continuation k)
    {
      _Discard (k);
    }
  return (struct pair) { -1, -1 };
}

static int seen;

static void __attribute__((noipa))
stop_at (int want)
{
  _Try
    {
      for (int i = 0; i < 10; i++)
	{
	  if (_Perform confirm.ask (i))
	    return;
	  seen++;
	}
    }
  _Handle (confirm.ask (int i) _Continuation k)
    {
      _Resume (k, i == want);
    }
  seen = -1;
}

int
main (void)
{
  struct __gcc_effect_handler *before = __gcc_effect_top ();

  if (pick (10, 4) != 40 || cleanups != 1 || __gcc_effect_top () != before)
    abort ();
  if (pick (10, 20) != -1 || cleanups != 2 || __gcc_effect_top () != before)
    abort ();

  struct pair p = pick_pair (3);
  if (p.a != 3 || p.b != 4 || cleanups != 4 || __gcc_effect_top () != before)
    abort ();

  stop_at (5);
  if (seen != 5 || __gcc_effect_top () != before)
    abort ();
  return 0;
}
