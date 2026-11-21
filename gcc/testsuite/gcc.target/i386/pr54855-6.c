/* { dg-do compile } */
/* { dg-options "-O2 -msse -mfpmath=sse" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */
/* { dg-final { scan-assembler-times "divss" 1 } } */
/* { dg-final { scan-assembler-not "movaps" } } */
/* { dg-final { scan-assembler-not "movss" } } */

typedef float vec __attribute__((vector_size(16)));

vec
foo (vec x, float f)
{
  x[0] /= f;
  return x;
}
