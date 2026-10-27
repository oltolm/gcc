/* { dg-do compile } */
/* { dg-options "-mavx512vl -O2" } */
/* { dg-final { scan-assembler-times "vdivps\[ \\t\]+\\(%(?:eax|rdi|edi)\\)\\\{1to\[1-8\]+\\\}, %ymm\[0-9\]+, %ymm0" 1 { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-not "vbroadcastss\[^\n\]*%ymm\[0-9\]+" { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-times "vbroadcastss\[^\n\]*%ymm\[0-9\]+" 1 { target { *-*-mingw* && { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "vdivps\[ \\t\]+\\(%r(?:ax|bx|cx|dx|si|di|8|9|1[0-5])\\), %ymm\[0-9\]+, %ymm0" 1 { target { *-*-mingw* && { ! ia32 } } } } */

#define type __m256
#define vec 256
#define op div
#define suffix ps
#define SCALAR float

#include "avx512-binop-1.h"
