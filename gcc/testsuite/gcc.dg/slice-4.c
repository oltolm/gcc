/* Slice expressions E[lo:hi]: the result has data &E[lo] and count
   hi - lo, omitted bounds default to 0 and the count of E, and E and lo
   are evaluated once.  */
/* { dg-do run } */
/* { dg-options "-fslices" } */

int a[5] = { 0, 1, 2, 3, 4 };
int calls;

static int
get (void)[:]
{
  calls++;
  return a;
}

int
main (void)
{
  int s[:] = a[1:4];
  if (s.data != &a[1] || s.count != 3 || s[0] != 1)
    __builtin_abort ();

  s = s[1:];
  if (s.data != &a[2] || s.count != 2 || s[0] != 2)
    __builtin_abort ();

  s = a[:];
  if (s.data != a || s.count != 5)
    __builtin_abort ();

  s = a[2:];
  if (s.data != &a[2] || s.count != 3)
    __builtin_abort ();

  s = a[:2];
  if (s.data != a || s.count != 2)
    __builtin_abort ();

  s = a[2:2];
  if (s.count != 0)
    __builtin_abort ();

  int i = 1;
  s = a[i++:4];
  if (i != 2 || s.data != &a[1] || s.count != 3)
    __builtin_abort ();

  s = get ()[1:];
  if (calls != 1 || s.data != &a[1] || s.count != 4)
    __builtin_abort ();

  return 0;
}
