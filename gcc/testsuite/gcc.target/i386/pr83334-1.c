/* PR target/83334 */
/* expand_builtin_va_copy must copy an MS va_list as a pointer, also in a
   function using the SysV ABI.  -O0 because at -O2 the copy is folded
   into an assignment before expansion.  */
/* { dg-do run { target { lp64 || llp64 } } } */
/* { dg-options "-O0" } */

extern void abort (void);

static int __attribute__ ((ms_abi))
sum (int n, __builtin_ms_va_list ap)
{
  int i, s = 0;
  for (i = 0; i < n; i++)
    s += __builtin_va_arg (ap, int);
  return s;
}

static int __attribute__ ((sysv_abi))
copy_and_sum (int n, __builtin_ms_va_list ap)
{
  __builtin_ms_va_list ap2;
  int r;
  __builtin_ms_va_copy (ap2, ap);
  r = sum (n, ap2);
  __builtin_ms_va_end (ap2);
  return r;
}

static int __attribute__ ((ms_abi))
varargs (int n, ...)
{
  __builtin_ms_va_list ap;
  int r;
  __builtin_ms_va_start (ap, n);
  r = copy_and_sum (n, ap);
  __builtin_ms_va_end (ap);
  return r;
}

int
main (void)
{
  if (varargs (3, 1, 2, 3) != 6)
    abort ();
  return 0;
}
