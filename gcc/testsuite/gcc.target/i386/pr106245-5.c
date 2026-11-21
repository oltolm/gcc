/* { dg-do compile { target int128 } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

__int128 f(__int128 a)
{
  return (a << 127) >> 127;
}

/* { dg-final { scan-assembler "andl" } } */
/* { dg-final { scan-assembler "negq" } } */
/* { dg-final { scan-assembler "cqto" } } */
