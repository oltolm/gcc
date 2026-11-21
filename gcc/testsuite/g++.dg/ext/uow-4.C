/* { dg-do compile } */
/* { dg-options "-Wall -pedantic" } */
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

extern "C" {

typedef int UOW;  /* { dg-message "declared here" } */
struct ABC {
  UOW UOW; /* { dg-error "changes meaning" } */
};

}

