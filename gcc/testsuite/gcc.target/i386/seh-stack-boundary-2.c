/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -mincoming-stack-boundary=3" } */
/* { dg-error "'-mincoming-stack-boundary=3' is not between 4 and 12" "" { target *-*-* } 0 } */

/* SEH cannot realign the stack, so the incoming stack boundary must be at
   least 16 bytes.  With 8 bytes, saving xmm6 needed realignment and ICEd
   in choose_baseaddr.  */

void
f (void)
{
  asm volatile ("" : : : "xmm6");
}
