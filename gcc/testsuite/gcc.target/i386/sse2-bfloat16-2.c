/* { dg-do compile } */
/* { dg-options "-O2 -msse2 -mno-avx512f" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

union flt
{
  __bf16 flt;
  short s;
};

__bf16
foo (union flt x)
{
  return x.flt;
}

/* { dg-final { scan-assembler {(?n)pinsrw[\t ].*%xmm0} { target ia32 } } } */
/* { dg-final { scan-assembler {(?n)movd[\t ].*%xmm0} { target { ! ia32 } } } } */
