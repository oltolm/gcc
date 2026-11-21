// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }
main() {}  // { dg-error "1:ISO C\\+\\+ forbids declaration" }
