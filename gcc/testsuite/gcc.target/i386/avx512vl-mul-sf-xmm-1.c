/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vmulps\[ \\t\]+\\(%(?:eax|rdi|edi)\\)\\\{1to\[1-8\]+\\\}, %xmm\[0-9\]+, %xmm0" 1 { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-not "vbroadcastss\[^\n\]*%xmm\[0-9\]+" { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-times "vbroadcastss\[^\n\]*%xmm\[0-9\]+" 1 { target { *-*-mingw* && { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "vmulps\[ \\t\]+\\(%r(?:ax|bx|cx|dx|si|di|8|9|1[0-5])\\), %xmm\[0-9\]+, %xmm0" 1 { target { *-*-mingw* && { ! ia32 } } } } */

#define type __m128
#define vec
#define op mul
#define suffix ps
#define SCALAR float

#include "avx512-binop-1.h"
