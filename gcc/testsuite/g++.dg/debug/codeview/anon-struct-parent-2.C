// { dg-do compile }
// { dg-options "-gcodeview" }

// get_type_num_struct must not re-enter an unfinished anonymous parent, and
// the name of the nested unnamed class must be qualified by its parent's.

struct S
{
  struct
  {
    struct { int a; } b;
  } c;
} s;

// { dg-final { scan-assembler-times {\.ascii "S::<unnamed-type-c>\\0"\n\t\.ascii "_ZTSN1SUt_E\\0"} 2 } }
// { dg-final { scan-assembler-times {\.ascii "S::<unnamed-type-c>::<unnamed-type-b>\\0"\n\t\.ascii "_ZTSN1SUt_Ut_E\\0"} 2 } }
