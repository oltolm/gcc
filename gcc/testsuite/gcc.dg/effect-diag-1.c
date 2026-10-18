/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* A _Try must have exactly one clause for each operation of its effect,
   and the body of a _Try that runs as a coroutine must not leave it with
   asm goto, which cannot switch to the stack of the label.  */

_Effect st { int get (void); void put (int); };
_Effect ask { int ask (int); };

void
f (void)
{
  _Try				/* { dg-error "does not handle operation 'put'" } */
    {
    }
  _Handle (st.get ())
    {
      _Resume (0);
    }

  _Try
    {
    }
  _Handle (ask.ask (int x))
    {
      _Resume (x);
    }
  _Handle (ask.ask (int y))	/* { dg-error "duplicate" } */
    {
      _Resume (y);
    }
}

void
g (void)
{
  __label__ out;

  _Try
    {
      asm goto ("" : : : : out);	/* { dg-error "jump out of" } */
    }
  _Handle (ask.ask (int x) _Continuation k)
    {
      _Discard (k);
    }
 out:;
}
