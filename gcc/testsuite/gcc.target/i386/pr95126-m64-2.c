/* { dg-do compile { target { ! ia32 } } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */

struct small{ short a,b; signed char c; };
static const struct small s = { 1,2,0 };
extern int func(struct small X);

void call_func(void)
{
  func(s);
}

/* { dg-final { scan-assembler "movl\[ \\t]*\\\$131073, " } } */
/* { dg-final { scan-assembler-not "movzwl" } } */
/* { dg-final { scan-assembler-not "salq" } } */
/* { dg-final { scan-assembler-not "orq" } } */

