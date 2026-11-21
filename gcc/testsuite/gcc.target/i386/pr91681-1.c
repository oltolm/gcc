/* { dg-do compile { target int128 } } */
/* { dg-options "-O2" } */
/* { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } } */
unsigned __int128 m;

unsigned __int128 foo(unsigned __int128 x, unsigned long long y)
{
    return x + y;
}

void bar(unsigned __int128 x, unsigned long long y)
{
    m = x + y;
}

void baz(unsigned long long y)
{
    m += y;
}

/* { dg-final { scan-assembler-not "xor" } } */
