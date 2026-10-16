/* Slice types T[:]: conversion from an array sets data and count, _Countof
   returns count, indexing accesses the underlying array, and slices are
   passed by value.  */
/* { dg-do run } */
/* { dg-options "-fslices" } */

typedef __SIZE_TYPE__ size_t;

static size_t
count (int s[:])
{
  return s.count;
}

static void
shrink (int s[:])
{
  s.count = 1;
}

int
main (void)
{
  int a[4] = { 1, 2, 3, 4 };
  int s[:] = a;

  if (s.data != a || s.count != 4 || _Countof (s) != 4)
    __builtin_abort ();
  if (count (a) != 4 || count (s) != 4)
    __builtin_abort ();

  shrink (s);
  if (s.count != 4)
    __builtin_abort ();

  s[2] = 10;
  if (a[2] != 10 || s[3] != 4)
    __builtin_abort ();

  s.data = &a[1];
  s.count = 2;
  if (s[0] != 2 || _Countof (s) != 2)
    __builtin_abort ();

  return 0;
}
