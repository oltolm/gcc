/* { dg-do compile } */
/* { dg-options "-O -ffixed-ebp -mno-accumulate-outgoing-args -mstackrealign -msse" } */
/* { dg-additional-options "-mno-stack-arg-probe" { target x86_64-*-mingw* } } */

/* { dg-warning "fixed ebp register requires" "" { target *-*-* } 0 } */

typedef float V __attribute__((vector_size(16)));

void bar (V a)
{
  volatile V b = a;
}
