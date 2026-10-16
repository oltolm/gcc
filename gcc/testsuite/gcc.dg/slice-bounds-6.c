/* Under -fsanitize=bounds, build_slice_ref rejects s[lo:] with
   lo > s.count.  */
/* { dg-do run } */
/* { dg-shouldfail "bounds" } */
/* { dg-options "-fslices -fsanitize=bounds -fsanitize-trap=bounds" } */

int a[4];
volatile int i5 = 5;

int
main (void)
{
  int s[:] = a;
  int t[:] = s[i5:];
  return t.count == 0;
}
