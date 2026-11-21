// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }
// PR c++/15696

struct A {};

typedef void (A::*ftype)();

void foo() { A().*ftype(); } // { dg-error "" }
