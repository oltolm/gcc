/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects -fno-optimize-sibling-calls" } */
/* { dg-shouldfail "capture across a foreign frame" } */
/* __gcc_effect_suspend must reject a capture whose frames include a
   function compiled without -feffects called from a function without
   allow_foreign_capture.  */

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

static const char eff;

static void __attribute__((noipa))
perform (void)
{
  __gcc_effect_suspend (__gcc_effect_find (&eff), NULL);
}

static void __attribute__((noipa, optimize ("no-effects")))
foreign (void (*fn) (void))
{
  fn ();
}

static void
body (void *arg __attribute__((unused)))
{
  foreign (perform);
}

int
main (void)
{
  struct __gcc_coro *c = __gcc_coro_create (body, 0, 0);

  __gcc_coro_handler (c)->effect = &eff;
  __gcc_coro_resume (c);
  return 0;
}
