/* PR target/54412 */
/* On Win64 a struct that is not 1, 2, 4 or 8 bytes is passed by reference,
   so CALLER makes a hidden copy of *P and passes its address.  That copy
   needs 32-byte alignment, but the frame only realigns to 16, so its slot
   must be over-allocated and its address rounded up, which is the "and $-32"
   below.  Before the fix the compiler took the copy's alignment from a value
   that was first capped at 16, the most any type needs naturally without
   AVX, so it never saw the 32 and left the copy on a plain 16-byte slot.

   Only alignment that comes from the attribute is affected: a 32-byte vector
   needs AVX, which raises that cap to 32 anyway.  The copy is the only
   over-aligned object in CALLER, so the masking can only come from it.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2" } */
/* { dg-final { scan-assembler {and[lq]?\t\$-32,} } } */

struct S { char c[32]; } __attribute__((aligned (32)));

void sink (struct S);

void
caller (struct S *p)
{
  sink (*p);
}
