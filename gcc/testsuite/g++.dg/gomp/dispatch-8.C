// { dg-do compile }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

// Check that an appropriate diagnostic is emitted when a dispatch directive
// appears in a pragma_member context.

void k();
struct t {
 #pragma omp dispatch  // { dg-error "expected declaration specifiers before end of line" }
  k();  // { dg-error ".*" }
};
