/* { dg-do compile } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-final { scan-assembler-times "vaddps\[ \\t\]+\\(%(?:eax|rdi|edi)\\)\\\{1to\[1-8\]+\\\}, %zmm\[0-9\]+, %zmm0" 1 { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-not "vbroadcastss\[^\n\]*%zmm\[0-9\]+" { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-times "vbroadcastss\[^\n\]*%zmm\[0-9\]+" 1 { target { *-*-mingw* && { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "vaddps\[ \\t\]+\\(%r(?:ax|bx|cx|dx|si|di|8|9|1[0-5])\\), %zmm\[0-9\]+, %zmm0" 1 { target { *-*-mingw* && { ! ia32 } } } } */

#define type __m512
#define vec 512
#define op add
#define suffix ps
#define SCALAR float

#include "avx512-binop-2.h"
