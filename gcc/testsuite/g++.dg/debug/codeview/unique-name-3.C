// { dg-do compile }
// { dg-options "-gcodeview" }

// cp_mangled_type_name must handle the types of anonymous aggregates in
// class, namespace, and function scope.

struct S
{
  union { int i; float f; };
  struct { int x; };
} s;

static union { int a; long b; };

int
use ()
{
  union { int c; short d; };
  c = 1;
  return c + s.i + s.x + a;
}
