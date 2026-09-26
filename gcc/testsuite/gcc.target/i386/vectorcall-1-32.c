/* Test that the vectorcall attribute is rejected for 32-bit code, where
   the convention is not implemented.  */
/* { dg-do compile } */
/* { dg-require-effective-target ia32 } */

void foo (int i, int j) __attribute__((vectorcall)); /* { dg-message "not supported for 32-bit code" } */
