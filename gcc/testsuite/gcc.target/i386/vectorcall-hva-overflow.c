/* Test that vectorcall correctly rejects aggregates exceeding the
   4-element HVA limit, passing them by reference instead.  */

/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel" } */

extern int __attribute__((vectorcall)) callee (void *);

/* 6-element flat HVA: fields a-e exceed the 4-element limit, so must
   be passed by reference (pointer in an integer register).  */
struct hva5f {
  float a, b, c, d, e;
};

void
caller5 (float a, float b, float c, float d, float e)
{
  struct hva5f s = { a, b, c, d, e };
  callee (&s);
}

/* Nested 3+3-element: 2 structs of 3 floats each = 6 leaf floats,
   exceeding 4-element HVA limit.  Must be passed by reference.  */
struct inner3f { float x, y, z; };
struct outer3_3 {
  struct inner3f a, b;
};

void
caller3_3 (float a1, float a2, float a3, float b1, float b2, float b3)
{
  struct outer3_3 s = { { a1, a2, a3 }, { b1, b2, b3 } };
  callee (&s);
}

/* Exceeding the limit triggers by-reference: lea rcx, offset[rsp].  */
/* { dg-final { scan-assembler {lea[ \t]+rcx,.*rsp} { target x86_64-*-mingw* } } } */
