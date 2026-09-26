/* Test that vectorcall passes aggregates of more than four HVA elements,
   flat or nested, by reference instead of in XMM registers.  */

/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel" } */

struct hva5f {
  float a, b, c, d, e;
};

struct inner3f { float x, y, z; };
struct outer3_3 {
  struct inner3f a, b;
};

extern void __attribute__((vectorcall)) callee5 (struct hva5f);
extern void __attribute__((vectorcall)) callee3_3 (struct outer3_3);

void
caller5 (float a, float b, float c, float d, float e)
{
  struct hva5f s = { a, b, c, d, e };
  callee5 (s);
}

void
caller3_3 (float a1, float a2, float a3, float b1, float b2, float b3)
{
  struct outer3_3 s = { { a1, a2, a3 }, { b1, b2, b3 } };
  callee3_3 (s);
}

/* { dg-final { scan-assembler-times {lea[ \t]+rcx, [0-9]+\[rsp\]} 2 { target x86_64-*-mingw* } } } */
