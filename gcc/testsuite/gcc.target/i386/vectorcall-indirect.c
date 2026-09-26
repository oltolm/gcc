/* A call through a pointer to a vectorcall function must lay out the
   stack like the callee: a has an 8-byte home slot, so e is at 32[rsp].  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel" } */

typedef float v4sf __attribute__((vector_size (16)));

void (__attribute__((vectorcall)) *volatile fp) (v4sf, int, int, int, int);

void
caller (void)
{
  v4sf a = { 1, 2, 3, 4 };
  fp (a, 2, 3, 4, 5);
}

/* { dg-final { scan-assembler {mov[ \t]+DWORD PTR 32\[rsp\], 5} } } */
