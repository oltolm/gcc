/* A union counts as an HVA with as many elements as its largest member,
   as with MSVC.  u1 is a one-element HVA in XMM0, and s2 is a two-element
   HVA in XMM0 and XMM1.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel" } */

typedef float v4sf __attribute__((vector_size (16)));

union u1 { v4sf a; v4sf b; };
struct s2 { union u1 x; v4sf y; };

v4sf gu, gy;

void __attribute__((vectorcall))
fu (union u1 u)
{
  gu = u.a;
}

void __attribute__((vectorcall))
fs (int i, struct s2 s)
{
  gy = s.y;
}

/* { dg-final { scan-assembler {movaps[ \t]+XMMWORD PTR "?gu"?\[rip\], xmm0} } } */
/* { dg-final { scan-assembler {movaps[ \t]+XMMWORD PTR "?gy"?\[rip\], xmm1} } } */
