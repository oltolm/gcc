/* { dg-do compile } */
/* { dg-options "-O2 -msse4.1 -mtune=intel" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */
/* { dg-require-effective-target int128 } */

__int128 test (__int128 a)
{
  asm ("" : "+x" (a));
  return a;
}

/* { dg-final { scan-assembler "pinsrq" } } */
/* { dg-final { scan-assembler "pextrq" } } */
