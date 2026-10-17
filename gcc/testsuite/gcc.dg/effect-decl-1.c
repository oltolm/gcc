/* { dg-do compile } */
/* { dg-options "-feffects" } */
/* c_parser_effect_declaration must accept file-scope effects whose members
   are function declarators.  */

struct point { int x, y; };

_Effect state { int get (void); void put (int); };
_Effect move { struct point step (struct point, int), *origin (void); };
_Effect tick { void tick (void); };
