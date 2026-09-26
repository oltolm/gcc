/* The x87 long double is not a vectorcall vector type.  It is passed by
   reference in its position's integer register and takes no XMM register,
   so the HVA h still gets XMM0 and XMM1.  A struct of long doubles is not
   an HVA.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel" } */

typedef float v4sf __attribute__((vector_size (16)));

struct hva2 { v4sf x, y; };
struct ld2 { long double a, b; };

long double gl, gm;
v4sf gx, gy;

void __attribute__((vectorcall))
f (long double a, struct ld2 s, struct hva2 h)
{
  gl = a;
  gm = s.b;
  gx = h.x;
  gy = h.y;
}

/* { dg-final { scan-assembler {fld[ \t]+TBYTE PTR \[rcx\]} } } */
/* { dg-final { scan-assembler {fld[ \t]+TBYTE PTR 16\[rdx\]} } } */
/* { dg-final { scan-assembler {movaps[ \t]+XMMWORD PTR "?gx"?\[rip\], xmm0} } } */
/* { dg-final { scan-assembler {movaps[ \t]+XMMWORD PTR "?gy"?\[rip\], xmm1} } } */
