/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -fexceptions" } */
/* A coroutine's handler chain extends the chain of whoever resumed it
   last: records pushed inside the coroutine are visible only while it
   runs, and after a resume from a different chain the coroutine finds
   the new resumer's handlers.  __gcc_coro_destroy runs the coroutine's
   cleanups with the coroutine's chain current.  */

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

extern struct __gcc_effect_handler *__gcc_effect_top (void);
extern void __gcc_effect_set_top (struct __gcc_effect_handler *);
extern struct __gcc_effect_handler *__gcc_effect_find (const void *);

static const char eff_a, eff_b;
static struct __gcc_effect_handler outer1, outer2;
static int popped;

static void
push (struct __gcc_effect_handler *h, const void *effect)
{
  h->next = __gcc_effect_top ();
  h->effect = effect;
  __gcc_effect_set_top (h);
}

static void
pop (struct __gcc_effect_handler *h)
{
  if (__gcc_effect_top () != h)
    abort ();
  __gcc_effect_set_top (h->next);
  popped = 1;
}

static void
body (void *arg __attribute__((unused)))
{
  struct __gcc_effect_handler in __attribute__((cleanup (pop)));

  if (__gcc_effect_find (&eff_a) != &outer1)
    abort ();
  push (&in, &eff_b);
  __gcc_coro_yield ();
  if (__gcc_effect_find (&eff_a) != &outer2
      || __gcc_effect_find (&eff_b) != &in)
    abort ();
  __gcc_coro_yield ();
  abort ();
}

int
main (void)
{
  struct __gcc_coro *c = __gcc_coro_create (body, 0, 0);

  push (&outer1, &eff_a);
  if (!__gcc_coro_resume (c) || __gcc_effect_top () != &outer1)
    abort ();

  push (&outer2, &eff_a);
  if (!__gcc_coro_resume (c) || __gcc_effect_top () != &outer2)
    abort ();
  __gcc_effect_set_top (outer2.next);

  __gcc_coro_destroy (c);
  if (!popped || __gcc_effect_top () != &outer1)
    abort ();
  return 0;
}
