/* Line numbers after a multi-line raw string pasted with ## must not
   count the raw string's newlines twice.  */
/* { dg-do compile { target { c || c++11 } } } */
/* { dg-options "-std=gnu99 -Wsign-compare" { target c } } */
/* { dg-options "-Wsign-compare" { target c++ } } */

#define W(x) L ## x

int a = sizeof (W(R"(
one
two
three)"));

int f (unsigned u, int i) { return i < u; } /* { dg-warning "comparison of integer expressions of different signedness" } */

/* A raw string that is not pasted must still advance the line map.  */
const char *b = R"(
one
two)";

int g (unsigned u, int i) { return i < u; } /* { dg-warning "comparison of integer expressions of different signedness" } */
