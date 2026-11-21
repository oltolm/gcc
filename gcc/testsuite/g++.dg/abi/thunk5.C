// PR c++/35067
// The thunks should be weak even on targets without one-only support.
// { dg-require-weak "" }
// { dg-skip-if "Linkonce not weak" { *-*-mingw* *-*-cygwin* } }
// { dg-final { scan-assembler "weak.*ZTv" } }

struct A
{
  virtual ~A() { }
};

struct B: virtual A { };

B b;
