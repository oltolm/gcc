/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2" } */
/* __gcc_effect_suspend from a coroutine running inside the coroutine whose
   handler record is targeted must switch directly to the outer
   coroutine's resumer, without running code of the outer coroutine, and
   the next resume of the outer coroutine must continue in the inner one
   with the inner one's handler chain.  */

#include <stddef.h>
#include <stdlib.h>

struct __gcc_coro;
extern struct __gcc_coro *__gcc_coro_create (void (*) (void *), void *,
					     size_t);
extern int __gcc_coro_resume (struct __gcc_coro *);
extern void __gcc_coro_destroy (struct __gcc_coro *);
extern void *__gcc_coro_message (struct __gcc_coro *);

struct __gcc_effect_handler
{
  struct __gcc_effect_handler *next;
  const void *effect;
  void *ops;
};

extern struct __gcc_effect_handler *__gcc_effect_find (const void *);
extern struct __gcc_effect_handler *__gcc_coro_handler (struct __gcc_coro *);
extern void __gcc_effect_suspend (struct __gcc_effect_handler *, void *);

static const char eff_a, eff_b;
static struct __gcc_coro *outer_c, *inner_c;
static char trace[8];
static int n;
static char marker;

static void
inner_body (void *arg __attribute__((unused)))
{
  trace[n++] = 'i';
  __gcc_effect_suspend (__gcc_effect_find (&eff_a), &marker);
  if (__gcc_effect_find (&eff_b) != __gcc_coro_handler (inner_c))
    abort ();
  trace[n++] = 'j';
}

static void
outer_body (void *arg __attribute__((unused)))
{
  inner_c = __gcc_coro_create (inner_body, 0, 0);
  __gcc_coro_handler (inner_c)->effect = &eff_b;
  trace[n++] = 'o';
  if (__gcc_coro_resume (inner_c))
    abort ();
  trace[n++] = 'p';
  __gcc_coro_destroy (inner_c);
}

int
main (void)
{
  outer_c = __gcc_coro_create (outer_body, 0, 0);
  __gcc_coro_handler (outer_c)->effect = &eff_a;
  if (!__gcc_coro_resume (outer_c)
      || __gcc_coro_message (outer_c) != &marker)
    abort ();
  trace[n] = 0;
  if (__builtin_strcmp (trace, "oi") != 0)
    abort ();
  if (__gcc_coro_resume (outer_c))
    abort ();
  trace[n] = 0;
  if (__builtin_strcmp (trace, "oijp") != 0)
    abort ();
  __gcc_coro_destroy (outer_c);
  return 0;
}
