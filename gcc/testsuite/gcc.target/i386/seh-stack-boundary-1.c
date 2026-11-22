/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -mpreferred-stack-boundary=3" } */
/* { dg-error "'-mpreferred-stack-boundary' is not supported for this target" "" { target *-*-* } 0 } */

/* SEH cannot realign the stack, so the preferred stack boundary must be
   16 bytes.  With 8 bytes, saving xmm6 needed realignment and ICEd in
   choose_baseaddr.  */

void
f (void)
{
  asm volatile ("" : : : "xmm6");
}
