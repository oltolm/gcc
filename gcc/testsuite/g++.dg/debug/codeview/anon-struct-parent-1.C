// { dg-do compile }
// { dg-options "-gcodeview" }

// get_type_num_struct must use a forward reference when a nested type refers
// to its anonymous parent.

struct outer
{
  struct
  {
    struct inner
    {
      int i;
    };

    inner value;
  } member;
};

outer o;
