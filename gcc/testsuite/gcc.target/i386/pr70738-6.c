/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-msse2 -mgeneral-regs-only" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

extern float a, b, c;

void
foo (void)
{
  c = a * b; /* { dg-error "SSE register return with SSE disabled" } */
}
