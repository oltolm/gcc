/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -fexceptions" } */
/* __gcc_coro_destroy of a suspended coroutine must run the cleanups of
   every frame on its stack, innermost first, without running any code
   after the yield.  Destroying a coroutine that never started must not
   run its body.  */

#include <stddef.h>
#include <stdlib.h>

struct __gcc_coro;
extern struct __gcc_coro *__gcc_coro_create (void (*) (void *), void *,
					     size_t);
extern int __gcc_coro_resume (struct __gcc_coro *);
extern void __gcc_coro_yield (void);
extern void __gcc_coro_destroy (struct __gcc_coro *);

static char trace[8];
static int n;

static void
record (char *p)
{
  trace[n++] = *p;
}

static void __attribute__((noipa))
leaf (void)
{
  char c __attribute__((cleanup (record))) = 'l';
  __gcc_coro_yield ();
  abort ();
}

static void
body (void *arg __attribute__((unused)))
{
  char c __attribute__((cleanup (record))) = 'b';
  leaf ();
  abort ();
}

static void
never (void *arg __attribute__((unused)))
{
  abort ();
}

int
main (void)
{
  struct __gcc_coro *c = __gcc_coro_create (body, 0, 0);

  if (!__gcc_coro_resume (c) || n != 0)
    abort ();
  __gcc_coro_destroy (c);
  trace[n] = 0;
  if (__builtin_strcmp (trace, "lb") != 0)
    abort ();

  __gcc_coro_destroy (__gcc_coro_create (never, 0, 0));
  return 0;
}
