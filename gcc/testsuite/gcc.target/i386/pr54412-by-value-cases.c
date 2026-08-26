/* PR target/54412 */
/* Same root cause as pr54412-m256-byval-param.c, across a plain vector,
   a __m256, and a struct wrapping a __m256, all passed by value.  A
   crash-based (dg-do run) test is not reliable here: whether the
   under-aligned home slot happens to coincidentally land on a 32-byte
   boundary depends on each function's exact frame size.  Check the
   selected instruction instead: vmovap{s,d} assumes alignment,
   vmovup{s,d} does not, and MEM_ALIGN (not the real runtime address)
   is what decides between them.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O0 -mavx" } */
/* { dg-final { scan-assembler-not {vmovap[sd]\t%ymm0, -[0-9]+\(%rbp\)} } } */

#include <immintrin.h>

typedef double v4d __attribute__ ((vector_size (32), aligned (32)));

struct avx_wrapper
{
  __m256 value;
};

__attribute__ ((noinline)) v4d
roundtrip_v4d (v4d x)
{
  return x;
}

__attribute__ ((noinline)) __m256
pass_m256 (__m256 x)
{
  return x;
}

__attribute__ ((noinline)) struct avx_wrapper
pass_wrapper (struct avx_wrapper x)
{
  return x;
}
