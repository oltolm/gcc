// PR target/94770
// { dg-do run { target { ! ia32 } } }
// { dg-options "-O2" }

// va_arg must step over an empty class as the caller passed it: one slot
// under the MS ABI, nothing under the SysV ABI, in register and in stack
// positions.

struct S {};

#define MS __attribute__((ms_abi, noipa))
#define SYSV __attribute__((sysv_abi, noipa))

MS long long
ms_reg (int n, ...)
{
  __builtin_ms_va_list ap;
  __builtin_ms_va_start (ap, n);
  __builtin_va_arg (ap, S);
  long long x = __builtin_va_arg (ap, long long);
  __builtin_ms_va_end (ap);
  return x;
}

MS long long
ms_stack (int a, int b, int c, int d, ...)
{
  __builtin_ms_va_list ap;
  __builtin_ms_va_start (ap, d);
  __builtin_va_arg (ap, S);
  long long x = __builtin_va_arg (ap, long long);
  __builtin_ms_va_end (ap);
  return x + a + b + c + d - 10;
}

SYSV long long
sysv_reg (int n, ...)
{
  __builtin_sysv_va_list ap;
  __builtin_sysv_va_start (ap, n);
  __builtin_va_arg (ap, S);
  long long x = __builtin_va_arg (ap, long long);
  __builtin_sysv_va_end (ap);
  return x;
}

SYSV long long
sysv_stack (int a, int b, int c, int d, int e, int f, ...)
{
  __builtin_sysv_va_list ap;
  __builtin_sysv_va_start (ap, f);
  __builtin_va_arg (ap, S);
  long long x = __builtin_va_arg (ap, long long);
  __builtin_sysv_va_end (ap);
  return x + a + b + c + d + e + f - 21;
}

int
main ()
{
  if (ms_reg (1, S (), 42LL) != 42)
    __builtin_abort ();
  if (ms_stack (1, 2, 3, 4, S (), 42LL) != 42)
    __builtin_abort ();
  if (sysv_reg (1, S (), 42LL) != 42)
    __builtin_abort ();
  if (sysv_stack (1, 2, 3, 4, 5, 6, S (), 42LL) != 42)
    __builtin_abort ();
}
