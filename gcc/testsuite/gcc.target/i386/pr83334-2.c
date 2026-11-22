/* PR target/83334 */
/* expand_builtin_va_copy must copy a SysV va_list as the whole structure,
   also in a function using the MS ABI.  */
/* { dg-do run { target { lp64 || llp64 } } } */
/* { dg-options "-O2" } */

extern void abort (void);

static int __attribute__ ((sysv_abi, noipa))
sum (int n, __builtin_sysv_va_list ap)
{
  int i, s = 0;
  for (i = 0; i < n; i++)
    s += __builtin_va_arg (ap, int);
  return s;
}

static int __attribute__ ((ms_abi, noipa))
copy_and_sum (int n, __builtin_sysv_va_list ap)
{
  __builtin_sysv_va_list ap2;
  int r;
  __builtin_sysv_va_copy (ap2, ap);
  r = sum (n, ap2);
  __builtin_sysv_va_end (ap2);
  return r;
}

static int __attribute__ ((sysv_abi, noipa))
varargs (int n, ...)
{
  __builtin_sysv_va_list ap;
  int r;
  __builtin_sysv_va_start (ap, n);
  r = copy_and_sum (n, ap);
  __builtin_sysv_va_end (ap);
  return r;
}

int
main (void)
{
  if (varargs (3, 1, 2, 3) != 6)
    abort ();
  return 0;
}
