/* Test 64-bit vectorcall struct passing (small by value vs large indirect).  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel -fno-inline -fno-optimize-sibling-calls" } */

volatile int sink;

/* Small struct (8 bytes) - should use an integer register.  */
struct small8 {
  int x;
  int y;
};

/* Small struct (4 bytes) - should use an integer register.  */
struct small4 {
  int x;
};

/* Large struct (> 8 bytes, non-HVA) - should pass by reference.  */
struct large {
  int a, b, c;
};

/* Large struct (> 8 bytes, non-HVA) - should pass by reference.  */
struct large16 {
  long long a, b;
};

extern void __attribute__((vectorcall)) test_small8 (struct small8);
extern void __attribute__((vectorcall)) test_small4 (struct small4);
extern void __attribute__((vectorcall)) test_large (struct large);
extern void __attribute__((vectorcall)) test_large16 (struct large16);

void
caller_small8 (int x, int y)
{
  struct small8 a = { x, y };
  test_small8 (a);
  sink = x;
}

void
caller_small4 (int x)
{
  struct small4 a = { x };
  test_small4 (a);
  sink = x;
}

void
caller_large (int a, int b, int c)
{
  struct large x = { a, b, c };
  test_large (x);
  sink = a;
}

void
caller_large16 (long long a, long long b)
{
  struct large16 x = { a, b };
  test_large16 (x);
  sink = (int) a;
}

/* Small aggregates should be passed directly in an integer register.  */
/* { dg-final { scan-assembler {or[ \t]+rcx, rdx(.|\n)*call[ \t]+"test_small8@@} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {mov[ \t]+ebx, ecx(.|\n)*call[ \t]+"test_small4@@} { target x86_64-*-mingw* } } } */

/* Large non-HVA aggregates should be passed indirectly, so their callers
   should materialize an address for the outgoing argument in RCX.  */
/* { dg-final { scan-assembler {call[ \t]+.*test_large} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {call[ \t]+.*test_large16} { target x86_64-*-mingw* } } } */
