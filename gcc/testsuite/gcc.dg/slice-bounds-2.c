/* Under -fsanitize=bounds, build_array_ref rejects s[i] with i == count.  */
/* { dg-do run } */
/* { dg-shouldfail "bounds" } */
/* { dg-options "-fslices -fsanitize=bounds -fsanitize-trap=bounds" } */

int a[4];
volatile int i4 = 4;
volatile int x;

int
main (void)
{
  int s[:] = a;
  x = s[i4];
  return 0;
}
