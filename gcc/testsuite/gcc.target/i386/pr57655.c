/* { dg-do compile } */
/* { dg-options "-mavx -mvzeroupper -mno-fp-ret-in-387" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

long double
foo (long double x) /* { dg-error "x87 register return with x87 disabled" "" { target { ! ia32 } } } */
{
  return __builtin_ilogbl (x);
}
