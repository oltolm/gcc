/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* c_parser_effect_for_statement must reject an operation with a result,
   an effect with more than one operation, and _Resume in the body, and
   check the value of return in the body against the return type of the
   function.  */

_Effect one { void op (int); };
_Effect value { int op (int); };
_Effect two { void op (int); void op2 (void); };

void producer (void);

int
f (void)
{
  for (value.op (int v) : producer ())	/* { dg-error "operation 'op' of a 'for' statement returns a value" } */
    ;
  for (two.op (int v) : producer ())	/* { dg-error "'for' statement over effect 'two', which has more than one operation" } */
    ;
  for (one.op (int v) : producer ())
    return "";				/* { dg-error "returning 'char \\*' from a function with return type 'int' makes integer from pointer without a cast" } */
  for (one.op (int v) : producer ())
    _Resume ();				/* { dg-error "'_Resume' outside a '_Handle' clause" } */
  return 0;
}
