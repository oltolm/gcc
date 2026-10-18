/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* Invalid _Let statements and address-of a _Dynamic variable, diagnosed
   by c_parser_let_statement and c_parser_unary_expression.  */

_Dynamic int width;
_Dynamic const int fixed = 1;
_Dynamic struct { int a; } pair;
int plain;

void
f (void)
{
  _Let (plain = 1) { }		/* { dg-error "'plain' is not a '_Dynamic' variable" } */
  _Let (nosuch = 1) { }		/* { dg-error "'nosuch' undeclared" } */
  _Let (width = 1, width = 2) { }	/* { dg-error "'width' bound more than once" } */
  _Let (width = pair) { }	/* { dg-error "incompatible types" } */
  _Let (width = 1) ;		/* { dg-error "expected '\{'" } */
  _Let (fixed = 2) { fixed = 3; }	/* { dg-error "assignment of read-only variable 'fixed'" } */
  int *p = &width;		/* { dg-error "address of '_Dynamic' variable 'width'" } */
  int *q = &pair.a;		/* { dg-error "address of '_Dynamic' variable 'pair'" } */
}
