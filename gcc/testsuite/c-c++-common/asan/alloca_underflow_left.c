/* { dg-do run } */
/* { dg-skip-if "" { *-*-mingw* } { "-flto" } { "" } } */
/* { dg-shouldfail "asan" } */

#include <assert.h>

volatile const int ten = 10;

__attribute__((noinline)) void foo(int index, int len) {
  volatile char str[len] __attribute__((aligned(32)));
  assert(!(((__UINTPTR_TYPE__) str) & 31));
  str[index] = '1'; // BOOM
}

int main(int argc, char **argv) {
  foo(-1, ten);
  return 0;
}

/* { dg-output "WRITE of size 1 at 0x\[0-9a-f\]+ thread T0\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "    #0 0x\[0-9a-f\]+ +(in _*foo(\[^\n\r]*alloca_underflow_left.c:12|\[^\n\r]*:0|\[^\n\r]*\\+0x\[0-9a-z\]*)|\[(\])\[^\n\r]*(\n|\r\n|\r)" } */
/* { dg-output "\[^\n\r]*Address 0x\[0-9a-f\]+ is located in stack of thread T0.*(\n|\r\n|\r)" */
/* { dg-output "\[^\n\r]*in foo.*alloca_underflow_left.c.*(\n|\r\n|\r)" */
