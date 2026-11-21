/* { dg-do compile } */
/* { dg-options "-masm=att" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

void bar(unsigned char x);
void foo()
{
  bar(225);
}

/* PROMOTE_PROTOTYPES should make us pass 225, not -31, both would
   be valid according to the ABI.  */
/* { dg-final { scan-assembler "movl\[ \t\]+\\\$225, %edi" { target { ! ia32 } } } } */
