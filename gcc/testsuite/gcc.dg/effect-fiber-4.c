/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* Errors for _Try statements whose clauses take a continuation.  */

_Effect ask { int ask (int); };

int
f (void)
{
  _Try
    {
      return 1;			/* { dg-error "runs as a coroutine" } */
    }
  _Handle (ask.ask (int x) _Continuation k)
    {
      _Resume (x);		/* { dg-error "without the continuation" } */
    }
  return 0;
}
