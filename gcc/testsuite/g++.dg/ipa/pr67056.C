/* { dg-do run } */
/* { dg-options "-std=c++11 -O3 -fdump-ipa-cp"  } */
/* { dg-additional-options "-fPIC" { target fpic } } */
#include <memory>

class EmptyClass {
public:
    EmptyClass();
};

/* Keep EmptyClass::EmptyClass opaque so the type walk for CompositeClass
   must cross a call before reaching its vptr store.  */
__attribute__((noipa)) EmptyClass::EmptyClass() {
}

class CompositeClass {
public:
    CompositeClass() {}
    virtual ~CompositeClass() {}
    EmptyClass object;
    bool bool1;
    bool bool2;
};

bool boolFunc() {
    return true;
}

static bool staticBoolFunc(CompositeClass * ptr) {
    std::unique_ptr<CompositeClass> up(ptr);
    (void)up;

    return boolFunc();
}

int main(int, char **) {
    staticBoolFunc(new CompositeClass);
    return 0;
}

/* { dg-final { scan-ipa-dump "Speculative outer type:struct CompositeClass" "cp"  } } */
