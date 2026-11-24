// { dg-do compile }
// { dg-options "-gcodeview" }

// The unnamed types of S::a and S::b must have distinct names and unique
// names on both their forward references and their definitions.

struct S
{
  struct { int x; } a;
  struct { double y; } b;
};

S s;

// { dg-final { scan-assembler-times {\.ascii "S::<unnamed-type-a>\\0"\n\t\.ascii "_ZTSN1SUt_E\\0"} 2 } }
// { dg-final { scan-assembler-times {\.ascii "S::<unnamed-type-b>\\0"\n\t\.ascii "_ZTSN1SUt0_E\\0"} 2 } }
