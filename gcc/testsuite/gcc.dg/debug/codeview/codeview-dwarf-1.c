/* { dg-do compile } */
/* { dg-options "-gcodeview -g" } */

/* dwarf2out_finish must emit both selected debug formats.  */

void
f (void)
{
}

/* { dg-final { scan-assembler {\.section[ \t]+\.debug\$S} } } */
/* { dg-final { scan-assembler {\.section[ \t]+\.debug\$T} } } */
/* { dg-final { scan-assembler {\.section[ \t]+\.debug_info} } } */
