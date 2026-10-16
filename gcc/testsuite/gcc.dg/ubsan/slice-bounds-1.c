/* Under -fsanitize=bounds, build_array_ref checks s[i] against s.count,
   and build_slice_ref checks lo <= hi <= count for E[lo:hi].  */
/* { dg-do run } */
/* { dg-options "-fslices -fsanitize=bounds -fsanitize-recover=bounds" } */

int a[4];
volatile int im1 = -1, i1 = 1, i2 = 2, i3 = 3, i4 = 4, i5 = 5;
volatile int x;

int
main (void)
{
  int s[:] = a;
  int t[:];

  x = s[i3];
  t = a[i1:i3];
  t = a[i4:i4];
  t = a[:i4];
  t = s[i2:];
  t = s[:i4];

  x = s[i4];
  x = s[im1];
  t = a[i1:i5];
  t = a[i3:i2];
  t = a[im1:i2];
  t = s[i5:];
  t = s[:i5];

  return 0;
}

/* { dg-output "index 4 out of bounds for type 'int \\\[\\\*\\\]'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*index -1 out of bounds for type 'int \\\[\\\*\\\]'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*index 5 out of bounds for type 'int \\\[4\\\]'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*index 3 out of bounds for type 'int \\\[4\\\]'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*index -1 out of bounds for type 'int \\\[4\\\]'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*index 5 out of bounds for type 'int \\\[\\\*\\\]'\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*index 5 out of bounds for type 'int \\\[\\\*\\\]'" } */
