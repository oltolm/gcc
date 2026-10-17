/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2" } */
/* __gcc_effect_find must return the innermost record for an effect in the
   current thread's handler chain, skipping records of other effects and
   records masked off by __gcc_effect_set_top.  A new thread starts with
   an empty chain.  */

#include <stdlib.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

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

static void
push (struct __gcc_effect_handler *h, const void *effect)
{
  h->next = __gcc_effect_top ();
  h->effect = effect;
  __gcc_effect_set_top (h);
}

static DWORD WINAPI
other_thread (void *arg)
{
  *(struct __gcc_effect_handler **) arg = __gcc_effect_top ();
  return 0;
}

int
main (void)
{
  struct __gcc_effect_handler a1, b, a2, *seen = &a1;
  HANDLE t;

  if (__gcc_effect_top ())
    abort ();
  push (&a1, &eff_a);
  push (&b, &eff_b);
  push (&a2, &eff_a);
  if (__gcc_effect_find (&eff_a) != &a2
      || __gcc_effect_find (&eff_b) != &b)
    abort ();

  /* An operation of a2 runs with the chain below a2.  */
  __gcc_effect_set_top (a2.next);
  if (__gcc_effect_find (&eff_a) != &a1)
    abort ();
  __gcc_effect_set_top (&a2);

  t = CreateThread (NULL, 0, other_thread, &seen, 0, NULL);
  if (!t || WaitForSingleObject (t, INFINITE) != WAIT_OBJECT_0 || seen)
    abort ();
  CloseHandle (t);
  if (__gcc_effect_top () != &a2)
    abort ();
  return 0;
}
