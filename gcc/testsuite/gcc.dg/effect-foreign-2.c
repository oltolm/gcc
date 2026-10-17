/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects -fno-optimize-sibling-calls" } */
/* __gcc_effect_suspend must accept a capture across a run of functions
   compiled without -feffects when their -feffects caller has
   allow_foreign_capture.  __gcc_coro_yield is not a capture and must not
   check frames at all.  */

#include <stddef.h>
#include <stdlib.h>

struct __gcc_coro;
extern struct __gcc_coro *__gcc_coro_create (void (*) (void *), void *,
					     size_t);
extern int __gcc_coro_resume (struct __gcc_coro *);
extern void __gcc_coro_yield (void);
extern void __gcc_coro_destroy (struct __gcc_coro *);

struct __gcc_effect_handler
{
  struct __gcc_effect_handler *next;
  const void *effect;
  void *ops;
};

extern struct __gcc_effect_handler *__gcc_effect_find (const void *);
extern struct __gcc_effect_handler *__gcc_coro_handler (struct __gcc_coro *);
extern void __gcc_effect_suspend (struct __gcc_effect_handler *, void *);

static const char eff;
static int done;

static void __attribute__((noipa))
perform (void)
{
  __gcc_effect_suspend (__gcc_effect_find (&eff), NULL);
}

static void __attribute__((noipa, optimize ("no-effects")))
foreign2 (void (*fn) (void))
{
  fn ();
}

static void __attribute__((noipa, optimize ("no-effects")))
foreign (void (*fn) (void))
{
  foreign2 (fn);
}

static void __attribute__((allow_foreign_capture))
body (void *arg __attribute__((unused)))
{
  foreign (perform);
  done = 1;
}

static void
yield_body (void *arg __attribute__((unused)))
{
  foreign (__gcc_coro_yield);
}

int
main (void)
{
  struct __gcc_coro *c = __gcc_coro_create (body, 0, 0);

  __gcc_coro_handler (c)->effect = &eff;
  if (!__gcc_coro_resume (c) || done || __gcc_coro_resume (c) || !done)
    abort ();
  __gcc_coro_destroy (c);

  c = __gcc_coro_create (yield_body, 0, 0);
  if (!__gcc_coro_resume (c) || __gcc_coro_resume (c))
    abort ();
  __gcc_coro_destroy (c);
  return 0;
}
