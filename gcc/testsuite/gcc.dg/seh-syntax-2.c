/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-fsyntax-only" } */
/* Errors for __try statements and the exception intrinsics.  The filter
   and the __finally block run as separate functions, so they cannot
   return from or jump out of the function containing them.  */

#include <excpt.h>

extern void work (void);

void
f (void)
{
  __label__ out;

  __try
    {
      work ();
    }
  work ();			/* { dg-error "expected '__except' or '__finally'" } */

  __try
    {
    }
  __except (1)
    {
      GetExceptionInformation ();	/* { dg-error "only valid in a filter" } */
    }

  _exception_code ();		/* { dg-error "only valid in a filter or" } */
  _abnormal_termination ();	/* { dg-error "only valid in a '__finally'" } */
  __leave;			/* { dg-error "not within a '__try'" } */

  __try
    {
    }
  __finally
    {
      __leave;			/* { dg-error "not within a '__try'" } */
      return;			/* { dg-error "'return' in a '__finally'" } */
    }

  __try
    {
    }
  __finally
    {
      goto out;			/* { dg-error "jump out of a '__finally'" } */
    }

  __try
    {
    }
  __except (({ return; 1; }))	/* { dg-error "'return' in a filter" } */
    {
    }
 out:;
}
