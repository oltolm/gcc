/* { dg-do run { target x86_64-*-mingw* } } */
/* { dg-options "-O0" } */
/* As seh-run-4.c, without optimization, which keeps the cleanup regions
   that end the lifetime of the variables.  */

#include "seh-run-4.c"
