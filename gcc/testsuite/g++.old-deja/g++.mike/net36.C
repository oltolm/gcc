// { dg-do assemble  }
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

class X;

class A {
public:
  void handlerFn(X*);
};

typedef void (A::*handler) (X*);

class B {
public:
  void setHandler(handler);
};

void f(B* b) {
  b->setHandler(A::handlerFn);	// { dg-error "" } 
}
