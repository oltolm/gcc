/* PR target/54412 */
/* X needs 32-byte alignment, but aarch64 only realigns the frame to 16, so
   its slot must be over-allocated and its address rounded up at run time,
   which is the "and xN, xN, -32" below.  Before the fix X sat at a plain SP
   offset that is only 16-byte aligned.

   The vector is 16 bytes so that it is passed by value and needs a slot at
   all; a bigger one would be passed by reference and &X would just be the
   caller's pointer.  The 32 comes from the attribute.  Not a run-time test:
   the bad slot is 32-byte aligned about half the time.  -O2: at -O0 the
   address is computed with add/lsr/lsl instead.  */
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
