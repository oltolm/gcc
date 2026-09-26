/* Test 64-bit vectorcall caller-side stack overflow arguments.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel -fno-inline -fno-optimize-sibling-calls" } */

volatile int sink;

extern void __attribute__((vectorcall))
test_int_overflow (int, int, int, int, int, int);

/* In x64 vectorcall, the first four integer arguments go in
   RCX, RDX, R8, and R9.  Additional integer arguments are passed
   on the stack in the outgoing argument area.

   The caller pushes arg 5 (value 5) and arg 6 (value 6) to the
   outgoing stack slot starting at offset 32 from RSP (after the
   32-byte shadow space).  */
void
caller (void)
{
  test_int_overflow (1, 2, 3, 4, 5, 6);
  sink = 1;
}

/* Arg 5 and arg 6 must appear on the stack, not in registers.  */
/* { dg-final { scan-assembler {mov[ \t]+DWORD PTR 32\[rsp\], 5} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {mov[ \t]+DWORD PTR 40\[rsp\], 6} { target x86_64-*-mingw* } } } */
