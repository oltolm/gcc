/* Test vectorcall nested HVA classification edge cases.  */

/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel" } */

struct inner { float x, y; };
struct outer2 { struct inner a, b; };

struct inner2d { double x, y; };
struct outer2d { struct inner2d a, b; };

volatile int sink;

extern int __attribute__((vectorcall)) callee2 (struct outer2 o);
extern int __attribute__((vectorcall)) callee2d (struct outer2d o);
extern struct outer2 __attribute__((vectorcall)) ret2 (void);
extern struct outer2d __attribute__((vectorcall)) ret2d (void);

/* A 4-float nested HVA must be passed in XMM0-XMM3 (not by reference).  */
int
caller2 (void)
{
  struct outer2 o = { { 1.0f, 2.0f }, { 3.0f, 4.0f } };
  return callee2 (o);
}

/* A 4-double nested HVA must be passed in XMM0-XMM3 (not by reference).  */
int
caller2d (void)
{
  struct outer2d o = { { 1.0, 2.0 }, { 3.0, 4.0 } };
  return callee2d (o);
}

/* Nested HVA returns must also use XMM registers rather than hidden
   sret pointers.  */
void
use_ret2 (void)
{
  struct outer2 o = ret2 ();
  sink = (int) (o.a.x + o.b.y);
}

void
use_ret2d (void)
{
  struct outer2d o = ret2d ();
  sink = (int) (o.a.x + o.b.y);
}

/* Nested HVAs fit in XMM registers: no lea-into-integer-reg passing or
   hidden sret-pointer setup.  */
/* { dg-final { scan-assembler-not {lea[ \t]+rcx,.*rsp} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler-not {mov[ \t]+rcx, rsp} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {callee2@@16} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {callee2d@@32} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {ret2@@0} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {ret2d@@0} { target x86_64-*-mingw* } } } */

