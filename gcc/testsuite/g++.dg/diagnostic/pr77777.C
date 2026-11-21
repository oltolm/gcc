// PR c++/77777
// { dg-do compile }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

struct X {
  int f();
};

void g(int);

int main()
{
  X x;
  g(
      x.f  // { dg-error "invalid use of non-static member function" }
   );
}
