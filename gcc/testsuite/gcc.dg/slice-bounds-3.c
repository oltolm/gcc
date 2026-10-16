/* Under -fsanitize=bounds, build_array_ref rejects s[i] with negative i.  */
/* { dg-do run } */
/* { dg-shouldfail "bounds" } */
/* { dg-options "-fslices -fsanitize=bounds -fsanitize-trap=bounds" } */

int a[4];
volatile int im1 = -1;
volatile int x;

int
main (void)
{
  int s[:] = a;
  x = s[im1];
  return 0;
}
