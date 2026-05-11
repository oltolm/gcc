/* Test that 64-bit vectorcall symbols are correctly mangled in C++
	 and that stdcall/fastcall do not leak into C++ mangled names.

	 On Win64, stdcall and fastcall attributes are ignored -- there is
	 only one 64-bit calling convention.  vectorcall is the exception,
	 and it must participate in function-pointer type mangling so that
	 distinct overloads and template arguments do not collide.  */

/* { dg-do compile { target { c++11 && x86_64-*-mingw* } } } */
/* { dg-options "-O2" } */
/* { dg-skip-if "" { *-*-* } { "-flto" } } */

/* stdcall function pointer -- must NOT get U7stdcallF in mangled type.  */
void __stdcall (*volatile fp) (long long, long long, long long);

void
call_stdcall_fp (long long a, long long b, long long c)
{
  fp (a, b, c);
}

/* { dg-final { scan-assembler {_Z15call_stdcall_fpxxx} { target x86_64-*-mingw* } } } */

/* dllimport class method with stdcall function pointer -- also must NOT
	 get U7stdcallF.  */
struct __declspec (dllimport) Foo
{
  void sort (long long (__stdcall *cmp) (long long, long long, long long));
};

void
test_dllimport_class (Foo *f, long long (__stdcall *cmp) (long long, long long,
							  long long))
{
  f->sort (cmp);
}

/* { dg-final { scan-assembler {_Z20test_dllimport_classP3FooPFxxxxE} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {__imp__ZN3Foo4sortEPFxxxxE} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler-not {U7stdcallF} { target x86_64-*-mingw* } } } */

/* Clang and GCC both append the vectorcall @@bytes suffix to the final
	 C++ mangled symbol.  3 args x 8 bytes = 24.  */
void __attribute__ ((vectorcall)) ex_vectorcall_func (int a, int b,
						      long long c);

void
call_vectorcall (void)
{
  ex_vectorcall_func (1, 2, 3);
}

/* { dg-final { scan-assembler {_Z18ex_vectorcall_funciix@@24} { target x86_64-*-mingw* } } } */

/* Vectorcall must be encoded into function pointer type mangling so that
	 distinct overloads do not collapse to the same external name.  */
void __attribute__ ((vectorcall)) (*volatile vfp) (int, int, int);

void
call_vectorcall_fp (int a, int b, int c)
{
  vfp (a, b, c);
}

/* This function's signature does not itself contain the vectorcall type,
	 so its name should stay unchanged.  */
/* { dg-final { scan-assembler {_Z18call_vectorcall_fpiii} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {U10vectorcallF} { target x86_64-*-mingw* } } } */

using normal_fp = void (*) ();
using vc_fp = void (__attribute__ ((vectorcall)) *) ();

int
overload (normal_fp)
{
	return 1;
}

int
overload (vc_fp)
{
	return 2;
}

/* { dg-final { scan-assembler {_Z8overloadPFvvE} { target x86_64-*-mingw* } } } */
/* { dg-final { scan-assembler {_Z8overloadPU10vectorcallFvvE} { target x86_64-*-mingw* } } } */
