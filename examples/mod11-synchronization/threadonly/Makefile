all: mutex rw sem stackMain ipcsem

mutex: mutex.o utils.o
	cc -o mutex mutex.o utils.o -lrt -lpthread

mutex.o: mutex.c
	cc -c  mutex.c

rw: rw.o utils.o
	cc -o rw rw.o utils.o -lrt -lpthread

rw.o: rw.c
	cc -g -c  rw.c -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast

sem: sem.o utils.o
	cc -o sem sem.o utils.o -lrt -lpthread

sem.o: sem.c
	cc -c  sem.c -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast

stackMain: stackMain.o stack.o utils.o
	cc -o stackMain stackMain.o stack.o utils.o -lrt -lpthread

stackMain.o: stackMain.c stack.h
	cc -c  stackMain.c

stack.o: stack.c stack.h
	cc -c  stack.c

ipcsem : ipcsem.o utils.o
	cc -o ipcsem ipcsem.o utils.o -lrt -lpthread

ipcsem.o: ipcsem.c
	cc -c  ipcsem.c -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast -Wno-implicit-function-declaration

utils.o: utils.c
	cc -c  utils.c

clean:
	rm -f *.o ipcsem stackMain sem rw mutex
