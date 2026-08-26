/* PR target/54412 */
/* Companion to pr54412-aligned-struct-byval-temp.c, covering the case where
   the over-alignment comes from BIGGEST_ALIGNMENT instead of an attribute.

   Win64 passes this struct by reference, so CALLER materialises a hidden
   copy through assign_temp.  The struct is BLKmode and asks for no
   alignment of its own, so REQUIRED_ALIGN stays at BITS_PER_UNIT and the
   copy is allocated on the ordinary path rather than through
   assign_stack_local_aligned.  On that path get_stack_local_alignment
   returns BIGGEST_ALIGNMENT, which -mavx raises to 256 bits, while
   assign_stack_local_1 caps the slot at MAX_SUPPORTED_STACK_ALIGNMENT,
   128 bits under SEH.  Taking the returned MEM's alignment from the request
   rather than from the allocated slot therefore claimed 32-byte alignment
   on a 16-byte slot, and the copy was stored with an aligned 32-byte move.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -mavx" } */
/* { dg-final { scan-assembler-not {vmov(aps|dqa)\t%ymm[0-9]+, [0-9-]*\(%r[sb]p\)} } } */

struct S { char c[32]; };

void sink (struct S);

void
caller (struct S *p)
{
  sink (*p);
}
