/* { dg-do compile { target int128 } } */
/* { dg-options "-O2 -msse2 -mtune=generic -dp" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

extern __int128 a, b;

__int128
foo (void)
{
  a = b;
  return b;
}

/* { dg-final { scan-assembler-not "movv1ti_internal" } } */
