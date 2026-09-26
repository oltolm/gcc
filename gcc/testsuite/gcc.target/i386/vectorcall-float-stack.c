/* Vectorcall passes SIMD vectors in the seventh and later positions by
   reference, but float and double there by value in their stack slot, as
   with MSVC.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel -fno-inline -fno-optimize-sibling-calls" } */

extern void __attribute__((vectorcall))
fd (int, int, int, int, int, int, double);
extern void __attribute__((vectorcall))
ff (int, int, int, int, int, int, float);

void
call_d (double x)
{
  fd (1, 2, 3, 4, 5, 6, x);
}

void
call_f (float x)
{
  ff (1, 2, 3, 4, 5, 6, x);
}

double __attribute__((vectorcall))
def_d (int a, int b, int c, int d, int e, int f, double g)
{
  return g;
}

/* { dg-final { scan-assembler {movsd[ \t]+QWORD PTR 48\[rsp\], xmm0} } } */
/* { dg-final { scan-assembler {movss[ \t]+DWORD PTR 48\[rsp\], xmm0} } } */
/* { dg-final { scan-assembler {movsd[ \t]+xmm0, QWORD PTR 56\[rsp\]} } } */
