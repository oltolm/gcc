/* { dg-do run { target *-*-mingw* } } */
/* { dg-options "-O2" } */
/* { dg-shouldfail "unhandled effect" } */
/* __gcc_effect_find of an effect with no handler in the chain is a
   runtime error.  */

struct __gcc_effect_handler;
extern struct __gcc_effect_handler *__gcc_effect_find (const void *);

static const char eff;

int
main (void)
{
  __gcc_effect_find (&eff);
  return 0;
}
