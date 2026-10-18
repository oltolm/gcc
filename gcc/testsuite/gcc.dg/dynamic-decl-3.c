/* { dg-do compile } */
/* { dg-options "-std=gnu23" } */
/* Without -feffects, c_parse_init must not make _Dynamic or _Let
   reserved words.  */

int _Dynamic, _Let;
