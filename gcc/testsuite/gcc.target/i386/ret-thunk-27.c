/* { dg-do compile } */
/* { dg-options "-O2 -mfunction-return=thunk -fcf-protection=none" } */

/* indirect_thunk_name must handle a null cfun when emitting a return thunk.  */
void
foo (void)
{
}
