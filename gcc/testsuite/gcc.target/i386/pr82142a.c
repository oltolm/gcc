/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mno-avx -msse2" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */
/* Keep labels and directives ('.cfi_startproc', '.cfi_endproc').  */
/* { dg-final { check-function-bodies "**" "" "" { target "*-*-*" } {^\t?\.}  } } */

/*
**assignzero:
**(
**.LFB0:
**	.cfi_startproc
**|
**	.seh_endprologue
**)
**	pxor	%xmm0, %xmm0
**	movups	%xmm0, 32\(%(?:r|e)di\)
**	movups	%xmm0, \(%(?:r|e)di\)
**	movups	%xmm0, 16\(%(?:r|e)di\)
**	movups	%xmm0, 44\(%(?:r|e)di\)
**	ret
**...
*/

typedef struct
{
  int a, b, c;
  char j, k, k1;
  int l, m, n[8];
  char c1, c2;
} foo;

void
assignzero (foo *p)
{
  foo tmp = {};
  *p = tmp;
}
