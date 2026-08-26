/* PR target/54412 */
/* Based on MSYS2/MINGW-packages#1209: passing an __m256 by value at -O0
   crashed on Win64 SEH targets because the incoming-argument copy did not
   get the 32-byte alignment the AVX load/store instructions assumed.

   A crash-based (dg-do run) test for this is not reliable: whether the
   under-aligned home slot happens to still land on a 32-byte boundary
   depends on this function's exact frame size, which varies with
   unrelated codegen changes.  Check the actual instruction GCC selects
   for the home-slot store instead: vmovaps assumes 32-byte alignment,
   vmovups does not, and MEM_ALIGN (not the real runtime address) is
   what decides between them.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O0 -mavx" } */
/* { dg-final { scan-assembler-not {vmovaps\t%ymm0, -[0-9]+\(%rbp\)} } } */

#include <immintrin.h>

void
foo (__m256 x)
{
  (void) x;
}
