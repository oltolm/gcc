/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* _Handler (eff) types of different effects must be incompatible and
   opaque, a handler name must be in scope only in the _Try body, and the
   clauses of a named _Try must handle its effect.  */

_Effect ask { int ask (int); };
_Effect trace { void trace (int); };

void take_trace (_Handler (trace));

void
f (_Handler (ask) h, int i)
{
  take_trace (h);			/* { dg-error "incompatible" } */
  h->next;			/* { dg-error "undefined type" } */
  _Perform h.tell (1);		/* { dg-error "has no operation" } */
  _Perform i.ask (1);		/* { dg-error "not an effect or a handler" } */
}

void
g (void)
{
  _Try (ask h)
    {
    }
  _Handle (ask.ask (int x))
    {
      _Resume (_Perform h.ask (x)); /* { dg-error "undeclared" } */
    }

  _Try (ask h2)
    {
    }
  _Handle (trace.trace (int x))	/* { dg-error "does not handle" } */
    {
      _Resume ();
    }

  _Try (f h3)			/* { dg-error "not an effect" } */
    {
    }
  _Handle (ask.ask (int x))
    {
      _Resume (x);
    }
}

/* { dg-prune-output "note:" } */
