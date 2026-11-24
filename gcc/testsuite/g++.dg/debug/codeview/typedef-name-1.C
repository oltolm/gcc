// { dg-do compile }
// { dg-options "-gcodeview" }

// An unnamed class named by a typedef must take the typedef's name and its
// typeinfo name for linkage purposes as unique name, replacing the mangled
// name that gen_struct_or_union_type_die recorded before the typedef.

typedef struct { int x; } T;

struct U
{
  T t;
} u;

// { dg-final { scan-assembler-times {\.ascii "T\\0"\n\t\.ascii "_ZTS1T\\0"} 2 } }
