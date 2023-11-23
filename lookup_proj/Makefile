# Makefile for progressive dictionary exercise

PROGS = \
	convert			\
	file_lookup		\
	isam_lookup		\
	fifo_client		\
	fifo_server		\
	msgq_client		\
	msgq_server		\
	mmap_lookup		\
	shm_client		\
	shm_server		\
	usock_client		\
	usock_server		\
	isock_client		\
	isock_server		\
	udp_client		\
	udp_server
	

all : $(PROGS)

file_lookup : lookup1.o main.o
	$(CC) -o $@ lookup1.o main.o

isam_lookup : lookup2.o main.o
	$(CC) -o $@ lookup2.o main.o

fifo_client : lookup3.o main.o
	$(CC) -o $@ lookup3.o main.o

fifo_server : lookup2.o fifo_server.o
	$(CC) -o $@ lookup2.o fifo_server.o

msgq_client : lookup4.o main.o
	$(CC) -o $@ lookup4.o main.o

msgq_server : lookup2.o msgq_server.o
	$(CC) -o $@ lookup2.o msgq_server.o

mmap_lookup : lookup5.o main.o
	$(CC) -o $@ lookup5.o main.o

shm_client : lookup6.o main.o
	$(CC) -o $@ lookup6.o main.o

shm_server : lookup5.o shm_server.o
	$(CC) -o $@ lookup5.o shm_server.o

usock_client : lookup7.o main.o
	$(CC) -o $@ lookup7.o main.o 

usock_server : lookup5.o usock_server.o
	$(CC) -o $@ lookup5.o usock_server.o 

isock_client : lookup8.o main.o
	$(CC) -o $@ lookup8.o main.o -lnsl

isock_server : lookup5.o isock_server.o
	$(CC) -o $@ lookup5.o isock_server.o -lnsl

udp_client : lookup9.o main.o
	$(CC) -o $@ lookup9.o main.o -lnsl

udp_server : lookup5.o udp_server.o
	$(CC) -o $@ lookup5.o udp_server.o -lnsl

clean :
	rm -f *.o $(PROGS)
