/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects -fexceptions" } */
/* __gcc_coro_destroy of a coroutine suspended by __gcc_effect_suspend from
   an inner coroutine must run the cleanups of both coroutines, inner
   first, and return to the destroyer with its handler chain intact.  */

#include <stddef.h>
#include <stdlib.h>

struct __gcc_coro;
extern struct __gcc_coro *__gcc_coro_create (void (*) (void *), void *,
					     size_t);
extern int __gcc_coro_resume (struct __gcc_coro *);
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
extern struct __gcc_effect_handler *__gcc_coro_handler (struct __gcc_coro *);
extern void __gcc_effect_suspend (struct __gcc_effect_handler *, void *);

static const char eff_a, eff_b;
static char trace[8];
static int n;

static void
record (char *p)
{
  trace[n++] = *p;
}

static void
destroy (struct __gcc_coro **c)
{
  __gcc_coro_destroy (*c);
}

static void
inner_body (void *arg __attribute__((unused)))
{
  char c __attribute__((cleanup (record))) = 'i';
  __gcc_effect_suspend (__gcc_effect_find (&eff_a), NULL);
  abort ();
}

static void
outer_body (void *arg __attribute__((unused)))
{
  char c __attribute__((cleanup (record))) = 'o';
  struct __gcc_coro *inner __attribute__((cleanup (destroy)))
    = __gcc_coro_create (inner_body, 0, 0);

  __gcc_coro_handler (inner)->effect = &eff_b;
  __gcc_coro_resume (inner);
  abort ();
}

int
main (void)
{
  struct __gcc_effect_handler outer = { NULL, &eff_b, NULL };
  struct __gcc_coro *c = __gcc_coro_create (outer_body, 0, 0);

  __gcc_effect_set_top (&outer);
  __gcc_coro_handler (c)->effect = &eff_a;
  if (!__gcc_coro_resume (c) || n != 0)
    abort ();
  __gcc_coro_destroy (c);
  trace[n] = 0;
  if (__builtin_strcmp (trace, "io") != 0 || __gcc_effect_top () != &outer)
    abort ();
  return 0;
}
