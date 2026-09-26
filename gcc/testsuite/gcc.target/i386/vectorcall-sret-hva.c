/* The hidden return-value pointer of a vectorcall function is argument
   position 0.  a is in position 1 and takes XMM1, so the HVA h gets the
   lowest free registers, XMM0 and XMM2, as with MSVC.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel" } */

typedef float v4sf __attribute__((vector_size (16)));

struct big { int a[5]; };
struct hva2 { v4sf x, y; };

v4sf gx, gy;
float ga;

struct big __attribute__((vectorcall))
f (float a, struct hva2 h)
{
  struct big r = { { 0 } };
  ga = a;
  gx = h.x;
  gy = h.y;
  return r;
}

/* { dg-final { scan-assembler {movss[ \t]+DWORD PTR "?ga"?\[rip\], xmm1} } } */
/* { dg-final { scan-assembler {movaps[ \t]+XMMWORD PTR "?gx"?\[rip\], xmm0} } } */
/* { dg-final { scan-assembler {movaps[ \t]+XMMWORD PTR "?gy"?\[rip\], xmm2} } } */
