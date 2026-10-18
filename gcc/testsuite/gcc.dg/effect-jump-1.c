/* { dg-do compile } */
/* { dg-options "-feffects -Wno-switch-unreachable" } */
/* A goto or case label that enters the body of a _Try or _Let would skip
   pushing its record on the handler chain; lookup_label_for_goto,
   check_earlier_gotos and c_check_switch_jump_warnings must reject it.  */

_Effect e { int op (void); };
_Dynamic int w, v;

void
f (int x)
{
  goto l1;			/* { dg-error "jump into the body of '_Let'" } */
  _Let (w = 1) { l1: ; }
  _Let (w = 1, v = 2) { l2: ; }
  goto l2;			/* { dg-error "jump into the body of '_Let'" } */
  goto l3;			/* { dg-error "jump into the body of '_Try'" } */
  _Try { l3: ; } _Handle (e.op ()) { _Resume (0); }
  _Try { l4: ; } _Handle (e.op ()) { _Resume (0); }
  goto l4;			/* { dg-error "jump into the body of '_Try'" } */

  switch (x)
    {
      _Let (w = 2) { case 1: ; }	/* { dg-error "switch jumps into the body of '_Let'" } */
    }
  switch (x)
    {
      _Try { case 2: ; } _Handle (e.op ()) { _Resume (0); }	/* { dg-error "switch jumps into the body of '_Try'" } */
    }

  /* Jumps within and out of a body are fine.  */
  _Let (w = 3)
    {
      goto l5;
    l5:
      goto l6;
    }
 l6:
  _Try { goto l7; } _Handle (e.op ()) { _Resume (0); }
 l7:
  switch (x)
    {
    case 3:
      _Let (w = 4) { break; }
    }
}
