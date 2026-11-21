// PR c++/79781
// { dg-do compile { target int128 } }
// { dg-options "" }

void c() {
  static __int128_t d = (__INTPTR_TYPE__)&&a - (__INTPTR_TYPE__)&&b;
a:
b:;
}
