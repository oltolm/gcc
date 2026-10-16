/* The C pretty-printer prints slice types as T[:], including slices of
   arrays, arrays of slices, pointers to slices and qualified elements.  */
/* { dg-do compile } */
/* { dg-options "-fslices" } */

void
f (int *p, int s[:], const int cs[:], int m[:][4], int (*ps)[:])
{
  int as[2][:];

  p = s;	/* { dg-error {assigning to type 'int \*' from type 'int\[:\]'} } */
  p = cs;	/* { dg-error {from type 'const int\[:\]'} } */
  p = m;	/* { dg-error {from type 'int\[:\]\[4\]'} } */
  p = ps;	/* { dg-error {from incompatible pointer type 'int \(\*\)\[:\]'} } */
  p = &as;	/* { dg-error {from incompatible pointer type 'int \(\*\)\[2\]\[:\]'} } */
  s.foo = 0;	/* { dg-error {'int\[:\]' has no member named 'foo'} } */
}
