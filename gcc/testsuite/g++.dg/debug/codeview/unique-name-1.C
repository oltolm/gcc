// { dg-do compile }
// { dg-options "-gcodeview" }

// The forward reference and the definition of each unnamed class must carry
// a name from its first declarator and the class's typeinfo name as unique
// name, so that debuggers resolve LF_MFUNCTION's class type.

struct S
{
  struct { void f () {} } a;
  struct { void g () {} } b;
} s;

// { dg-final { scan-assembler-times {\.ascii "S::<unnamed-type-a>\\0"\n\t\.ascii "_ZTSN1SUt_E\\0"} 2 } }
// { dg-final { scan-assembler-times {\.ascii "S::<unnamed-type-b>\\0"\n\t\.ascii "_ZTSN1SUt0_E\\0"} 2 } }
