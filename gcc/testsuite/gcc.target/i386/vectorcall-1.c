/* Test 64-bit vectorcall attribute recognition.  */
/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-msse2" } */

/* Basic vectorcall function declaration.  */
void foo1(int i, int j) __attribute__((vectorcall));

/* Vectorcall with variadic should error on 64-bit too.  */
void foo2(int i, ...) __attribute__((vectorcall)); /* { dg-error "vectorcall attribute cannot be used with variadic functions" } */

/* Keep mixed ABI attributes accepted: this target/toolchain no longer
	diagnoses these combinations at declaration time.  */
void foo3(int i, int j) __attribute__((vectorcall, ms_abi));
void foo4(int i, int j) __attribute__((vectorcall, sysv_abi));
