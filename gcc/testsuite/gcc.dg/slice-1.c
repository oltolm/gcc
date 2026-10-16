/* Slice types T[:]: arrays convert to slices, slices never convert to
   pointers, and the data and count fields are accessible and assignable.  */
/* { dg-do compile } */
/* { dg-options "-fslices" } */

typedef __SIZE_TYPE__ size_t;

int a[4];

void take (int s[:]);

void
conversions (int *p)
{
  int s[:] = a;
  int t[:];

  t = a;
  t = s;
  take (a);
  take (s);

  int *q = s;	/* { dg-error "incompatible types" } */
  p = s;	/* { dg-error "incompatible types" } */
  take (p);	/* { dg-error "incompatible type" } */
}

void
fields (int *p, size_t n)
{
  int s[:];

  s.data = p;
  s.count = n;

  int *q = s.data;
  size_t c = s.count;
  size_t d = _Countof (s);
}

void
parameter (int s[:])
{
  /* A slice parameter is not adjusted to a pointer.  */
  int *q = s;	/* { dg-error "incompatible types" } */
  size_t c = s.count;
}
