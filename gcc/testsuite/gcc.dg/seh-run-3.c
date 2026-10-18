/* { dg-do run { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -feffects" } */
/* Discarding a continuation unwinds the coroutine with RtlUnwindEx, which
   must call __C_specific_handler for a frame with __try, so that its
   __finally block runs as an abnormal termination.  */

#include <windows.h>
#include <stdlib.h>

_Effect stop { void stop (void); };

static int finals;

static void __attribute__((noipa))
work (void)
{
  _Perform stop.stop ();
}

int
main (void)
{
  _Try
    {
      __try
	{
	  work ();
	}
      __finally
	{
	  finals += AbnormalTermination () ? 10 : 1;
	}
    }
  _Handle (stop.stop () _Continuation k)
    {
      _Discard (k);
    }
  if (finals != 10)
    abort ();
  return 0;
}
