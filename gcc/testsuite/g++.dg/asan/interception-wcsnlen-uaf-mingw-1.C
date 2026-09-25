// Verify that wcsnlen interception is active on MinGW/Windows ASan builds.

// { dg-do run { target *-*-mingw* } }
// { dg-options "-fno-builtin-wcsnlen" }
// { dg-shouldfail "asan" }

#include <stdlib.h>
#include <wchar.h>

__attribute__((noinline))
static size_t
call_wcsnlen(const wchar_t *value, size_t limit)
{
  return wcsnlen(value, limit);
}

int
main()
{
  wchar_t *value = (wchar_t *)malloc(2 * sizeof(wchar_t));
  value[0] = L'A';
  value[1] = 0;
  free(value);

  return (int)call_wcsnlen(value, 2);
}

// { dg-output "ERROR: AddressSanitizer:? heap-use-after-free on address.*(\n|\r\n|\r)" }
