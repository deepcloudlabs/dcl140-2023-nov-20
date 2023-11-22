all: proc_mutex proc_rw proc_sem proc_stackMain

proc_mutex: proc_mutex.o
	cc -o proc_mutex proc_mutex.o -lpthread -lrt

proc_mutex.o: proc_mutex.c
	cc -c -lpthread proc_mutex.c

proc_rw: proc_rw.o utils.o
	cc -o proc_rw proc_rw.o utils.o -lpthread -lrt 

proc_rw.o: proc_rw.c
	cc -g -c -lpthread proc_rw.c -Wno-int-to-pointer-cast -Wno-pointer-to-int-cast

proc_sem: proc_sem.o utils.o
	cc -o proc_sem proc_sem.o utils.o -lpthread -lrt

proc_sem.o: proc_sem.c
	cc -c -lpthread proc_sem.c -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

proc_stackMain: proc_stackMain.o proc_stack.o utils.o
	cc -o proc_stackMain proc_stackMain.o proc_stack.o utils.o -lrt -lpthread

proc_stackMain.o: proc_stackMain.c proc_stack.h
	cc -c -lpthread proc_stackMain.c

proc_stack.o: proc_stack.c proc_stack.h
	cc -c -lpthread proc_stack.c

utils.o: utils.c
	cc -c -lpthread utils.c

clean:
	rm -f *.o proc_rw proc_stackMain proc_mutex proc_sem
