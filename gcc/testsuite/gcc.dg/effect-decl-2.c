/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* Invalid effect declarations and uses of an effect name, diagnosed by
   c_parser_effect_declaration, diagnose_mismatched_decls and
   build_external_ref.  */

_Effect e { int op (void); };	/* { dg-message "previous" } */
_Effect e { int op (void); };	/* { dg-error "redefinition of effect 'e'" } */

int v;				/* { dg-message "previous" } */
_Effect v { int op (void); };	/* { dg-error "redeclared as different kind of symbol" } */

_Effect w { int op (void); };	/* { dg-message "previous" } */
int w;				/* { dg-error "redeclared as different kind of symbol" } */

_Effect f { int x; };		/* { dg-error "effect operation 'x' is not a function" } */
_Effect g { int (*p) (void); };	/* { dg-error "effect operation 'p' is not a function" } */
_Effect h { int op (void); int op (int); };	/* { dg-error "duplicate member 'op'" } */

void
block (void)
{
  _Effect b { void op (void); };	/* { dg-error "'_Effect' declaration not at file scope" } */
}

void *
value (void)
{
  return &e;			/* { dg-error "effect 'e' used as a value" } */
}
