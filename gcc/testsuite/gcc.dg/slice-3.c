/* Slice expressions E[lo:hi]: E must be an array or a slice, the bounds
   must be integers and may be omitted, and the result is a slice of E's
   element type.  */
/* { dg-do compile } */
/* { dg-options "-fslices" } */

int a[4];
int m[3][4];

void
valid (int s[:], int i, long j)
{
  int t[:];

  t = a[1:3];
  t = a[i:j];
  t = a[:];
  t = a[1:];
  t = a[:2];
  t = s[1:2];
  t = s[:];
  t = t[1:];

  int r[:][4] = m[1:3];
}

void
invalid (int *p, int x, int s[:])
{
  int t[:];

  t = p[0:2];		/* { dg-error "cannot slice" } */
  t = x[0:1];		/* { dg-error "cannot slice" } */
  t = a[1.0:2];		/* { dg-error "slice bound is not an integer" } */
  t = s[0:p];		/* { dg-error "slice bound is not an integer" } */

  int *q = a[1:3];	/* { dg-error "incompatible types" } */
}
