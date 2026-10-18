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

  _Try
    {
    }
  _Handle (ask.ask (int x) _Continuation k)
    {
      void *p = k;		/* { dg-message "unimplemented" } */
      _Resume (k, x);
    }

  _Try
    {
    }
  _Handle (ask.ask (int x))
    {
      _Discard (k);		/* { dg-error "not within" } */
      _Resume (x);
    }

  _Try
    {
    }
  _Handle (ask.ask (int x) _Continuation k) /* { dg-message "unimplemented" } */
    {
      x++;
    }
  return 0;
}
