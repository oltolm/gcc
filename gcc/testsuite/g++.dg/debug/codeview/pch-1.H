// { dg-do compile }
// { dg-options "-gcodeview" }

// c_common_post_options must allow CodeView when creating a precompiled
// header.

struct record
{
  int value;
};
