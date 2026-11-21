// { dg-do assemble  }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }
// GROUPS passed prefix-postfix
class foo {
public:
      operator ++ (); // { dg-error "" } no type or storage class
};

int main()
{
  foo x;

  // This should fall back to calling operator++(), and be an error with
  // the -pedantic flag.
  x++;// { dg-error "" } 
}
