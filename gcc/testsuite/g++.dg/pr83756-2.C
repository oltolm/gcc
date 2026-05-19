// { dg-do run { target c++11 } }
// { dg-options "-fms-extensions -Wno-pmf-conversions" }

template <class Fp, class A0, class... Args>
inline auto
invoke (Fp &&f, A0 &&a0, Args &&...args)
  -> decltype (((*(a0)).*f) ((args)...))
{
  return ((*(a0)).*f) ((args)...);
}

template <class Fp, class A0>
inline auto
invoke (Fp &&f, A0 &&a0) -> decltype ((*(a0)).*f)
{
  return (*(a0)).*f;
}

struct A {
  int n;
  void doB () { ++n; }
};

int
main ()
{
  A a{0}, b{0};
  invoke (&A::doB, &a) (&a);
  (b.*(&A::doB)) ();
  if (a.n != 1 || b.n != 1)
    __builtin_abort ();
}
