/* { dg-do compile } */
/* { dg-require-effective-target strub } */
/* { dg-options "-fstrub=all -O2 -mno-accumulate-outgoing-args" } */
/* { dg-additional-options "-mno-stack-arg-probe" { target x86_64-*-mingw* } } */

__attribute__((noipa))
long _raw_syscall(void *, long, long) {
  __builtin_abort();
}

static int privileged_traced_syscall() {
  return _raw_syscall(0, 0, 0);
}

void privileged_traced_raise() {
  privileged_traced_syscall();
  __builtin_unreachable ();
}
