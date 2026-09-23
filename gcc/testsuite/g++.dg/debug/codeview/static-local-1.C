// { dg-do compile }
// { dg-options "-O3 -gcodeview" }
// { dg-final { scan-assembler-not "_ZZ9get_valuevE5value" } }

// resolve_addr must remove the location of a static local whose storage was
// optimized away before CodeView emits a relocation for it.

struct S
{
  int a, b;
};

int
get_value ()
{
  static const S value = { 1, 2 };
  return value.a;
}

int
main ()
{
  return 0;
}
