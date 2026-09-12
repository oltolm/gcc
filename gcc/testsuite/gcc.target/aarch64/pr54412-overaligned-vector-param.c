/* PR target/54412 */
/* SINK gets the address of X, whose type needs 32-byte alignment, so X must
   really have it.  aarch64 only aligns the frame to 16, so the address of its
   slot must be rounded up at run time: the "and xN, xN, -32" below.  The
   vector is 16 bytes because a bigger one is passed by reference and X gets
   no slot of its own.  -O2 because at -O0 the address is rounded with shifts
   instead.  */
/* { dg-do compile } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler {\tand\tx[0-9]+, x[0-9]+, -32} } } */

typedef float v4f __attribute__ ((vector_size (16), aligned (32)));

extern void sink (v4f *);

__attribute__ ((noinline)) void
overaligned_param (v4f x)
{
  sink (&x);
}
