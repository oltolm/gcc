/* convert_for_assignment keeps data and count when it converts int[:] to
   const int[:].  */
/* { dg-do run } */
/* { dg-options "-fslices" } */

int a[4] = { 1, 2, 3, 4 };

static int
first (const int s[:])
{
  return s[0];
}

int
main (void)
{
  int s[:] = a[1:];
  const int t[:] = s;

  if (t.data != &a[1] || t.count != 3 || first (s) != 2)
    __builtin_abort ();

  return 0;
}
