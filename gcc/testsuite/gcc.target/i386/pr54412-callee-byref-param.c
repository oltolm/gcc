/* PR target/54412 */
/* Same root cause as pr54412-m256-byval-param.c, for a plain
   vector_size(32) by-value parameter.  A crash-based (dg-do run) test
   is not reliable here: whether the under-aligned home slot happens to
   coincidentally land on a 32-byte boundary depends on this function's
   exact frame size.  Check the selected instruction instead: vmovaps
   assumes alignment, vmovups does not, and MEM_ALIGN (not the real
   runtime address) is what decides between them.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O0 -mavx" } */
/* { dg-final { scan-assembler-not {vmovaps\t%ymm0, -[0-9]+\(%rbp\)} } } */

typedef float v8f __attribute__ ((vector_size (32), aligned (32)));

__attribute__ ((noinline)) v8f
param_matches (v8f x)
{
  return x;
}
