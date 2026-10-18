/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* c_lex_one_token must treat the effect keywords as identifiers in system
   headers, which use such names for parameters.  */

#include "effect-sysheader-1.h"
