/* The MinGW LIBASAN_SPEC must reject -static-libasan, since there is no
   static ASan runtime.  */
/* { dg-do link { target x86_64-*-mingw* } } */
/* { dg-options "-fsanitize=address -static-libasan" } */

int
main (void)
{
  return 0;
}

/* { dg-error "-static-libasan is not supported on this target" "" { target x86_64-*-mingw* } 0 } */
