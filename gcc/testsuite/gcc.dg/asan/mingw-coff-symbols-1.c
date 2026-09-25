/* Without debug information, the libbacktrace symbolizer names functions
   from the COFF symbol table, which must be relocated to the address where
   the image is loaded.  */
/* { dg-do run { target x86_64-*-mingw* } } */
/* { dg-additional-options "-g0" } */
/* { dg-shouldfail "asan" } */

#include <stdlib.h>

__attribute__((noipa)) void
coff_symbol_test (char *p)
{
  p[16] = 0;
}

int
main (void)
{
  char *p = (char *) malloc (16);
  coff_symbol_test (p);
  free (p);
  return 0;
}

/* { dg-output "WRITE of size 1 at 0x\[0-9a-f\]+ thread T0\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "    #0 0x\[0-9a-f\]+ +in _*coff_symbol_test\[^\n\r]*(\n|\r\n|\r)" } */
