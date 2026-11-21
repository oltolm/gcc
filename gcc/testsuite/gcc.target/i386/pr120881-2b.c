/* { dg-do compile } */
/* { dg-options "-O2 -fdump-rtl-pro_and_epilogue -march=x86-64" } */
/* { dg-skip-if "SEH has no simple_return, so no shrink-wrapping" { x86_64-*-mingw* } } */
/* { dg-final { scan-rtl-dump "Now spread 1 times" "pro_and_epilogue" } } */

#include "pr120881-2a.c"

