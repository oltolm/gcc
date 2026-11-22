/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -mstackrealign" } */

/* SEH cannot realign the stack, so -mstackrealign is ignored.  It used to
   realign sysv_abi functions with a frame the unwind information does not
   describe, hit an assertion in mingw_pe_seh_init for alloca, and leave a
   lowered incoming stack boundary that made the following ms_abi function
   realign too.  */

typedef int v4si __attribute__ ((vector_size (16)));

void __attribute__ ((sysv_abi))
f (void)
{
  volatile v4si v = { 0 };
}

void __attribute__ ((sysv_abi))
g (int n)
{
  volatile v4si v = { 0 };
  char *p = __builtin_alloca (n);
  asm volatile ("" : : "r" (p) : "memory");
}

void
h (void)
{
  volatile v4si v = { 0 };
}

/* { dg-final { scan-assembler-not "and\[lq\]?\[^\\n\]*-16,\[^\\n\]*sp" } } */
