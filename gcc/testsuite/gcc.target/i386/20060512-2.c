/* { dg-do compile } */
/* { dg-options "-std=gnu99 -mpreferred-stack-boundary=4" } */
/* { dg-skip-if "SEH does not support stack realignment" { x86_64-*-mingw* } } */
int
outer_function (int x, int y)
{
  int __attribute__ ((__noinline__, __force_align_arg_pointer__))
  nested_function (int x, int y)
    {
      return (x + y);
    }
  return (3 + nested_function (x, y));
}
