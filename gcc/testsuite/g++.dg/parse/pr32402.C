// { dg-do compile }

class pure
{
public:
  virtual bool isPure () = 0;
};

class notPure : public pure
{
public:
  bool isPure () { return false; }
};

void f()
{
  pure **list;
  list = new (pure (*[3]));
  for (int i = 0; i < 3; ++i)
    list[i] = new notPure ();
}
