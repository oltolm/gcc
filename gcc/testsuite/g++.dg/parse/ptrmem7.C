// PR c++/80043
// { dg-options -fpermissive }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

struct A
{
  template<int> void foo()
  {
    void (A::* fp)();
    fp = A::foo<0>;		// { dg-warning "assuming pointer to member" }
  }
};

void bar()
{
  A().foo<0>();
}
