/* Test 64-bit vectorcall register-home area sizing.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel -fno-inline -fno-optimize-sibling-calls" } */

typedef float v4sf __attribute__((vector_size (16)));

extern void __attribute__((vectorcall)) callee2v (v4sf, v4sf);
extern void __attribute__((vectorcall)) callee6v (v4sf, v4sf, v4sf, v4sf,
						  v4sf, v4sf);

/* Fewer than four register-passed arguments still require the baseline
   32-byte Win64 home area.  With stack alignment, this appears as a
   40-byte outgoing allocation.  */
void __attribute__((vectorcall, noipa))
call_2v (v4sf a, v4sf b)
{
  callee2v (a, b);
}

/* Six register-passed vectorcall arguments require six 8-byte home slots,
   so the outgoing home area grows to 48 bytes.  With stack alignment, this
   appears as a 56-byte outgoing allocation.  */
void __attribute__((vectorcall, noipa))
call_6v (v4sf a, v4sf b, v4sf c, v4sf d, v4sf e, v4sf f)
{
  callee6v (a, b, c, d, e, f);
}

/* { dg-final { scan-assembler {sub[ \t]+rsp, 40} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {sub[ \t]+rsp, 56} { target x86_64-*-mingw* } } } */
