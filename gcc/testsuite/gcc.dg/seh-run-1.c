/* { dg-do run { target x86_64-*-mingw* } } */
/* { dg-options "-O2" } */
/* __C_specific_handler must find the scope records of main: a fault in
   the __try block itself, an exception raised in a callee that an inner
   filter declines, __finally blocks on the exception path and on normal
   exits, and a filter that continues execution.  Filters and __finally
   blocks read and write locals of main.  */

#include <windows.h>
#include <stdlib.h>

static volatile int * __attribute__((noipa))
null_pointer (void)
{
  return NULL;
}

static void __attribute__((noipa))
raise_code (DWORD code)
{
  RaiseException (code, 0, 0, NULL);
}

int
main (void)
{
  volatile int *bad = null_pointer ();
  int expected = 1, r = 0;
  DWORD code = 0;

  __try
    {
      r = *bad;
      abort ();
    }
  __except (expected == 1
	    && GetExceptionCode () == EXCEPTION_ACCESS_VIOLATION
	    && (GetExceptionInformation ()->ExceptionRecord->ExceptionCode
		== EXCEPTION_ACCESS_VIOLATION))
    {
      code = GetExceptionCode ();
    }
  if (code != EXCEPTION_ACCESS_VIOLATION || r != 0)
    abort ();

  int abnormal = -1;
  __try
    {
      __try
	{
	  __try
	    {
	      raise_code (0xE0000001);
	    }
	  __finally
	    {
	      abnormal = AbnormalTermination ();
	    }
	}
      __except (GetExceptionCode () == 0xE0000002)
	{
	  abort ();
	}
    }
  __except (EXCEPTION_EXECUTE_HANDLER)
    {
      code = GetExceptionCode ();
    }
  if (abnormal != 1 || code != 0xE0000001)
    abort ();

  int runs = 0;
  for (int i = 0; i < 2; i++)
    __try
      {
	if (i)
	  __leave;
	runs += 10;
      }
    __finally
      {
	runs += AbnormalTermination () ? 100 : 1;
      }
  if (runs != 12)
    abort ();

  int seen = 0;
  __try
    {
      raise_code (0xE0000003);
      seen += 10;
    }
  __except (seen++, EXCEPTION_CONTINUE_EXECUTION)
    {
      abort ();
    }
  if (seen != 11)
    abort ();
  return 0;
}
