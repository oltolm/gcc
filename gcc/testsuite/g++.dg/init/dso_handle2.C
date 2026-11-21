// PR c++/58846
// { dg-require-effective-target cxa_atexit }
// { dg-skip-if "destructors are registered with atexit" { *-*-mingw* } }
// { dg-options "-fuse-cxa-atexit" }

extern "C" { char* __dso_handle; }

struct A
{
  ~A();
};

A a;  // { dg-error "conflicting declaration" }
