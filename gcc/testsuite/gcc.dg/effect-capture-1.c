/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* __gcc_effect_suspend through the handler record of a coroutine must
   return from the __gcc_coro_resume that ran it, with the message
   available to the resumer and the coroutine's handler record not in the
   resumer's chain.  The next resume returns from the suspend.  */

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

extern struct __gcc_effect_handler *__gcc_effect_top (void);
extern void __gcc_effect_set_top (struct __gcc_effect_handler *);
extern struct __gcc_effect_handler *__gcc_effect_find (const void *);
extern struct __gcc_effect_handler *__gcc_coro_handler (struct __gcc_coro *);
extern void __gcc_effect_suspend (struct __gcc_effect_handler *, void *);

static const char eff, eff_other;
static struct __gcc_effect_handler *handler;
static int result;

struct ask
{
  int question;
  int answer;
};

static int __attribute__((noipa))
ask (int question)
{
  struct ask m = { question, 0 };
  struct __gcc_effect_handler *h = __gcc_effect_find (&eff);

  if (h != handler)
    abort ();
  __gcc_effect_suspend (h, &m);
  return m.answer;
}

static void
body (void *arg __attribute__((unused)))
{
  result = ask (1) + ask (2);
}

int
main (void)
{
  struct __gcc_effect_handler outer = { NULL, &eff_other, NULL };
  struct __gcc_coro *c = __gcc_coro_create (body, 0, 0);
  int n = 0;

  __gcc_effect_set_top (&outer);
  handler = __gcc_coro_handler (c);
  handler->effect = &eff;
  while (__gcc_coro_resume (c))
    {
      struct ask *m = __gcc_coro_message (c);

      if (__gcc_effect_top () != &outer)
	abort ();
      m->answer = m->question * 10;
      n++;
    }
  if (n != 2 || result != 30 || __gcc_effect_top () != &outer)
    abort ();
  __gcc_coro_destroy (c);
  return 0;
}
