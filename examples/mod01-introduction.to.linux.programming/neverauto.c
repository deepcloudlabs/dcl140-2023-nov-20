#include <stdio.h>

int another_function_call();

int main() {
   int *foo(int);
   int *yp;
   int *zp;

   yp = foo(3);
   another_function_call();
   zp = foo(8);
   printf(" %d \n", *yp );
   free(yp);
   free(zp);
}

int *
foo( int p )
{
   int *a=(int*) malloc(sizeof(int));

   *a = p + 2;
   return( a );
}

int another_function_call() {
  int i=42;
  return i%2;
}
