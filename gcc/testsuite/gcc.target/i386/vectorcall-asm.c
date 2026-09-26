/* Test 64-bit vectorcall argument passing - force parameters to be used.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -fno-inline -masm=intel -fno-optimize-sibling-calls" } */

typedef float v4sf __attribute__((vector_size (16)));
typedef double v2df __attribute__((vector_size (16)));

volatile int sink;
v4sf gv;
v2df gd;

/* Force use of vector arguments by returning the result.  */
v4sf __attribute__((vectorcall))
test_vec_args (v4sf a, v4sf b)
{
  return a + b;  /* Force use of XMM0, XMM1 */
}

/* Registers are assigned by position: a in RCX, b in XMM1, c in R8,
   d in XMM3.  */
float __attribute__((vectorcall))
test_mixed_args (int a, v4sf b, int c, float d)
{
  return (float)a + b[0] + (float)c + d;
}

/* HVA with 1 float field.  */
struct hva1f {
  float x;
};

float __attribute__((vectorcall))
test_hva1f (struct hva1f a)
{
  return a.x + a.x;  /* Force arithmetic use of XMM0.  */
}

/* HVA with 2 float fields.  */
struct hva2f {
  float x, y;
};

float __attribute__((vectorcall))
test_hva2f (struct hva2f a)
{
  return a.y;  /* Should use XMM1.  */
}

/* HVA with 3 float fields.  */
struct hva3f {
  float x, y, z;
};

float __attribute__((vectorcall))
test_hva3f (struct hva3f a)
{
  return a.z;  /* Should use XMM2.  */
}

/* HVA with 4 float fields.  */
struct hva4f {
  float x, y, z, w;
};

float __attribute__((vectorcall))
test_hva4f (struct hva4f a)
{
  return a.w;  /* Should use XMM3.  */
}

/* HVA with 2 double fields.  */
struct hva2d {
  double x, y;
};

extern void __attribute__((vectorcall)) callee_hva2f (struct hva2f);
extern void __attribute__((vectorcall)) callee_hva4f (struct hva4f);
extern void __attribute__((vectorcall)) callee_hva2d (struct hva2d);

/* Caller passes a 2-float HVA: fields must be in XMM0 and XMM1.  */
void
call_hva2f (float x, float y)
{
  struct hva2f a = { x, y };
  callee_hva2f (a);
}

/* Caller passes a 4-float HVA: fields must be in XMM0-XMM3.  */
void
call_hva4f (float x, float y, float z, float w)
{
  struct hva4f a = { x, y, z, w };
  callee_hva4f (a);
}

/* Caller passes a 2-double HVA: fields must be in XMM0 and XMM1.  */
void
call_hva2d (double x, double y)
{
  struct hva2d a = { x, y };
  callee_hva2d (a);
}

/* Additional HVA / non-HVA classification coverage.  */
struct hva2v4sf {
  v4sf a, b;
};

void __attribute__((vectorcall))
test_hva2v4sf (struct hva2v4sf a)
{
  gv = a.b;
}

struct hva4v2df {
  v2df a, b, c, d;
};

void __attribute__((vectorcall))
test_hva4v2df (struct hva4v2df a)
{
  gd = a.d;
}

struct not_hva {
  float x;
  int y;
};

void __attribute__((vectorcall))
test_not_hva (struct not_hva a)
{
  sink = a.y;
}

struct hva5f {
  float a, b, c, d, e;
};

void __attribute__((vectorcall))
test_hva5f (struct hva5f a)
{
  sink = (int)a.a;
}

struct hva4f __attribute__((vectorcall))
test_hva4f_return (void)
{
  struct hva4f r = {1.0f, 2.0f, 3.0f, 4.0f};
  return r;
}

/* { dg-final { scan-assembler "addps\\txmm0, xmm1" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {"test_mixed_args@@[0-9]+":[^"]*addss[ \t]+xmm0, xmm1} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {cvtsi2ss[ \t]+xmm[0-9]+, r8d} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {addss[ \t]+xmm0, xmm3} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {"test_hva1f@@[0-9]+":\s*\.seh_endprologue\s*addss[ \t]+xmm0, xmm0} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {"test_hva2f@@[0-9]+":[^"]*movaps[ \t]+xmm0, xmm1} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {"test_hva3f@@[0-9]+":[^"]*movaps[ \t]+xmm0, xmm2} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {"test_hva4f@@[0-9]+":[^"]*movaps[ \t]+xmm0, xmm3} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {movaps[ \t]+XMMWORD PTR "?gv"?\[rip\], xmm1} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {movap[sd][ \t]+XMMWORD PTR "?gd"?\[rip\], xmm3} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {sar[ \t]+rcx, 32} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {cvttss2si[ \t]+eax, DWORD PTR \[rcx\]} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {"test_hva4f_return@@0":[^"]*[ \t]xmm3, } { target x86_64-*-mingw* } } } */
/* Caller-side HVA checks: HVA fields must go to XMM, not be spilled to stack
   with a lea into an integer register.  */
/* { dg-final { scan-assembler-not {lea[ \t]+[er]cx,.*rsp} { target x86_64-*-mingw* } } } */
