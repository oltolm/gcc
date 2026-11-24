// { dg-do compile { target c++11 } }
// { dg-options "-gcodeview" }

// The forward reference and the definition of a local unnamed class and of a
// closure type must be scoped and carry a name qualified by the function and
// the type's typeinfo name as unique name.

int
use ()
{
  struct { int k () { return 1; } } loc;
  auto lam = [] () { return 2; };
  return loc.k () + lam ();
}

// { dg-final { scan-assembler-times {\.word\t0x3[08]0\n(?:\t[^\n]*\n){4}\t\.ascii "use::<unnamed-type-loc>\\0"\n\t\.ascii "_ZTSZ3usevEUt_\\0"} 2 } }
// { dg-final { scan-assembler-times {\.word\t0x3[08]0\n(?:\t[^\n]*\n){4}\t\.ascii "use::<lambda_0>\\0"\n\t\.ascii "_ZTSZ3usevEUlvE_\\0"} 2 } }
