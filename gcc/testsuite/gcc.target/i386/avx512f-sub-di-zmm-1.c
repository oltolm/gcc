/* { dg-do compile } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-final { scan-assembler-times "vpsubq\[ \\t\]+\\(%(?:eax|rdi|edi)\\)\\\{1to\[1-8\]+\\\}, %zmm\[0-9\]+, %zmm0" 1 { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-not "vpbroadcastq\[^\n\]*%zmm\[0-9\]+" { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-times "vpbroadcastq\[^\n\]*%zmm\[0-9\]+" 1 { target { *-*-mingw* && { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "vpsubq\[ \\t\]+\\(%r(?:ax|bx|cx|dx|si|di|8|9|1[0-5])\\), %zmm\[0-9\]+, %zmm0" 1 { target { *-*-mingw* && { ! ia32 } } } } */

#define type __m512i
#define vec 512
#define op sub
#define suffix epi64
#define SCALAR long long

#include "avx512-binop-1.h"
