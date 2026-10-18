/* { dg-do compile } */
/* { dg-options "-std=gnu23 -feffects" } */
/* Invalid _Dynamic declarations, diagnosed by declspecs_add_scspec,
   grokdeclarator and diagnose_mismatched_decls.  */

_Dynamic _Dynamic int d1;	/* { dg-error "duplicate '_Dynamic'" } */
typedef _Dynamic int d2;	/* { dg-error "'_Dynamic' used with 'typedef'" } */
_Dynamic typedef int d3;	/* { dg-error "'_Dynamic' used with 'typedef'" } */
_Dynamic _Thread_local int d4;	/* { dg-error "'_Dynamic' used with '_Thread_local'" } */
_Thread_local _Dynamic int d5;	/* { dg-error "'_Dynamic' used with '_Thread_local'" } */
constexpr _Dynamic int d6 = 1;	/* { dg-error "'_Dynamic' used with 'constexpr'" } */
_Dynamic int f (void);		/* { dg-error "invalid storage class for function 'f'" } */
_Dynamic int a[2];		/* { dg-error "'_Dynamic' variable 'a' has array type" } */

int r1;				/* { dg-message "previous" } */
_Dynamic int r1;		/* { dg-error "'_Dynamic' declaration of 'r1' follows non-'_Dynamic' declaration" } */
_Dynamic int r2;		/* { dg-message "previous" } */
extern int r2;			/* { dg-error "non-'_Dynamic' declaration of 'r2' follows '_Dynamic' declaration" } */

_Dynamic int ok;

struct s { _Dynamic int m; };	/* { dg-error "storage class specified for structure field 'm'" } */
void g (_Dynamic int p);	/* { dg-error "storage class specified for parameter 'p'" } */

void
h (void)
{
  _Dynamic int l1;		/* { dg-error "block-scope declaration of 'l1' specifies '_Dynamic'" } */
  static _Dynamic int l2;	/* { dg-error "block-scope declaration of 'l2' specifies '_Dynamic'" } */
  extern _Dynamic int ok;	/* { dg-error "block-scope declaration of 'ok' specifies '_Dynamic'" } */
  register _Dynamic int l3;	/* { dg-error "'_Dynamic' used with 'register'" } */
}
