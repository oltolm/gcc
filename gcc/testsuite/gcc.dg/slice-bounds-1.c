/* Under -fsanitize=bounds, the checks emitted by build_array_ref for s[i]
   and by build_slice_ref for E[lo:hi] accept i < count and
   lo <= hi <= count, and static initializers are not instrumented.  */
/* { dg-do run } */
/* { dg-options "-fslices -fsanitize=bounds -fsanitize-trap=bounds" } */

int a[4];
int gs[:] = a[1:3];
volatile int i0 = 0, i2 = 2, i3 = 3, i4 = 4;
volatile int x;

int
main (void)
{
  int s[:] = a;
  int t[:];

  x = s[i0];
  x = s[i3];
  t = a[i0:i4];
  t = a[i4:i4];
  t = a[i4:];
  t = a[:i4];
  t = s[i2:i3];
  t = s[i4:];
  t = s[:i4];
  t = t[i0:i0];

  return 0;
}
