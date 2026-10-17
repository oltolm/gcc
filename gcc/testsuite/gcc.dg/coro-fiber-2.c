/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2" } */
/* __gcc_coro_yield must return to the coroutine that resumed the current
   one, not to the thread's original context: an inner coroutine resumed
   by an outer one yields back into the outer one.  */

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
inner (void *arg __attribute__((unused)))
{
  trace[n++] = 'i';
  __gcc_coro_yield ();
  trace[n++] = 'j';
}

static void
outer (void *arg __attribute__((unused)))
{
  struct __gcc_coro *c = __gcc_coro_create (inner, 0, 0);

  trace[n++] = 'o';
  if (!__gcc_coro_resume (c))
    abort ();
  trace[n++] = 'p';
  __gcc_coro_yield ();
  if (__gcc_coro_resume (c))
    abort ();
  trace[n++] = 'q';
  __gcc_coro_destroy (c);
}

int
main (void)
{
  struct __gcc_coro *c = __gcc_coro_create (outer, 0, 0);

  if (!__gcc_coro_resume (c))
    abort ();
  trace[n++] = 'm';
  if (__gcc_coro_resume (c))
    abort ();
  __gcc_coro_destroy (c);
  trace[n] = 0;
  if (__builtin_strcmp (trace, "oipmjq") != 0)
    abort ();
  return 0;
}
