all:	myftruncate \
	mymmap \
	mycp \
	pos_shm_play

pos_shm_play: pos_shm_play.c
	cc -o pos_shm_play pos_shm_play.c -lrt

clean:
	rm  -f myftruncate mymmap pos_shm_play mycp
