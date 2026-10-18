/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2" } */
/* __gcc_dynamic_find (VAR) must return the storage of the innermost
   record for VAR in the handler chain, skipping records of other
   variables and effects, named-handler records (null effect) and records
   masked off by __gcc_effect_set_top, and must return VAR itself when no
   record matches.  */

#include <stdlib.h>

struct __gcc_effect_handler
{
  struct __gcc_effect_handler *next;
  const void *effect;
  void *ops;
};

extern struct __gcc_effect_handler *__gcc_effect_top (void);
extern void __gcc_effect_set_top (struct __gcc_effect_handler *);
extern void *__gcc_dynamic_find (void *);

static int x, y;
static const char eff;

static void
push (struct __gcc_effect_handler *h, const void *effect, void *ops)
{
  h->next = __gcc_effect_top ();
  h->effect = effect;
  h->ops = ops;
  __gcc_effect_set_top (h);
}

int
main (void)
{
  struct __gcc_effect_handler x1, e, y1, named, x2;
  int sx1, sy1, sx2;

  if (__gcc_dynamic_find (&x) != &x)
    abort ();
  push (&x1, &x, &sx1);
  push (&e, &eff, NULL);
  push (&y1, &y, &sy1);
  push (&named, NULL, NULL);
  if (__gcc_dynamic_find (&x) != &sx1
      || __gcc_dynamic_find (&y) != &sy1)
    abort ();
  push (&x2, &x, &sx2);
  if (__gcc_dynamic_find (&x) != &sx2)
    abort ();

  __gcc_effect_set_top (y1.next);
  if (__gcc_dynamic_find (&x) != &sx1
      || __gcc_dynamic_find (&y) != &y)
    abort ();
  __gcc_effect_set_top (NULL);
  if (__gcc_dynamic_find (&x) != &x)
    abort ();
  return 0;
}
