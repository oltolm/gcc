// PR c++/33840
// { dg-do compile }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

template<int> struct A
{
  struct {} : 2;   // { dg-error "expected ';' after struct" "expected" }
};
// { dg-error "ISO C.. forbids declaration" "declaration" { target *-*-* } 7 }
// { dg-error "10:ISO C.. prohibits anonymous" "anonymous" { target *-*-* } 7 }
