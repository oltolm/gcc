/* { dg-do compile } */
/* { dg-options "-O2 -mno-avx512f -mavx -mtune=generic" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target { ! ia32 } } {^\t?\.} } } */

/*
**foo:
**(
**.LFB[0-9]+:
**	.cfi_startproc
**|
**	.seh_endprologue
**)
**	vmovdqa	.LC[0-9]+\(%rip\), %xmm2
**	vpandn	%xmm0, %xmm2, %xmm0
**	vpand	%xmm2, %xmm1, %xmm1
**	vpor	%xmm1, %xmm0, %xmm0
**	ret
**...
*/

#include "builtin-copysign-8a.c"
