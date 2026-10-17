/* { dg-do run { target { x86_64-*-mingw* && { ! ia32 } } } } */
/* { dg-options "-fexceptions -O2" } */
/* _Unwind_ForcedUnwind must call the unwind handler of a frame that
   does not use _GCC_specific_handler before the stop function sees
   the frames above it.  */

#include <windows.h>
#include <unwind.h>
#include <setjmp.h>
#include <stdlib.h>
#include <string.h>

static jmp_buf env;
static int foreign_unwinds;

EXCEPTION_DISPOSITION __attribute__((used))
foreign_handler (PEXCEPTION_RECORD rec, void *frame __attribute__((unused)),
		 PCONTEXT ctx __attribute__((unused)),
		 PDISPATCHER_CONTEXT disp __attribute__((unused)))
{
  if (rec->ExceptionFlags & EXCEPTION_UNWINDING)
    ++foreign_unwinds;
  return ExceptionContinueSearch;
}

/* void foreign_frame (void (*fn) (void)): calls FN in a frame whose
   only language handler is foreign_handler, registered for unwind.  */
void foreign_frame (void (*) (void));
asm (".text\n"
     "\t.globl\tforeign_frame\n"
     "\t.def\tforeign_frame;\t.scl\t2;\t.type\t32;\t.endef\n"
     "\t.seh_proc\tforeign_frame\n"
     "foreign_frame:\n"
     "\t.seh_handler\tforeign_handler, @unwind\n"
     "\tsubq\t$40, %rsp\n"
     "\t.seh_stackalloc\t40\n"
     "\t.seh_endprologue\n"
     "\tcall\t*%rcx\n"
     "\tnop\n"
     "\taddq\t$40, %rsp\n"
     "\tret\n"
     "\t.seh_endproc\n");

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
    {
      if (foreign_unwinds != 1)
	abort ();
      longjmp (env, 1);
    }
  return _URC_NO_REASON;
}

static void
force_unwind (void)
{
  struct _Unwind_Exception *exc = malloc (sizeof (*exc));
  memset (&exc->exception_class, 0, sizeof (exc->exception_class));
  exc->exception_cleanup = 0;
  _Unwind_ForcedUnwind (exc, force_unwind_stop, 0);
  abort ();
}

static void
target_cleanup (int *p __attribute__((unused)))
{
  abort ();
}

static void __attribute__((noipa))
target (void)
{
  int x __attribute__((cleanup (target_cleanup)));
  foreign_frame (force_unwind);
}

int
main (void)
{
  if (setjmp (env) == 0)
    {
      target ();
      abort ();
    }
  return 0;
}
