// { dg-do assemble  }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }
// PRMS Id: 4484 (bug 3)
// Bug: g++ does implicitly take the address of methods passed to fns.

struct A {
  void f ();
};

void g (void (A::*)());

void h () {
  g (A::f);			// { dg-error "" } failed conversion to method pointer
}
