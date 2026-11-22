/* { dg-do run } */
/* { dg-require-effective-target indirect_jumps } */

/* ix86_output_call_insn must emit a nop between a call and the epilogue
   when only a USE of the returned value separates them.  On SEH targets
   longjmp fails with STATUS_BAD_STACK while a frame's return address is
   its epilogue.  */

#include <setjmp.h>
#include <stdlib.h>

jmp_buf env;

__attribute__ ((noipa)) int
f (void)
{
  longjmp (env, 1);
}

__attribute__ ((noipa)) int
g (void)
{
  if (setjmp (env))
    return 99;
  return f ();
}

int
main (void)
{
  if (g () != 99)
    abort ();
  return 0;
}
