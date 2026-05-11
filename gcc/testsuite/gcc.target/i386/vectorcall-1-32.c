/* Test basic vectorcall attribute recognition and incompatibility errors.  */
/* { dg-do compile } */
/* { dg-options "-msse2" } */
/* { dg-require-effective-target ia32 } */

/* Basic vectorcall function declaration.  */
void foo1(int i, int j) __attribute__((vectorcall));

/* Vectorcall with regparm should error.  */
void foo2(int i, int j) __attribute__((vectorcall, regparm(2))); /* { dg-error "not compatible" } */

/* Vectorcall with cdecl should error.  */
void foo3(int i, int j) __attribute__((vectorcall, cdecl)); /* { dg-error "not compatible" } */

/* Vectorcall with stdcall should error.  */
void foo4(int i, int j) __attribute__((vectorcall, stdcall)); /* { dg-error "not compatible" } */

/* Vectorcall with fastcall should error.  */
void foo5(int i, int j) __attribute__((vectorcall, fastcall)); /* { dg-error "not compatible" } */

/* Vectorcall with thiscall should error.  */
void foo6(int i, int j) __attribute__((vectorcall, thiscall)); /* { dg-error "not compatible" } */

/* Vectorcall with sseregparm should error.  */
void foo7(int i, int j) __attribute__((vectorcall, sseregparm)); /* { dg-error "not compatible" } */

/* Vectorcall with variadic should error.  */
void foo8(int i, ...) __attribute__((vectorcall)); /* { dg-error "cannot be used with variadic functions" } */
