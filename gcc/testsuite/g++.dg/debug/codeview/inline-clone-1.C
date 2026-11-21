// { dg-do compile }
// { dg-options "-O2 -gcodeview" }

// An IPA clone's DIE is created only after final, after
// codeview_debug_early_finish has collected the functions.  The clone must
// still get an S_LPROC32_ID, an S_INLINESITE for callee, and binary
// annotations, which require cv_func_htab to find the clone's lines.

__attribute__ ((always_inline)) inline int
callee (int i)
{
  return i * 3;
}

__attribute__ ((noinline)) static int
cloned (int a, int b)
{
  return callee (a) + b;
}

int
use1 (int x)
{
  return cloned (x, 5);
}

int
use2 (int x)
{
  return cloned (x + 1, 5);
}

// { dg-final { scan-assembler {_ZL6clonedii\.(constprop|isra)} } }
// { dg-final { scan-assembler "0x1146" } }
// { dg-final { scan-assembler "0x114d" } }
// { dg-final { scan-assembler {\.cv_ucomp} } }
