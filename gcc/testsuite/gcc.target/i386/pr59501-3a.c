/* PR target/59501 */
/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2 -mavx -mno-accumulate-outgoing-args" } */
/* { dg-additional-options "-mno-stack-arg-probe" { target x86_64-*-mingw* } } */

typedef double V __attribute__ ((vector_size (32)));

V
foo (double *x, int a, int b, int c, int d, int e, int f, unsigned *y)
{
  V r = { x[y[0]], x[y[1]], x[y[2]], x[y[3]] };
  return r;
}

/* Verify no dynamic realignment is performed.  */
/* { dg-final { scan-assembler-not "and\[^\n\r]*sp" } } */
