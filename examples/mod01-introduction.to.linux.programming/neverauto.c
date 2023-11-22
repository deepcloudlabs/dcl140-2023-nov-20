#include <stdio.h>

int another_function_call();

int main() {
   int *foo(int);
   int *yp;

   yp = foo(3);
   another_function_call();
   printf(" %d \n", *yp );
}

int *
foo( int p )
{
   int a;

   a = p + 2;
   return( &a );
}

int another_function_call() {
  int i=42;
  return i%2;
}
