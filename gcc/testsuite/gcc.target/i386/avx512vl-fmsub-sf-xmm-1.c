/* { dg-do compile } */
/* { dg-options "-mfma -mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vfmsub...ps\[ \\t\]+\\(%(?:eax|rdi|edi)\\)\\\{1to\[1-8\]+\\\}, %xmm\[0-9\]+, %xmm0" 1 { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-not "vbroadcastss\[^\n\]*%xmm\[0-9\]+" { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-times "vbroadcastss\[^\n\]*%xmm\[0-9\]+" 1 { target { *-*-mingw* && { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "vfmsub...ps\[ \\t\]+\\(%r(?:ax|bx|cx|dx|si|di|8|9|1[0-5])\\), %xmm\[0-9\]+, %xmm0" 1 { target { *-*-mingw* && { ! ia32 } } } } */

#define type __m128
#define vec
#define op fmsub
#define suffix ps
#define SCALAR float

#include "avx512-fma-1.h"
