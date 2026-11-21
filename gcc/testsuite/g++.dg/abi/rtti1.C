// Test that we don't emit the type_info for a polymorphic class other than
// with the vtable.

struct A {
  virtual ~A();
};

void f ()
{
  throw A();
}

// PE (MULTIPLE_SYMBOL_SPACES) emits class data in every TU that uses it.
// { dg-final { scan-assembler-dem-not {\ntypeinfo for A[: \t\n]} { target { ! { *-*-mingw* *-*-cygwin* } } } } }
