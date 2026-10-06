// { dg-do compile }
// { dg-options "-O0 -gcodeview" }

// The indirect constant for the typeinfo of S is emitted into .data after
// the CodeView sections.  The emutls control variable for t is the last
// thing emitted into .data before them, so the section switch must not be
// elided and the constant must not end up in .debug$T.

struct S { virtual ~S (); };
void f ();
thread_local int t;

int
g ()
{
  try { f (); }
  catch (S &) { return t; }
  return 0;
}

// { dg-final { scan-assembler "\.LDFCM0:" } }
// { dg-final { scan-assembler-not "_end:\n\[ \t\]*\.align\[ \t\]+8\n\.LDFCM0:" } }
