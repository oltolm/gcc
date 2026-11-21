/* { dg-do compile } */
/* { dg-options "-O -fcf-protection=return -mshstk" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */
/* { dg-final { scan-assembler "rdsspd|incsspd\[ \t]+(%|)eax" { target ia32 } } } */
/* { dg-final { scan-assembler "rdssp\[dq]\[ \t]+(%|)\[re]ax"  { target { ! ia32 } } } } */
/* { dg-final { scan-assembler "incssp\[dq]\[ \t]+(%|)\[re]di" { target { ! ia32 } } } } */

#include <immintrin.h>
#include "cet-intrin-3.c"
