/* asan_intercepted_p (BUILT_IN_STRCMP) must be true so that expand_builtin
   emits a call to the intercepted strcmp instead of expanding the comparison
   with a short constant string inline without any checks.  */
/* { dg-do run } */
/* { dg-shouldfail "asan" } */

#include <stdlib.h>

int
main ()
{
  char *p = (char *) malloc (4);
  p[0] = 'a';
  p[1] = 'b';
  p[2] = 0;
  free (p);
  __asm__ ("" : "+r" (p));
  return __builtin_strcmp (p, "ab") != 0;
}

/* { dg-output "ERROR: AddressSanitizer:? heap-use-after-free on address.*(\n|\r\n|\r)" } */
