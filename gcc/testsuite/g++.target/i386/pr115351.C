/* { dg-do compile } */
/* { dg-options "-O3 -std=c++11" } */
// { dg-additional-options "-mabi=sysv" { target x86_64-*-mingw* } }

struct complex
{
    double real;
    double imag;
};

complex blub(complex z)
{
  return {
	   z.real * z.real - z.imag * z.imag,
	   2 * z.real * z.imag
	  };
}

/* { dg-final { scan-assembler-not "movq" } } */
/* { dg-final { scan-assembler-not "xchg" } } */
