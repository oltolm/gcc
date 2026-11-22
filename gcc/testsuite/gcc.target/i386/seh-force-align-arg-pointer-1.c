/* { dg-do compile { target x86_64-*-mingw* } } */

/* With SEH, GCC does not realign the stack, so force_align_arg_pointer is
   rejected on function definitions of either ABI.  Declarations are
   accepted.  */

void __attribute__ ((force_align_arg_pointer))
f (void)	/* { dg-error "not supported with SEH" } */
{
}

void __attribute__ ((force_align_arg_pointer, sysv_abi))
g (void)	/* { dg-error "not supported with SEH" } */
{
}

void __attribute__ ((force_align_arg_pointer)) h (void);

void
call_h (void)
{
  h ();
}
