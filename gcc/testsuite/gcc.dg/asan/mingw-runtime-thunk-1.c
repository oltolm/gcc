/* LIBASAN_SPEC must link every member of libasan_dynamic_runtime_thunk.a,
   including the members that no instrumented code references.  */
/* { dg-do link { target x86_64-*-mingw* } } */

int
main (void)
{
  return 0;
}

/* { dg-final { scan-symbol "__asan_schedule_unregister_globals" } } */
/* { dg-final { scan-symbol "__sanitizer_thunk_init_ptr" } } */
