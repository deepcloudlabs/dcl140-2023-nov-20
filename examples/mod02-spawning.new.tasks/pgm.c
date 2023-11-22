int x;
static int b = 4;

int foo(int p) ;

int main() {
  int y;
  static int z;

  y = b;
  z = foo(y);
}

int foo( int p ) {
  int a;
  a = p + 2;
  return(a);
}
