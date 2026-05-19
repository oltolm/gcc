// PR c++/120233
// { dg-do compile { target c++11 } }
// { dg-options "-fms-extensions -Wno-pmf-conversions" }

template <class Fp, class A0, class... Args>
inline auto
invoke (Fp &&f, A0 &&a0, Args &&...args)
  -> decltype (((*(a0)).*f) ((args)...))
{ }

template <class Fp, class A0>
inline auto
invoke (Fp &&f, A0 &&a0) -> decltype ((*(a0)).*f)
{
  return (*(a0)).*f;
}

struct A {
  void doA ();
  void doB () { }
};

void
A::doA ()
{
  invoke (&A::doB, this);
}
