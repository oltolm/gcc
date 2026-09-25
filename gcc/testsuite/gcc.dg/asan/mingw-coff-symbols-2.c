/* The COFF symbols of the executable must not cover addresses in DLLs
   loaded above it: the frame in kernel32.dll, which has no COFF symbols,
   must be named from its exports.  */
/* { dg-do run { target x86_64-*-mingw* } } */
/* { dg-shouldfail "asan" } */

#include <stdlib.h>

int
main (void)
{
  char *volatile p = (char *) malloc (16);
  p[16] = 0;
  free (p);
  return 0;
}

/* { dg-output "WRITE of size 1 at 0x\[0-9a-f\]+ thread T0\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output ".*in BaseThreadInitThunk" } */
