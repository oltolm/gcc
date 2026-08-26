/* PR target/54412 */
/* An over-aligned prvalue passed down a chain of noinline calls.  This is the
   escaped-pointer case: the object reaches VEC8IB's constructor as a
   VEC256B const & and the callee accesses it assuming the 32-byte alignment
   its type declares, so the object has to really have it.  */
/* { dg-do run { target { x86_64-*-mingw* && avx } } } */
/* { dg-options "-O0 -mavx -std=gnu++17 -fno-omit-frame-pointer" } */

#include "avx-check.h"

typedef float v8sf __attribute__ ((vector_size (32), aligned (32)));

static inline v8sf
make_v8sf (float a0, float a1, float a2, float a3,
	   float a4, float a5, float a6, float a7)
{
  return (v8sf) { a0, a1, a2, a3, a4, a5, a6, a7 };
}

struct Vec8f
{
  v8sf ymm;

  Vec8f () = default;
  explicit Vec8f (v8sf x) : ymm (x) {}

  __attribute__ ((noinline, noclone))
  float
  extract (int index) const
  {
    float elems[8];
    __builtin_memcpy (elems, &ymm, sizeof (elems));
    return elems[index & 7];
  }
};

struct Vec16f
{
  Vec8f z0;
  Vec8f z1;

  Vec16f () = default;
  Vec16f (Vec8f a, Vec8f b) : z0 (a), z1 (b) {}

  __attribute__ ((noinline, noclone)) Vec8f get_low () const { return z0; }
  __attribute__ ((noinline, noclone)) Vec8f get_high () const { return z1; }
};

struct Vec256b
{
  int v[8];

  Vec256b () = default;
  explicit Vec256b (int x0, int x1, int x2, int x3,
		    int x4, int x5, int x6, int x7)
    : v { x0, x1, x2, x3, x4, x5, x6, x7 }
  {}

  __attribute__ ((noinline, noclone))
  int
  get_low () const
  {
    return v[0];
  }
};

struct Vec8ib
{
  int first;

  __attribute__ ((noinline, noclone))
  explicit Vec8ib (Vec256b const &x) : first (x.get_low ()) {}
};

__attribute__ ((noinline, noclone))
static Vec16f
passthru (Vec16f x)
{
  return x;
}

__attribute__ ((noinline, noclone))
static Vec8f
low_after_passthru (Vec16f x)
{
  return passthru (x).get_low ();
}

__attribute__ ((noinline, noclone))
static Vec16f
split_rebuild_after_passthru (Vec16f x)
{
  Vec16f y = passthru (x);
  return Vec16f (y.get_low (), y.get_high ());
}

__attribute__ ((noinline, noclone))
static Vec8ib
wrap_low_ref (Vec256b x)
{
  return Vec8ib (x);
}

static void
avx_test ()
{
  Vec16f x (Vec8f (make_v8sf (1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f)),
	   Vec8f (make_v8sf (9.f, 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f)));
  Vec8f y = low_after_passthru (x);

  if (y.extract (0) != 1.f || y.extract (7) != 8.f)
    __builtin_abort ();

  Vec16f z = split_rebuild_after_passthru (x);
  Vec8f zl = z.get_low ();
  Vec8f zh = z.get_high ();

  if (zl.extract (0) != 1.f || zl.extract (7) != 8.f)
    __builtin_abort ();

  if (zh.extract (0) != 9.f || zh.extract (7) != 16.f)
    __builtin_abort ();

  Vec8ib w = wrap_low_ref (Vec256b (11, 12, 13, 14, 15, 16, 17, 18));
  if (w.first != 11)
    __builtin_abort ();
}
