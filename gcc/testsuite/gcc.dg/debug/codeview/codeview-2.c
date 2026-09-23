/* { dg-do assemble } */
/* { dg-options "-gcodeview -g" } */

/* Plain -g after -gcodeview must not enable DWARF output.  */

void
f (void)
{
}
