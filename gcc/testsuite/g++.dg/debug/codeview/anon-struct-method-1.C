// { dg-do compile }
// { dg-options "-gcodeview" }

// get_mfunction_type must not re-enter get_type_num_struct for an unfinished
// unnamed class that contains the member function.

struct S
{
  struct
  {
    void f () {}
  } a;
} s;
