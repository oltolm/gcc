/* PR c/80163 */
/* { dg-do compile { target int128 } } */
/* { dg-require-effective-target label_values } */

volatile int v;

__attribute__((noinline, noclone)) void
bar (void)
{
  v++;
  asm volatile ("" : : : "memory");
}

__attribute__((noinline, noclone)) __int128_t *
foo (__UINTPTR_TYPE__ **p)
{
a:
  bar ();
b:
  bar ();
  static __int128_t d = (__UINTPTR_TYPE__) &&a - (__UINTPTR_TYPE__) &&b;
  static __UINTPTR_TYPE__ e = (__UINTPTR_TYPE__) &&a - (__UINTPTR_TYPE__) &&b;
  *p = &e;
  return &d;
}

int
main ()
{
  __int128_t *p;
  __UINTPTR_TYPE__ *q;
  p = foo (&q);
  if (*p != *q)
    __builtin_abort ();
  return 0;
}
