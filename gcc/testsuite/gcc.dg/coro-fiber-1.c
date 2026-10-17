/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2" } */
/* __gcc_coro_create must not run the body; __gcc_coro_resume must run it
   up to the next __gcc_coro_yield and return nonzero, or return 0 once
   the body has returned.  Locals of the body survive each yield.  */

#include <stddef.h>
#include <stdlib.h>

struct __gcc_coro;
extern struct __gcc_coro *__gcc_coro_create (void (*) (void *), void *,
					     size_t);
extern int __gcc_coro_resume (struct __gcc_coro *);
extern void __gcc_coro_yield (void);
extern void __gcc_coro_destroy (struct __gcc_coro *);

static int trace[3], n;

static void
body (void *arg)
{
  volatile int *p = arg;
  volatile int local = 10;

  trace[n++] = *p;
  __gcc_coro_yield ();
  trace[n++] = ++local;
  __gcc_coro_yield ();
  trace[n++] = *p;
}

int
main (void)
{
  volatile int x = 1;
  struct __gcc_coro *c = __gcc_coro_create (body, (void *) &x, 0);

  if (!c || n != 0)
    abort ();
  if (!__gcc_coro_resume (c) || n != 1 || trace[0] != 1)
    abort ();
  if (!__gcc_coro_resume (c) || n != 2 || trace[1] != 11)
    abort ();
  x = 5;
  if (__gcc_coro_resume (c) || n != 3 || trace[2] != 5)
    abort ();
  __gcc_coro_destroy (c);
  return 0;
}
