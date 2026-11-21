// { dg-do compile }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }
// { dg-additional-options "-Wno-non-c-typedef-for-linkage" }

template <int> void a() {
  typedef struct {
    void b() try { b; } catch (short) { // { dg-error "invalid use" }
    }
  } c;
}

int
main() {
  a<0>();
}
