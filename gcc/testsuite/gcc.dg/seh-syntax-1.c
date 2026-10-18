/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-fsyntax-only" } */
/* The forms of __try that MSVC accepts must parse, with the exception
   intrinsics from excpt.h in the contexts where they are valid, and with
   the filter and the __finally block using locals of the function.  */

#include <excpt.h>

extern void work (void);
extern int pick (unsigned long, void *);

int
f (int n)
{
  int code = 0, cleaned = 0;

  __try
    {
      work ();
      if (n)
	__leave;
      work ();
    }
  __except (n > 0 ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
    {
      code = (int) GetExceptionCode ();
    }

  __try
    {
      __try
	{
	  work ();
	}
      __finally
	{
	  cleaned += AbnormalTermination () ? 2 : 1;
	}
    }
  __except (pick (GetExceptionCode (), GetExceptionInformation ()))
    {
      code = (int) _exception_code ();
    }

  __try
    {
      work ();
    }
  __except (EXCEPTION_EXECUTE_HANDLER)
    {
    }
  return code + cleaned;
}
