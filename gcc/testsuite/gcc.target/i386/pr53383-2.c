/* { dg-do compile } */
/* { dg-options "-O2 -mpreferred-stack-boundary=3 -mincoming-stack-boundary=3" } */
/* { dg-skip-if "SEH limits -mpreferred-stack-boundary to 4" { x86_64-*-mingw* } } */

int
bar (int x)
{
  return x + 9;
}
