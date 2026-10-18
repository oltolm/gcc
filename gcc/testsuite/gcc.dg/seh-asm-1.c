/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2" } */
/* A function with __try must use __C_specific_handler with a scope table
   instead of GCC's personality, and a call that ends a scope must be
   followed by a nop, since the runtime looks up the return address.  */

extern void work (void);

int
f (void)
{
  int r = 0;
  __try
    {
      work ();
    }
  __except (1)
    {
      r = 1;
    }
  return r;
}

/* { dg-final { scan-assembler "\\.seh_handler\t__C_specific_handler, @unwind, @except" } } */
/* { dg-final { scan-assembler-not "__gcc_personality" } } */
/* { dg-final { scan-assembler "call\twork\n\tnop" } } */
/* { dg-final { scan-assembler "\\.seh_handlerdata\n\t\\.long\t1\n\t\\.rva\t" } } */
