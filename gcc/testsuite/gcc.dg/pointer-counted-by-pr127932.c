/* PR c/127932 */
/* { dg-do compile } */
/* { dg-options "-Wunused-but-set-parameter -Wunused-but-set-variable" } */

struct buffer {
  int *ptr __attribute__ ((counted_by (len)));
  unsigned int len;
};

struct box {
  struct buffer in;
};

int
f1 (struct buffer const buf)	/* { dg-bogus "set but not used" } */
{
  return buf.ptr[0];
}

int
f2 (struct box b)		/* { dg-bogus "set but not used" } */
{
  return b.in.ptr[1];
}

int
f3 (void)
{
  struct buffer buf = { 0, 0 };	/* { dg-bogus "set but not used" } */
  return buf.ptr[0];
}

void
f4 (struct buffer buf)		/* { dg-warning "set but not used" } */
{
  buf.ptr = 0;
}
