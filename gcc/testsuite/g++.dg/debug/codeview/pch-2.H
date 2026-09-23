// { dg-do compile }
// { dg-options "-gcodeview -g" }

// c_common_post_options must allow CodeView combined with DWARF when
// creating a precompiled header.

struct record
{
  int value;
};
