// PR target/94770
// { dg-do compile { target { ! ia32 } } }
// { dg-options "-O2 -maccumulate-outgoing-args" }

// An empty class passed on the stack takes an 8-byte slot when the callee
// uses the MS ABI, as with MSVC, and no space when it uses the SysV ABI.

struct S {};

__attribute__((ms_abi)) void fm (long long, long long, long long, long long,
				 S, long long);
__attribute__((sysv_abi)) void fs (long long, long long, long long, long long,
				   long long, long long, S, long long);

void
gm ()
{
  fm (1, 2, 3, 4, S (), 42);
}

void
gs ()
{
  fs (1, 2, 3, 4, 5, 6, S (), 43);
}

// { dg-final { scan-assembler {movq\t\$42, 40\(%rsp\)} } }
// { dg-final { scan-assembler {movq\t\$43, \(%rsp\)} } }
