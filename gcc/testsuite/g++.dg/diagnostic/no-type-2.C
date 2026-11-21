// { dg-options "-w" }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

foo() {}  // { dg-error "1:ISO C\\+\\+ forbids declaration" }
