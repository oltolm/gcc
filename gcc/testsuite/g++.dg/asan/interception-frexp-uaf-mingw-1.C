// Verify that frexp interception is active on MinGW/Windows ASan builds.

// { dg-do run { target *-*-mingw* } }
// { dg-options "-fno-builtin-frexp" }
// { dg-shouldfail "asan" }

#include <math.h>
#include <stdlib.h>

__attribute__((noinline))
static double
call_frexp(double value, int *exp)
{
  return frexp(value, exp);
}

int
main()
{
  int *exp = (int *)malloc(sizeof(int));
  free(exp);

  return call_frexp(8.0, exp) != 0.0;
}

// { dg-output "ERROR: AddressSanitizer:? heap-use-after-free on address.*(\n|\r\n|\r)" }
