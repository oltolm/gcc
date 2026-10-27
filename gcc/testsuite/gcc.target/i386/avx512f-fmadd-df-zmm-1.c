/* { dg-do compile } */
/* { dg-options "-mavx512f -O2" } */
/* { dg-final { scan-assembler-times "vfmadd...pd\[ \\t\]+\\(%(?:eax|rdi|edi)\\)\\\{1to\[1-8\]+\\\}, %zmm\[0-9\]+, %zmm0" 1 { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-not "vbroadcastsd\[^\n\]*%zmm\[0-9\]+" { target { { ! *-*-mingw* } || ia32 } } } */
/* { dg-final { scan-assembler-times "vbroadcastsd\[^\n\]*%zmm\[0-9\]+" 1 { target { *-*-mingw* && { ! ia32 } } } } */
/* { dg-final { scan-assembler-times "vfmadd...pd\[ \\t\]+\\(%r(?:ax|bx|cx|dx|si|di|8|9|1[0-5])\\), %zmm\[0-9\]+, %zmm0" 1 { target { *-*-mingw* && { ! ia32 } } } } */

#define type __m512d
#define vec 512
#define op fmadd
#define suffix pd
#define SCALAR double

#include "avx512-fma-1.h"
