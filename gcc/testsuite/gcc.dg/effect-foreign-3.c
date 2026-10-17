/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects -fno-optimize-sibling-calls" } */
/* { dg-shouldfail "capture across a foreign frame" } */
/* __gcc_effect_suspend must check the frames of every coroutine in the
   captured computation, not only the performer's: a foreign frame in the
   outer coroutine, which runs the performing inner one, is rejected.  */

#include <stddef.h>

struct __gcc_coro;
extern struct __gcc_coro *__gcc_coro_create (void (*) (void *), void *,
					     size_t);
extern int __gcc_coro_resume (struct __gcc_coro *);

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

static void
inner_body (void *arg __attribute__((unused)))
{
  __gcc_effect_suspend (__gcc_effect_find (&eff_a), NULL);
}

static void __attribute__((noipa))
run_inner (void)
{
  struct __gcc_coro *inner = __gcc_coro_create (inner_body, 0, 0);

  __gcc_coro_handler (inner)->effect = &eff_b;
  __gcc_coro_resume (inner);
}

static void __attribute__((noipa, optimize ("no-effects")))
foreign (void (*fn) (void))
{
  fn ();
}

static void
outer_body (void *arg __attribute__((unused)))
{
  foreign (run_inner);
}

int
main (void)
{
  struct __gcc_coro *c = __gcc_coro_create (outer_body, 0, 0);

  __gcc_coro_handler (c)->effect = &eff_a;
  __gcc_coro_resume (c);
  return 0;
}
