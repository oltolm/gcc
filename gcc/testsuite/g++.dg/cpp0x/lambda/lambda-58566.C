// PR c++/58566
// { dg-do compile { target c++11 } }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

struct A
{
  int foo()
  {
    [this]{ return foo; }; // { dg-error "invalid use of member function|cannot convert" }
    return 0;
  }
};
