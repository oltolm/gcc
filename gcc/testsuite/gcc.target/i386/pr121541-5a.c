/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -march=x86-64" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

extern long double d;

__attribute__ ((target("80387","general-regs-only")))
void
func1 (void)
{
  d *= 3; /* { dg-error "x87 register return with x87 disabled" } */
}
