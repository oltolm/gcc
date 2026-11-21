// { dg-do assemble  }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }
// Bug: g++ allows two different meanings of a name in the same scope.

typedef int foo;		// { dg-message "declared here" }
struct A {
  A (foo);
  int foo ();			// { dg-error "changes meaning" }
};
