/* { dg-do compile { target int128 } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

__int128 foo(long long x)
{
  return (__int128)x;
}
/* { dg-final { scan-assembler "cqt?o" } } */
