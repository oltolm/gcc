/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* c_parser_effect_try_statement must reject a goto out of a _Handle
   clause of a _Try whose body runs as a coroutine.  */

_Effect e { int op (void); int op2 (void); };

void
f (void)
{
  __label__ out;
  _Try
    {
      _Perform e.op ();
    }
  _Handle (e.op () _Continuation k)
    {
      goto out;			/* { dg-message "sorry, unimplemented: jump out of a '_Handle' clause" } */
      _Discard (k);
    }
  _Handle (e.op2 ())
    {
      _Resume (0);
    }
 out:;
}
