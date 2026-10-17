/* { dg-do run } */
/* { dg-options "-fexceptions -O2" } */
/* { dg-require-effective-target exceptions } */
/* _Unwind_ForcedUnwind must call the stop function for each frame,
   including frames reached after a cleanup has run and called
   _Unwind_Resume, so that the stop function can end the unwind at a
   chosen frame.  */

#include <unwind.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf env;
static int cleanups;

static void
counter (int *p __attribute__((unused)))
{
  ++cleanups;
}

static void
target_cleanup (int *p __attribute__((unused)))
{
  abort ();
}

static void target (void);

static _Unwind_Reason_Code
force_unwind_stop (int version, _Unwind_Action actions,
		   _Unwind_Exception_Class exc_class,
		   struct _Unwind_Exception *exc_obj,
		   struct _Unwind_Context *context,
		   void *stop_parameter)
{
  if (actions & _UA_END_OF_STACK)
    abort ();
  if (_Unwind_GetRegionStart (context) == (_Unwind_Ptr) target)
    longjmp (env, 1);
  return _URC_NO_REASON;
}

static void
force_unwind (void)
{
  struct _Unwind_Exception *exc = malloc (sizeof (*exc));
  memset (&exc->exception_class, 0, sizeof (exc->exception_class));
  exc->exception_cleanup = 0;

#ifndef __USING_SJLJ_EXCEPTIONS__
  _Unwind_ForcedUnwind (exc, force_unwind_stop, 0);
#else
  _Unwind_SjLj_ForcedUnwind (exc, force_unwind_stop, 0);
#endif

  abort ();
}

static void __attribute__((noipa))
fn2 (void)
{
  int x __attribute__((cleanup (counter)));
  force_unwind ();
}

static void __attribute__((noipa))
fn1 (void)
{
  int x __attribute__((cleanup (counter)));
  fn2 ();
}

static void __attribute__((noipa))
target (void)
{
  int x __attribute__((cleanup (target_cleanup)));
  fn1 ();
}

int
main (void)
{
  if (setjmp (env) == 0)
    {
      target ();
      abort ();
    }
  if (cleanups != 2)
    abort ();
  return 0;
}
