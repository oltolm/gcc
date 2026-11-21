/* { dg-do compile } */
/* { dg-options "-O2 -mavx2" } */
/* { dg-skip-if "SEH limits stack alignment to 16 bytes" { x86_64-*-mingw* } } */
/* { dg-final { scan-assembler "and\[lq\]?\[\\t \]*\\$-32,\[\\t \]*%\[re\]?sp" } } */

#include "pr120839-1a.c"
