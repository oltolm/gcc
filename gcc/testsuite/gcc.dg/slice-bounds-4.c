/* Under -fsanitize=bounds, build_slice_ref rejects E[lo:hi] with
   hi > count.  */
/* { dg-do run } */
/* { dg-shouldfail "bounds" } */
/* { dg-options "-fslices -fsanitize=bounds -fsanitize-trap=bounds" } */

int a[4];
volatile int i1 = 1, i5 = 5;

int
main (void)
{
  int t[:] = a[i1:i5];
  return t.count == 0;
}
