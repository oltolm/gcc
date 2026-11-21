// { dg-do assemble  }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

class foo {
public:
  operator <<(const void *);  //{ dg-error "" } no return type
  operator <<(char *);        //{ dg-error "" } no return type
};

void main()		      // { dg-error "must return .int" }
{
  foo f;
  f << (void*)0;
}

