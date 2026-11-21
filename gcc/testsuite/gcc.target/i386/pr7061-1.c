/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */
float re(float _Complex a) { return __real__ a; }
/* { dg-final { scan-assembler-not "mov" } } */
