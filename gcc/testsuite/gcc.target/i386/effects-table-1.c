/* { dg-do compile { target { *-*-mingw* && lp64 } } } */
/* { dg-options "-O2 -feffects" } */
/* mingw_pe_seh_init must emit a .gcceff$m entry for each function compiled
   with -feffects, with flags 1 under allow_foreign_capture, and the object
   must carry the COMDAT .gcceff$a table header.  */

void f (void) {}
void __attribute__((allow_foreign_capture)) g (void) {}
void __attribute__((optimize ("no-effects"))) h (void) {}

/* { dg-final { scan-assembler-times {\.section\t\.gcceff\$m,} 2 } } */
/* { dg-final { scan-assembler-times {\.rva\t\S+\n\t\.long\t0\n} 1 } } */
/* { dg-final { scan-assembler-times {\.rva\t\S+\n\t\.long\t1\n} 1 } } */
/* { dg-final { scan-assembler {\.section\t\.gcceff\$a, "dw"\n\t\.linkonce\tdiscard} } } */
