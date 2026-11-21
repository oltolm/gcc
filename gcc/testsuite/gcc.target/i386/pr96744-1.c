/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-msse2" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

typedef int int32x2_t __attribute__ ((__vector_size__ ((8))));

__attribute__((__target__("general-regs-only")))
int32x2_t test (int32x2_t a, int32x2_t b) /* { dg-error "SSE register return with SSE disabled" } */
{
  return a + b;
}
