/* { dg-do compile { target { *-*-mingw* && lp64 } } } */
/* { dg-require-effective-target freorder } */
/* { dg-options "-O2 -feffects -freorder-blocks-and-partition" } */
/* mingw_pe_seh_cold_init must emit a .gcceff$m entry for the cold part of
   a function, which has its own .pdata entry.  */

void
foo (int x)
{
  if (x)
    __builtin_abort ();
}

/* { dg-final { scan-assembler {\.seh_proc\tfoo\.cold} } } */
/* { dg-final { scan-assembler-times {\.section\t\.gcceff\$m,} 2 } } */
