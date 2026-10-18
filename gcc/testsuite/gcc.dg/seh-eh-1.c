/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -fdump-tree-gimple -fdump-tree-optimized-eh" } */
/* A __try must become an EH region that survives optimization and names
   its filter or __finally function, which must not be inlined into the
   function.  A local that the filter or the __finally block reads must
   be accessed as volatile, since they read it at points that dataflow
   does not see: here the store x = 1 must stay although x = 2 follows,
   because the filter runs when *p faults in between.  */

extern void work (void);
extern void use (int);

int
f (int *p)
{
  int x = 0, r = 0;

  __try
    {
      x = 1;
      r = *p;
      x = 2;
    }
  __except (x == 1)
    {
      r = -1;
    }

  __try
    {
      work ();
    }
  __finally
    {
      use (x);
    }
  return r;
}

/* { dg-final { scan-tree-dump "x =\\{v\\} 1;" "optimized" } } */
/* { dg-final { scan-tree-dump "__builtin_eh_pointer \\(0\\)" "gimple" } } */
/* { dg-final { scan-tree-dump-times "\\.SEH_FUNCLET \\(" 2 "gimple" } } */
/* { dg-final { scan-tree-dump "try land:\[^\n\]* seh:__seh_filter" "optimized" } } */
/* { dg-final { scan-tree-dump "cleanup land:\[^\n\]* seh:__seh_finally" "optimized" } } */
/* { dg-final { scan-assembler "__seh_filter" } } */
/* { dg-final { scan-assembler "__seh_finally" } } */
