/* { dg-do compile } */
/* { dg-options "-O2 -msse2 -mtune=generic -mno-stackrealign" } */
/* { dg-additional-options "-mcmodel=small" { target x86_64-*-mingw* } } */

extern char *dst, *src;

void
foo (void)
{
  __builtin_memcpy (dst, src, 15);
}

/* { dg-final { scan-assembler-times "movq\[\\t \]+\\(%\[\^,\]+\\)," 1 } } */
/* { dg-final { scan-assembler-times "movq\[\\t \]+7\\(%\[\^,\]+\\)," 1 } } */
