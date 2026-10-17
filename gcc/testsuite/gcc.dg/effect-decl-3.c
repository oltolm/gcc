/* { dg-do compile } */
/* { dg-options "-std=gnu23" } */
/* Without -feffects, c_parse_init must not make _Effect a reserved
   word.  */

int _Effect;
