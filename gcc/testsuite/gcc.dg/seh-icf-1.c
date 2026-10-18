/* { dg-do compile { target x86_64-*-mingw* } } */
/* { dg-options "-O2 -fdump-ipa-icf-details" } */
/* Identical __finally blocks of different functions must not be merged
   by identical code folding: they are noipa, like the functions with
   __try that contain them.  */

int flag;

void
f (void)
{
  __try
    {
    }
  __finally
    {
      flag = 1;
    }
}

void
g (void)
{
  __try
    {
    }
  __finally
    {
      flag = 1;
    }
}

/* { dg-final { scan-ipa-dump "Merge operation is skipped due to no_icf attribute" "icf" } } */
