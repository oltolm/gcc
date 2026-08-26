/* PR target/54412 */
/* Same root cause as pr54412-m256-byval-param.c, for a vector_size(32)
   double by-value parameter (no explicit "aligned" attribute -- the
   vector type's natural alignment already requires 32 bytes).  A
   crash-based (dg-do run) test is not reliable here: whether the
   under-aligned home slot happens to coincidentally land on a 32-byte
   boundary depends on this function's exact frame size.  Check the
   selected instruction instead: vmovapd assumes alignment, vmovupd
   does not, and MEM_ALIGN (not the real runtime address) is what
   decides between them.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O0 -mavx" } */
/* { dg-final { scan-assembler-not {vmovapd\t%ymm0, -[0-9]+\(%rbp\)} } } */

typedef double v4d __attribute__ ((vector_size (32)));

__attribute__ ((noinline)) v4d
f (v4d x)
{
  return x;
}
