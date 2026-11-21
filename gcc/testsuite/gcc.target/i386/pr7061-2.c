/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */
float im(float _Complex a) { return __imag__ a; }
/* { dg-final { scan-assembler "shufps" } } */
/* { dg-final { scan-assembler-not "movd" } } */
/* { dg-final { scan-assembler-not "movq" } } */
/* { dg-final { scan-assembler-not "movss" } } */
/* { dg-final { scan-assembler-not "rsp" } } */
/* { dg-final { scan-assembler-not "shr" } } */
