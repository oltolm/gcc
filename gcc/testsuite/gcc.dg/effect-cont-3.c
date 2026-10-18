/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* Errors for continuations used outside their clause.  */

_Effect ask { int ask (int); };
_Effect trace { void trace (int); };

_Continuation (ask.tell) bad;		/* { dg-error "has no operation" } */
_Continuation (ask.ask) saved;
_Continuation (trace.trace) tsaved;

void
f (void)
{
  _Try
    {
      _Resume (saved, 1);	/* { dg-error "does not handle" } */
    }
  _Handle (trace.trace (int x) _Continuation k)
    {
      tsaved = k;
    }

  _Try (ask h)			/* { dg-error "handler name" } */
    {
      _Resume (saved, 1);
    }
  _Handle (ask.ask (int x) _Continuation k)
    {
      saved = k;
    }

  _Try
    {
      _Resume (saved, 1);	/* { dg-error "only statement" } */
      saved = saved;
    }
  _Handle (ask.ask (int x) _Continuation k)
    {
      saved = k;
    }

  _Try
    {
    }
  _Handle (ask.ask (int x) _Continuation k)
    {
      k = saved;		/* { dg-error "read-only" } */
      _Resume (k, x);
    }

  _Discard (1);			/* { dg-error "not a continuation" } */
}
