/* PR target/54412 */
/* Like ../pr54412-overaligned-vector-param.c, for a parameter whose size is
   only known at run time.  It only has to compile.  */
/* { dg-do compile } */
/* { dg-options "-O0" } */

#include <arm_sve.h>

typedef svint32_t aligned_svint32_t __attribute__ ((aligned (32)));

extern void consume (aligned_svint32_t *);

void
f (aligned_svint32_t x)
{
  consume (&x);
}
