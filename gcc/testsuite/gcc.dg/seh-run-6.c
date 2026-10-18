/* { dg-do run { target x86_64-*-mingw* } } */
/* { dg-options "-O2" } */
/* AbnormalTermination () in a __finally block must be zero only when its
   __try body is left at its end, by falling through or by __leave, and
   nonzero when return, goto, break or continue leaves it, as with the
   Microsoft compiler.  */

#include <windows.h>
#include <stdlib.h>

static int abnormal;

static int __attribute__((noipa))
by_return (int x)
{
  __try
    {
      if (x)
	return 1;
    }
  __finally
    {
      abnormal = AbnormalTermination ();
    }
  return 0;
}

static void __attribute__((noipa))
by_leave (int x)
{
  __try
    {
      if (x)
	__leave;
      abnormal = 2;
    }
  __finally
    {
      abnormal = AbnormalTermination ();
    }
}

static void __attribute__((noipa))
by_goto (int x)
{
  __try
    {
      if (x)
	goto out;
    }
  __finally
    {
      abnormal = AbnormalTermination ();
    }
 out:;
}

static void __attribute__((noipa))
by_break_continue (int x)
{
  for (int i = 0; i < 2; i++)
    {
      __try
	{
	  if (i == 0)
	    continue;
	  if (x)
	    break;
	}
      __finally
	{
	  abnormal += AbnormalTermination ();
	}
    }
}

int
main (void)
{
  if (by_return (1) != 1 || !abnormal)
    abort ();
  if (by_return (0) != 0 || abnormal)
    abort ();

  abnormal = 1;
  by_leave (1);
  if (abnormal)
    abort ();
  abnormal = 1;
  by_leave (0);
  if (abnormal)
    abort ();

  by_goto (1);
  if (!abnormal)
    abort ();
  by_goto (0);
  if (abnormal)
    abort ();

  abnormal = 0;
  by_break_continue (1);
  if (abnormal != 2)
    abort ();
  return 0;
}
