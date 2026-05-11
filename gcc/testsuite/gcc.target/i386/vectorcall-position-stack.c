/* Test that stack-passed arguments still consume vectorcall positions.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -msse2 -masm=intel -fno-inline -fno-optimize-sibling-calls" } */

typedef float v4sf __attribute__((vector_size (16)));

struct big {
  int a, b, c;
};

extern void __attribute__((vectorcall))
callee_mix (v4sf, struct big, v4sf, v4sf, v4sf, v4sf, v4sf);

/* The large aggregate in position 2 is passed indirectly, but still consumes
   a vectorcall position.  As a result, among the six vector arguments only
   the first five fit in XMM positions 0, 2, 3, 4, and 5; the final vector
   argument must be passed indirectly from the stack.  */
void
caller (v4sf a, struct big b, v4sf c, v4sf d, v4sf e, v4sf f, v4sf g)
{
  callee_mix (a, b, c, d, e, f, g);
}

/* The final vector argument must be copied to a stack slot and passed by
   address rather than assigned an XMM register.  */
/* { dg-final { scan-assembler {lea[ \t]+rax, 80\[rsp\]} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {mov[ \t]+QWORD PTR 72\[rsp\], rax} { target x86_64-*-mingw* } } } */
