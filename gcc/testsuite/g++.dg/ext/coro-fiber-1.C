// { dg-do run { target *-*-mingw* } }
// { dg-options "-O2" }
// An exception thrown and caught inside a coroutine after it has been
// resumed must be handled on the coroutine's stack.
// __gcc_coro_destroy of a suspended coroutine must run the destructors
// of every frame on its stack, innermost first.

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

struct __gcc_coro;
extern "C" {
  struct __gcc_coro *__gcc_coro_create (void (*) (void *), void *, size_t);
  int __gcc_coro_resume (struct __gcc_coro *);
  void __gcc_coro_yield (void);
  void __gcc_coro_destroy (struct __gcc_coro *);
}

static char trace[16];
static int n;

struct S
{
  char c;
  S (char c) : c (c) { }
  ~S () { trace[n++] = c; }
};

static void __attribute__((noipa))
thrower (int v)
{
  S s ('t');
  throw v;
}

static void __attribute__((noipa))
leaf ()
{
  S s ('l');
  __gcc_coro_yield ();
  try
    {
      thrower (42);
    }
  catch (int v)
    {
      if (v != 42)
	abort ();
      trace[n++] = 'c';
    }
  __gcc_coro_yield ();
  abort ();
}

static void
body (void *)
{
  S s ('b');
  leaf ();
  abort ();
}

int
main ()
{
  struct __gcc_coro *c = __gcc_coro_create (body, 0, 0);

  if (!__gcc_coro_resume (c) || n != 0)
    abort ();
  if (!__gcc_coro_resume (c))
    abort ();
  __gcc_coro_destroy (c);
  trace[n] = 0;
  if (strcmp (trace, "tclb") != 0)
    abort ();
  return 0;
}
