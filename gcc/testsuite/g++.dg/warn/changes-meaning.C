/* { dg-do compile } */
/* { dg-options "-fpermissive" } */
// { dg-additional-options "-fno-ms-extensions" { target *-*-mingw* } }

template <class _Tp> class auto_ptr {};  /* { dg-message "declared here" } */
template <class _Tp>
class counted_ptr
{
public:
  auto_ptr<_Tp> auto_ptr(); /* { dg-warning "17:declaration of .auto_ptr\\<_Tp\\>" } */
};

