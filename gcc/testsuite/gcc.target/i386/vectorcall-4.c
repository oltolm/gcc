/* Test 64-bit vectorcall runtime behavior, including sysv_abi interop.  */
/* { dg-do run { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2" } */

extern void abort (void);

typedef float v4sf __attribute__((vector_size (16)));
typedef double v2df __attribute__((vector_size (16)));

volatile float f1 = 1.0f;
volatile float f2 = 2.0f;
volatile float f3 = 3.0f;
volatile float f4 = 4.0f;
volatile float f5 = 5.0f;
volatile float f6 = 6.0f;
volatile float f7 = 7.0f;
volatile float f8 = 8.0f;
volatile float fpi = 3.14f;
volatile int i1 = 1;
volatile int i2 = 2;
volatile int i3 = 3;
volatile int i4 = 4;
volatile int i42 = 42;

/* Test vector arguments.  */
void __attribute__((vectorcall, noipa))
test_vec (v4sf a, v4sf b)
{
  if (a[0] != 1.0f || a[1] != 2.0f || a[2] != 3.0f || a[3] != 4.0f)
    abort ();
  if (b[0] != 5.0f || b[1] != 6.0f || b[2] != 7.0f || b[3] != 8.0f)
    abort ();
}

/* Test HVA argument.  */
struct hva2f {
  float x, y;
};

void __attribute__((vectorcall, noipa))
test_hva (struct hva2f a)
{
  if (a.x != 1.0f || a.y != 2.0f)
    abort ();
}

/* Test vector return.  */
v4sf __attribute__((vectorcall, noipa))
test_vec_return (void)
{
  return (v4sf){1.0f, 2.0f, 3.0f, 4.0f};
}

/* Test HVA return.  */
struct hva2f __attribute__((vectorcall, noipa))
test_hva_return (void)
{
  struct hva2f r = {1.0f, 2.0f};
  return r;
}

/* Test integer arguments.  */
void __attribute__((vectorcall, noipa))
test_int (int a, int b, int c, int d)
{
  if (a != 1 || b != 2 || c != 3 || d != 4)
    abort ();
}

/* Test mixed arguments.  */
void __attribute__((vectorcall, noipa))
test_mixed (int a, v4sf b, float c)
{
  if (a != 42)
    abort ();
  if (b[0] != 1.0f)
    abort ();
  if (c != 3.14f)
    abort ();
}

/* Cross-ABI order-sensitive encode helpers.  */
static int __attribute__((vectorcall, noipa))
vc_encode6 (int a, int b, int c, int d, int e, int f)
{
  return a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f;
}

static int __attribute__((sysv_abi, noipa))
sv_encode6 (int a, int b, int c, int d, int e, int f)
{
  return a + 10 * b + 100 * c + 1000 * d + 10000 * e + 100000 * f;
}

static int __attribute__((sysv_abi, noipa))
call_vc_from_sysv (void)
{
  return vc_encode6 (1, 2, 3, 4, 5, 6);
}

static int __attribute__((vectorcall, noipa))
call_sv_from_vc (void)
{
  return sv_encode6 (1, 2, 3, 4, 5, 6);
}

int main (void)
{
  v4sf va = {f1, f2, f3, f4};
  v4sf vb = {f5, f6, f7, f8};

  test_vec (va, vb);
  test_int (i1, i2, i3, i4);
  test_mixed (i42, va, fpi);

  struct hva2f h = {f1, f2};
  test_hva (h);

  v4sf vr = test_vec_return ();
  if (vr[0] != f1 || vr[1] != f2 || vr[2] != f3 || vr[3] != f4)
    abort ();

  struct hva2f hr = test_hva_return ();
  if (hr.x != f1 || hr.y != f2)
    abort ();

  if (call_vc_from_sysv () != 654321)
    abort ();

  if (call_sv_from_vc () != 654321)
    abort ();

  return 0;
}
