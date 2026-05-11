/* Test 64-bit vectorcall argument passing - force parameters to be used.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -fno-inline -masm=intel -fno-optimize-sibling-calls" } */

typedef float v4sf __attribute__((vector_size (16)));
typedef double v2df __attribute__((vector_size (16)));
typedef int v4si __attribute__((vector_size (16)));

volatile int sink;

/* Force use of vector arguments by returning the result.  */
v4sf __attribute__((vectorcall))
test_vec_args (v4sf a, v4sf b)
{
  return a + b;  /* Force use of XMM0, XMM1 */
}

/* Force use of integer arguments.  */
int __attribute__((vectorcall))
test_int_args (int a, int b, int c, int d)
{
  return a + b + c + d;  /* Force use of RCX, RDX, R8, R9 */
}

/* Force use of mixed arguments.  */
float __attribute__((vectorcall))
test_mixed_args (int a, v4sf b, int c, float d)
{
  return (float)a + b[0] + (float)c + d;  /* Force use of RCX, XMM0, RDX, XMM1 */
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
  return a.x + a.y;  /* Should use XMM0, XMM1 */
}

/* HVA with 3 float fields.  */
struct hva3f {
  float x, y, z;
};

float __attribute__((vectorcall))
test_hva3f (struct hva3f a)
{
  return a.x + a.y + a.z;  /* Should use XMM0-XMM2.  */
}

/* HVA with 4 float fields.  */
struct hva4f {
  float x, y, z, w;
};

float __attribute__((vectorcall))
test_hva4f (struct hva4f a)
{
  return a.x + a.y + a.z + a.w;  /* Should use XMM0-XMM3 */
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

/* Check that vectorcall uses callee cleanup for stack args.  */
void __attribute__((vectorcall))
test_callee_cleanup (int a, int b, int c, int d, int e, int f)
{
  /* e and f should be on stack, callee should pop them.  */
}

/* Additional HVA / non-HVA classification coverage.  */
struct hva2v4sf {
  v4sf a, b;
};

void __attribute__((vectorcall))
test_hva2v4sf (struct hva2v4sf a)
{
  sink = (int)a.a[0];
}

struct hva4v2df {
  v2df a, b, c, d;
};

void __attribute__((vectorcall))
test_hva4v2df (struct hva4v2df a)
{
  sink = (int)a.a[0];
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

/* Return-value coverage consolidated from vectorcall-4.c.  */
v4sf __attribute__((vectorcall))
test_vec_return (void)
{
  return (v4sf){1.0f, 2.0f, 3.0f, 4.0f};
}

float __attribute__((vectorcall))
test_float_return (void)
{
  return 1.0f;
}

double __attribute__((vectorcall))
test_double_return (void)
{
  return 1.0;
}

int __attribute__((vectorcall))
test_int_return (void)
{
  return 42;
}

void* __attribute__((vectorcall))
test_ptr_return (void)
{
  return (void*)0;
}

struct hva2f __attribute__((vectorcall))
test_hva2f_return (void)
{
  struct hva2f r = {1.0f, 2.0f};
  return r;
}

struct hva4f __attribute__((vectorcall))
test_hva4f_return (void)
{
  struct hva4f r = {1.0f, 2.0f, 3.0f, 4.0f};
  return r;
}

v4si __attribute__((vectorcall))
test_vec_int_return (void)
{
  return (v4si){1, 2, 3, 4};
}

/* { dg-final { scan-assembler "addps\\txmm0, xmm1" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "addss\\txmm0, xmm0" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "addss\\txmm0, xmm1" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "\"test_hva1f@@" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "\"test_hva3f@@" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler-not "movd\\tedx, xmm1" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "ecx" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "edx" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "r8d" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "\"test_hva5f@@" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "\"test_hva4f_return@@" { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler "ret" { target x86_64-*-mingw* } } } */
/* Caller-side HVA checks: HVA fields must go to XMM, not be spilled to stack
   with a lea into an integer register.  The only lea in the output should be
   from integer ADD (eax, [rcx+r9]), not from struct-by-reference.  */
/* { dg-final { scan-assembler-not {lea[ \t]+[er]cx,.*rsp} { target x86_64-*-mingw* } } } */
