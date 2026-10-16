/* Under -fsanitize=bounds, build_slice_ref rejects E[lo:hi] with lo > hi.  */
/* { dg-do run } */
/* { dg-shouldfail "bounds" } */
/* { dg-options "-fslices -fsanitize=bounds -fsanitize-trap=bounds" } */

int a[4];
volatile int i2 = 2, i3 = 3;

int
main (void)
{
  int t[:] = a[i3:i2];
  return t.count == 0;
}
