/* { dg-do compile } */
/* { dg-options "-O2 -mindirect-branch=thunk -mharden-sls=all" } */
/* { dg-additional-options "-fno-pic" { target { ! *-*-darwin* } } } */
/* { dg-skip-if "SEH does not use indirect branch thunks" { x86_64-*-mingw* } } */

extern void (*fptr) (void);

void
foo (void)
{
  fptr ();
}

/* { dg-final { scan-assembler "jmp\[ \t\]+_?__x86_indirect_thunk_(r|e)ax" } } */
/* { dg-final { scan-assembler-times "int3" 2 } } */
