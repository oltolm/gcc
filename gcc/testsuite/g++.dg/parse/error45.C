// PR c++/51429
// { dg-do compile }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

struct A
{
  void foo (double);
  void foo (int);
  A () { foo = 0; }	// { dg-error "invalid use of member function" }
};
