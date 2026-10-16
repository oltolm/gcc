/* convert_for_assignment converts a slice to a slice whose element type
   adds qualifiers, and rejects conversions that remove them.  */
/* { dg-do compile } */
/* { dg-options "-fslices" } */

void takec (const int s[:]);
void takev (volatile int s[:]);

const int
retc (int s[:])[:]
{
  return s;
}

void
add (int s[:], const int cs[:], volatile int vs[:])
{
  const int t[:] = s;
  const volatile int u[:] = s;

  u = cs;
  u = vs;
  takec (s);
  takev (s);
}

void
drop (int s[:], const int cs[:], volatile int vs[:], int ss[:][:])
{
  int r[:] = cs;		/* { dg-error "incompatible types" } */
  s = vs;			/* { dg-error "incompatible types" } */
  takec (vs);			/* { dg-error "incompatible type" } */
  const int cr[:][:] = ss;	/* { dg-error "incompatible types" } */
}
