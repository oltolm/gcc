/* PR target/54412 */
/* Caller-side counterpart to the callee home-slot tests: passing a 32-byte
   vector rvalue by value on Win64 uses an invisible by-reference temporary in
   the caller.  Before the slot-realignment fix, assign_temp built that
   temporary with ordinary fixed frame-slot allocation, so MEM_ALIGN still
   claimed 32 bytes while the address was only 16-byte aligned, and GCC chose
   an aligned store into a plain rbp-relative stack slot.  The fixed code
   preserves the requested alignment by overallocating and aligning the
   effective address inside the raw slot instead, so the caller-side store no
   longer goes straight to a fixed rbp-relative slot.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O0 -mavx -fno-omit-frame-pointer" } */
/* { dg-final { scan-assembler-not {vmovaps\t%ymm[0-9]+, -[0-9]+\(%rbp\)} } } */

typedef float v8f __attribute__ ((vector_size (32), aligned (32)));

__attribute__ ((noinline, noclone))
static float
first_lane (v8f x)
{
  return x[0];
}

__attribute__ ((noinline, noclone))
float
caller (v8f a, v8f b)
{
  return first_lane (a + b);
}
