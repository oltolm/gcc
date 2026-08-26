/* PR target/54412 */
/* Same root cause as pr54412-m256-byval-param.c, for a 64-byte aligned
   AVX-512 by-value parameter.  A crash-based (dg-do run) test is not
   reliable here either: whether the under-aligned home slot happens to
   coincidentally land on a 64-byte boundary depends on this function's
   exact frame size.  Check the selected instruction instead: vmovapd
   assumes alignment, vmovupd does not, and MEM_ALIGN (not the real
   runtime address) is what decides between them.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O0 -mavx512f" } */
/* { dg-final { scan-assembler-not {vmovapd\t%zmm0, -[0-9]+\(%rbp\)} } } */

typedef double v8d __attribute__ ((vector_size (64), aligned (64)));

__attribute__ ((noinline)) v8d
roundtrip_v8d (v8d x)
{
  return x;
}
