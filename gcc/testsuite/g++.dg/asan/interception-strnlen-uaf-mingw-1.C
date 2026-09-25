// Verify that strnlen interception is active on MinGW/Windows ASan builds.

// { dg-do run { target *-*-mingw* } }
// { dg-options "-fno-builtin-strnlen" }
// { dg-shouldfail "asan" }

#include <stdlib.h>
#include <string.h>

__attribute__((noinline))
static size_t
call_strnlen(const char *value, size_t limit)
{
  return strnlen(value, limit);
}

int
main()
{
  char *value = (char *)malloc(4);
  value[0] = 'A';
  value[1] = 0;
  value[2] = 'B';
  value[3] = 0;
  free(value);

  return (int)call_strnlen(value, 4);
}

// { dg-output "ERROR: AddressSanitizer:? heap-use-after-free on address.*(\n|\r\n|\r)" }
