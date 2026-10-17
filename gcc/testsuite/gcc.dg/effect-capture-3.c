/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* A suspended coroutine resumed from a different place with a different
   operation table in its handler record (shallow re-handle) must see the
   new table and the new resumer's chain, not the ones of the original
   resume.  */

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

static const char eff, eff_ctx;
static int ops1, ops2;
static struct __gcc_effect_handler ctx2 = { NULL, &eff_ctx, NULL };
static int checked;

static void
body (void *arg __attribute__((unused)))
{
  __gcc_effect_suspend (__gcc_effect_find (&eff), NULL);
  if (__gcc_effect_find (&eff)->ops != &ops2
      || __gcc_effect_find (&eff_ctx) != &ctx2)
    abort ();
  checked = 1;
}

static struct __gcc_coro * __attribute__((noipa))
start (void)
{
  struct __gcc_effect_handler ctx1 = { __gcc_effect_top (), &eff_ctx, NULL };
  struct __gcc_coro *c = __gcc_coro_create (body, 0, 0);
  struct __gcc_effect_handler *h = __gcc_coro_handler (c);

  __gcc_effect_set_top (&ctx1);
  h->effect = &eff;
  h->ops = &ops1;
  if (!__gcc_coro_resume (c))
    abort ();
  __gcc_effect_set_top (ctx1.next);
  return c;
}

int
main (void)
{
  struct __gcc_coro *c = start ();

  __gcc_effect_set_top (&ctx2);
  __gcc_coro_handler (c)->ops = &ops2;
  if (__gcc_coro_resume (c) || !checked)
    abort ();
  __gcc_coro_destroy (c);
  return 0;
}
